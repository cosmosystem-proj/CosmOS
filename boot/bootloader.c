/* Creator: Kurt M. Weber
 *
 * Created on: 2025-11-18
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#include <efi/efi.h>
#include <efi/efilib.h>

#include <boot/boot.h>
#include <boot/convenience.h>
#include <boot/file.h>
#include <boot/loadkernel.h>
#include <boot/memorymap.h>
#include <boot/paging.h>
#include <boot/wrappers.h>
#include <types.h>

const CHAR16 *kernel_path = u"\\EFI\\BOOT\\test.txt";

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,
                           EFI_SYSTEM_TABLE *SystemTable) {
  InitializeLib(ImageHandle, SystemTable);

  con_out_clear_screen();

  Print(L"Preparing to load CosmOS\r\n");

  wait_key_press();

  // Open kernel, get size, and set up page tables so we can jump to it
  EFI_LOADED_IMAGE *img = get_loaded_image(ImageHandle);
  EFI_FILE_HANDLE volume = get_volume_handle(img);
  EFI_FILE_HANDLE kernel = open_file(volume, kernel_path);
  uint64 kernel_size = (uint64)get_file_size(kernel);
  Print(L"Kernel size: %lu\r\n", kernel_size);
  Print(L"Pages required: %lu\r\n", PAGES_REQUIRED_4K(kernel_size));

  reg_cr3 cr3 = read_cr3();
  Print(L"CR3: %lu\r\n", cr3.raw);

  Print(L"Obtaining system memory map...");

  EFI_MEMORY_DESCRIPTOR *map;
  UINTN map_entries;
  UINTN descriptor_size;
  memory_map(&map, &map_entries, &descriptor_size);
  uefi_memory_map mem_map = {map, map_entries, descriptor_size};

  physical_map_list *pmap = convert_memory_map(&mem_map);
  Print(L"Sorting memory map...");
  pmap = sort_memory_map(pmap);
  Print(L"sorted\r\n");
  pmap = reduce_memory_map(pmap);

#if 0
  physical_map_list *tmp = pmap;
  uint64 i = 0;
  while (tmp) {
    Print(L"Base: %lX Size: %lX Type: %u\r\n", tmp->pm->base, tmp->pm->len,
          tmp->pm->type);
    tmp = tmp->next;
    if (!(i % 25) && (i > 0)) {
      wait_key_press();
    }
    i++;
  }
#endif

  uint64 memtab_size = 0;
  physical_map *memtab = tabularize_memory_map(pmap, &memtab_size);

#if 0
  for (uint64 i = 0; i < memtab_size; i++) {
    Print(L"Base: %lX Size: %lX Type: %u\r\n", memtab[i].base, memtab[i].len,
          memtab[i].type);
  }
#endif
  uint64 kernel_text_pages = PAGES_REQUIRED_4K(kernel_size);
  void *kernel_text_paddr = 0;
  allocate_pages(AllocateAnyPages, EfiLoaderData, kernel_text_pages,
                 (EFI_PHYSICAL_ADDRESS *)&kernel_text_paddr);

  uint64 kernel_heap_pages = PAGES_REQUIRED_4K(INITIAL_KERNEL_HEAP_SIZE);
  void *kernel_heap_paddr = 0;
  allocate_pages(AllocateAnyPages, EfiLoaderData, kernel_heap_pages,
                 (EFI_PHYSICAL_ADDRESS *)&kernel_heap_paddr);

  uint64 kernel_stack_pages = PAGES_REQUIRED_4K(INITIAL_KERNEL_STACK_SIZE);
  void *kernel_stack_paddr = 0;
  allocate_pages(AllocateAnyPages, EfiLoaderData, kernel_stack_pages,
                 (EFI_PHYSICAL_ADDRESS *)&kernel_stack_paddr);

  setup_page_tables(kernel_text_pages, kernel_heap_pages, kernel_stack_pages,
                    kernel_text_paddr, kernel_heap_paddr, kernel_stack_paddr,
                    memtab_size, memtab, memtab_size);
  //  uefi_call_wrapper(BS->ExitBootServices, 0);

  load_kernel(ImageHandle);

  return EFI_SUCCESS;
}
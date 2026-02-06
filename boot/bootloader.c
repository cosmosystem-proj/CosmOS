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

const CHAR16 *kernel_path = u"\\EFI\\BOOT\\cosmos.bin";
void jump_to_kernel();

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
  UINTN map_key = memory_map(&map, &map_entries, &descriptor_size);
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

  reg_cr3 new_cr3 = setup_page_tables(
      kernel_text_pages, kernel_heap_pages, kernel_stack_pages,
      kernel_text_paddr, kernel_heap_paddr, kernel_stack_paddr,
      PAGES_REQUIRED_4K(sizeof(physical_map) * memtab_size), memtab,
      memtab_size);

  void *phys_map_vaddr =
      (void *)KERNEL_BASE_ADDRESS + (kernel_text_pages * ONE_KILOBYTE * 4);
  void *kernel_heap_vaddr =
      phys_map_vaddr + (PAGES_REQUIRED_4K(sizeof(physical_map) * memtab_size) *
                        ONE_KILOBYTE * 4);
#if 0

  Print(L"Kernel text at physical %lX, virtual %lX, mapped to %lX\r\n",
        kernel_text_paddr, KERNEL_BASE_ADDRESS,
        virtual_to_physical((void *)KERNEL_BASE_ADDRESS, new_cr3));
  Print(L"     Virtual %lX mapped to physical %lX\r\n", kernel_text_paddr,
        virtual_to_physical(kernel_text_paddr, new_cr3));

  void *phys_map_vaddr =
      (void *)KERNEL_BASE_ADDRESS + (kernel_text_pages * ONE_KILOBYTE * 4);
  Print(L"Physical map at physical %lX, virtual %lX, mapped to %lX\r\n", memtab,
        phys_map_vaddr, virtual_to_physical((void *)phys_map_vaddr, new_cr3));
  Print(L"     Virtual %lX mapped to physical %lX\r\n", memtab,
        virtual_to_physical(memtab, new_cr3));

  void *kernel_heap_vaddr =
      phys_map_vaddr + (PAGES_REQUIRED_4K(sizeof(physical_map) * memtab_size) *
                        ONE_KILOBYTE * 4);
  Print(L"Kernel heap at physical %lX, virtual %lX, mapped to %lX\r\n",
        kernel_heap_paddr, kernel_heap_vaddr,
        virtual_to_physical(kernel_heap_vaddr, new_cr3));
  Print(L"     Virtual %lX mapped to physical %lX\r\n", kernel_heap_paddr,
        virtual_to_physical(kernel_heap_paddr, new_cr3));

  Print(L"Kernel stack at physical %lX, virtual %lX, mapped to %lX\r\n",
        kernel_stack_paddr, KERNEL_STACK_INIT_BOTTOM,
        virtual_to_physical((void *)KERNEL_STACK_INIT_BOTTOM, new_cr3));
  Print(L"     Virtual %lX mapped to physical %lX\r\n", kernel_stack_paddr,
        virtual_to_physical(kernel_stack_paddr, new_cr3));

  Print(L"BS->ExitBootServices at physical %lX, virtual %lX, old mapped to "
        L"%lX, new mapped to %lX\r\n",
        &(BS->ExitBootServices), &(BS->ExitBootServices),
        virtual_to_physical(&(BS->ExitBootServices), cr3),
        virtual_to_physical(&(BS->ExitBootServices), new_cr3));
#endif
  //  uefi_call_wrapper(BS->ExitBootServices, 0);

  Print(L"EFIMain virtual address: %lX, physical in old CR3: %lX, new CR3: "
        L"%lX\r\n",
        &efi_main, virtual_to_physical(&efi_main, cr3),
        virtual_to_physical(&efi_main, new_cr3));

  Print(L"write_cr3 virtual address: %lX, physical in old CR3: %lX, new CR3: "
        L"%lX\r\n",
        &write_cr3, virtual_to_physical(&write_cr3, cr3),
        virtual_to_physical(&write_cr3, new_cr3));

  /*Print(L"Swapping CR3\r\n");
  write_cr3(new_cr3);
  Print(L"CR3 swapped\r\n");*/

  load_kernel(ImageHandle, (byte *)kernel_text_paddr);

  // we have to call GetMemoryMap again to get a valid map key, since
  // ExitBootServices fails with an invalid parameter error if any memory has
  // been allocated since the GetMemoryMap call the map key refers to was made

  Print(L"jump_to_kernel addr: %lX\r\n", jump_to_kernel);
  wait_key_press();

  EFI_MEMORY_DESCRIPTOR *map2;
  UINTN map_entries2;
  UINTN descriptor_size2;
  UINTN map_key2;
  UINTN map_size = 0;
  UINTN descriptor_version;

  // map_key = memory_map(&map2, &map_entries2, &descriptor_size2);
  uefi_call_wrapper(BS->GetMemoryMap, 5, &map_size, NULL, &map_key2,
                    &descriptor_size2, &descriptor_version);
  // Print(L"Memkey: %lX\r\n", map_key2);

  EFI_STATUS ebs_status;
  ebs_status =
      uefi_call_wrapper(BS->ExitBootServices, 2, ImageHandle, map_key2);

  if (ebs_status == EFI_INVALID_PARAMETER) {
    Print(L"EBS failed...\r\n");
    wait_key_press();
  } else {
    jump_to_kernel();
  }

  return EFI_SUCCESS;
}
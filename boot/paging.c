/* Creator: Kurt M. Weber
 *
 * Created on: 2025-12-09
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#include <efi/efi.h>
#include <efi/efilib.h>

#include <boot/convenience.h>
#include <boot/memorymap.h>
#include <mm/mm.h>
#include <types.h>

uint64 count_phys_pages(uefi_memory_map map) {
  void *last_address = 0;
  void *top = 0;

  for (uint64 i = 0; i < (uint64)map.count; i++) {
    EFI_MEMORY_DESCRIPTOR *cur =
        (EFI_MEMORY_DESCRIPTOR *)(((uint8 *)map.mem_map) +
                                  (map.descriptor_size * i));
    void *top = (void *)(cur->PhysicalStart +
                         (cur->NumberOfPages * ONE_KILOBYTE * 4) - 1);
    // Print(L"Top: %lX\r\n", (uint64)top);
    Print(L"Range: %lX Physical start: %lX Top: %lX Pages: "
          L"%lX Type: %lX\r\n",
          i, cur->PhysicalStart, (uint64)top, cur->NumberOfPages, cur->Type);

    if (!(i % 25) && (i > 0)) {
      wait_key_press();
    }
  }

  return 0;
}

reg_cr3 read_cr3() {
  reg_cr3 cr3;

  asm volatile("mov %%cr3 ,%0" : "=r"(cr3.raw));

  return cr3;
}

reg_cr3 setup_page_tables(uint64 num_kernel_pages, uefi_memory_map mem_map) {
  /*
   * num_kernel_pages:  how many 2M pages are needed for the executable image
   *                    (straightforward if it's a raw binary, after needed
   *                    processing if it's an ELF image).  This does not include
   *                    the extra space afterwards for the kernel heap OR the
   *                    kernel stack pages.
   */

  /*
   * UEFI identity-maps the entire physical address space.  However, its page
   * tables are read-only, so it's not as simple as adding what we need into
   * them.  We *could* flip the protection bits and just modify them directly,
   * but honestly it's easier to control and manage if we just start from
   * scratch ourself.
   *
   * The other thing to keep in mind is that in the
   * bootloader, instruction and data addresses are going to be resolved based
   * on the identity map, so we need to keep an identity map in the page table
   * until we switch to the kernel.  However, once we jump tot he kernel we
   * don't need the identity map anymore, and all physical references will be to
   * the direct map.  So in addition to identity-mapping the entire physical
   * address space, we also map it in the direct-map range.  Then, we create two
   * different PML4Es: one that contains the lower-half identity map and the
   * higher-half kernel, kernel heap, stack, and direct map; and another that
   * includes JUST the higher-half portions.  Then, first thing the kernel does
   * after we jump to its entry point is switch the CR3 to point to the
   * higher-half-only PML4E.
   *
   * So here's what we do:
   *
   * 1) Work out how many physical
   * 4k pages the physical address space spans. This is the length of the
   * physical page index (PPI).
   *
   * 2) Allocate space for the physical page index.  Even
   * though we'll be allocating 2-megabyte pages for the most part, the index
   * uses a granularity of 4k so it can handle when we DO use 4k pages.
   *
   * 3) Figure out how many pages we need for the identity map page tables.
   * These are 4k pages.
   *
   * 4) Figure out how many (4k) pages we need for the direct map page tables.
   *
   * 5) Allocate those pages
   *
   * 6) Mark the direct map physical pages in the physical page index, but do
   * not mark the identity map because they won't be used after we jump to the
   * kernel so the kernel (who uses the index) doesn't need to concern itself
   * with them, it can treat those pages as free.
   *
   * 7) Mark the hardware-reserved pages appropriately in the PPI
   */
}

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
#include <boot/wrappers.h>
#include <mm/mm.h>
#include <types.h>

void *new_ptt() {
  void *ptt = 0;

  // note that what we're allocating here is the 4096 bytes (512 entries * 8
  // bytes per entry) for the next-lower level of PTT

  // So for example, when we call this from new_pml4e(), we're creating a PML4
  // table *entry* rather than an entire PML4 table.  That entry includes a
  // pointer to the PDP table, which we are allocating and returning the address
  // of.
  allocate_pages(AllocateAnyPages, EfiLoaderData, 1,
                 (EFI_PHYSICAL_ADDRESS *)&ptt);

  for (uint64 i = 0; i < 512; i++) {
    ((uint64 *)ptt)[i] = 0;
  }

  return ptt;
}

pde new_pde() {
  pde pde;

  pde.pde = (uint64)new_ptt();
  pde.present = 1;
  pde.rw = 1;
  pde.user = 0;
  pde.pwt = 0;
  pde.pcd = 0;
  pde.accessed = 0;
  pde.ign_lo = 0;
  pde.mbz = 0;
  pde.ign_hi = 0;
  pde.avl_lo = 0;
  pde.avl_hi = 0;
  pde.nx = 0;

  return pde;
}

pdpe new_pdpe() {
  pdpe pdpe;

  pdpe.pdpe = (uint64)new_ptt();
  pdpe.present = 1;
  pdpe.rw = 1;
  pdpe.user = 0;
  pdpe.pwt = 0;
  pdpe.pcd = 0;
  pdpe.accessed = 0;
  pdpe.ign_lo = 0;
  pdpe.mbz = 0;
  pdpe.ign_hi = 0;
  pdpe.avl_lo = 0;
  pdpe.avl_hi = 0;
  pdpe.nx = 0;

  return pdpe;
}

pte new_pte(void *phys, bool executable) {
  // executable controls the noexec flag, but we invert it to true means it IS
  // executable (meaning the flag is cleared), again this just seems easier to
  // parse mentally

  pte pte;

  pte.pte = (uint64)phys;
  pte.present = 1;
  pte.rw = 1;
  pte.user = 0;
  pte.pwt = 0;
  pte.pcd = 0;
  pte.accessed = 0;
  pte.dirty = 0;
  pte.global = 0;
  pte.pat = 0;
  pte.avl_lo = 0;
  pte.avl_hi = 0;
  pte.nx = !executable;

  return pte;
}

pml4e new_pml4e() {
  pml4e pml4e;

  pml4e.pml4e = (uint64)new_ptt();
  pml4e.present = 1;
  pml4e.rw = 1;
  pml4e.user = 0;
  pml4e.pwt = 0;
  pml4e.pcd = 0;
  pml4e.accessed = 0;
  pml4e.ign = 0;
  pml4e.mbz = 0;
  pml4e.avl_lo = 0;
  pml4e.avl_hi = 0;
  pml4e.nx = 0;

  return pml4e;
}

void map_at(pml4e *pml4, void *phys, void *virt, uint64 pages,
            bool executable) {
  // executable controls the noexec flag, but we invert it to true means it IS
  // executable (meaning the flag is cleared), again this just seems easier to
  // parse mentally

  for (uint64 i = 0; i < pages; i++) {
    void *cur_virt = virt + (i * ONE_KILOBYTE * 4);
    void *cur_phys = phys + (i * ONE_KILOBYTE * 4);

    uint64 pml4_idx = EXTRACT_PML4_IDX((uint64)cur_virt);
    pml4e pml4e;
    // if the entry at the given index is 0 we allocate a new one, create the
    // PML4 entry, and insert it into the table
    if (pml4[pml4_idx].pml4e == 0) {
      pml4e = new_pml4e();
      pml4[pml4_idx] = pml4e;
    }
    pml4e = pml4[pml4_idx];

    // and now the same for the PDP
    pdpe *pdp = (pdpe *)EXTRACT_PTTENTRY_BASE((uint64)pml4e.pml4e);
    uint64 pdp_idx = EXTRACT_PDP_IDX((uint64)cur_virt);
    pdpe pdpe;
    if (pdp[pdp_idx].pdpe == 0) {
      pdpe = new_pdpe();
      pdp[pdp_idx] = pdpe;
    }
    pdpe = pdp[pdp_idx];

    // and PD
    pde *pd = (pde *)EXTRACT_PTTENTRY_BASE((uint64)pdpe.pdpe);
    uint64 pd_idx = EXTRACT_PD_IDX((uint64)cur_virt);
    pde pde;
    if (pd[pd_idx].pde == 0) {
      pde = new_pde();
      pd[pd_idx] = pde;
    } else {
    }
    pde = pd[pd_idx];
    0;

    // now we do the PT, requires a bit different handling since it points
    // directly to the physical page
    pte *pt = (pte *)EXTRACT_PTTENTRY_BASE((uint64)pde.pde);
    uint64 pt_idx = EXTRACT_PT_IDX((uint64)cur_virt);
    pte pte = new_pte(cur_phys, executable);
    pt[pt_idx] = pte;
  }
}

reg_cr3 read_cr3() {
  reg_cr3 cr3;

  asm volatile("mov %%cr3 ,%0" : "=r"(cr3.raw));

  return cr3;
}

reg_cr3 setup_page_tables(uint64 kernel_text_pages, uint64 kernel_heap_pages,
                          uint64 kernel_stack_pages, void *kernel_text_phys_loc,
                          void *kernel_heap_phys_loc,
                          void *kernel_stack_phys_loc, uint64 phys_map_pages,
                          physical_map *phys_map, uint64 phys_map_size) {
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
   * until we switch to the kernel.  However, once we jump to the kernel we
   * don't need the identity map anymore, and all physical references will be
   * via the direct map.  So in addition to identity-mapping the entire physical
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
  pml4e *id;

  id = (pml4e *)new_ptt();

  void *kernel_text_vaddr = (void *)KERNEL_BASE_ADDRESS;
  void *phys_map_vaddr =
      kernel_text_vaddr + (kernel_text_pages * ONE_KILOBYTE * 4);
  void *kernel_heap_vaddr =
      phys_map_vaddr + (phys_map_pages * ONE_KILOBYTE * 4);
  void *kernel_stack_vaddr = (void *)KERNEL_STACK_INIT_BOTTOM;

  Print(L"Mapping kernel text...");
  map_at(id, kernel_text_phys_loc, kernel_text_vaddr, kernel_text_pages, TRUE);
  Print(L"done\r\n");

  Print(L"Mapping physical block map...");
  map_at(id, phys_map, phys_map_vaddr, phys_map_pages, FALSE);
  Print(L"done\r\n");

  Print(L"Mapping kernel heap...");
  map_at(id, kernel_heap_phys_loc, kernel_heap_vaddr, kernel_heap_pages, FALSE);
  Print(L"done\r\n");

  Print(L"Mapping kernel stack...");
  map_at(id, kernel_stack_phys_loc, kernel_stack_vaddr, kernel_stack_pages,
         FALSE);
  Print(L"done\r\n");

  // For the direct map and identity map, we loop through the physical block
  // table and do each block separately
  Print(L"Mapping direct map...");
  for (uint64 i = 0; i < phys_map_size; i++) {
    void *base = phys_map[i].base;
    uint64 num_pages = PAGES_REQUIRED_4K(phys_map[i].len);
    map_at(id, base, DIRECT_MAP_BASE_ADDRESS + base, num_pages, FALSE);
  }
  Print(L"done\r\n");

  Print(L"Mapping identity map...");
  for (uint64 i = 0; i < phys_map_size; i++) {
    void *base = phys_map[i].base;
    uint64 num_pages = PAGES_REQUIRED_4K(phys_map[i].len);
    // identity map needs to be executable because UEFI code will still reside
    // there after we switch the cr3
    map_at(id, base, base, num_pages, TRUE);
  }
  Print(L"done\r\n");
}

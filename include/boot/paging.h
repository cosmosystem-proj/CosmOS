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

#ifndef _BOOT_PAGING_H
#define _BOOT_PAGING_H

#include <mm/mm.h>
#include <types.h>

void *new_ptt();
reg_cr3 read_cr3();
reg_cr3 setup_page_tables(uint64 kernel_text_pages, uint64 kernel_heap_pages,
                          uint64 kernel_stack_pages, void *kernel_text_phys_loc,
                          void *kernel_heap_phys_loc,
                          void *kernel_stack_phys_loc, uint64 phys_map_pages,
                          physical_map *phys_map, uint64 phys_map_size);
#endif
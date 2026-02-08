/* Creator: Kurt M. Weber
 *
 * Created on: 2026-02-05
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#include <sys/console/early_console.h>
#include <types.h>

void *heap_base;

void CosmOS(reg64 cr3, void *phys_map_vaddr, void *heap_vaddr) {
  heap_base = heap_vaddr;

  early_console_buffer_init();
  set_system_console(NULL);

  while (1) {
    asm volatile("hlt");
  }
  return;
}
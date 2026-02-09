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
#include <sys/kmalloc/kmalloc.h>
#include <types.h>

void *heap_base;

void CosmOS(reg64 cr3, void *phys_map_vaddr, void *heap_vaddr) {
  heap_base = heap_vaddr;

  early_console_buffer_init();
  // set_system_console(NULL);
  kmalloc_init();
  void *a = kmalloc(55);
  void *b = kmalloc(748);
  void *c = kmalloc(256);

  kfree(b);

  void *d = kmalloc(26);
  void *e = kmalloc(19);

  while ((a == b) && (b == c) && d && e)
    ;

  while (1) {
    asm volatile("hlt");
  }
  return;
}
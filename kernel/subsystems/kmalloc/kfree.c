/* Creator: Kurt M. Weber
 *
 * Created on: 2026-02-09
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#include "kmalloc_internal.h"
#include <sys/kmalloc/kmalloc.h>

void kfree(void *p) {
  // TODO: merge adjacent free blocks
  if (!p) {
    return;
  }

  kmalloc_header *head = p - sizeof(kmalloc_header);
  head->inuse = false;

  return;
}
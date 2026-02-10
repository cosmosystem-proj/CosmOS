/* Creator: Kurt M. Weber
 *
 * Created on: 2026-02-08
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#include <cosmos.h>
#include <sys/kmalloc/kmalloc.h>
#include <types.h>

#include "kmalloc_internal.h"

void kmalloc_init() {
  kmalloc_header *start;
  start = KMALLOC_HEAP_BASE;

  start->prev = 0;
  start->base = start + 1; // remember pointer arithmetic
  start->len =
      0xFFFFFFFFFFFFFFFF -
      sizeof(
          kmalloc_header); // TODO: change this to a saner value that respects
                           // other things that have space reserved for them in
                           // the higher half (e.g. stack, direct map, etc.)
  start->inuse = false;
  start->owner = 0;
  start->next = 0;

  return;
}

void *kmalloc(size_t size) {
  kmalloc_header *cur;
  kmalloc_header *new;

  size_t size_padded = PAD_BYTES(size, 8);

  // Detect wraparound for extremely large values and fail if it happens
  if (size_padded < size) {
    return NULL;
  }

  cur = KMALLOC_HEAP_BASE;

  while (cur->inuse || (cur->len < size_padded)) {
    cur = cur->next;
    if (!cur) {
      return NULL;
    }
  }

  // if the length of the block we've found is greater than twice the length of
  // (size_padded + sizeof(kmalloc_header)) then we split it, otherwise we just
  // return the entire current block.
  // TODO: Make this multiplier tunable
  if (cur->len > 2 * (size_padded + sizeof(kmalloc_header))) {
    cur->len = size_padded;
    new = cur->base + size_padded;
    new->next = cur->next;
    new->prev = cur;
    new->base = new + 1; // remember pointer arithmetic--we're adding 1 *
                         // sizeof(kmalloc_header)
    new->len = cur->len - size_padded - sizeof(kmalloc_header);
    new->inuse = false;
    new->owner = 0;
    cur->next = new;
  }

  cur->inuse = true;
  cur->owner = 0;

  return cur->base;
}
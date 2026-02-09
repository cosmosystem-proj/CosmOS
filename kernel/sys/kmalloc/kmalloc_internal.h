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

#ifndef KMALLOC_INTERNAL_H
#define KMALLOC_INTERNAL_H

#include <types.h>

#define KMALLOC_HEAP_BASE (kmalloc_header *)heap_base
#define PAD_BYTES_8(x) ((x) % 8 ? ((x) + (8 - ((x) % 8))) : (x))

#define INUSE(x) ((x) & 1)

// don't export this into a header, it's not part of the public kmalloc API
typedef struct __attribute__((packed, aligned(8))) {
  void *prev;
  void *base;
  size_t len;
  bool inuse;
  uint64 owner;
  void *next;
} kmalloc_header;

#endif
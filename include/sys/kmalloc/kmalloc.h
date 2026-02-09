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

#ifndef KMALLOC_H
#define KMALLOC_H

#include <types.h>

void kfree(void *p);
void *kmalloc(size_t size);
void kmalloc_init();

#endif

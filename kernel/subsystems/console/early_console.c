/* Creator: Kurt M. Weber
 *
 * Created on: 2026-02-06
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#include <cosmos.h>
#include <types.h>
#include <wchar.h>

// We use the early console buffer before we have dynamic memory allocation
// initialized.  Because we don't have dynamic memory allocation
// initialized, we can just move around through the heap since nothing else
// is using it yet.  We superimpose, via pointers, a uint64 at the start of
// the heap space as a counter of how many characters we've added, and then
// place each new character immediately after the last one.

void early_console_buffer_init() {
  // all we need to do is initialize the counter
  uint64 *counter = heap_base;
  *counter = 0;

  return;
}

void early_console_buffer_write(const utf32 ch) {
  uint64 counter = (*(uint64 *)heap_base);
  utf32 *base = (utf32 *)((byte *)heap_base + 8);

  base[counter] = ch;
  (*(uint64 *)heap_base)++;

  return;
}
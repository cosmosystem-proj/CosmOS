/* Creator: Kurt M. Weber
 *
 * Created on: 2025-11-28
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#include <boot/wrappers.h>

void wait_key_press() {
  EFI_INPUT_KEY Key;

  con_in_reset(FALSE);

  while (con_in_read_key_stroke(&Key) == EFI_NOT_READY) {
    ;
  }

  return;
}
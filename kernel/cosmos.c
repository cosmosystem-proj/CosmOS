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

void CosmOS() {
  asm volatile("mov 5, %rax");

  while (1) {
    asm volatile("hlt");
  }
  return;
}
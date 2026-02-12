/* Creator: Kurt M. Weber
 *
 * Created on: 2026-02-10
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#ifndef INTERRUPTS_ISR_H
#define INTERRUPTS_ISR_H

#include <types.h>

// One of these two structs is passed as an argument to interrupt handlers,
// depending on whether it includes an error or not.  It is passed as a pointer
// to the bottom of the return stack frame, in which rip (or error if
// applicable) is the last to be pushed and so at the lowest address, thus why
// the various values are ordered as they are
typedef struct {
  uint64 rip;
  uint64 cs;
  uint64 rflags;
  uint64 rsp;
  uint64 ss;
} __attribute__((packed)) isr_stack_frame;

typedef struct {
  uint64 error;
  uint64 rip;
  uint64 cs;
  uint64 rflags;
  uint64 rsp;
  uint64 ss;
} __attribute__((packed)) isr_stack_frame_error;

void isr_generic();

#endif
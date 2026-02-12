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

#ifndef INTERRUPTS_VECTORS_H
#define INTERRUPTS_VECTORS_H

#include <types.h>

#define PIC_PRIMARY_OFFSET 0x20
#define PIC_SECONDARY_OFFSET (PIC_PRIMARY_OFFSET + 8)

typedef enum {
  INTERRUPT_VECTOR_DE = 0,  // division error
  INTERRUPT_VECTOR_DB = 1,  // debug
  INTERRUPT_VECTOR_NMI = 2, // non-maskable interrupt
  INTERRUPT_VECTOR_BP = 3,  // breakpoint
  INTERRUPT_VECTOR_OF = 4,  // overflow
  INTERRUPT_VECTOR_BR = 5,  // bound range exceeded
  INTERRUPT_VECTOR_UD = 6,  // invalid opcode
  INTERRUPT_VECTOR_NM = 7,  // device not available
  INTERRUPT_VECTOR_DF = 8,  // double fault
  INTERRUPT_VECTOR_CS = 9,  // coprocessor segment overrun (no longer used)
  INTERRUPT_VECTOR_TS = 10, // invalid TSS
  INTERRUPT_VECTOR_NP = 11, // segment not present
  INTERRUPT_VECTOR_SS = 12, // stack-segment fault
  INTERRUPT_VECTOR_GP = 13, // general protection fault
  INTERRUPT_VECTOR_PF = 14, // page fault
  INTERRUPT_VECTOR_MF = 16, // x87 floating-point exception
  INTERRUPT_VECTOR_AC = 17, // alignment check
  INTERRUPT_VECTOR_MC = 18, // machine check
  INTERRUPT_VECTOR_XM = 19, // SIMD floating-point exception
  INTERRUPT_VECTOR_VE = 20, // virtualization exception
  INTERRUPT_VECTOR_CP = 21, // control protection exception
  INTERRUPT_VECTOR_HV = 28, // hypervisor injection
  INTERRUPT_VECTOR_VC = 29, // VMM communication
  INTERRUPT_VECTOR_SX = 30, // security
  INTERRUPT_VECTOR_IRQ0 = PIC_PRIMARY_OFFSET,
  INTERRUPT_VECTOR_IRQ1,
  INTERRUPT_VECTOR_IRQ2,
  INTERRUPT_VECTOR_IRQ3,
  INTERRUPT_VECTOR_IRQ4,
  INTERRUPT_VECTOR_IRQ5,
  INTERRUPT_VECTOR_IRQ6,
  INTERRUPT_VECTOR_IRQ7,
  INTERRUPT_VECTOR_IRQ8 = PIC_SECONDARY_OFFSET,
  INTERRUPT_VECTOR_IRQ9,
  INTERRUPT_VECTOR_IRQ10,
  INTERRUPT_VECTOR_IRQ11,
  INTERRUPT_VECTOR_IRQ12,
  INTERRUPT_VECTOR_IRQ13,
  INTERRUPT_VECTOR_IRQ14,
  INTERRUPT_VECTOR_IRQ15
} interrupt_vector;

#endif
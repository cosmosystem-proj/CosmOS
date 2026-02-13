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

#ifndef INTERRUPTS_IDT_H
#define INTERRUPTS_IDT_H

#include <types.h>

#define IDT_ATTR_INTR 0b00001110
#define IDT_ATTR_TRAP 0b00001111

#define IDT_ATTR_DPL_0 0b00000000
#define IDT_ATTR_DPL_3 0b01000000

#define IDT_ATTR_PRESENT 0b10000000

typedef struct {
  uint16 offset_word_low;
  uint16 selector;
  uint8 ist; // always zero, we don't use IST
  uint8 attrs;
  uint16 offset_word_mid;
  uint32 offset_dword_high;
  uint32 reserved;
} __attribute__((packed)) idt_entry;

typedef struct {
  uint16 limit;
  uint64 base;
} __attribute__((packed)) idtr;

void idt_init();

#endif
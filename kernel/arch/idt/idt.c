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

#include <interrupts/idt.h>
#include <interrupts/isr.h>
#include <interrupts/vectors.h>

#define IDT_SIZE 256

idt_entry idt[IDT_SIZE] = {0};

void idt_add_isr(void *isr, interrupt_vector vec, uint8 type) {
  idt[vec].offset_word_low = (uint16)((uint64)isr & 0xFFFF);
  idt[vec].selector = 8;
  idt[vec].ist = 0;
  idt[vec].attrs = type | IDT_ATTR_DPL_0 | IDT_ATTR_PRESENT;
  idt[vec].offset_word_mid = (uint16)(((uint64)isr >> 16) & 0xFFFF);
  idt[vec].offset_dword_high = (uint32)(((uint64)isr >> 32) & 0xFFFF);

  return;
}

void idt_init() {
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_DE, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_DB, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_NMI, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_BP, IDT_ATTR_TRAP);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_OF, IDT_ATTR_TRAP);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_BR, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_UD, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_NM, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_DF, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_CS, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_CS, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_NP, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_SS, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_GP, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_PF, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_MF, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_AC, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_MC, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_XM, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_VE, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_CP, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_HV, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_VC, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_SX, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ0, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ1, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ2, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ3, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ4, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ5, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ6, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ7, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ8, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ9, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ10, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ11, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ12, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ13, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ14, IDT_ATTR_INTR);
  idt_add_isr(isr_generic, INTERRUPT_VECTOR_IRQ15, IDT_ATTR_INTR);

  idtr idtr;

  idtr.limit = (IDT_SIZE * sizeof(idt_entry)) - 1;
  idtr.base = (uint64)&idt;

  asm volatile("lidt %0" ::"m"(idtr));

  return;
}
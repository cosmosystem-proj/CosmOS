/* Creator: Kurt M. Weber
 *
 * Created on: 2025-12-09
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#ifndef _TYPES_H
#define _TYPES_H

#ifdef __LP64__
// unsigned integer types
typedef unsigned long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

// raw bit data types
typedef unsigned long qword;
typedef unsigned int dword;
typedef unsigned short word;
typedef unsigned char byte;

// register types
typedef unsigned long reg64;
typedef unsigned int reg32;
typedef unsigned short reg16;
typedef unsigned char reg8;

// specific registers
typedef union {
  reg64 raw;
  struct {
    qword : 3;
    qword pwt : 1;
    qword pct : 1;
    qword : 11;
    qword pml4_base : 40;
    qword : 12;
  } values;
} reg_cr3;
#endif

#endif
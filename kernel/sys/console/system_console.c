/* Creator: Kurt M. Weber
 *
 * Created on: 2026-02-07
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#include <types.h>

uint64 system_console;

bool set_system_console(uint64 console) { console = system_console; }
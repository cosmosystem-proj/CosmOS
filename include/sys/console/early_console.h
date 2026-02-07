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

#ifndef SYS_CONSOLE_EARLY_CONSOLE_H
#define SYS_CONSOLE_EARLY_CONSOLE_H

#include <types.h>

void early_console_buffer_init();
void early_console_buffer_write(const utf32 ch);

#endif
/* Creator: Kurt M. Weber
 *
 * Created on: 2025-11-30
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#ifndef _BOOT_ERROR_H
#define _BOOT_ERROR_H

#include <efi/efi.h>

void status_msg(EFI_STATUS status, const char *file, const char *func,
                unsigned int line);

#endif
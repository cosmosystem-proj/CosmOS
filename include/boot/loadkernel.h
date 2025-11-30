/* Creator: Kurt M. Weber
 *
 * Created on: 2025-11-29
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#ifndef _BOOT_LOADKERNEL_H
#define _BOOT_LOADKERNEL_H

#include <efi/efi.h>

void load_kernel(EFI_HANDLE image_handle);

#endif
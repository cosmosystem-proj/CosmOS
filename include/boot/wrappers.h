/* Creator: Kurt M. Weber
 *
 * Created on: 2025-11-20
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#ifndef _WRAPPERS_H
#define _WRAPPERS_H

#include <efi/efi.h>
#include <efi/efilib.h>

EFI_STATUS ConOutClearScreen();
EFI_STATUS ConOutOutputString(const CHAR16 *s);
EFI_STATUS ConInReset(BOOLEAN ExtendedVerification);
EFI_STATUS ReadKeyStroke(EFI_INPUT_KEY *Key);

#endif

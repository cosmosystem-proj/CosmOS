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

#include <efi/efi.h>
#include <efi/efilib.h>

EFI_STATUS ConOutClearScreen() {
  return uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
}

EFI_STATUS ConOutOutputString(const CHAR16 *s) {
  return uefi_call_wrapper(ST->ConOut->OutputString, 2, ST->ConOut, s);
}

EFI_STATUS ConInReset(BOOLEAN ExtendedVerification) {
  return uefi_call_wrapper(ST->ConIn->Reset, 2, ST->ConIn, FALSE);
}

EFI_STATUS ConInReadKeyStroke(EFI_INPUT_KEY *Key) {
  return uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, Key);
}
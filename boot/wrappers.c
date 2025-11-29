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

EFI_STATUS con_out_clear_screen() {
  return uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
}

EFI_STATUS con_out_output_string(const CHAR16 *s) {
  return uefi_call_wrapper(ST->ConOut->OutputString, 2, ST->ConOut, s);
}

EFI_STATUS con_in_reset(BOOLEAN ExtendedVerification) {
  return uefi_call_wrapper(gST->ConIn->Reset, 2, ST->ConIn, FALSE);
}

EFI_STATUS con_in_read_key_stroke(EFI_INPUT_KEY *Key) {
  return uefi_call_wrapper(gST->ConIn->ReadKeyStroke, 2, ST->ConIn, Key);
}
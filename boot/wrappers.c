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

EFI_STATUS allocate_pages(EFI_ALLOCATE_TYPE alloc_type,
                          EFI_MEMORY_TYPE mem_type, UINTN num_pages,
                          EFI_PHYSICAL_ADDRESS *mem) {
  return uefi_call_wrapper(BS->AllocatePages, 4, alloc_type, mem_type,
                           num_pages, mem);
}

EFI_STATUS allocate_pool(EFI_MEMORY_TYPE pool_type, UINTN size, VOID **buffer) {
  return uefi_call_wrapper(BS->AllocatePool, 3, pool_type, size, buffer);
}

EFI_STATUS free_pool(VOID *buffer) {
  return uefi_call_wrapper(BS->FreePool, 1, buffer);
}

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
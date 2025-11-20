/* Creator: Kurt M. Weber
 *
 * Created on: 2025-11-18
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#include <efi/efi.h>
#include <efi/efilib.h>

#include <boot/wrappers.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,
                           EFI_SYSTEM_TABLE *SystemTable) {
  EFI_INPUT_KEY Key;
  InitializeLib(ImageHandle, SystemTable);

  ConOutClearScreen();

  ConOutOutputString(L"Preparing to load CosmOS\r\n");

  ConInReset(FALSE);

  while (ConInReadKeyStroke(&Key) == EFI_NOT_READY) {
    ;
  }

  Print(L"Obtaining system memory map...");
  return EFI_SUCCESS;
}
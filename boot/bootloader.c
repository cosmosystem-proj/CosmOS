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

#include <boot/convenience.h>
#include <boot/memorymap.h>
#include <boot/wrappers.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,
                           EFI_SYSTEM_TABLE *SystemTable) {
  InitializeLib(ImageHandle, SystemTable);

  con_out_clear_screen();

  Print(L"Preparing to load CosmOS\r\n");

  wait_key_press();

  Print(L"Obtaining system memory map...");

  memory_map();
  // uefi_call_wrapper(BS->ExitBootServices, 0);

  return EFI_SUCCESS;
}
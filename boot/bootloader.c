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

#include <boot/boot.h>
#include <boot/convenience.h>
#include <boot/loadkernel.h>
#include <boot/memorymap.h>
#include <boot/paging.h>
#include <boot/wrappers.h>
#include <types.h>

const CHAR16 *kernel_path = u"\\EFI\\BOOT\\test.txt";

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,
                           EFI_SYSTEM_TABLE *SystemTable) {
  InitializeLib(ImageHandle, SystemTable);

  con_out_clear_screen();

  Print(L"Preparing to load CosmOS\r\n");

  wait_key_press();

  Print(L"Obtaining system memory map...");

  reg_cr3 cr3 = setup_page_tables();

  memory_map();
  // uefi_call_wrapper(BS->ExitBootServices, 0);

  load_kernel(ImageHandle);

  return EFI_SUCCESS;
}
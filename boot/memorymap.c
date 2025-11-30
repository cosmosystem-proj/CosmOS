/* Creator: Kurt M. Weber
 *
 * Created on: 2025-11-27
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
#include <boot/wrappers.h>

void memory_map() {
  EFI_STATUS map_status = ~EFI_SUCCESS;
  EFI_STATUS pool_status;
  EFI_MEMORY_DESCRIPTOR *map = NULL;
  UINTN map_size = 0;
  UINTN map_key;
  UINTN descriptor_size;
  UINTN descriptor_version;

  while (map_status != EFI_SUCCESS) {
    map_status =
        uefi_call_wrapper(BS->GetMemoryMap, 5, &map_size, map, &map_key,
                          &descriptor_size, &descriptor_version);
    /*Print(L"Status: %lx\r\n", map_status);

    wait_key_press();*/

    if (map_status == EFI_BUFFER_TOO_SMALL) {
      UINTN new_size = map_size;

      if (map) {
        uefi_call_wrapper(BS->FreePool, 1, map);
      }

      pool_status =
          uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, new_size, &map);
    }
  }

  // Print(L"Map obtained\r\n");

  UINTN num_ranges = map_size / descriptor_size;
  Print(L"Map size: %lu\r\nDescriptor size: %lu\r\n", map_size,
        descriptor_size);
  Print(L"Number of descriptors: %lu\r\n", num_ranges);
  wait_key_press();

  /*for (UINTN i = 0; i < num_ranges; i++) {
    EFI_MEMORY_DESCRIPTOR *cur =
        (EFI_MEMORY_DESCRIPTOR *)(((uint8_t *)map) + (descriptor_size * i));
    Print(L"Range: %lX Physical start: %lX Virtual start: %lX Pages: "
          L"%lX Type: %lX\r\n",
          i, cur->PhysicalStart, cur->VirtualStart, cur->NumberOfPages,
          cur->Type);

    if (!(i % 25) && (i > 0)) {
      wait_key_press();
    }
  }
  wait_key_press();*/
}
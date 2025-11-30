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

#include <efi/efi.h>
#include <efi/efilib.h>

#include <boot/convenience.h>
#include <boot/error.h>

void load_kernel(EFI_HANDLE image_handle) {
  const EFI_GUID prot_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
  EFI_LOADED_IMAGE *loaded_image;
  EFI_FILE_HANDLE volume_handle;
  EFI_STATUS status;

  status = uefi_call_wrapper(BS->HandleProtocol, 3, image_handle, &prot_guid,
                             (void **)&loaded_image);

  if (status != EFI_SUCCESS) {
    Print(L"Obtaining image handle failed\r\n");
    wait_key_press();
    return;
  }

  volume_handle = LibOpenRoot(loaded_image->DeviceHandle);
  if (!volume_handle) {
    Print(L"Opening device root failed\r\n");
    wait_key_press();
    return;
  }

  CHAR16 *path = u"\\EFI\\BOOT\\test.txt";
  EFI_FILE_HANDLE file_handle;
  status =
      uefi_call_wrapper(volume_handle->Open, 5, volume_handle, &file_handle,
                        path, EFI_FILE_MODE_READ,
                        EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);

  if (status != EFI_SUCCESS) {
    status_msg(status, __FILE__, __func__, __LINE__);
  }

  // AllocatePool
  EFI_FILE_INFO *fi;
  fi = LibFileInfo(file_handle);
  UINT64 file_size = fi->FileSize;
  Print(L"File size: %u\r\n", file_size);
  FreePool(fi);

  char *buf;
  buf = AllocatePool(file_size + 1);
  uefi_call_wrapper(file_handle->Read, 3, file_handle, &file_size,
                    (UINT8 *)buf);
  buf[file_size] = '\0';

  Print(L"%a", buf);

  wait_key_press();

  return;
}
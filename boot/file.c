/* Creator: Kurt M. Weber
 *
 * Created on: 2025-12-08
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
#include <boot/wrappers.h>

UINT64 get_file_size(EFI_FILE_HANDLE file_handle) {
  // make sure file_handle is valid before calling, because this function has no
  // way to return errors

  EFI_FILE_INFO *fi;

  fi = LibFileInfo(file_handle);
  UINT64 file_size = fi->FileSize;
  FreePool(fi);

  return file_size;
}

EFI_LOADED_IMAGE *get_loaded_image(EFI_HANDLE image_handle) {
  EFI_STATUS status;
  const EFI_GUID prot_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
  EFI_LOADED_IMAGE *tmp;

  status =
      allocate_pool(EfiLoaderData, sizeof(EFI_LOADED_IMAGE), (VOID **)&tmp);
  if (status != EFI_SUCCESS) {
    status_msg(status, __FILE__, __func__, __LINE__);
    return NULL;
  }

  status = uefi_call_wrapper(BS->HandleProtocol, 3, image_handle, &prot_guid,
                             (VOID **)&tmp);
  if (status != EFI_SUCCESS) {
    status_msg(status, __FILE__, __func__, __LINE__);
    return NULL;
  }

  return tmp;
}

EFI_FILE_HANDLE get_volume_handle(EFI_LOADED_IMAGE *image) {
  EFI_FILE_HANDLE volume_handle;

  volume_handle = LibOpenRoot(image->DeviceHandle);
  if (!volume_handle) {
    Print(L"Opening device root failed\r\n");
    wait_key_press();
    return NULL;
  }

  return volume_handle;
}

EFI_FILE_HANDLE open_file(EFI_FILE_HANDLE volume_handle, CHAR16 *path) {
  EFI_STATUS status;
  EFI_FILE_HANDLE file_handle;
  status =
      uefi_call_wrapper(volume_handle->Open, 5, volume_handle, &file_handle,
                        path, EFI_FILE_MODE_READ,
                        EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);

  if (status != EFI_SUCCESS) {
    status_msg(status, __FILE__, __func__, __LINE__);
    return NULL;
  }

  return file_handle;
}
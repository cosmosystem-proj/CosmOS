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

EFI_STATUS get_loaded_image(EFI_HANDLE image_handle,
                            EFI_LOADED_IMAGE **loaded_image) {
  EFI_STATUS status;
  const EFI_GUID prot_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
  EFI_LOADED_IMAGE *tmp = 0;

  status = uefi_call_wrapper(BS->HandleProtocol, 3, image_handle, &prot_guid,
                             (VOID **)&tmp);

  *loaded_image = tmp;

  return status;
}

EFI_FILE_HANDLE get_volume_handle(EFI_LOADED_IMAGE *loaded_image) {
  EFI_FILE_HANDLE volume_handle;

  volume_handle = LibOpenRoot(loaded_image->DeviceHandle);
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

UINT64 get_file_size(EFI_FILE_HANDLE file_handle) {
  // make sure file_handle is valid before calling, because this function has no
  // way to return errors

  EFI_FILE_INFO *fi;

  fi = LibFileInfo(file_handle);
  UINT64 file_size = fi->FileSize;
  FreePool(fi);

  return file_size;
}

void load_kernel(EFI_HANDLE image_handle) {

  EFI_LOADED_IMAGE *loaded_image;
  EFI_FILE_HANDLE volume_handle, file_handle;
  EFI_STATUS status;

  status = get_loaded_image(image_handle, &loaded_image);
  if (status != EFI_SUCCESS) {
    status_msg(status, __FILE__, __func__, __LINE__);
    return;
  }

  volume_handle = get_volume_handle(loaded_image);

  CHAR16 *path = u"\\EFI\\BOOT\\test.txt";
  file_handle = open_file(volume_handle, path);

  UINT64 file_size = get_file_size(file_handle);

  char *buf;
  buf = AllocatePool(file_size + 1);
  uefi_call_wrapper(file_handle->Read, 3, file_handle, &file_size,
                    (UINT8 *)buf);
  buf[file_size] = '\0';

  Print(L"%a", buf);

  wait_key_press();

  return;
}
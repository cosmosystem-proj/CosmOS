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

#include <boot/boot.h>
#include <boot/convenience.h>
#include <boot/error.h>
#include <boot/file.h>
#include <boot/wrappers.h>
#include <mm/mm.h>

EFI_STATUS read_file_into_buffer(EFI_FILE_HANDLE file_handle, UINT8 *buffer,
                                 UINT64 buf_size) {
  return uefi_call_wrapper(file_handle->Read, 3, file_handle, &buf_size,
                           (UINT8 *)buffer);
}

void load_kernel(EFI_HANDLE image_handle) {
  EFI_LOADED_IMAGE *loaded_image;
  EFI_FILE_HANDLE volume_handle, file_handle;
  EFI_STATUS status;

  loaded_image = get_loaded_image(image_handle);

  volume_handle = get_volume_handle(loaded_image);

  file_handle = open_file(volume_handle, kernel_path);

  UINT64 file_size = get_file_size(file_handle);

  EFI_PHYSICAL_ADDRESS base_addr;

  status = allocate_pages(AllocateAnyPages, EfiLoaderData,
                          PAGES_REQUIRED_4K(file_size + 1), &base_addr);
  if (status != EFI_SUCCESS) {
    status_msg(status, __FILE__, __func__, __LINE__);
  }

  UINT8 *buffer = (UINT8 *)base_addr;

  status = read_file_into_buffer(file_handle, buffer, file_size);
  if (status != EFI_SUCCESS) {
    status_msg(status, __FILE__, __func__, __LINE__);
  }

  buffer[file_size] = '\0';

  Print(L"%a", buffer);

  wait_key_press();

  return;
}
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
#include <types.h>

EFI_STATUS read_file_into_buffer(EFI_FILE_HANDLE file_handle, UINT8 *buffer,
                                 UINT64 buf_size) {
  return uefi_call_wrapper(file_handle->Read, 3, file_handle, &buf_size,
                           (UINT8 *)buffer);
}

void load_kernel(EFI_HANDLE image_handle, byte *phys_addr) {
  EFI_LOADED_IMAGE *loaded_image;
  EFI_FILE_HANDLE volume_handle, file_handle;
  EFI_STATUS status;

  loaded_image = get_loaded_image(image_handle);

  volume_handle = get_volume_handle(loaded_image);

  file_handle = open_file(volume_handle, kernel_path);

  UINT64 file_size = get_file_size(file_handle);

  status = read_file_into_buffer(file_handle, phys_addr, file_size);
  if (status != EFI_SUCCESS) {
    status_msg(status, __FILE__, __func__, __LINE__);
  }

  // this is only necessary during testing when we load a text file and print it
  // out, not the actual kernel
  phys_addr[file_size] = '\0';

  //  Print(L"%a", phys_addr);

  // wait_key_press();

  return;
}
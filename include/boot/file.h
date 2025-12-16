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

#ifndef _BOOT_FILE_H
#define _BOOT_FILE_H

#include <efi/efi.h>

UINT64 get_file_size(EFI_FILE_HANDLE file_handle);
EFI_LOADED_IMAGE *get_loaded_image(EFI_HANDLE image_handle);
EFI_FILE_HANDLE get_volume_handle(EFI_LOADED_IMAGE *image);
EFI_FILE_HANDLE open_file(EFI_FILE_HANDLE volume_handle, CHAR16 *path);

#endif
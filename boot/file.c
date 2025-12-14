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

UINT64 get_file_size(EFI_FILE_HANDLE file_handle) {
  // make sure file_handle is valid before calling, because this function has no
  // way to return errors

  EFI_FILE_INFO *fi;

  fi = LibFileInfo(file_handle);
  UINT64 file_size = fi->FileSize;
  FreePool(fi);

  return file_size;
}
/* Creator: Kurt M. Weber
 *
 * Created on: 2025-11-30
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#include <efi/efi.h>
#include <efi/efilib.h>

#define STATUS_MSG(x) Print(x "\r\n")

void status_msg(EFI_STATUS status, const char *file, const char *func,
                unsigned int line) {
  Print(L"EFI Status in %a, function %a at line %u: ");

  switch (status) {
  case EFI_NOT_FOUND:
    STATUS_MSG(L"EFI_NOT_FOUND");
    break;
  case EFI_NO_MEDIA:
    STATUS_MSG(L"EFI_NO_MEDIA");
    break;
  default:
    STATUS_MSG(L"Unimplemented status");
    break;
  }
  return;
}
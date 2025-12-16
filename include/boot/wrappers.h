/* Creator: Kurt M. Weber
 *
 * Created on: 2025-11-20
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#ifndef _BOOT_WRAPPERS_H
#define _BOOT_WRAPPERS_H

#include <efi/efi.h>
#include <efi/efilib.h>

EFI_STATUS allocate_pages(EFI_ALLOCATE_TYPE alloc_type,
                          EFI_MEMORY_TYPE mem_type, UINTN num_pages,
                          EFI_PHYSICAL_ADDRESS *mem);
EFI_STATUS allocate_pool(EFI_MEMORY_TYPE pool_type, UINTN size, VOID **buffer);
EFI_STATUS con_out_clear_screen();
EFI_STATUS con_out_output_string(const CHAR16 *s);
EFI_STATUS con_in_reset(BOOLEAN ExtendedVerification);
EFI_STATUS con_in_read_key_stroke(EFI_INPUT_KEY *Key);

#endif

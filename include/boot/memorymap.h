/* Creator: Kurt M. Weber
 *
 * Created on: 2025-11-28
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#ifndef _BOOT_MEMORYMAP_H
#define _BOOT_MEMORYMAP_H

#include <efi/efi.h>

#include <mm/mm.h>
#include <types.h>

typedef struct physical_map_list physical_map_list;
typedef struct physical_map_list {
  physical_map *pm;
  physical_map_list *next;
} physical_map_list;

typedef struct {
  EFI_MEMORY_DESCRIPTOR *mem_map;
  UINTN count;
  UINTN descriptor_size;
} uefi_memory_map;

physical_map_list *convert_memory_map(uefi_memory_map *uefi_map);
void memory_map(EFI_MEMORY_DESCRIPTOR **mem_map, UINTN *count,
                UINTN *descriptor_size);
physical_map_list *reduce_memory_map(physical_map_list *pml);
physical_map_list *sort_memory_map(physical_map_list *pml);
physical_map *tabularize_memory_map(physical_map_list *pml, uint64 *count);

#endif
/* Creator: Kurt M. Weber
 *
 * Created on: 2025-11-27
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
#include <boot/memorymap.h>
#include <boot/wrappers.h>
#include <mm/mm.h>
#include <types.h>

physical_map_list *convert_memory_map(uefi_memory_map *uefi_map) {
  physical_map_list *list = NULL;
  physical_map *pmap = NULL;
  physical_map_list *cur = NULL;
  physical_map_list *tmp = NULL;

  allocate_pool(EfiLoaderData, uefi_map->count * sizeof(physical_map),
                (VOID **)&pmap);

  for (uint64 i = 0; i < (uint64)uefi_map->count; i++) {
    allocate_pool(EfiLoaderData, sizeof(physical_map_list), (VOID **)&tmp);
    if (!list) {
      list = tmp;
    }

    if (cur) {
      cur->next = tmp;
    }

    cur = tmp;

    cur->next = 0;
    cur->pm = &pmap[i];
    EFI_MEMORY_DESCRIPTOR *umm =
        (EFI_MEMORY_DESCRIPTOR *)(((uint8 *)uefi_map->mem_map) +
                                  (uefi_map->descriptor_size * i));

    cur->pm->base = (void *)umm->PhysicalStart;
    cur->pm->len = umm->NumberOfPages * (ONE_KILOBYTE * 4);
    switch (umm->Type) {
    // fallthrough here is intended behavior
    case EfiLoaderCode:
    case EfiLoaderData:
    case EfiBootServicesCode:
    case EfiBootServicesData:
    case EfiRuntimeServicesCode:
    case EfiRuntimeServicesData:
    case EfiConventionalMemory:
      cur->pm->type = USABLE;
      break;
    case EfiUnusableMemory:
      cur->pm->type = BAD;
      break;
    case EfiACPIReclaimMemory:
      cur->pm->type = ACPI_RECLAIM;
      break;
    case EfiACPIMemoryNVS:
      cur->pm->type = ACPI_NVS;
      break;
    default:
      cur->pm->type = RESERVED;
      break;
    }
  }
  return list;
}

void memory_map(EFI_MEMORY_DESCRIPTOR **mem_map, UINTN *count,
                UINTN *descriptor_size) {
  EFI_STATUS map_status = ~EFI_SUCCESS;
  EFI_STATUS pool_status;
  EFI_MEMORY_DESCRIPTOR *map = NULL;
  UINTN map_size = 0;
  UINTN map_key;
  UINTN descriptor_version;

  while (map_status != EFI_SUCCESS) {
    map_status =
        uefi_call_wrapper(BS->GetMemoryMap, 5, &map_size, map, &map_key,
                          descriptor_size, &descriptor_version);
    /*Print(L"Status: %lx\r\n", map_status);

    wait_key_press();*/

    if (map_status == EFI_BUFFER_TOO_SMALL) {
      UINTN new_size = map_size;

      if (map) {
        uefi_call_wrapper(BS->FreePool, 1, map);
      }

      pool_status =
          uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, new_size, &map);
    }
  }

  UINTN num_entries = map_size / *descriptor_size;
  *count = num_entries;
  *mem_map = map;

  Print(L"Map obtained\r\n");

  for (UINTN i = 0; i < num_entries; i++) {
    EFI_MEMORY_DESCRIPTOR *cur =
        (EFI_MEMORY_DESCRIPTOR *)(((uint8_t *)map) + (*descriptor_size * i));
    Print(L"Range: %lX Physical start: %lX Virtual start: %lX Pages: "
          L"%lX Type: %lX\r\n",
          i, cur->PhysicalStart, cur->VirtualStart, cur->NumberOfPages,
          cur->Type);

    if (!(i % 25) && (i > 0)) {
      wait_key_press();
    }
  }
  wait_key_press();
}

bool overlap_or_adjacent(physical_map *a, physical_map *b) {
  void *top = 0;

  if (!a || !b) {
    return FALSE;
  }

  top = a->base + a->len - 1;

  if ((b->base > a->base) && (b->base < top)) {
    return TRUE;
  }

  if (b->base == top) {
    return TRUE;
  }

  if (b->base == top + 1) {
    return TRUE;
  }

  return FALSE;
}

physical_map_list *reduce_memory_map(physical_map_list *pml) {
  // This assumes that the list has been sorted in ascending order of base
  // address, and will not (or at least should not be counted on to) produce
  // correct results if that is not the case!

  // If two consecutive list items (a) have the same type, and (b) overlap or
  // are immediately adjacent, then we combine them into a single block.

  physical_map_list *cur = 0, *tmp = 0;

  // treating void * as a pointer to a single byte for the purposes of pointer
  // arithmetic is a very useful GNU extension to C
  void *curtop = 0, *nexttop = 0;

  cur = pml;
  while (cur) {
    if (overlap_or_adjacent(cur->pm, cur->next->pm) &&
        (cur->pm->type == cur->next->pm->type)) {
      curtop = cur->pm->base + cur->pm->len - 1;
      nexttop = cur->next->pm->base + cur->next->pm->len - 1;

      // the next line is commented out because it's not necessary since the
      // list is (supposed to be) already sorted, but left in to clarify the
      // process

      // cur->pm->base = MIN(cur->pm->base, cur->next->pm->base);

      cur->pm->len = MAX(curtop, nexttop) - cur->pm->base + 1;
      tmp = cur->next;
      cur->next = tmp->next;
      free_pool(tmp);
    } else {
      // we only advance cur if we don't do any merging--if we do merge, then we
      // keep cur the same because we have to check if we need to merge it again
      // with the subsequent entry in the list

      cur = cur->next;
    }
  }

  return pml;
}

physical_map_list *sort_memory_map(physical_map_list *pml) {
  physical_map_list *tmp, *cur;
  physical_map_list **table;
  uint64 count = 0;

  tmp = pml;
  while (tmp) {
    count++;
    tmp = tmp->next;
  }
  Print(L"Count: %lu\r\n", count);

  allocate_pool(EfiLoaderData, count * sizeof(physical_map_list **),
                (VOID **)&table);

  // convert the linked list into a table to make sorting a bit easier to
  // grasp conceptually
  tmp = pml;
  for (uint64 i = 0; i < count; i++) {
    table[i] = tmp;
    tmp = tmp->next;
  }

  // this is a simple selection sort, inefficient perhaps but it only runs
  // once and the list should be fairly small
  uint64 min;
  for (uint64 i = 0; i < count; i++) {
    min = i;
    for (uint64 j = i; j < count;
         j++) { // since the table is already sorted up to i, we we can begin
                // our search for the next-lowest item at table[j] = table[i]
      if (table[j]->pm->base < table[min]->pm->base) {
        min = j;
      }
    }
    tmp = table[i];
    table[i] = table[min];
    table[min] = tmp;
  }

  tmp = table[0];
  cur = tmp;

  for (uint64 i = 1; i < count; i++) {
    cur->next = table[i];
    cur = cur->next;
    cur->next = 0;
  }

  free_pool((VOID *)table);

  return tmp;
}

physical_map *tabularize_memory_map(physical_map_list *pml, uint64 *count) {
  physical_map_list *tmp = 0;
  physical_map *map = 0;
  uint64 num_items = 0;

  tmp = pml;
  while (tmp) {
    num_items++;
    tmp = tmp->next;
  }

  allocate_pages(AllocateAnyPages, EfiLoaderData,
                 PAGES_REQUIRED_4K(sizeof(physical_map) * num_items),
                 (EFI_PHYSICAL_ADDRESS *)&map);

  tmp = pml;
  for (uint64 i = 0; i < num_items; i++) {
    map[i] = *(tmp->pm);
    tmp = tmp->next;
  }

  *count = num_items;
  return map;
}
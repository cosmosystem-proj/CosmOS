/* Creator: Kurt M. Weber
 *
 * Created on: 2025-12-01
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#ifndef _MM_MM_H
#define _MM_MM_H

#include <types.h>

#define ONE_KILOBYTE 1024
#define ONE_MEGABYTE 1048576

#define PAGES_REQUIRED_4K(x)                                                   \
  (((x) / 4096) + (((x) % (ONE_KILOBYTE * 4)) ? 1 : 0))
#define PAGES_REQUIRED_2M(x)                                                   \
  (((x) / 4096) + (((x) % (ONE_MEGABYTE * 2)) ? 1 : 0))

typedef enum {
  USABLE,
  RESERVED,
  ACPI_RECLAIM,
  ACPI_NVS,
  BAD,
  HOLE
} physical_region_type;

typedef struct {
  void *base;
  uint64 len;
  physical_region_type type;
} physical_map;

typedef enum page_directory_types {
  PDT_PHYS_AVAIL,        // Physical memory available for allocation
  PDT_SYSTEM_RESERVED,   // Reserved by operating system
  PDT_HARDWARE_RESERVED, // Reserved by hardware or BIOS - NEVER ALLOCATE
  PDT_HOLE,              // Hole in memory map - NEVER ALLOCATE
  PDT_BAD,               // Flagged as bad by BIOS - NEVER ALLOCATE
  PDT_INUSE,             // generic in-use flag
} page_directory_types;

typedef struct {
  uint64 ref_count;
  union {
    uint64 backing_handle;
    void *backing_addr;
  };
  page_directory_types type;
  uint64 flags;
} physical_page_index;
#endif
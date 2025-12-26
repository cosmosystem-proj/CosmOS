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
#define ONE_MEGABYTE (ONE_KILOBYTE * ONE_KILOBYTE)
#define ONE_GIGABYTE (ONE_MEGABYTE * ONE_KILOBYTE)
#define ONE_TERABYTE (ONE_GIGABYTE * ONE_KILOBYTE)

// multiply this by the # of kernel text pages to determine the # of pages to
// allocate for the initial kernel heap
#define INITIAL_KERNEL_HEAP_RATIO 1

// multiply this by the # of kernel text pages to determine the # of pages to
// allocate for the initial kernel stack
#define INITIAL_KERNEL_STACK_RATIO 1

// alternatively, we may want to use fixed values instead of ratios, experiment
// to see what works
#define INITIAL_KERNEL_HEAP_SIZE (ONE_MEGABYTE * 2)
#define INITIAL_KERNEL_STACK_SIZE (ONE_MEGABYTE * 2)

#define PAGES_REQUIRED_4K(x)                                                   \
  (((x) / 4096) + (((x) % (ONE_KILOBYTE * 4)) ? 1 : 0))
#define PAGES_REQUIRED_2M(x)                                                   \
  (((x) / 4096) + (((x) % (ONE_MEGABYTE * 2)) ? 1 : 0))

#define PTT_EXTRACT_BASE(x) ((x) & 0x000FFFFFFFFFF000)

#ifndef KERNEL_BASE_ADDRESS
#define KERNEL_BASE_ADDRESS 0xFFFF800000000000
#endif

#ifndef KERNEL_STACK_INIT_BOTTOM
#define KERNEL_STACK_INIT_BOTTOM                                               \
  (0x0000000000000000 - INITIAL_KERNEL_STACK_SIZE)
#endif

#ifndef DIRECT_MAP_BASE_ADDRESS
#define DIRECT_MAP_BASE_ADDRESS 0xFFFFC00000000000
#endif

#define PTTENTRY_BASE_MASK 0x000FFFFFFFFFF000
#define EXTRACT_PTTENTRY_BASE(x) ((x) & PTTENTRY_BASE_MASK)

#define PML4_INDEX_MASK 0x0000FF8000000000
#define PML4_INDEX_SHIFT 39
#define EXTRACT_PML4_IDX(x) (((x) & PML4_INDEX_MASK) >> PML4_INDEX_SHIFT)

#define PDP_INDEX_MASK 0x0000007FC0000000
#define PDP_INDEX_SHIFT 30
#define EXTRACT_PDP_IDX(x) (((x) & PDP_INDEX_MASK) >> PDP_INDEX_SHIFT)

#define PD_INDEX_MASK 0x000000003FE00000
#define PD_INDEX_SHIFT 21
#define EXTRACT_PD_IDX(x) (((x) & PD_INDEX_MASK) >> PD_INDEX_SHIFT)

#define PT_INDEX_MASK 0x00000000001FF000
#define PT_INDEX_SHIFT 12
#define EXTRACT_PT_IDX(x) (((x) & PT_INDEX_MASK) >> PT_INDEX_SHIFT)

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

typedef enum page_index_types {
  PDT_PHYS_AVAIL,        // Physical memory available for allocation
  PDT_SYSTEM_RESERVED,   // Reserved by operating system
  PDT_HARDWARE_RESERVED, // Reserved by hardware or BIOS - NEVER ALLOCATE
  PDT_HOLE,              // Hole in memory map - NEVER ALLOCATE
  PDT_BAD,               // Flagged as bad by BIOS - NEVER ALLOCATE
  PDT_INUSE,             // generic in-use flag
} page_index_types;

typedef struct {
  uint64 ref_count;
  uint64 block; // which block in the physical map this is part of
  union {
    uint64 backing_handle;
    void *backing_addr;
  };
  page_index_types type;
  uint64 flags;
} physical_page_index;

typedef union {
  uint64 pml4e;
  struct {
    uint64 present : 1;
    uint64 rw : 1;
    uint64 user : 1; // its formal name is user/supervisor, but we use just
                     // "user" for semantic reasons: when the "user" field is
                     // set to 1, user access is allowed (in addition to
                     // supervisor access, which is always allowed)
    uint64 pwt : 1;  // 0 = writeback caching, 1 = writethrough caching
    uint64 pcd : 1;  // 1 = caching disabled, 0 = caching not disabled (note
                     // carefully the negative semantics)
    uint64 accessed : 1;
    uint64 ign
        : 1; // ignored at all but lowest level of page translation hierarchy
    uint64 mbz : 2;
    uint64 avl_lo : 3;
    uint64 base : 40;
    uint64 avl_hi : 11;
    uint64 nx : 1;
  };
} pml4e;

typedef union {
  uint64 pdpe;
  struct {
    uint64 present : 1;
    uint64 rw : 1;
    uint64 user : 1;
    uint64 pwt : 1;
    uint64 pcd : 1;
    uint64 accessed : 1;
    uint64 ign_lo : 1;
    uint64 mbz : 1;
    uint64 ign_hi : 1;
    uint64 avl_lo : 3;
    uint64 base : 40;
    uint64 avl_hi : 11;
    uint64 nx : 1;
  };
} pdpe;

typedef union {
  uint64 pde;
  struct {
    uint64 present : 1;
    uint64 rw : 1;
    uint64 user : 1;
    uint64 pwt : 1;
    uint64 pcd : 1;
    uint64 accessed : 1;
    uint64 ign_lo : 1;
    uint64 mbz : 1;
    uint64 ign_hi : 1;
    uint64 avl_lo : 3;
    uint64 base : 40;
    uint64 avl_hi : 11;
    uint64 nx : 1;
  };
} pde;

typedef union {
  uint64 pte;
  struct {
    uint64 present : 1;
    uint64 rw : 1;
    uint64 user : 1;
    uint64 pwt : 1;
    uint64 pcd : 1;
    uint64 accessed : 1;
    uint64 dirty : 1;
    uint64 global : 1;
    uint64 pat : 1;
    uint64 avl_lo : 3;
    uint64 base : 40;
    uint64 avl_hi : 11;
    uint64 nx : 1;
  };
} pte;

// typedef pml4e pml4[512];
#endif
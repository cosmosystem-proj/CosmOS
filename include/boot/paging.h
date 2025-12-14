/* Creator: Kurt M. Weber
 *
 * Created on: 2025-12-09
 *
 * This file is part of CosmOS, a component of the Cosmoverse.
 *
 * Licensed under Hippocratic License with clauses:
 * HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR See file LICENSE for full
 * licensing information.
 */

#ifndef _BOOT_PAGING_H
#define _BOOT_PAGING_H

#include <mm/mm.h>
#include <types.h>

reg_cr3 setup_page_tables();

#endif
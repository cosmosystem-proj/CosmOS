# system/Makefile
# Created 2025-11-16 by Kurt M. Weber
# This file is part of CosmOS, a component of the Cosmoverse
# Licensed under Hippocratic License with clauses:
# HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR
# See file LICENSE for full licensing information.

WGET = wget

efi-img: OVMF.fd

OVMF.fd:
	$(WGET) https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd -O $@
# system/Makefile
# Created 2025-11-16 by Kurt M. Weber
# This file is part of CosmOS, a component of the Cosmoverse
# Licensed under Hippocratic License with clauses:
# HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR
# See file LICENSE for full licensing information.

WGET = wget

OVMF_URL = https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd

boot-img: OVMF.fd efi.img

efi.img:
# FAT32 partitions have a minimum size of 32 megabytes, we go with 48 to account for overhead
	dd if=/dev/zero of=efi.img bs=512 count=93750	

OVMF.fd:
	$(WGET) $(OVMF_URL) -O $@

clean:
	rm -f OVMF.fd
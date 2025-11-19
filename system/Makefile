# system/Makefile
# Created 2025-11-16 by Kurt M. Weber
# This file is part of CosmOS, a component of the Cosmoverse
# Licensed under Hippocratic License with clauses:
# HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR
# See file LICENSE for full licensing information.

WGET = wget

OVMF_URL = https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd

all: qemu-run

qemu-run: boot-img
	qemu-system-x86_64 -bios OVMF.fd -drive file=efi.img,if=ide

boot-img: OVMF.fd efi.img

efi.img: tmp.img
# FAT32 partitions have a minimum size of 32 megabytes, we go with 48 to account for overhead
#	dd if=/dev/zero of=efi.img bs=512 count=93750
#	/usr/sbin/parted efi.img -s -a minimal mklabel gpt
#	/usr/sbin/parted efi.img -s -a minimal mkpart EFI FAT32 2048s 93716s
#	/usr/sbin/parted efi.img -s -a minimal toggle 1 boot
#	dd if=tmp.img of=efi.img bs=512 count=91669 seek=2048 conv=notrunc
	dd if=/dev/zero of=fat.img bs=1k count=1440
	mformat -i fat.img -f 1440 ::
	mmd -i fat.img ::/EFI
	mmd -i fat.img ::/EFI/BOOT
	mcopy -i fat.img BOOTX64.EFI ::/EFI/BOOT


tmp.img: ../boot/BOOTX64.efi
#	dd if=/dev/zero of=tmp.img bs=512 count=91669
#	mformat -i tmp.img -F -h 32 -t 32 -n 64 -c 1
#	mmd -i tmp.img /BOOT
#	mmd -i tmp.img /BOOT/EFI
#	mcopy -i tmp.img $< ::/BOOT/EFI

OVMF.fd:
	$(WGET) $(OVMF_URL) -O $@

clean:
	rm -f OVMF.fd *.img
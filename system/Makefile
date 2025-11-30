# system/Makefile
# Created 2025-11-16 by Kurt M. Weber
# This file is part of CosmOS, a component of the Cosmoverse
# Licensed under Hippocratic License with clauses:
# HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR
# See file LICENSE for full licensing information.

WGET = wget

OVMF_URL = https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd

MKGPT = ../tools/3rdparty/mkgpt/mkgpt

all: qemu-run

qemu-run: boot-img
#qemu-system-x86_64 -bios OVMF.fd -drive file=efi.img,if=ide
	qemu-system-x86_64 -L . -pflash OVMF.fd -hda efi.img

boot-img: OVMF.fd efi.img

efi.img: tmp.img $(MKGPT)
	$(MKGPT) -o efi.img --image-size 4096 --part tmp.img --type system
	


tmp.img: ../boot/BOOTX64.efi test.txt
	dd if=/dev/zero of=tmp.img bs=1k count=1440
	mformat -i tmp.img -f 1440 ::
	mmd -i tmp.img ::/EFI
	mmd -i tmp.img ::/EFI/BOOT
	mcopy -i tmp.img ../boot/BOOTX64.efi ::/EFI/BOOT
	mcopy -i tmp.img test.txt ::/EFI/BOOT

../boot/BOOTX64.efi:
	cd ../boot && make BOOTX64.efi

OVMF.fd:
	$(WGET) $(OVMF_URL) -O $@

$(MKGPT):
	cd ../tools/3rdparty && make mkgpt

clean:
	rm -f OVMF.fd *.img
# boot/Makefile
# Created 2025-11-18 by Kurt M. Weber
# This file is part of CosmOS, a component of the Cosmoverse
# Licensed under Hippocratic License with clauses:
# HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR
# See file LICENSE for full licensing information.

SOURCES=$(shell find . -type f -iname '*.c' | sort)
OBJECTS=$(SOURCES:.c=.o)
DEPFILES=$(SOURCES:.c=.d)


CC = gcc
CFLAGS = -DDEBUG -I ../include -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args

.PHONY: clean

all: BOOTX64.efi

BOOTX64.efi: bootloader.so
	objcopy -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j \
	.rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 \
	--subsystem=10 bootloader.so $@

bootloader.so: $(OBJECTS)
	ld -shared -Bsymbolic -L/usr/lib64 -T/usr/lib64/gnuefi/elf_x86_64_efi.lds /usr/lib64/gnuefi/crt0-efi-x86_64.o $^ -o $@ -lgnuefi -lefi

.c.o: 
	$(CC) -MMD -MP $(CFLAGS) -c $< -o $@

clean:
	rm -f *.efi *.so $(OBJECTS) $(DEPFILES)

-include $(DEPFILES)
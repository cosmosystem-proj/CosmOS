# boot/Makefile
# Created 2025-11-18 by Kurt M. Weber
# This file is part of CosmOS, a component of the Cosmoverse
# Licensed under Hippocratic License with clauses:
# HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR
# See file LICENSE for full licensing information.

SOURCES_C = $(shell find . -type f -iname '*.c' | sort)
SOURCES_ASM = $(shell find . -type f -iname '*.asm' | sort)
OBJECTS_C = $(SOURCES_C:.c=.o)
OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)
DEPFILES = $(SOURCES:.c=.d)


ASM = nasm

CC = gcc
CFLAGS = -MMD -MP -I ../include -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args
LDFLAGS = -shared -Bsymbolic -L/usr/lib64 -T/usr/lib64/gnuefi/elf_x86_64_efi.lds /usr/lib64/gnuefi/crt0-efi-x86_64.o

.PHONY: clean

all: BOOTX64.efi

debug: DEBUG = -DDEBUG -g

debug: all

BOOTX64.efi: bootloader.so
	objcopy -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j \
	.rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 \
	--subsystem=10 bootloader.so $@

bootloader.so: $(OBJECTS_C) $(OBJECTS_ASM)
	ld $(LDFLAGS) $^ -o $@ -lgnuefi -lefi

%.o: %.asm
	$(ASM) -f elf64 $< -o $@

.c.o: 
	$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.efi *.so $(OBJECTS_ASM) $(OBJECTS_C) $(DEPFILES)

-include $(DEPFILES)
# kernel/Makefile
# Created 2026-02-05 by Kurt M. Weber
# This file is part of CosmOS, a component of the Cosmoverse
# Licensed under Hippocratic License with clauses:
# HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR
# See file LICENSE for full licensing information.

SOURCES_C = $(shell find . -type f -iname '*.c' | sort)
OBJECTS_C = $(SOURCES_C:.c=.o)
SOURCES_ASM = $(shell find . -type f -iname '*.asm' | sort)
OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)
DEPFILES = $(SOURCES_C:.c=.d)

ASM = nasm


KERNEL_ELF = cosmos.elf
KERNEL_BIN = cosmos.bin
KERNEL_MAP = cosmos.map
LDSCRIPT = cosmos.ld

CC = gcc
CFLAGS = -MMD -MP -I ../include -m64 -mno-red-zone -ffreestanding -Werror -Wuninitialized -Wunused-variable -Wreturn-type -Wparentheses -fPIC -O0

LD = ld
LDFLAGS = -m elf_x86_64 -T $(LDSCRIPT) -Map $(KERNEL_MAP) -nostdlib --no-relax

OBJCOPY = objcopy
OBJCOPYFLAGS = -O binary

.PHONY: clean

all: $(KERNEL_BIN)

debug: DEBUG = -DDEBUG -gdwarf-4
debug: ASM_DEBUG = -gdwarf

debug: all

$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(KERNEL_ELF) $(KERNEL_BIN)


$(KERNEL_ELF): $(OBJECTS_C) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(OBJECTS_C) $(OBJECTS_ASM)

%.o: %.asm
	$(ASM) -f elf64 $(ASM_DEBUG) $< -o $@

.c.o: 
	$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS_C) $(DEPFILES) *.elf *.bin

-include $(DEPFILES)
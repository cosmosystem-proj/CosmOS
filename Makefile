# Makefile
# Created 2025-11-16 by Kurt M. Weber
# This file is part of CosmOS, a component of the Cosmoverse
# Licensed under Hippocratic License with clauses:
# HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR
# See file LICENSE for full licensing information.

.PHONY = all tools clean

all: boot

tools: FORCE
	cd tools && make

boot: FORCE
	cd boot && make

system: boot tools FORCE
	cd system && make

FORCE:


clean: tools-clean boot-clean system-clean

tools-clean:
	cd tools && make clean

boot-clean:
	cd boot && make clean

system-clean:
	cd system && make clean
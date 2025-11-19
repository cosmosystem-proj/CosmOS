# tools/3rdparty/Makefile
# Created 2025-11-18 by Kurt M. Weber
# This file is part of CosmOS, a component of the Cosmoverse
# Licensed under Hippocratic License with clauses:
# HL3-CL-ECO-EXTR-FFD-MEDIA-MY-SUP-SV-TAL-USTA-XUAR
# See file LICENSE for full licensing information.

.PHONY: mkgpt clean

mkgpt: mkgpt/

mkgpt/: 
	git clone https://github.com/jncronin/mkgpt.git
	cd mkgpt && aclocal
	cd mkgpt &&	automake --add-missing
	cd mkgpt && autoreconf
	cd mkgpt && ./configure
	cd mkgpt && make

clean:
	rm -rf mkgpt/
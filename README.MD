#	MiSTer Input Tester

## Overview

A custom MiSTercade input test utility.  There are 3 tests available:
 - Analog video: RGB color ramps in the background
 - Analog audio: Pressing Player 1 Button 1 plays an audio sample in the left audio channel, and Player 1 Button 2 plays an audio sample in the right audio channel
 - Button test: 2 or 4 players, 8 buttons each, plus Coin, Start, Service, and Test
 -- Note: MiSTercade V1 only supports 6 buttons per player. MiSTercade V2 supports 8 buttons per player

## Acknowledgements

Created by JimmyStones (http://github.com/jimmystones), with thanks to:
- alanswx (https://github.com/alanswx) for ideas, cross-platform simulation support, and the tutorial this core was based on
- Jotego (https://github.com/jotego) for his very tidy JTFRAME modules
- Porkchop Express (https://misteraddons.com/) idea guy
- Sorgelig (https://github.com/sorgelig) for testing, finding bugs and suggesting new features.  Oh, and the MiSTer project :)

## Software

/src contains a C program compiled to Z80 assembly using sdcc.  

All required ROMs (compiled program and font) are built into this core.

Selecting Load BIOS in the core allows hot loading of binary OS ROM files for easier development.

### Building the OS

#### Required packages
- `sdcc` 
- `srecord`

#### Build steps
- Run `make` in `/src/Makefile` to create binary ROM
- Run `od -An -t x1 -v src/os.bin > rtl/rom.hex` to create hex version of ROM for Quartus build

## License
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

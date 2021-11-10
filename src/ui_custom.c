/*============================================================================
	Input Test - Custom UI functions

	Author: Jim Gregory - https://github.com/JimmyStones/
	Version: 1.0
	Date: 2021-07-12

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 3 of the License, or (at your option)
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program. If not, see <http://www.gnu.org/licenses/>.
===========================================================================*/
#pragma once
#include <stdbool.h>

#define color_pad_outline 0xFE

void page_frame()
{
	clear_chars(0);
	write_string("MiSTercade Tester", 0b11111111, 11, 1);
	panel_shaded(0, 0, 39, 2, 0b00000111, 0b00000110, 0b00000100);
}
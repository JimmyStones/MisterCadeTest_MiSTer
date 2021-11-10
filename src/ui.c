/*============================================================================
	Aznable OS - User interface functions

	Author: Jim Gregory - https://github.com/JimmyStones/
	Version: 1.1
	Date: 2021-07-15

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
#include "sys.c"

char asc_0 = 48;
char asc_1 = 49;

#define char_corner_round_tl 149
#define char_corner_round_tr 137
#define char_corner_round_bl 138
#define char_corner_round_br 139
#define char_line_h 131
#define char_line_v 130
#define char_t_up 177
#define char_dot 27
#define char_cross 155

// Set all character RAM to specified character
void clear_chars(char c)
{
	for (unsigned int p = 0; p < chram_size; p++)
	{
		chram[p] = c;
		fgcolram[p] = c;
		bgcolram[p] = c;
	}
}

// Set all character background colours to specified
void clear_bgcolor(char color)
{
	for (unsigned int p = 0; p < chram_size; p++)
	{
		bgcolram[p] = color;
	}
}

// Write string to character RAM
void write_string(const char *string, char color, unsigned char x, unsigned char y)
{
	unsigned int p = (y * chram_cols) + x;
	unsigned char l = strlen(string);
	for (char c = 0; c < l; c++)
	{
		chram[p] = string[c];
		fgcolram[p] = color;
		p++;
	}
}

// Write formatted string to character RAM (signed char data)
void write_stringfs(const char *format, char color, unsigned char x, unsigned char y, signed char data)
{
	unsigned int p = (y * chram_cols) + x;
	char temp[30];
	sprintf(temp, format, data);
	unsigned char l = strlen(temp);
	for (char c = 0; c < l; c++)
	{
		if (temp[c] == 0)
		{
			return;
		}
		chram[p] = temp[c];
		fgcolram[p] = color;
		p++;
	}
}

// Write formatted string to character RAM (unsigned char data)
void write_stringf(const char *format, char color, unsigned char x, unsigned char y, char data)
{
	unsigned int p = (y * chram_cols) + x;
	char temp[30];
	sprintf(temp, format, data);
	unsigned char l = strlen(temp);
	for (char c = 0; c < l; c++)
	{
		if (temp[c] == 0)
		{
			return;
		}
		chram[p] = temp[c];
		fgcolram[p] = color;
		p++;
	}
}

// Write formatted string to character RAM (unsigned short data)
void write_stringf_ushort(const char *format, char color, unsigned char x, unsigned char y, unsigned short data)
{
	unsigned int p = (y * chram_cols) + x;
	char temp[40];
	sprintf(temp, format, data);
	unsigned char l = strlen(temp);
	for (char c = 0; c < l; c++)
	{
		if (temp[c] == 0)
		{
			return;
		}
		chram[p] = temp[c];
		fgcolram[p] = color;
		p++;
	}
}

// Write formatted string to character RAM (signed short data)
void write_stringf_short(const char *format, char color, unsigned char x, unsigned char y, short data)
{
	unsigned int p = (y * chram_cols) + x;
	char temp[40];
	sprintf(temp, format, data);
	unsigned char l = strlen(temp);
	for (char c = 0; c < l; c++)
	{
		if (temp[c] == 0)
		{
			return;
		}
		chram[p] = temp[c];
		fgcolram[p] = color;
		p++;
	}
}

// Write formatted string to character RAM (unsigned long data)
void write_stringf_ulong(const char *format, char color, unsigned char x, unsigned char y, unsigned long data)
{
	unsigned int p = (y * chram_cols) + x;
	char temp[40];
	sprintf(temp, format, data);
	unsigned char l = strlen(temp);
	for (char c = 0; c < l; c++)
	{
		if (temp[c] == 0)
		{
			return;
		}
		chram[p] = temp[c];
		fgcolram[p] = color;
		p++;
	}
}

// Write single char to character RAM and colour RAM
void write_char(unsigned char c, char color, unsigned char x, unsigned char y)
{
	unsigned int p = (y * chram_cols) + x;
	chram[p] = c;
	fgcolram[p] = color;
}

// Write row of consecutive chars to character RAM and colour RAM
void write_char_row(unsigned char c, char color, unsigned char x, unsigned char y, unsigned char count)
{
	unsigned int p = (y * chram_cols) + x;
	for (char b = 0; b < count; b++)
	{
		chram[p] = c;
		fgcolram[p] = color;
		p++;
	}
}

// Set colour of single char
void set_fgcolour(char color, char x, char y)
{
	unsigned int p = (y * chram_cols) + x;
	fgcolram[p] = color;
}

// Set background colour of single char
void set_bgcolour(char color, char x, char y)
{
	unsigned int p = (y * chram_cols) + x;
	bgcolram[p] = color;
}

// Write row of consecutive chars to character RAM and colour RAM
void write_bgcol_row(char color, unsigned char x, unsigned char y, unsigned char count)
{
	unsigned int p = (y * chram_cols) + x;
	for (char b = 0; b < count; b++)
	{
		bgcolram[p] = color;
		p++;
	}
}

// Write grouped bits to character RAM
void write_bits(char bits[], char multi, unsigned char first, unsigned char length, char color, unsigned char x, unsigned char y)
{
	for (char b = first; b < first + length; b++)
	{
		write_char((b) ? asc_1 : asc_0, color, x, y - 1);
		char m = 0b00000001;
		for (char i = 0; i < 8; i++)
		{
			write_char((bits[b * multi] & m) ? asc_1 : asc_0, color, x, y);
			x++;
			m <<= 1;
		}
		x++;
	}
}

// Draw box outline with specified character
void box(unsigned char tx, unsigned char ty, unsigned char bx, unsigned char by, char c, char color)
{
	for (unsigned char x = tx; x <= bx; x++)
	{
		write_char(c, color, x, ty);
		write_char(c, color, x, by);
	}
	for (unsigned char y = ty + 1; y < by; y++)
	{
		write_char(c, color, tx, y);
		write_char(c, color, bx, y);
	}
}

// Draw UI panel
void panel(char tx, char ty, char bx, char by, char color)
{
	write_char(char_corner_round_tl, color, tx, ty);
	write_char(char_corner_round_tr, color, bx, ty);
	write_char(char_corner_round_bl, color, tx, by);
	write_char(char_corner_round_br, color, bx, by);
	for (char x = tx + 1; x < bx; x++)
	{
		write_char(char_line_h, color, x, ty);
		write_char(char_line_h, color, x, by);
	}
	for (char y = ty + 1; y < by; y++)
	{
		write_char(char_line_v, color, tx, y);
		write_char(char_line_v, color, bx, y);
	}
}

// Shaded panel
void panel_shaded(char tx, char ty, char bx, char by, char color_high, char color1, char color2)
{
	write_char(char_corner_round_tl, color_high, tx, ty);
	write_char(char_corner_round_tr, color1, bx, ty);
	write_char(char_corner_round_bl, color1, tx, by);
	write_char(char_corner_round_br, color2, bx, by);
	for (char x = tx + 1; x < bx; x++)
	{
		write_char(char_line_h, color1, x, ty);
		write_char(char_line_h, color2, x, by);
	}
	for (char y = ty + 1; y < by; y++)
	{
		write_char(char_line_v, color1, tx, y);
		write_char(char_line_v, color2, bx, y);
	}
}

void fill(char tx, char ty, char bx, char by, char c, char color)
{
	for (char y = ty; y <= by; y++)
	{
		write_char_row(c, color, tx, y, (bx - tx) + 1);
	}
}
void fill_bgcolor(char tx, char ty, char bx, char by, char bgcolor)
{
	for (char y = ty; y <= by; y++)
	{
		write_bgcol_row(bgcolor, tx, y, (bx - tx) + 1);
	}
}
void fill_bgcol(char tx, char ty, char bx, char by, char color)
{
	for (char y = ty; y <= by; y++)
	{
		write_bgcol_row(color, tx, y, bx - tx);
	}
}

// Draw page border
void page_border(char color)
{
	panel(0, 0, 39, 29, color);
}

void draw_charactermap()
{
	char c = 94;
	for (char y = 0; y < 29; y += 2)
	{
		for (char x = 0; x < 39; x += 4)
		{
			write_stringf("%d", 0xFF, x, y, c);
			write_char(c, 0, x, y + 1);
			c++;
		}
	}
}

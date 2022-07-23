/*============================================================================
	MiSTer test harness OS - Main application

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

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "sys.c"
#include "sys_custom.c"
#include "ui.c"
#include "ui_custom.c"
#include "inputtester.c"

int tp[] = {
	// Frequencies related to MIDI note numbers
	15289, 14431, 13621, 12856, 12135, 11454, 10811, 10204, // 0-o7
	9631, 9091, 8581, 8099, 7645, 7215, 6810, 6428,			// 8-15
	6067, 5727, 5405, 5102, 4816, 4545, 4290, 4050,			// 16-23
	3822, 3608, 3405, 3214, 3034, 2863, 2703, 2551,			// 24-31
	2408, 2273, 2145, 2025, 1911, 1804, 1703, 1607,			// 32-39
	1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012,			// 40-47
	956, 902, 851, 804, 758, 716, 676, 638,					// 48-55
	602, 568, 536, 506, 478, 451, 426, 402,					// 56-63
	379, 358, 338, 319, 301, 284, 268, 253,					// 64-71
	239, 225, 213, 201, 190, 179, 169, 159,					// 72-79
	150, 142, 134, 127, 119, 113, 106, 100,					// 80-87
	95, 89, 84, 80, 75, 71, 67, 63,							// 88-95
	60, 56, 53, 50, 47, 45, 42, 40,							// 96-103
	38, 36, 34, 32, 30, 28, 27, 25,							// 104-111
	24, 22, 21, 20, 19, 18, 17, 16,							// 112-119
	15, 14, 13, 13, 12, 11, 11, 10,							// 120-127
	0														// off
};

void ay_write(unsigned char addr, unsigned char data)
{
	sndram[addr] = data;
}

void ay_set_ch(unsigned char c, int i)
{
	ay_write(c * 2, tp[i] & 0xff);
	ay_write((c * 2) + 1, (tp[i] >> 8) & 0x0f);
}

unsigned char channel_on[2];
unsigned char channel_high[2];
unsigned char channel_low[2];
unsigned char channel_pos[2];
signed char channel_dir[2];
unsigned char channel_speed[2];
unsigned char channel_tick[2];

// Main entry and state machine
void main()
{

	channel_high[0] = 64;
	channel_low[0] = 60;
	channel_dir[0] = 1;
	channel_speed[0] = 10;
	channel_pos[0] = channel_low[0];

	channel_high[1] = 78;
	channel_low[1] = 74;
	channel_dir[1] = 1;
	channel_speed[1] = 15;
	channel_pos[1] = channel_low[1];

	chram_size = chram_cols * chram_rows;

	// ay_write(0x10, 0xFF);
	for (char a = 0; a < 16; a++)
	{
		ay_write(a, 0x00);
	}
	 ay_write(0x07, 0b11111000);

	while (1)
	{
		hsync = input0 & 0x80;
		vsync = input0 & 0x40;
		hblank = input0 & 0x20;
		vblank = input0 & 0x10;
		switch (state)
		{
		case STATE_START_INPUTTESTERMISTERCADE:
			start_inputtester_mistercade();
			break;
		case STATE_INPUTTESTERMISTERCADE:
			inputtester_mistercade();
			break;
		default:
			start_inputtester_mistercade();
			break;
		}

		if (VBLANK_RISING)
		{

			if (input_a && !channel_on[0])
			{
				ay_write(0x08, 0x0f);
				channel_on[0] = 1;
			}
			if (!input_a && channel_on[0])
			{
				ay_write(0x08, 0x00);
				channel_on[0] = 0;
			}
			if (input_b && !channel_on[1])
			{
				ay_write(0x09, 0x0f);
				channel_on[1] = 1;
			}
			if (!input_b && channel_on[1])
			{
				ay_write(0x09, 0x00);
				channel_on[1] = 0;
			}

			for (unsigned char c = 0; c < 2; c++)
			{
				if (channel_on[c])
				{
					channel_tick[c]++;
					if (channel_tick[c] == channel_speed[c])
					{
						channel_pos[c] += channel_dir[c];
						ay_set_ch(c, channel_pos[c]);
						if (channel_pos[c] >= channel_high[c])
						{
							channel_dir[c] = -channel_dir[c];
						}
						if (channel_pos[c] <= channel_low[c])
						{
							channel_dir[c] = -channel_dir[c];
						}
						channel_tick[c] = 0;
					}
				}
			}
		}

		hsync_last = hsync;
		vsync_last = vsync;
		hblank_last = hblank;
		vblank_last = vblank;
	}
}

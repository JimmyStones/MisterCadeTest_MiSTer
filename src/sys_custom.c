/*============================================================================
	Input Test - Custom system functions

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
#include "sys.c"

// Application states
#define STATE_START_INPUTTESTERMISTERCADE 11
#define STATE_INPUTTESTERMISTERCADE 12

#define GET_TIMER ((unsigned short)timer[1] << 8) | (unsigned char)timer[0]

// DPAD tracker
bool input_a;
bool input_a_last = 0;
bool input_b;
bool input_b_last = 0;

// Track joypad 1 directions and start for menu control
void basic_input()
{
	input_a_last = input_a;
	input_b_last = input_b;

	input_a = CHECK_BIT(joystick[0], 4);
	input_b = CHECK_BIT(joystick[0], 5);

}

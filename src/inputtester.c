/*============================================================================
    Input Test - Input test state handlers

    Author: Jim Gregory - https://github.com/JimmyStones/
    Version: 1.0
    Date: 2021-07-13

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

// Input tester variables
unsigned char joystick_last[12];

#define PAD_COUNT 2
#define BUTTON_COUNT 14

char pad_offset_x[PAD_COUNT] = {7, 7};
char pad_offset_y[PAD_COUNT] = {5, 17};
char pad_size_x = 26;
char pad_size_y = 10;
char button_symbol[BUTTON_COUNT][14] = {
    "R",
    "L",
    "D",
    "U",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "Coin",
    "Start",
    "Pause/Service",
    "Test"};
unsigned char button_x[BUTTON_COUNT] = {6, 2, 4, 4, 20, 22, 24, 20, 22, 24, 2, 2, 12, 21};
unsigned char button_y[BUTTON_COUNT] = {3, 3, 4, 2, 2, 2, 2, 4, 4, 4, 6, 8, 6, 8};
unsigned char button_hit[BUTTON_COUNT][2];
#define color_button_active 0xFF
#define color_button_inactive 0b01010010
#define color_button_hit 0b00111000

// Draw static elements for mistercase input test page
void page_inputtester_mistercade()
{
    page_frame();

    // Draw pads
    for (char j = 0; j < PAD_COUNT; j++)
    {
        write_stringf("%dUP", 0xFF, pad_offset_x[j] - 5, pad_offset_y[j] + 5, j + 1);
        panel(pad_offset_x[j], pad_offset_y[j], pad_offset_x[j] + pad_size_x, pad_offset_y[j] + pad_size_y, color_pad_outline);
    }
}

void reset_inputstates()
{
    for (char i = 0; i < 12; i++)
    {
        joystick_last[i] = 1;
    }
}

// Initialise mistercade inputtester state and draw static elements
void start_inputtester_mistercade()
{
    state = STATE_INPUTTESTERMISTERCADE;

    // Draw page
    page_inputtester_mistercade();

    // Reset last states for inputs
    reset_inputstates();
}

// Mistercade input tester state
void inputtester_mistercade()
{

    // Handle secret code detection (joypad 1 directions)
    if (HBLANK_RISING)
    {
        basic_input();
    }

    // As soon as vsync is detected start drawing screen updates
    if (VBLANK_RISING)
    {

        // Draw control pad buttons
        for (char joy = 0; joy < PAD_COUNT; joy++)
        {
            char index = joy * 4;
            char nohit = 0;
            for (char button = 0; button < BUTTON_COUNT; button++)
            {
                unsigned char active = (button < 8 ? CHECK_BIT(joystick[index], button) : CHECK_BIT(joystick[index + 1], button - 8));
                if (active)
                {
                    button_hit[button][joy] = 1;
                }
                else
                {
                    if (!button_hit[button][joy])
                    {
                        nohit = 1;
                    }
                }
                unsigned char color = active ? color_button_active : button_hit[button][joy] ? color_button_hit
                                                                                        : color_button_inactive;
                write_string(button_symbol[button], color, pad_offset_x[joy] + button_x[button], pad_offset_y[joy] + button_y[button]);
            }
            if (nohit == 0)
            {
                write_string("OK!", 0b00111000, pad_offset_x[joy] - 5, pad_offset_y[joy] + 6);
            }
        }
    }
}

unsigned short abs(signed short value)
{
    if (value < 0)
    {
        value = value * -1;
    }
    return value;
}
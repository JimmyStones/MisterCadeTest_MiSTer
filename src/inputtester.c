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

#define PAD_COUNT 4
#define BUTTON_COUNT 14

char pad_offset_x[PAD_COUNT] = {3, 21, 3, 21};
char pad_offset_y[PAD_COUNT] = {5, 5, 18, 18};
char pad_size_x = 16;
char pad_size_y = 8;
char button_symbol[BUTTON_COUNT][8] = {
    "R",
    "L",
    "D",
    "U",
    "O",
    "O",
    "O",
    "O",
    "O",
    "O",
    "O",
    "O",
    "Coin",
    "Start"};

unsigned char player_count = 4;

unsigned char button_x[BUTTON_COUNT] = {6, 2, 4, 4, 8, 10, 12, 14, 8, 10, 12, 14, 2, 10};
unsigned char button_y[BUTTON_COUNT] = {3, 3, 4, 2, 2, 2, 2, 2, 4, 4, 4, 4, 6, 6};
unsigned char button_hit[BUTTON_COUNT][PAD_COUNT];
bool pause_hit;
bool test_hit;

unsigned char pause_label_x = 8;
unsigned char pause_label_y = 15;
unsigned char test_label_x = 24;
unsigned char test_label_y = 15;

#define color_button_active 0xFF
#define color_button_inactive 0b01010010
#define color_button_hit 0b00111000

// Draw static elements for mistercase input test page
void page_inputtester_mistercade()
{
    page_frame();

    player_count = input0 & 0x8 ? 4 : 2;

    // Draw pads
    for (char j = 0; j < player_count; j++)
    {
        write_stringf("%dUP", 0xFF, pad_offset_x[j], pad_offset_y[j] - 1, j + 1);
        panel(pad_offset_x[j], pad_offset_y[j], pad_offset_x[j] + pad_size_x, pad_offset_y[j] + pad_size_y, color_pad_outline);
    }
}

// Initialise mistercade inputtester state and draw static elements
void start_inputtester_mistercade()
{
    state = STATE_INPUTTESTERMISTERCADE;

    // Draw page
    page_inputtester_mistercade();
}

// Mistercade input tester state
void inputtester_mistercade()
{

    if (HBLANK_RISING)
    {
        basic_input();
    }

    // As soon as vsync is detected start drawing screen updates
    if (VBLANK_RISING)
    {

        // Handle pause and test buttons
        bool pause_active = CHECK_BIT(joystick[1], BUTTON_COUNT - 8);
        bool test_active = CHECK_BIT(joystick[1], BUTTON_COUNT - 7);
        if (pause_active)
        {
            pause_hit = 1;
        }
        if (test_active)
        {
            test_hit = 1;
        }
        unsigned char pause_color = pause_active ? color_button_active : pause_hit ? color_button_hit
                                                                                   : color_button_inactive;
        write_string("Pause/Service", pause_color, pause_label_x, pause_label_y);
        unsigned char test_color = test_active ? color_button_active : test_hit ? color_button_hit
                                                                                : color_button_inactive;
        write_string("Test", test_color, test_label_x, test_label_y);

        // Draw control pad buttons
        for (char joy = 0; joy < player_count; joy++)
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
                write_stringf("%dUP", 0b00111000, pad_offset_x[joy], pad_offset_y[joy] - 1, joy + 1);
                write_string("OK!", 0b00111000, pad_offset_x[joy] + 14, pad_offset_y[joy] - 1);
                write_string("OK!", 0b00111000, pad_offset_x[joy] + 14, pad_offset_y[joy] - 1);
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
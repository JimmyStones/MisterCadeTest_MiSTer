`timescale 1ns / 1ps
/*============================================================================
	Input Test - Verilator emu module

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

module emu (

	input clk_sys,
	input reset,
	
	input [31:0] joystick_0,
	input [31:0] joystick_1,
	input [31:0] joystick_2,
	input [31:0] joystick_3,
	input [31:0] joystick_4,
	input [31:0] joystick_5,
	
	output [7:0] VGA_R,
	output [7:0] VGA_G,
	output [7:0] VGA_B,
	
	output VGA_HS,
	output VGA_VS,
	output VGA_HB,
	output VGA_VB,
	
	input				ioctl_download,
	input				ioctl_wr,
	input [24:0]	ioctl_addr,
	input [7:0]		ioctl_dout,
	input [7:0]		ioctl_index,
	output reg		ioctl_wait=1'b0,

	output [15:0] AUDIO_L,
	output [15:0] AUDIO_R
);

// Clock divider from JTFRAME
wire ce_pix;
/* verilator lint_off PINMISSING */
jtframe_cen24 divider
(
	.clk(clk_sys),
	.cen12(ce_pix), // <-- dodgy video speed for faster simulation, will cause bearable char map corruption
	//.cen4(ce_pix) // <-- correct video speed
);
/* verilator lint_on PINMISSING */

system system(
	.clk_sys(clk_sys),
	.ce_pix(ce_pix),
	.reset(reset | ioctl_download),
	.VGA_HS(VGA_HS),
	.VGA_VS(VGA_VS),
	.VGA_R(VGA_R),
	.VGA_G(VGA_G),
	.VGA_B(VGA_B),
	.VGA_HB(VGA_HB),
	.VGA_VB(VGA_VB),
	.dn_addr(ioctl_addr[13:0]),
	.dn_data(ioctl_dout),
	.dn_wr(ioctl_wr),
	.dn_index(ioctl_index),
	.player_count(1),
	
	.joystick({joystick_5,joystick_4,joystick_3,joystick_2,joystick_1,joystick_0}),

	.AUDIO_L(AUDIO_L),
	.AUDIO_R(AUDIO_R)
);

endmodule 


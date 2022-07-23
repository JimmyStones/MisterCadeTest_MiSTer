`timescale 1ns / 1ps
/*============================================================================
	Aznable (custom 8-bit computer system) - System module

	Author: Jim Gregory - https://github.com/JimmyStones/
	Version: 1.0
	Date: 2021-07-03

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

module system (
	input			clk_sys,
	input 			ce_pix,
	input			reset,
	input [13:0]	dn_addr,
	input			dn_wr,
	input [7:0]		dn_data,
	input [7:0]		dn_index,

	input 			player_count,

	// 6 devices, 32 buttons each
	input [191:0]	joystick,
	
	output			VGA_HS,
	output			VGA_VS,
	output [7:0]	VGA_R,
	output [7:0]	VGA_G,
	output [7:0]	VGA_B,
	output			VGA_HB,
	output			VGA_VB,
	output	[15:0]	AUDIO_L,
	output	[15:0]	AUDIO_R
);

localparam [8:0] VGA_WIDTH = 9'd320;
localparam [8:0] VGA_HEIGHT = 9'd240;

wire _hb;
wire _vb;
assign VGA_HB = ~_hb;
assign VGA_VB = ~_vb;

wire [8:0] hcnt;
wire [8:0] vcnt;

// Display timing module from JTFRAME
jtframe_vtimer #(
	.HB_START(VGA_WIDTH - 1'b1),
	.VB_START(VGA_HEIGHT- 1'b1)
) vtimer 
(
	.clk(clk_sys),
	.pxl_cen(ce_pix),
	.V(vcnt),
	.H(hcnt),
	.Hinit(),
	.Vinit(),
	.LHBL(_hb),
	.LVBL(_vb),
	.HS(VGA_HS),
	.VS(VGA_VS)
);

// Millisecond timer
reg  [15:0]	timer;
reg  [14:0]	timer_divider = 15'd0;

always @(posedge clk_sys) 
begin
	if(timer_cs == 1'b1 && cpu_wr_n == 1'b0)
	begin
		timer <= 16'd0;
		timer_divider <= 15'd0;
	end
	else
	begin
		if(timer_divider==15'd24000)
		begin
			timer <= timer + 16'd1;
			timer_divider <= 15'd0;
		end
		else
		begin
			timer_divider <= timer_divider + 15'd1;
	 	end
	end
end

// Character map
wire [3:0] chpos_x = 4'd7 - hcnt[2:0];
wire [2:0] chpos_y = vcnt[2:0];
wire [5:0] chram_x = hcnt[8:3];
wire [5:0] chram_y = vcnt[8:3];
wire [11:0] chram_addr = {chram_y, chram_x};
wire [11:0] chrom_addr = {1'b0, chmap_data_out[7:0], chpos_y};
wire chpixel = chrom_data_out[chpos_x[2:0]];

// Background gradients
reg [7:0] bgcol_r;
reg [7:0] bgcol_g;
reg [7:0] bgcol_b;

wire [9:0] hcnt_o = hcnt - 9'd32;

always @(posedge clk_sys) begin
	bgcol_r <= 8'b0;
	bgcol_g <= 8'b0;
	bgcol_b <= 8'b0;
	if(hcnt >= 9'd32 && hcnt <= 9'd294)
	begin
		if(vcnt >= 9'd24 && vcnt < 9'd72)
		begin
			bgcol_r <= hcnt_o[7:0];
		end
		if(vcnt >= 9'd78 && vcnt < 9'd124)
		begin
			bgcol_g <= hcnt_o[7:0];
		end
		if(vcnt >= 9'd130 && vcnt < 9'd178)
		begin
			bgcol_b <= hcnt_o[7:0];
		end
		if(vcnt >= 9'd184 && vcnt < 9'd232)
		begin
			bgcol_r <= hcnt_o[7:0];
			bgcol_g <= hcnt_o[7:0];
			bgcol_b <= hcnt_o[7:0];
		end
	end
end

 

// RGB mixer
wire [2:0] r_temp = chpixel ? fgcolram_data_out[2:0] : bgcolram_data_out[2:0];
wire [2:0] g_temp = chpixel ? fgcolram_data_out[5:3] : bgcolram_data_out[5:3];
wire [1:0] b_temp = chpixel ? fgcolram_data_out[7:6] : bgcolram_data_out[7:6];

// Convert RGb to 24bpp
assign VGA_R = fgcolram_data_out > 8'b0 ? {{2{r_temp}},2'b0} : bgcol_r;
assign VGA_G = fgcolram_data_out > 8'b0 ? {{2{g_temp}},2'b0} : bgcol_g;
assign VGA_B = fgcolram_data_out > 8'b0 ? {{3{b_temp}},2'b0} : bgcol_b;

// CPU control signals
wire [15:0] cpu_addr;
wire [7:0] cpu_din;
wire [7:0] cpu_dout;
wire cpu_rd_n;
wire cpu_wr_n;
wire cpu_mreq_n;

// include Z80 CPU
tv80s T80x  (
	.reset_n   ( !reset ),
	.clk       ( clk_sys ),
	.wait_n    ( 1'b1 ),
	.int_n     ( 1'b1 ),
	.nmi_n     ( 1'b1 ),
	.busrq_n   ( 1'b1 ),
	.mreq_n    ( cpu_mreq_n ),
	.rd_n      ( cpu_rd_n ), 
	.wr_n      ( cpu_wr_n ),
	.A         ( cpu_addr ),
	.di        ( cpu_din ),
	.dout      ( cpu_dout ),
	.m1_n      (),
	.iorq_n    (),
	.rfsh_n    (),
	.halt_n    (),
	.busak_n   ()
  );

// RAM data to CPU
wire [7:0] pgrom_data_out;
wire [7:0] chrom_data_out;
wire [7:0] wkram_data_out;
wire [7:0] chram_data_out;
wire [7:0] fgcolram_data_out;
wire [7:0] bgcolram_data_out;

// RAM data to GFX
wire [7:0] chmap_data_out;

// Hardware inputs
wire [7:0] in0_data_out = {VGA_HS, VGA_VS,VGA_HB, VGA_VB, player_count, 3'b000};
wire [7:0] joystick_data_out = joystick[{cpu_addr[4:0],3'd0} +: 8];
wire [7:0] timer_data_out = timer[{cpu_addr[0],3'd0} +: 8];

// CPU address decodes
wire pgrom_cs = cpu_addr[15:14] == 2'b00;
wire chram_cs = cpu_addr[15:11] == 5'b10000;
wire fgcolram_cs = cpu_addr[15:11] == 5'b10001;
wire bgcolram_cs = cpu_addr[15:11] == 5'b10010;
wire wkram_cs = cpu_addr[15:14] == 2'b11;
wire in0_cs = cpu_addr == 16'h6000;
wire joystick_cs = cpu_addr[15:8] == 8'b01110000;
wire timer_cs = cpu_addr[15:8] == 8'b01111000;
wire snd_cs = cpu_addr[15:8] == 8'b10110000;
wire snd_reset_cs = cpu_addr == 16'b1011000000010000;

// CPU data mux
assign cpu_din = pgrom_cs ? pgrom_data_out :
				 wkram_cs ? wkram_data_out :
				 chram_cs ? chram_data_out :
				 fgcolram_cs ? fgcolram_data_out :
				 bgcolram_cs ? bgcolram_data_out :
				 in0_cs ? in0_data_out :
				 joystick_cs ? joystick_data_out :
				 timer_cs ? timer_data_out :
				 8'b00000000;

// Rom upload write enables
wire pgrom_wr = dn_wr && dn_index == 8'b0;
wire chrom_wr = dn_wr && dn_index == 8'b1;

// Ram write enables
wire wkram_wr = !cpu_wr_n && wkram_cs;
wire chram_wr = !cpu_wr_n && chram_cs;
wire fgcolram_wr = !cpu_wr_n && fgcolram_cs;
wire bgcolram_wr = !cpu_wr_n && bgcolram_cs;


// MEMORY
// ------
// Program ROM - 0x0000 - 0x3FFF (0x4000 / 16384 bytes)
dpram #(14,8, "rom.hex") pgrom
(
	.clock_a(clk_sys),
	.address_a(cpu_addr[13:0]),
	.wren_a(1'b0),
	.data_a(),
	.q_a(pgrom_data_out),

	.clock_b(clk_sys),
	.address_b(dn_addr[13:0]),
	.wren_b(pgrom_wr),
	.data_b(dn_data),
	.q_b()
);

// Char ROM - 0x4000 - 0x47FF (0x0400 / 2048 bytes)
dpram #(11,8, "font.hex") chrom
(
	.clock_a(clk_sys),
	.address_a(chrom_addr[10:0]),
	.wren_a(1'b0),
	.data_a(),
	.q_a(chrom_data_out),

	.clock_b(clk_sys),
	.address_b(dn_addr[10:0]),
	.wren_b(chrom_wr),
	.data_b(dn_data),
	.q_b()
);


// Char RAM - 0x8000 - 0x87FF (0x0800 / 2048 bytes)
dpram #(11,8) chram
(
	.clock_a(clk_sys),
	.address_a(cpu_addr[10:0]),
	.wren_a(chram_wr),
	.data_a(cpu_dout),
	.q_a(chram_data_out),

	.clock_b(clk_sys),
	.address_b(chram_addr[10:0]),
	.wren_b(1'b0),
	.data_b(),
	.q_b(chmap_data_out)
);

// Char foreground color RAM - 0x8800 - 0x8FFF (0x0800 / 2048 bytes)
dpram #(11,8) fgcolram
(
	.clock_a(clk_sys),
	.address_a(cpu_addr[10:0]),
	.wren_a(fgcolram_wr),
	.data_a(cpu_dout),
	.q_a(),

	.clock_b(clk_sys),
	.address_b(chram_addr[10:0]),
	.wren_b(1'b0),
	.data_b(),
	.q_b(fgcolram_data_out)
);

// Char background color RAM - 0x9000 - 0x97FF (0x0800 / 2048 bytes)
dpram #(11,8) bgcolram
(
	.clock_a(clk_sys),
	.address_a(cpu_addr[10:0]),
	.wren_a(bgcolram_wr),
	.data_a(cpu_dout),
	.q_a(),

	.clock_b(clk_sys),
	.address_b(chram_addr[10:0]),
	.wren_b(1'b0),
	.data_b(),
	.q_b(bgcolram_data_out)
);

// Work RAM - 0xC000 - 0xFFFF (0x4000 / 16384 bytes)
spram #(14,8) wkram
(
	.clock(clk_sys),
	.address(cpu_addr[13:0]),
	.wren(wkram_wr),
	.data(cpu_dout),
	.q(wkram_data_out)
);



/// ---- sound


reg clk_2_en;
always @(posedge clk_sys) begin
	reg [3:0] cnt;
	clk_2_en <= (cnt == 0);
	cnt <= cnt + 1'd1;
	if(cnt>= 4'd12) cnt <= 4'd0;
end

wire [3:0] snd_addr = cpu_addr[3:0];
wire [7:0] snd_data_out;

wire [9:0] audio_out;
wire [9:0] audio_out_a;
wire [9:0] audio_out_b;

jt49 jt49 (
	.clk         ( clk_sys ),
	.clk_en      ( clk_2_en ),
	.rst_n       ( ~(reset | (snd_reset_cs & ~cpu_wr_n)) ),
	.addr        ( snd_addr ),
	.din         ( cpu_dout ),
	.dout        ( snd_data_out ),
	.sound       ( audio_out ),
	.sample(),
	.A(audio_out_a),
	.B(audio_out_b),
	.C(),
	.sel(1'b1),
	.cs_n(1'b0),
	.wr_n(~(snd_cs && ~cpu_wr_n)),
	.IOA_in(),
	.IOA_out(),
	.IOB_in(),
	.IOB_out()
);

// audio output processing
//assign AUDIO_L = {1'b0, audio_out[9:5]};
//assign AUDIO_R = {1'b0, audio_out[9:5]};
assign AUDIO_L =  { 1'b0, audio_out_a[9:0],5'd0};
assign AUDIO_R = { 1'b0, audio_out_b[9:0],5'd0};
// assign AUDIO_L = audio_out[9:0];
// assign AUDIO_R = audio_out[9:0];


always @(posedge clk_sys) begin
	//if((snd_reset_cs && ~cpu_wr_n)) $display("SND RESET");
	// if(audio_out>16'd0) $display("audio_out %b", audio_out);
	// if(audio_out_a>16'd0) $display("audio_out_a %b", audio_out_a);
	// if(audio_out_b>16'd0) $display("audio_out_b %b", audio_out_b);
	if(snd_cs && ~cpu_wr_n) $display("SND_CS : %x %x", snd_addr, cpu_dout);
end

endmodule

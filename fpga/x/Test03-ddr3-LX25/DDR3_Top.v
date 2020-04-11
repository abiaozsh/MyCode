`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
// WEB:
// BBS:
// Create Date:    09:34:12 07/20/2016 
// Design Name: 	 DDR3_TEST
// Module Name:    DDR3_Top
// Project Name: 	 DDR3_Top
// Target Devices: XC6SLX16-FTG256/XC6SLX25-FTG256 qm_ddr3
// Tool versions:  ISE14.7
// Description: 	 DDR3 memory Test
// Revision: 		 V1.0
// Additional Comments: 
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
module DDR3_Top#
(
	parameter C3_NUM_DQ_PINS				= 16,
	parameter C3_MEM_ADDR_WIDTH       	= 15,  
   parameter C3_MEM_BANKADDR_WIDTH   	= 3,
	parameter C3_P0_MASK_SIZE           = 16,	
	parameter C3_P0_DATA_PORT_SIZE		= 128	
 )
(
	input sys_rst_i,
	input sys_clk_i,
  // output                                           error,
   inout  [C3_NUM_DQ_PINS-1:0]                      mcb3_dram_dq,
   output [C3_MEM_ADDR_WIDTH-1:0]                   mcb3_dram_a,
   output [C3_MEM_BANKADDR_WIDTH-1:0]               mcb3_dram_ba,
   output                                           mcb3_dram_ras_n,
   output                                           mcb3_dram_cas_n,
   output                                           mcb3_dram_we_n,
   output                                           mcb3_dram_odt,
   output                                           mcb3_dram_reset_n,
   output                                           mcb3_dram_cke,
   output                                           mcb3_dram_dm,
   inout                                            mcb3_dram_udqs,
   inout                                            mcb3_dram_udqs_n,
   inout                                            mcb3_rzq,
   inout                                            mcb3_zio,
   output                                           mcb3_dram_udm,
   inout                                            mcb3_dram_dqs,
   inout                                            mcb3_dram_dqs_n,
   output                                           mcb3_dram_ck,
   output                                           mcb3_dram_ck_n,
 
 

	 input A14,
	 input C13,
	 input B12,
	 input C11,
	 input B10,
	 input C9 ,
	 input B8 ,
	 input C7 ,
	 input B6 ,
	 input B5 ,
	 input E10,
	 input E11,
	 input F9 ,
	 input C8 ,
	 input E7 ,
	 input F7 ,
	 input D6 ,
	 input M7 ,
	 input N8 ,
	 input P9 ,
	 input T5 ,
	 input T6 ,
	 input N9 ,
	 input L8 ,
	 input L10,
	 output P12,
	 input R9Led,


	 input B14,
	 input A13,
	 input A12,
	 input A11,
	 input A9 ,
	 input A8 ,
	 input A7 ,
	 input A6 ,
	 input A5 ,
	 input A4 ,
	 input C10,
	 input F10,
	 input D9 ,
	 input D8 ,
	 input E6 ,
	 input C6 ,
	 input N6 ,
	 input P6 ,
	 input L7 ,
	 input T4 ,
	 input R5 ,
	 input T7 ,
	 input M9 ,
	 input M10,
	 input P11,
	 input M11,
	 input T9Led,

    output E12,
	 output B15,
	 output C15,
	 output D14,
	 output E15,
	 output F15,
	 output G11,
	 output F14,
	 input G16,
	 input H15,
	 input G12,
	 input H13,
	 input J14,
	 input J11,
	 input K14,
	 input K15,
	 input L16,
	 input K11,
	 input M15,
	 input N14,
	 input M13,
	 input L12,
	 input P15,
	 input R15,
	 input R14,
	 input T13,
	 input T12,

	 input E13,
	 input B16,
	 input C16,
	 input D16,
	 input E16,
	 input F16,
	 input F12,
	 input F13,
	 input G14,
	 input H16,
	 input H11,
	 input H14,
	 input J16,
	 input J12,
	 input J13,
	 input K16,
	 input L14,
	 input K12,
	 input M16,
	 input N16,
	 input M14,
	 input L13,
	 input P16,
	 input R16,
	 input T15,
	 input T14,
	 input R12	 
	 	
    );

parameter 		DLY_CNT = 32'd50000000;
parameter 		HALF_DLY_CNT = 32'd25000000;
reg 				r_led;
reg 				[31:0]count;

//ddr3
wire				mcb3_cmd_empty;
wire				mcb3_cmd_full;

wire				mcb3_wr_full;
wire				mcb3_wr_empty;
wire[6:0]		mcb3_wr_count;
wire				mcb3_wr_underrun;
wire				mcb3_wr_error;

wire				mcb3_rd_full;
wire[6:0]		mcb3_rd_count;
wire				mcb3_rd_overflow;
wire				mcb3_rd_error;

wire calib_done;
wire clk_50m;


wire c3_p0_cmd_en;
wire cmd_rw;
wire [5:0] c3_p0_cmd_bl;
wire [29:0] c3_p0_cmd_byte_addr;

wire c3_p0_wr_en;
wire [3:0] c3_p0_wr_mask;
wire [31:0] c3_p0_wr_data;

wire c3_p0_rd_en;
wire [31:0] c3_p0_rd_data;
wire c3_p0_rd_empty;
(*KEEP = "TRUE" *)  wire c3_calib_done;
//assign error = ~(c1_error | c3_error);
assign calib_done = ~(c3_calib_done);
wire 	 calib_rst;
assign calib_rst=c3_calib_done;//

wire rst_ddr_n;
wire clk_ddr;



qm_ddr3 # (
    .DEBUG_EN(0),
    .C3_P0_MASK_SIZE(16),
    .C3_P0_DATA_PORT_SIZE(128),
    .C3_MEMCLK_PERIOD(2500),
    .C3_CALIB_SOFT_IP("TRUE"),
    .C3_SIMULATION("FALSE"),
    .C3_RST_ACT_LOW(0),
    .C3_INPUT_CLK_TYPE("SINGLE_ENDED"),
    .C3_MEM_ADDR_ORDER("ROW_BANK_COLUMN"),
    .C3_NUM_DQ_PINS(16),
    .C3_MEM_ADDR_WIDTH(15),
    .C3_MEM_BANKADDR_WIDTH(3)
)
u_ddr3_mig (
  .c3_sys_clk             (clk_ddr),
  .c3_sys_rst_i           (~rst_ddr_n),                        
  .mcb3_dram_dq           (mcb3_dram_dq),  
  .mcb3_dram_a            (mcb3_dram_a),  
  .mcb3_dram_ba           (mcb3_dram_ba),
  .mcb3_dram_ras_n        (mcb3_dram_ras_n),                        
  .mcb3_dram_cas_n        (mcb3_dram_cas_n),                        
  .mcb3_dram_we_n         (mcb3_dram_we_n),                          
  .mcb3_dram_odt          (mcb3_dram_odt),
  .mcb3_dram_cke          (mcb3_dram_cke),                          
  .mcb3_dram_ck           (mcb3_dram_ck),                          
  .mcb3_dram_ck_n         (mcb3_dram_ck_n),       
  .mcb3_dram_dqs          (mcb3_dram_dqs),                          
  .mcb3_dram_dqs_n        (mcb3_dram_dqs_n),
  .mcb3_dram_udqs         (mcb3_dram_udqs),    // for X16 parts                        
  .mcb3_dram_udqs_n       (mcb3_dram_udqs_n),  // for X16 parts
  .mcb3_dram_udm          (mcb3_dram_udm),     // for X16 parts
  .mcb3_dram_dm           (mcb3_dram_dm),
  .mcb3_dram_reset_n      (mcb3_dram_reset_n),
  .c3_clk0		        	  (c3_clk0),
  .c3_rst0		        	  (c3_rst0),
  .c3_calib_done    		  (c3_calib_done),
  .mcb3_rzq               (mcb3_rzq),    
  .mcb3_zio               (mcb3_zio),
	
   .c3_p0_cmd_clk                          (clk_50m),
   .c3_p0_cmd_en                           (c3_p0_cmd_en),
   .c3_p0_cmd_instr                        ({2'b0,cmd_rw}),
   .c3_p0_cmd_bl                           (c3_p0_cmd_bl),
   .c3_p0_cmd_byte_addr                    (c3_p0_cmd_byte_addr),
   .c3_p0_cmd_empty                        (mcb3_cmd_empty),
   .c3_p0_cmd_full                         (mcb3_cmd_full),
   .c3_p0_wr_clk                           (clk_50m),
   .c3_p0_wr_en                            (mcb3_wr_en),
   .c3_p0_wr_mask                          (c3_p0_wr_en),
   .c3_p0_wr_data                          (c3_p0_wr_data),
   .c3_p0_wr_full                          (mcb3_wr_full),
   .c3_p0_wr_empty                         (mcb3_wr_empty),
   .c3_p0_wr_count                         (mcb3_wr_count),
   .c3_p0_wr_underrun                      (mcb3_wr_underrun),
   .c3_p0_wr_error                         (mcb3_wr_error),
   .c3_p0_rd_clk                           (clk_50m),
   .c3_p0_rd_en                            (c3_p0_rd_en),
   .c3_p0_rd_data                          (c3_p0_rd_data),
   .c3_p0_rd_full                          (mcb3_rd_full),
   .c3_p0_rd_empty                         (c3_p0_rd_empty),
   .c3_p0_rd_count                         (mcb3_rd_count),
   .c3_p0_rd_overflow                      (mcb3_rd_overflow),
   .c3_p0_rd_error                         (mcb3_rd_error)
	
	/*
      input		c3_p1_cmd_clk,
      input		c3_p1_cmd_en,
      input [2:0]	c3_p1_cmd_instr,
      input [5:0]	c3_p1_cmd_bl,
      input [29:0]	c3_p1_cmd_byte_addr,
      output		c3_p1_cmd_empty,
      output		c3_p1_cmd_full,
      input		c3_p1_wr_clk,
      input		c3_p1_wr_en,
      input [C3_P1_MASK_SIZE - 1:0]	c3_p1_wr_mask,
      input [C3_P1_DATA_PORT_SIZE - 1:0]	c3_p1_wr_data,
      output		c3_p1_wr_full,
      output		c3_p1_wr_empty,
      output [6:0]	c3_p1_wr_count,
      output		c3_p1_wr_underrun,
      output		c3_p1_wr_error,
      input		c3_p1_rd_clk,
      input		c3_p1_rd_en,
      output [C3_P1_DATA_PORT_SIZE - 1:0]	c3_p1_rd_data,
      output		c3_p1_rd_full,
      output		c3_p1_rd_empty,
      output [6:0]	c3_p1_rd_count,
      output		c3_p1_rd_overflow,
      output		c3_p1_rd_error,
      input		c3_p2_cmd_clk,
      input		c3_p2_cmd_en,
      input [2:0]	c3_p2_cmd_instr,
      input [5:0]	c3_p2_cmd_bl,
      input [29:0]	c3_p2_cmd_byte_addr,
      output		c3_p2_cmd_empty,
      output		c3_p2_cmd_full,
      input		c3_p2_rd_clk,
      input		c3_p2_rd_en,
      output [31:0]	c3_p2_rd_data,
      output		c3_p2_rd_full,
      output		c3_p2_rd_empty,
      output [6:0]	c3_p2_rd_count,
      output		c3_p2_rd_overflow,
      output		c3_p2_rd_error,
      input		c3_p3_cmd_clk,
      input		c3_p3_cmd_en,
      input [2:0]	c3_p3_cmd_instr,
      input [5:0]	c3_p3_cmd_bl,
      input [29:0]	c3_p3_cmd_byte_addr,
      output		c3_p3_cmd_empty,
      output		c3_p3_cmd_full,
      input		c3_p3_rd_clk,
      input		c3_p3_rd_en,
      output [31:0]	c3_p3_rd_data,
      output		c3_p3_rd_full,
      output		c3_p3_rd_empty,
      output [6:0]	c3_p3_rd_count,
      output		c3_p3_rd_overflow,
      output		c3_p3_rd_error,
      input		c3_p4_cmd_clk,
      input		c3_p4_cmd_en,
      input [2:0]	c3_p4_cmd_instr,
      input [5:0]	c3_p4_cmd_bl,
      input [29:0]	c3_p4_cmd_byte_addr,
      output		c3_p4_cmd_empty,
      output		c3_p4_cmd_full,
      input		c3_p4_wr_clk,
      input		c3_p4_wr_en,
      input [3:0]	c3_p4_wr_mask,
      input [31:0]	c3_p4_wr_data,
      output		c3_p4_wr_full,
      output		c3_p4_wr_empty,
      output [6:0]	c3_p4_wr_count,
      output		c3_p4_wr_underrun,
      output		c3_p4_wr_error,
      input		c3_p5_cmd_clk,
      input		c3_p5_cmd_en,
      input [2:0]	c3_p5_cmd_instr,
      input [5:0]	c3_p5_cmd_bl,
      input [29:0]	c3_p5_cmd_byte_addr,
      output		c3_p5_cmd_empty,
      output		c3_p5_cmd_full,
      input		c3_p5_wr_clk,
      input		c3_p5_wr_en,
      input [3:0]	c3_p5_wr_mask,
      input [31:0]	c3_p5_wr_data,
      output		c3_p5_wr_full,
      output		c3_p5_wr_empty,
      output [6:0]	c3_p5_wr_count,
      output		c3_p5_wr_underrun,
      output		c3_p5_wr_error
		*/	
	
);
//wire led_1;

clk_rst_gen #(.WDOG_CALC_INIT_WIDTH(26)) 
u_pll (
    .sys_clk(sys_clk_i),
    .sys_rst_n(sys_rst_i), 
    .clk_ddr(clk_ddr),
	 .clk_50m(clk_50m),
    .clk_100m(clk_100m),
    .calc_done(calib_rst),
    .rst_ddr_n(rst_ddr_n)
);


wire segled_clk;
wire segled_dat;
wire segled_str;


wire [7:0] seg_data0;
wire [7:0] seg_data1;
wire [7:0] seg_data2;
wire [7:0] seg_data3;
seg_led_hex595 ins_seg_led_hex595(
  .sys_clk(clk_50m), 
  .sys_rst_n(sys_rst_i),
 
  .clk(segled_clk),
  .dat(segled_dat),
  .str(segled_str),
 
  .data0(seg_data0),
  .data1(seg_data1),
  .data2(seg_data2),
  .data3(seg_data3)
);


wire rxd = R9Led;
assign P12 = txd;
wire txd;

assign E12 = debug8[0];
assign B15 = debug8[1];
assign C15 = debug8[2];
assign D14 = debug8[3];
assign E15 = debug8[4];
assign F15 = debug8[5];
assign G11 = debug8[6];
assign F14 = debug8[7];

wire [7:0] debug8;
wire [31:0] debug32;


uart_mcu uart_mcu_inst(
 .clk(clk_50m), 
 .reset_n(sys_rst_i),

 .uart_txd(txd),
 .uart_rxd(rxd),
 
 .debug8(debug8),
 .debug32(debug32),

.c3_p0_cmd_en(c3_p0_cmd_en),                   //output reg 
.cmd_rw(cmd_rw),                         //output reg 
.c3_p0_cmd_bl(c3_p0_cmd_bl),             //[5:0] output reg 
.c3_p0_cmd_byte_addr(c3_p0_cmd_byte_addr),     //[29:0] output reg 
                                
.c3_p0_wr_en(c3_p0_wr_en),                    //output reg 
.c3_p0_wr_mask(c3_p0_wr_mask),            //[3:0] output reg 
.c3_p0_wr_data(c3_p0_wr_data),           //[31:0] output reg 
                                
.c3_p0_rd_en(c3_p0_rd_en),                    //output reg 
.c3_p0_rd_data(c3_p0_rd_data),           //[31:0] input wire 
.c3_p0_rd_empty(c3_p0_rd_empty)                  //input wire 

  );

endmodule

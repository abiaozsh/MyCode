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
 module DDR3_Top
(
    input         sys_clk_i,
 
    inout  [15:0] mcb3_dram_dq,
    output [14:0] mcb3_dram_a,
    output [2:0]  mcb3_dram_ba,
    output        mcb3_dram_ras_n,
    output        mcb3_dram_cas_n,
    output        mcb3_dram_we_n,
    output        mcb3_dram_odt,
    output        mcb3_dram_reset_n,
    output        mcb3_dram_cke,
    output        mcb3_dram_dm,
    inout         mcb3_dram_udqs,
    inout         mcb3_dram_udqs_n,
    inout         mcb3_rzq,
    inout         mcb3_zio,
    output        mcb3_dram_udm,
    inout         mcb3_dram_dqs,
    inout         mcb3_dram_dqs_n,
    output        mcb3_dram_ck,
    output        mcb3_dram_ck_n,
 
  input key0,
  input key1,

//左左
	 input A14,
	 input C13,
	 input B12,
	 input C11,
	 output B10,
	 output C9 ,
	 output B8 ,
	 output C7 ,
	 output B6 ,
	 output B5 ,
	 output E10,
	 output E11,
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
	 input P12,
	 input R9Led,

//左中
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
	 input R5 ,//segled
	 input T7 ,//segled
	 input M9 ,//segled
	 input M10,//rx1
	 output P11,//tx1
	 input M11,//rx0
	 output T9Led,//tx0

//右中
    output E12,
	 output B15,
	 output C15,
	 output D14,
	 output E15,
	 output F15,
	 output G11,
	 input F14,
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

//右右
	 output E13,
	 output B16,
	 output C16,
	 output D16,
	 output E16,
	 output F16,
	 output F12,
	 output F13,
	 output G14,
	 output H16,
	 output H11,
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

wire sys_rst_i = key0;

//ddr3
//wire				mcb3_cmd_empty;
//wire				mcb3_wr_full;
//wire				mcb3_wr_empty;
//wire[6:0]		mcb3_wr_count;
//wire				mcb3_wr_underrun;
//wire				mcb3_wr_error;
//wire				mcb3_rd_full;
//wire[6:0]		mcb3_rd_count;
//wire				mcb3_rd_overflow;
//wire				mcb3_rd_error;

wire calib_done;
wire clk_50m;


wire          c3_p0_cmd_en;
wire          c3_p0_cmd_full;
wire          c3_p0_cmd_rw;
wire [5:0]    c3_p0_cmd_bl;
wire [29:0]   c3_p0_cmd_byte_addr;
wire          c3_p0_wr_en;
wire          c3_p0_wr_full;
wire [3:0]    c3_p0_wr_mask;
wire [31:0]   c3_p0_wr_data;
wire          c3_p0_rd_en;
wire [31:0]   c3_p0_rd_data;
wire          c3_p0_rd_empty;


wire          c3_p1_cmd_en;
wire          c3_p1_cmd_full;
wire          c3_p1_cmd_rw;
wire [5:0]    c3_p1_cmd_bl;
wire [29:0]   c3_p1_cmd_byte_addr;
wire          c3_p1_wr_en=0;
wire          c3_p1_wr_full;
wire [3:0]    c3_p1_wr_mask;
wire [31:0]   c3_p1_wr_data;
wire          c3_p1_rd_en;
wire [31:0]   c3_p1_rd_data;
wire          c3_p1_rd_empty;
wire          c3_p1_rd_full;
wire          c3_p1_rd_overflow;




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
  .c3_p0_cmd_instr                        ({2'b0,c3_p0_cmd_rw}),
  .c3_p0_cmd_bl                           (c3_p0_cmd_bl),
  .c3_p0_cmd_byte_addr                    (c3_p0_cmd_byte_addr),
  //.c3_p0_cmd_empty                        (mcb3_cmd_empty),
  .c3_p0_cmd_full                         (c3_p0_cmd_full),
  .c3_p0_wr_clk                           (clk_50m),
  .c3_p0_wr_en                            (c3_p0_wr_en),
  .c3_p0_wr_mask                          (c3_p0_wr_mask),
  .c3_p0_wr_data                          (c3_p0_wr_data),
  .c3_p0_wr_full                          (c3_p0_wr_full),
  //.c3_p0_wr_empty                         (mcb3_wr_empty),
  //.c3_p0_wr_count                         (mcb3_wr_count),
  //.c3_p0_wr_underrun                      (mcb3_wr_underrun),
  //.c3_p0_wr_error                         (mcb3_wr_error),
  .c3_p0_rd_clk                           (clk_50m),
  .c3_p0_rd_en                            (c3_p0_rd_en),
  .c3_p0_rd_data                          (c3_p0_rd_data),
  //.c3_p0_rd_full                          (mcb3_rd_full),
  .c3_p0_rd_empty                         (c3_p0_rd_empty),
  //.c3_p0_rd_count                         (mcb3_rd_count),
  //.c3_p0_rd_overflow                      (mcb3_rd_overflow),
  //.c3_p0_rd_error                         (mcb3_rd_error),



  .c3_p1_cmd_clk                          (clk_100m),
  .c3_p1_cmd_en                           (c3_p1_cmd_en),
  .c3_p1_cmd_instr                        ({2'b0,c3_p1_cmd_rw}),
  .c3_p1_cmd_bl                           (c3_p1_cmd_bl),
  .c3_p1_cmd_byte_addr                    (c3_p1_cmd_byte_addr),
  //.c3_p1_cmd_empty                        (mcb3_cmd_empty),
  .c3_p1_cmd_full                         (c3_p1_cmd_full),
  .c3_p1_wr_clk                           (clk_100m),
  .c3_p1_wr_en                            (c3_p1_wr_en),
  .c3_p1_wr_mask                          (c3_p1_wr_mask),
  .c3_p1_wr_data                          (c3_p1_wr_data),
  .c3_p1_wr_full                          (c3_p1_wr_full),
  //.c3_p1_wr_empty                         (mcb3_wr_empty),
  //.c3_p1_wr_count                         (mcb3_wr_count),
  //.c3_p1_wr_underrun                      (mcb3_wr_underrun),
  //.c3_p1_wr_error                         (mcb3_wr_error),
  .c3_p1_rd_clk                           (clk_100m),
  .c3_p1_rd_en                            (c3_p1_rd_en),
  .c3_p1_rd_data                          (c3_p1_rd_data),
  .c3_p1_rd_full                          (c3_p1_rd_full),
  .c3_p1_rd_empty                         (c3_p1_rd_empty),
  //.c3_p1_rd_count                         (mcb3_rd_count),
  .c3_p1_rd_overflow                      (c3_p1_rd_overflow)
  //.c3_p1_rd_error                         (mcb3_rd_error),

   
   
   
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

wire vga_clk_25M;
wire vga_clk_65M;

clk_rst_gen #(.WDOG_CALC_INIT_WIDTH(26)) 
u_pll (
    .sys_clk(sys_clk_i),
    .sys_rst_n(sys_rst_i), 
    .clk_ddr(clk_ddr),
	 .clk_50m(clk_50m),
    .clk_100m(clk_100m),
    .calc_done(calib_rst),
    .rst_ddr_n(rst_ddr_n),
	 .vga_clk_25M(vga_clk_25M),
	 .vga_clk_65M(vga_clk_65M)
);


reg delay_rst;

reg [31:0] delay;
always@(posedge clk_50m or negedge sys_rst_i) begin // sdram 主控
  if(!sys_rst_i) begin
    delay_rst <= 0;
  end else begin
    if(delay==5*1000*1000)begin
      delay_rst <= 1;
    end else begin
      delay<=delay+1'b1;
    end
  end
end



wire segled_clk;
wire segled_dat;
wire segled_str;


wire [7:0] seg_data0;
wire [7:0] seg_data1;
wire [7:0] seg_data2;
wire [7:0] seg_data3;
seg_led_hex595 ins_seg_led_hex595(
  .sys_clk(clk_50m), 
  .sys_rst_n(delay_rst),
 
  .clk(segled_clk),
  .dat(segled_dat),
  .str(segled_str),
 
  .data0(seg_data0),
  .data1(seg_data1),
  .data2(seg_data2),
  .data3(seg_data3)
);


wire rx1 = M10;
assign P11 = tx1;
wire tx1 = 1'bz;

wire rx0 = M11;
assign T9Led = tx0;
wire tx0;

wire [7:0] vga_debug8;

assign B10 = c3_p1_rd_full;//vga_debug8[0];
assign C9  = c3_p1_rd_overflow;//vga_debug8[1];
assign B8  = vga_debug8[2];
assign C7  = vga_debug8[3];
assign B6  = vga_debug8[4];
assign B5  = vga_debug8[5];
assign E10 = vga_debug8[6];
assign E11 = vga_debug8[7];

//wire          c3_p1_rd_full;
//wire          c3_p1_rd_overflow;

wire [7:0] debug8;
wire [31:0] debug32;
wire [31:0] vga_debug32;
uart_mcu uart_mcu_inst(
 .clk(clk_50m), 
 .reset_n(delay_rst),

 .uart_txd(tx0),
 .uart_rxd(rx0),
 
 .debug8(debug8),
 .debug32(debug32),
 .debugin32(vga_debug32),

.c3_p0_cmd_en(c3_p0_cmd_en),                   //output reg 
.c3_p0_cmd_full(c3_p0_cmd_full),
.c3_p0_cmd_rw(c3_p0_cmd_rw),                         //output reg 
.c3_p0_cmd_bl(c3_p0_cmd_bl),             //[5:0] output reg 
.c3_p0_cmd_byte_addr(c3_p0_cmd_byte_addr),     //[29:0] output reg 
                                
.c3_p0_wr_en(c3_p0_wr_en),                    //output reg 
.c3_p0_wr_full(c3_p0_wr_full),
.c3_p0_wr_mask(c3_p0_wr_mask),            //[3:0] output reg 
.c3_p0_wr_data(c3_p0_wr_data),           //[31:0] output reg 
                                
.c3_p0_rd_en(c3_p0_rd_en),                    //output reg 
.c3_p0_rd_data(c3_p0_rd_data),           //[31:0] input wire 
.c3_p0_rd_empty(c3_p0_rd_empty)                  //input wire 

  );

  
  
//右中
assign E12 = vga_rgb[0];
assign B15 = vga_rgb[1];
assign C15 = vga_rgb[2];
assign E13 = vga_rgb[3];
assign B16 = vga_rgb[4];
assign F12 = vga_rgb[5];
assign E16 = vga_rgb[6];
assign D16 = vga_rgb[7];
assign E15 = vga_rgb[8];
assign D14 = vga_rgb[9];
assign C16 = vga_rgb[10];
assign F16 = vga_rgb[11];
assign F13 = vga_rgb[12];
assign G14 = vga_rgb[13];
assign H16 = vga_rgb[14];
assign H11 = vga_rgb[15];

assign F15 = vga_hs    ;
assign G11 = vga_vs    ;

//右右

wire [7:0] sdcard;


wire vga_hs;
wire vga_vs;
wire [15:0] vga_rgb;
vga_driverX(
    .sys_clk (clk_100m),//input           sys_clk,
    .sys_rst_n(delay_rst),//input           sys_rst_n,    //复位信号
	.vga_clk_25M(vga_clk_25M),
	.vga_clk_65M(vga_clk_65M),
  .debug8(vga_debug8),
  .debug32(vga_debug32),
    //
    ////00 640*480 txt
    ////01 640*480 img
    ////1x 1024*768 img
    .vga_mode(2),
    //output reg blanking,
    //input blockvga,
    .read_line_base_addr(0),
    //
    .c3_p1_cmd_en        (c3_p1_cmd_en       ),//output reg c3_p1_cmd_en,
    .c3_p1_cmd_full      (c3_p1_cmd_full     ),//input c3_p1_cmd_full,
    .c3_p1_cmd_rw        (c3_p1_cmd_rw       ),//output reg c3_p1_cmd_rw,
    .c3_p1_cmd_bl        (c3_p1_cmd_bl       ),//output reg [5:0] c3_p1_cmd_bl,
    .c3_p1_cmd_byte_addr (c3_p1_cmd_byte_addr),//output reg [29:0] c3_p1_cmd_byte_addr,
    //
    .c3_p1_rd_en         (c3_p1_rd_en        ),//output reg c3_p1_rd_en,
    .c3_p1_rd_data       (c3_p1_rd_data      ),//input wire [31:0] c3_p1_rd_data,
    .c3_p1_rd_empty      (c3_p1_rd_empty     ),//input wire c3_p1_rd_empty,
    //
    //
    ////VGA接口
    .vga_hs (vga_hs),//output     reg     vga_hs,       //行同步信号
    .vga_vs (vga_vs),//output     reg     vga_vs,       //场同步信号
    .vga_rgb(vga_rgb)//output  [15:0]  vga_rgb      //红绿蓝三原色输出
    
  );

endmodule

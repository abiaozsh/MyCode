
module flow_led(
  input sys_clk,
  input key1,
  input key2,
  output led,
 
  //uart接口
  input uart_rxd,
  output uart_txd,
  input uart2_rxd,
  output uart2_txd,

  output segled_clk,
  output segled_dat, 
  output segled_str,
  output [7:0] debug,
 
  //SDRAM 芯片接口
  output        sdram_clk_out,                //SDRAM 芯片时钟
  output        sdram_cke,                //SDRAM 时钟有效
  output        sdram_cs_n,               //SDRAM 片选
  output        sdram_ras_n,              //SDRAM 行有效
  output        sdram_cas_n,              //SDRAM 列有效
  output        sdram_we_n,               //SDRAM 写有效
  output [ 1:0] sdram_ba,                 //SDRAM Bank地址
  output [12:0] sdram_addr,               //SDRAM 行/列地址
  inout  [15:0] sdram_data,               //SDRAM 数据
  output [ 1:0] sdram_dqm,                //SDRAM 数据掩码

    //VGA接口                          
    output          vga_hs,         //行同步信号
    output          vga_vs,         //场同步信号
    output  [15:0]  vga_rgb,         //红绿蓝三原色输出 

    inout key_data,
    inout key_clk,
    inout mouse_data,
    inout mouse_clk,
    
  input  wire        spi_MISO,        //     spi.MISO
  output wire        spi_MOSI,        //        .MOSI
  output wire        spi_SCLK,        //        .SCLK
  output wire [2:0]  spi_SS_n         //        .SS_n
 
);
wire sys_rst_n;
assign sys_rst_n = key1 && locked_vga && locked_sdram && locked_cpu;

assign led = 0;
//`include "config.v"

wire vga_clk_25M;
wire vga_clk_65M;
wire clk_100m;
wire clk_100m_shift;
wire clk_cpu;
wire locked_vga;
wire locked_sdram;
wire locked_cpu;
//例化PLL, 产生各模块所需要的时钟
pll_clk u_pll_sdram(
  .inclk0             (sys_clk),
  .c0                 (clk_100m),
  .c1                 (clk_100m_shift),
	.locked             (locked_sdram)
);
 
//例化PLL, 产生各模块所需要的时钟
pll_vga u_pll_vga(
  .inclk0             (sys_clk),

  .c0                 (vga_clk_25M),
  .c1                 (vga_clk_65M),
	.locked             (locked_vga)
);

//例化PLL, 产生各模块所需要的时钟
pll_cpu u_pll_cpu(
  .inclk0             (sys_clk),
  .c0                 (clk_cpu),
	.locked             (locked_cpu)
);
 
     
wire [7:0] seg_data0;
wire [7:0] seg_data1;
wire [7:0] seg_data2;
wire [7:0] seg_data3;
seg_led_hex595 ins_seg_led_hex595(
  .sys_clk(sys_clk), 
  .sys_rst_n(sys_rst_n),
 
  .clk(segled_clk),
  .dat(segled_dat),
  .str(segled_str),
 
  .data0(seg_data0),
  .data1(seg_data1),
  .data2(seg_data2),
  .data3(seg_data3)
);

 
assign seg_data3 = debug32[31:24];//outpin32  debug32
assign seg_data2 = debug32[23:16];//outpin32  debug32
assign seg_data1 = debug32[15:8]; //outpin32  debug32
assign seg_data0 = debug32[7:0];  //outpin32  debug32




wire [31:0] outpin32;

wire [7:0] debug8;
wire [31:0] debug32;

wire [7:0] sdrambus_debug8;

assign debug = sdrambus_debug8;//debug8;

wire dummy;
system system_inst(
  .clk      (clk_cpu),        //     clk.clk
	.clk_50M  (sys_clk),
	.vga_clk_25M(vga_clk_25M),
  .vga_clk_65M(vga_clk_65M),
  .clk_100m(clk_100m),
  .clk_100m_shift(clk_100m_shift),
	
  .reset_n  (sys_rst_n),  //   reset.reset_n

  .mycpu_uart_rxd (uart_rxd), //        .uart_rxd
  .mycpu_uart_txd (uart_txd), //   mycpu.uart_txd
  .debug8         (debug8),    //        .debug
  .debug32        (debug32),   //        .debug0

  .sdram_clk_out (sdram_clk_out	),   // sdram_0.addr
  .sdram_cke     (sdram_cke    	),     //        .ba
  .sdram_cs_n    (sdram_cs_n   	),  //        .cas_n
  .sdram_ras_n   (sdram_ras_n  	),    //        .cke
  .sdram_cas_n   (sdram_cas_n  	),   //        .cs_n
  .sdram_we_n    (sdram_we_n   	),     //        .dq
  .sdram_ba      (sdram_ba     	),    //        .dqm
  .sdram_addr    (sdram_addr   	),  //        .ras_n
  .sdram_data    (sdram_data   	),   //        .we_n
  .sdram_dqm     (sdram_dqm      ),
  .sdrambus_debug8 (sdrambus_debug8),
  
  .myuart_rxd     (uart2_rxd),     //  myuart.rxd
  .myuart_txd     (uart2_txd),      //        .txd
	
  .key_data   (key_data   ),
  .key_clk    (key_clk    ),
  .mouse_data (mouse_data ),
  .mouse_clk  (mouse_clk  ),

  .softspi_MISO        (spi_MISO),        //     spi.MISO
  .softspi_MOSI        (spi_MOSI),        //        .MOSI
  .softspi_SCLK        (spi_SCLK),        //        .SCLK
  .softspi_SS_n        (spi_SS_n),         //        .SS_n    
  
  //.pio0out_export (outpin32), // pio0out.export
  //.pio1in_export  (inpin8),  //  pio1in.export

    .vga_hs         (vga_hs),       
    .vga_vs         (vga_vs),       
    .vga_rgb        (vga_rgb),      
       

		.dummy(dummy)
  
 );




endmodule
            

  
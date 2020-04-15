
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
 
    
     //SDRAM8m 芯片接口
    output        sdram8m_clk_out,            //SDRAM 芯片时钟
    output        sdram8m_cke,                //SDRAM 时钟有效
    output        sdram8m_cs_n,               //SDRAM 片选
    output        sdram8m_ras_n,              //SDRAM 行有效
    output        sdram8m_cas_n,              //SDRAM 列有效
    output        sdram8m_we_n,               //SDRAM 写有效
    output  [1:0] sdram8m_ba,                 //SDRAM Bank地址
    output [11:0] sdram8m_addr,               //SDRAM 行/列地址
    inout  [15:0] sdram8m_data,               //SDRAM 数据
/*
  //input cy_SCL,
  //input cy_SDA,
  inout [7:0] cy_D,
  inout [7:0] cy_B,
  output cy_IFCLK_out                     ,
  input cy_to_fpga_CTL0_FLAGA        ,
  input cy_to_fpga_CTL2_FLAGC        ,
  input cy_to_fpga_CTL1_FLAGB        ,
  input cy_to_fpga_A7_FLAGD          ,
  output  cy_from_fpga_RDY1_SLWR       ,//output
  output  cy_from_fpga_RDY0_SLRD       ,//output
  input cy_A0_INT0                   ,
  output cy_A1_INT1                   ,
  output  cy_from_fpga_A2_SLOE         ,//output
  input cy_A3_WU2                    ,
  //output  cy_from_fpga_A4_FIFOADR0     ,//output
  output  cy_from_fpga_A5_FIFOADR1     ,//output
  //output  cy_from_fpga_A6_PKTEND       ,//output
*/
    //VGA接口                          
    output          vga_hs,         //行同步信号
    output          vga_vs,         //场同步信号
    output  [15:0]  vga_rgb,         //红绿蓝三原色输出 

    
    
  input  wire        spi_MISO,        //     spi.MISO
  output wire        spi_MOSI,        //        .MOSI
  output wire        spi_SCLK,        //        .SCLK
  output wire [2:0]  spi_SS_n         //        .SS_n

);
wire sys_rst_n;
assign sys_rst_n = key1;

assign led = 0;
/*
assign led = cyok;
wire cyok;
//25Mhz
reg clk_cy;
always @(posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    clk_cy <= 0;
  end else begin
    clk_cy <= !clk_cy;
  end
end
*/

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
  .clk      (sys_clk),        //     clk.clk
  .reset_n  (sys_rst_n),  //   reset.reset_n

  .mycpu_uart_rxd (uart_rxd), //        .uart_rxd
  .mycpu_uart_txd (uart_txd), //   mycpu.uart_txd
  .mycpu_debug8   (debug8),    //        .debug
  .mycpu_debug32  (debug32),   //        .debug0

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

  .softspi_MISO        (spi_MISO),        //     spi.MISO
  .softspi_MOSI        (spi_MOSI),        //        .MOSI
  .softspi_SCLK        (spi_SCLK),        //        .SCLK
  .softspi_SS_n        (spi_SS_n),         //        .SS_n    
  
  //.pio0out_export (outpin32), // pio0out.export
  //.pio1in_export  (inpin8),  //  pio1in.export

/*
	 .cyok(cyok),
   .cy_cmd(cy_cmd),
   .cy_dat(cy_dat),
   .cy_snd_data0(cy_snd_data0),
   .cy_snd_data1(cy_snd_data1),


   .cy_D(cy_D),
   .cy_B(cy_B),
   //.cy_SCL(cy_SCL)       ,
   //.cy_SDA(cy_SDA)       ,
   .cy_IFCLK(cy_IFCLK_in),
   .cy_to_fpga_CTL0_FLAGA(cy_to_fpga_CTL0_FLAGA),
   .cy_to_fpga_CTL2_FLAGC(cy_to_fpga_CTL2_FLAGC),
   .cy_to_fpga_CTL1_FLAGB(cy_to_fpga_CTL1_FLAGB),
   .cy_to_fpga_A7_FLAGD(cy_to_fpga_A7_FLAGD),
   .cy_from_fpga_RDY1_SLWR(cy_from_fpga_RDY1_SLWR)       ,//output
   .cy_from_fpga_RDY0_SLRD(cy_from_fpga_RDY0_SLRD)       ,//output
   .cy_from_fpga_A2_SLOE(cy_from_fpga_A2_SLOE)         ,//output
   .cy_A0_INT0(cy_A0_INT0)                   ,
   .cy_A1_INT1(cy_A1_INT1)                   ,
   .cy_A3_WU2(cy_A3_WU2)                    ,
   //.cy_from_fpga_A4_FIFOADR0(cy_from_fpga_A4_FIFOADR0)     ,//output
   .cy_from_fpga_A5_FIFOADR1(cy_from_fpga_A5_FIFOADR1)     ,//output
   //.cy_from_fpga_A6_PKTEND(cy_from_fpga_A6_PKTEND)       ,//output
*/

    //SDRAM 芯片接口
    .sdram8m_clk_out   (sdram8m_clk_out),
    .sdram8m_cke       (sdram8m_cke),    //SDRAM 时钟有效
    .sdram8m_cs_n      (sdram8m_cs_n),    //SDRAM 片选
    .sdram8m_ras_n     (sdram8m_ras_n),    //SDRAM 行有效  
    .sdram8m_cas_n     (sdram8m_cas_n),    //SDRAM 列有效
    .sdram8m_we_n      (sdram8m_we_n),    //SDRAM 写有效
    .sdram8m_ba        (sdram8m_ba),      //SDRAM Bank地址
    .sdram8m_addr      (sdram8m_addr),    //SDRAM 行/列地址
    .sdram8m_data      (sdram8m_data),    //SDRAM 数据  

    .vga_hs         (vga_hs),       
    .vga_vs         (vga_vs),       
    .vga_rgb        (vga_rgb),      
       

		.dummy(dummy)
  
 );




endmodule
            

  

module flow_led(
  input           sys_clk,          //外部50M时钟
  input key1,
  input key2,
  output led,
 
  output segled_clk,
  output segled_dat, 
  output segled_str,
 
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
 
     //SDRAM2m 芯片接口
    //output        sdram2m_clk_out,            //SDRAM 芯片时钟
    //output        sdram2m_cke,                //SDRAM 时钟有效
    //output        sdram2m_cs_n,               //SDRAM 片选
    //output        sdram2m_ras_n,              //SDRAM 行有效
    //output        sdram2m_cas_n,              //SDRAM 列有效
    //output        sdram2m_we_n,               //SDRAM 写有效
    //output        sdram2m_ba,                 //SDRAM Bank地址
    //output [10:0] sdram2m_addr,               //SDRAM 行/列地址
    //inout  [15:0] sdram2m_data,               //SDRAM 数据
    
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

  output [7:0] debug,
 
  inout [7:0] cy_D,
  inout [7:0] cy_B,
  input cy_SCL,
  input cy_SDA,
  output cy_IFCLK_out                     ,
  input cy_to_fpga_CTL0_FLAGA        ,
  input cy_to_fpga_CTL2_FLAGC        ,
  input cy_to_fpga_CTL1_FLAGB        ,
  output  cy_from_fpga_RDY1_SLWR       ,//output
  output  cy_from_fpga_RDY0_SLRD       ,//output
  input cy_A0_INT0                   ,
  output cy_A1_INT1                   ,
  output  cy_from_fpga_A2_SLOE         ,//output
  input cy_A3_WU2                    ,
  output  cy_from_fpga_A4_FIFOADR0     ,//output
  output  cy_from_fpga_A5_FIFOADR1     ,//output
  output  cy_from_fpga_A6_PKTEND       ,//output
  input cy_to_fpga_A7_FLAGD          ,

    //VGA接口                          
    output          vga_hs,         //行同步信号
    output          vga_vs,         //场同步信号
    output  [15:0]  vga_rgb         //红绿蓝三原色输出 
 

);
wire sys_rst_n;
assign sys_rst_n = key1;


wire cy_IFCLK_in;
assign cy_IFCLK_out = clk_cy;
assign cy_IFCLK_in = !clk_cy;

//25Mhz
reg clk_cy;
always @(posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    clk_cy <= 0;
  end else begin
    clk_cy <= !clk_cy;
  end
end

//wire clk_cy;
//pll_cy(
//  .inclk0             (sys_clk),
//  .areset             (~sys_rst_n),
//  .c0                 (clk_cy)
//);
//
//
//


  //assign led[0] = !uart_rxd;
  //assign led[1] = !uart_txd;
  //assign led[2] = busy;
  //assign led = busy;

 
  wire readreq;
  reg readack;
  
  reg [7:0] timer;
 
  reg [16:0] data0;
  reg [16:0] data1;
  reg [16:0] data2;
  reg [16:0] data3;
 
  assign seg_data3 = debug83;//debug1[31:24];//cy_snd_data1;//cy_dat;
  assign seg_data2 = debug82;//debug1[23:16];//cy_snd_data0;//cy_cmd;
  assign seg_data1 = debug81;//debug1[15:8];//cy_D;
  assign seg_data0 = debug80;//debug1[7:0];//cy_B;
 
 
  assign debug[0] = debug0;
  assign debug[1] = debug1;
  assign debug[2] = debug2;
  assign debug[3] = debug3;
  assign debug[4] = debug4;
  assign debug[5] = debug5;
  assign debug[6] = debug6;
  assign debug[7] = debug7;
  
wire buffok;
assign buffok = cy_to_fpga_CTL0_FLAGA == 0 &&
 cy_to_fpga_CTL1_FLAGB == 1 &&
 cy_to_fpga_CTL2_FLAGC == 0 &&
 cy_to_fpga_A7_FLAGD   == 1;
wire bufferr;
assign bufferr = !buffok;
assign led = !bufferr;

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
 
  wire busy;
  wire blanking;

  wire debug0;
  wire debug1;
  wire debug2;
  wire debug3;
  wire debug4;
  wire debug5;
  wire debug6;
  wire debug7;
  wire [7:0] debug80;
  wire [7:0] debug81;
  wire [7:0] debug82;
  wire [7:0] debug83;
              
  wire [7:0] cy_cmd;
  wire [7:0] cy_dat;
  wire [7:0] cy_snd_data0;
  wire [7:0] cy_snd_data1;
cy68013_mcu ins_cy68013_mcu(
    .sys_clk    (sys_clk  ),       // 时钟信号
    .sys_rst_n  (sys_rst_n),       // 复位信号
 
   .cy_D(cy_D),
   .cy_B(cy_B),
   .cy_SCL(cy_SCL)       ,
   .cy_SDA(cy_SDA)       ,
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
   .cy_from_fpga_A4_FIFOADR0(cy_from_fpga_A4_FIFOADR0)     ,//output
   .cy_from_fpga_A5_FIFOADR1(cy_from_fpga_A5_FIFOADR1)     ,//output
   .cy_from_fpga_A6_PKTEND(cy_from_fpga_A6_PKTEND)       ,//output

   .cy_cmd(cy_cmd),
   .cy_dat(cy_dat),
   .cy_snd_data0(cy_snd_data0),
   .cy_snd_data1(cy_snd_data1),

    .debug0(debug0),
    .debug1(debug1),
    .debug2(debug2),
    .debug3(debug3),
    .debug4(debug4),
    .debug5(debug5),
    .debug6(debug6),
    .debug7(debug7),
    
    .debug80(debug80),
    .debug81(debug81),
    .debug82(debug82),
    .debug83(debug83),

 
    //SDRAM 芯片接口
    .sdram_clk_out   (sdram_clk_out),
    .sdram_cke       (sdram_cke),    //SDRAM 时钟有效
    .sdram_cs_n      (sdram_cs_n),    //SDRAM 片选
    .sdram_ras_n     (sdram_ras_n),    //SDRAM 行有效  
    .sdram_cas_n     (sdram_cas_n),    //SDRAM 列有效
    .sdram_we_n      (sdram_we_n),    //SDRAM 写有效
    .sdram_ba        (sdram_ba),      //SDRAM Bank地址
    .sdram_addr      (sdram_addr),    //SDRAM 行/列地址
    .sdram_data      (sdram_data),    //SDRAM 数据  
    .sdram_dqm       (sdram_dqm),

    //SDRAM 芯片接口
    //.sdram2m_clk_out  (sdram2m_clk_out),
    //.sdram2m_cke      (sdram2m_cke),    //SDRAM 时钟有效
    //.sdram2m_cs_n     (sdram2m_cs_n),    //SDRAM 片选
    //.sdram2m_ras_n    (sdram2m_ras_n),    //SDRAM 行有效  
    //.sdram2m_cas_n    (sdram2m_cas_n),    //SDRAM 列有效
    //.sdram2m_we_n     (sdram2m_we_n),    //SDRAM 写有效
    //.sdram2m_ba       (sdram2m_ba),      //SDRAM Bank地址
    //.sdram2m_addr     (sdram2m_addr),    //SDRAM 行/列地址
    //.sdram2m_data     (sdram2m_data),    //SDRAM 数据  
    
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
       
    .busy(busy)

  );

endmodule 













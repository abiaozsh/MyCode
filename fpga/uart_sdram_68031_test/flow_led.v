
module flow_led(
  input           sys_clk,          //外部50M时钟
  input key1,
  input key2,
  output reg led,
 
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
 
  output [7:0] debug,

  inout [7:0] cy_D,
  inout [7:0] cy_B,
  input cy_SCL,
  input cy_SDA,
  input cy_IFCLK                     ,
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

  //uart接口
  input uart_rx_from_pc,
  output uart_tx_to_pc
);
wire sdram_prob_refresh;

wire sys_rst_n;
assign sys_rst_n = key1;

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
  
  assign seg_data3 = out_pin3;//cy_snd_data1;//cy_dat;
  assign seg_data2 = out_pin2;//cy_snd_data0;//cy_cmd;
  assign seg_data1 = cy_D;
  assign seg_data0 = cy_B;
  
  assign debug[0] = cy_A0_INT0;
  assign debug[1] = cy_A1_INT1;
  assign debug[2] = cy_A3_WU2;
  
  //assign cy_A1_INT1 = key2;
  
  assign debug[4] = cy_to_fpga_CTL0_FLAGA;
  assign debug[5] = cy_to_fpga_CTL1_FLAGB;
  assign debug[6] = cy_to_fpga_CTL2_FLAGC;
  assign debug[7] = cy_to_fpga_A7_FLAGD;
  
 

wire [7:0] seg_data0;
wire [7:0] seg_data1;
wire [7:0] seg_data2;
wire [7:0] seg_data3;
seg_led_hex595 (
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
  wire out_clk;
  wire out_rst;
  wire [7:0] in_pin0;
  wire [7:0] in_pin1;
  wire [7:0] in_pin2;
  wire [7:0] in_pin3;
  wire [7:0] in_pin4;
  wire [7:0] in_pin5;
  wire [7:0] in_pin6;
  wire [7:0] in_pin7;

  wire [7:0] out_pin0;
  wire [7:0] out_pin1;
  wire [7:0] out_pin2;
  wire [7:0] out_pin3;
  wire [7:0] out_pin4;
  wire [7:0] out_pin5;
  wire [7:0] out_pin6;
  wire [7:0] out_pin7;
  
  wire [7:0] cy_cmd;
  wire [7:0] cy_dat;
  wire [7:0] cy_snd_data0;
  wire [7:0] cy_snd_data1;
  uart_mcu_slavefifo(
    .sys_clk    (sys_clk  ),       // 时钟信号
    .sys_rst_n  (sys_rst_n),       // 复位信号
    
    .uart_rxd  (uart_rx_from_pc),
    .uart_txd  (uart_tx_to_pc),

	 .cy_D(cy_D),
	 .cy_B(cy_B),
   .cy_SCL(cy_SCL)       ,
   .cy_SDA(cy_SDA)       ,
	 //input cy_IFCLK                     ,
	 //input cy_to_fpga_CTL0_FLAGA        ,
	 //input cy_to_fpga_CTL2_FLAGC        ,
	 //input cy_to_fpga_CTL1_FLAGB        ,
	 //input cy_to_fpga_A7_FLAGD          ,
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

    .busy(busy),

    //SDRAM 芯片接口
    .sdram_clk_out     (sdram_clk_out),
    .sdram_cke			(sdram_cke),		//SDRAM 时钟有效
    .sdram_cs_n			(sdram_cs_n),		//SDRAM 片选
    .sdram_ras_n		(sdram_ras_n),		//SDRAM 行有效	
    .sdram_cas_n		(sdram_cas_n),		//SDRAM 列有效
    .sdram_we_n			(sdram_we_n),		//SDRAM 写有效
    .sdram_ba			  (sdram_ba),			//SDRAM Bank地址
    .sdram_addr			(sdram_addr),		//SDRAM 行/列地址
    .sdram_data			(sdram_data),		//SDRAM 数据	
    .sdram_dqm		(sdram_dqm),
    .sdram_prob_refresh (sdram_prob_refresh),

    .out_clk (out_clk),
    .out_rst (out_rst),

    .in_pin0          ( in_pin0 ),//数据低
    .in_pin1          ( in_pin1 ),//数据高
    .in_pin2          ( in_pin2 ),//sdram_wr_ack,sdram_rd_ack
    .in_pin3          ( in_pin3 ),
    .in_pin4          ( in_pin4 ),
    .in_pin5          ( in_pin5 ),
    .in_pin6          ( in_pin6 ),
    .in_pin7          ( in_pin7 ),

    .out_pin0          ( out_pin0  ),
    .out_pin1          ( out_pin1  ),
    .out_pin2          ( out_pin2  ),
    .out_pin3          ( out_pin3  ),
    .out_pin4          ( out_pin4  ),
    .out_pin5          ( out_pin5  ),
    .out_pin6          ( out_pin6  ),
    .out_pin7          ( out_pin7  )

  );

endmodule 













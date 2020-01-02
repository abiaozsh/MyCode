
module flow_led(
  input           sys_clk,          //外部50M时钟
  input           sys_rst_n,        //外部复位信号，低有效
  input      [3:0]  key,
  output     [3:0]  led,         //4个LED灯
  //seg_led interface
  output    [5:0]  seg_sel,       // 数码管位选信号
  output    [7:0]  seg_led,        // 数码管段选信号

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
  
  //raspberrypi
  output T4,
  output T5,
  output T6,
  output T7,
  output T8,
  output T9,
  output T10,
  output T11,
  output T12,
  output T13,
  output T14,
  output M8,
  output P8,
  output L9,
  output M9,
  output P9,
  ////////////
  input P3,
  input R3,
  input R4,
  input R5,
  
  input R6,
  input R7,
  input R8,
  output R9,

  //uart接口
  input           uart_rxd,         //UART接收端口
  output          uart_txd          //UART发送端口
);

  assign led[0] = !uart_rxd;
  assign led[1] = !uart_txd;
  assign led[2] = busy;

  //assign led = out_pin0[3:0];
  assign in_pin0[3:0] = key;

  wire [7:0] seg_data0;
  wire [7:0] seg_data1;
  wire [7:0] seg_data2;
  //数码管动态显示模块
  seg_led_hex ins_seg_led_hex(
    .sys_clk           (sys_clk  ),       // 时钟信号
    .sys_rst_n         (sys_rst_n),       // 复位信号

    .data0          (seg_data0),       // 显示的数值
    .data1          (seg_data1),       // 显示的数值
    .data2          (seg_data2),       // 显示的数值

    .seg_sel       (seg_sel  ),       // 位选
    .seg_led       (seg_led  )        // 段选
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
  uart_mcu ins_uart_mcu(
    .sys_clk    (sys_clk  ),       // 时钟信号
    .sys_rst_n  (sys_rst_n),       // 复位信号
    
    .uart_rxd  (uart_rxd),
    .uart_txd  (uart_txd),

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













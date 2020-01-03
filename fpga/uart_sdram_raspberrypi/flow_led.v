
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
  
  input [7:0]   adc_in,
  output        adc_clk,
  
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
/*
#define  CTL1P3  21
#define  CTL2R3  20//TODO
#define  CTL3R4  26
#define  CTL4R5  19
#define  CTL5R6  25
#define  CTL6R7  22
#define  CTL7R8  27
#define  CTL8R9  17
*/

  //uart接口
  input           uart_rxd,         //UART接收端口
  output          uart_txd          //UART发送端口
);


  //assign led = out_pin0[3:0];

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
  assign seg_data0 = outdata[7:0];
  assign seg_data1 = outdata[15:8];

  wire [15:0]outdata;
  assign T11 = outdata[0];
  assign T7  = outdata[1];
  assign L9  = outdata[2];
  assign P8  = outdata[3];
  assign M8  = outdata[4];
  assign T10 = outdata[5];
  assign T6  = outdata[6];
  assign T8  = outdata[7];
  assign T9  = outdata[8];
  assign T13 = outdata[9];
  assign T14 = outdata[10];
  assign T12 = outdata[11];
  assign T5  = outdata[12];
  assign T4  = outdata[13];
  assign P9  = outdata[14];
  assign M9  = outdata[15];
  
  
  //assign led[0] = !uart_rxd || rbp_req;
  //assign led[1] = !uart_txd || rbp_ack;
  //assign led[2] = busy;
  assign led = rbp_cmd;

  wire rbp_req;
  assign rbp_req = R8;//CTL7R8
  wire rbp_ack;
  assign R9 = rbp_ack;//CTL8R9
  wire rbp_rst;
  assign rbp_rst = R7;//CTL6R7

  wire rbp_dat;
  assign rbp_dat = R6;//#define  DAT CTL5R6
  
  wire [3:0]rbp_cmd;
  assign rbp_cmd = {R5,R4,R3,P3};//CTL1P3,CTL2R3,CTL3R4,CTL4R5
  

  wire busy;
  uart_mcu_rbp ins_uart_mcu_rbp(
    .sys_clk    (sys_clk  ),       // 时钟信号
    .sys_rst_n  (sys_rst_n),       // 复位信号
    
    .uart_rxd  (uart_rxd),
    .uart_txd  (uart_txd),

    .rbp_data(outdata),
    .rbp_req(rbp_req),
    .rbp_ack(rbp_ack),
    .rbp_rst(rbp_rst),
    .rbp_dat(rbp_dat),
    .rbp_cmd(rbp_cmd),

    //SDRAM 芯片接口
    .sdram_clk_out  (sdram_clk_out),
    .sdram_cke			(sdram_cke),		//SDRAM 时钟有效
    .sdram_cs_n			(sdram_cs_n),		//SDRAM 片选
    .sdram_ras_n		(sdram_ras_n),		//SDRAM 行有效	
    .sdram_cas_n		(sdram_cas_n),		//SDRAM 列有效
    .sdram_we_n			(sdram_we_n),		//SDRAM 写有效
    .sdram_ba			  (sdram_ba),			//SDRAM Bank地址
    .sdram_addr			(sdram_addr),		//SDRAM 行/列地址
    .sdram_data			(sdram_data),		//SDRAM 数据	
    .sdram_dqm		  (sdram_dqm),

    .busy(busy)
);

endmodule














module flow_led(
  input           sys_clk,          //外部50M时钟
  input key1,
  input key2,
  output led,

  output segled_clk,
  output segled_dat,
  output segled_str,

  output [15:0]rbp_dat,
  input   [7:0]rbp_cmd_in,
  input rbp_dat24,
  input rbp_rst25,
  output rbp_ack26,
  input rbp_req27,
//CMD[23:16]

  input R2,
  input P2, 
  input N2,
  input M2,
  input J2,
  input H2,
  input F2,
  input D2,
  input C2,
  input B2,
  input A3,
  input A4,
  input C3,

  output R1,
  input P1,
  input N1,
  input M1,
  input J1,
  input H1,
  input F1,
  input E1,
  input C1,
  input B1,
  input B3,
  input B4,
  input C4,

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
  
  input dummy
  
);

wire sys_rst_n;
assign sys_rst_n = key1;

  
  wire [7:0]  adcA;
  wire [7:0]  adcB;
  wire        adcClk;
  
  //ad9280
  //A8 clk
  //A6 d0
  //A5 d1
  //A4 d2
  //A3 d3
  //A2 d4
  //C2 d5
  //F2 d6
  //F3 d7
  /*
  assign adcA = {D2,F2,H2,J2,M2,N2,P2,R2};
  assign adcB = 0;
  */
  
  //ad9288
assign adcA = {D2,F2,H2,J2,M2,N2,P2,R2};
assign adcB = {B3,B1,C1,C3,A4,A3,B2,C2};




assign R1 = adcClk;

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

  assign seg_data0 = fetch_data[7:0];
  assign seg_data1 = fetch_data[15:8];
  assign seg_data2 = dump_address[23:16];
  assign seg_data3 = rbp_cmd;

  wire [15:0]outdata;
  assign rbp_dat = outdata;
  
  assign led = write_address[23];
  //assign led = 
  
  wire [3:0]rbp_cmd;
  assign rbp_cmd = rbp_cmd_in[3:0];//CTL1P3,CTL2R3,CTL3R4,CTL4R5
  
  wire[23:0] write_address;
  
  wire[23:0] dump_address;
  wire[15:0] fetch_data;
  wire busy;
  uart_mcu_rbp ins_uart_mcu_rbp(
    .sys_clk    (sys_clk  ),       // 时钟信号
    .sys_rst_n  (sys_rst_n),       // 复位信号
    
    .uart_rxd  (uart_rxd),
    .uart_txd  (uart_txd),

    .adc_in1(adcA),
    .adc_in2(adcB),
    .adc_clk(adcClk),

    .rbp_data(outdata),
    .rbp_req(rbp_req27),
    .rbp_ack(rbp_ack26),
    .rbp_rst(rbp_rst25),
    .rbp_dat(rbp_dat24),
    .rbp_cmd(rbp_cmd),

    .write_address(write_address),
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

    .fetch_data(fetch_data),
    .dump_address(dump_address),
    .busy(busy)
);

endmodule













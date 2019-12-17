
module flow_led(
		input           sys_clk,          //外部50M时钟
		input           sys_rst_n,        //外部复位信号，低有效
		input 		[3:0]			key,
		output     [3:0]  led,         //4个LED灯

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

		//uart接口
		input           uart_rxd,         //UART接收端口
		output          uart_txd          //UART发送端口
		);
 

assign led = out_pin0[3:0];
assign in_pin0[3:0] = key;



  
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
	uart_mcu(
			.sys_clk    (sys_clk  ),       // 时钟信号
			.sys_rst_n  (sys_rst_n),       // 复位信号
			.uart_rxd  (uart_rxd),
			.uart_txd  (uart_txd),

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













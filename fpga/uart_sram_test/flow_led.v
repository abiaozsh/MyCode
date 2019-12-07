
module flow_led(
		input           sys_clk,          //外部50M时钟
		input           sys_rst_n,        //外部复位信号，低有效
		input 		[3:0]			key,
		output     [3:0]  led,         //4个LED灯

		//uart接口
		input           uart_rxd,         //UART接收端口
		output          uart_txd          //UART发送端口
		);
 
	wire  [3:0]  key_stable;

	genvar i;
	generate
		for(i=0; i<4; i=i+1) begin:BLOCK1
			key_debounce(
					.sys_clk        (sys_clk),
					.sys_rst_n      (sys_rst_n),
    
					.key            (key[i]),
					.key_value      (key_stable[i])
				);
		end
	endgenerate

	/*
	input	[9:0]  address;
	input	  clock;
	input	[15:0]  data;
	input	  rden;
	input	  wren;
	output	[15:0]  q;
	*/
  
	wire	[9:0]  address_sig;
	wire	  clock_sig;
	wire	[15:0]  data_in_sig;
	wire	  rden_sig;
	wire	  wren_sig;
	wire	[15:0]  q_sig;//out
	ram	ram_inst (
	.address ( address_sig ),
	.clock ( clock_sig ),
	.data ( data_in_sig ),
	.rden ( rden_sig ),
	.wren ( wren_sig ),
	.q ( q_sig )
	);
/*

o402
o000
o100
o212
o334
clk0
clk1

o000
o101
o256
o378
clk0
clk1

o401
o000
o100
clk0
clk1
i1
i0

o000
o101
clk0
clk1
i1
i0

*/
  assign address_sig = {out_pin1[0],out_pin0}; //address ob,oa
  assign clock_sig = out_clk;                    //clock clk
  assign data_in_sig = {out_pin3,out_pin2};//data od,oc
  assign rden_sig = out_pin4[0];
  assign wren_sig = out_pin4[1];
  assign in_pin0 = q_sig[7:0];
  assign in_pin1 = q_sig[15:8];
  
	wire out_clk;
	wire out_rst;
	wire [7:0] in_pin0;
	wire [7:0] in_pin1;
	wire [7:0] in_pin2;
	wire [7:0] in_pin3;
	wire [7:0] out_pin0;
	wire [7:0] out_pin1;
	wire [7:0] out_pin2;
	wire [7:0] out_pin3;
	wire [7:0] out_pin4;
	uart_mcu(
			.sys_clk           (sys_clk  ),       // 时钟信号
			.sys_rst_n         (sys_rst_n),       // 复位信号
			.uart_rxd(uart_rxd),
			.uart_txd(uart_txd),
      
			.out_clk    (out_clk),
			.out_rst (out_rst),
      
			.in_pin0          ( in_pin0 ),
			.in_pin1          ( in_pin1 ),
			.in_pin2          ( in_pin2 ),
			.in_pin3          ( in_pin3 ),

			.out_pin0          ( out_pin0  ),
			.out_pin1          ( out_pin1  ),
			.out_pin2          ( out_pin2  ),
			.out_pin3          ( out_pin3  ),
			.out_pin4          ( out_pin4  ) 
		);

    
endmodule 
















module flow_led(
		input           sys_clk,          //外部50M时钟
		input           sys_rst_n,        //外部复位信号，低有效
    
		input 		[3:0]			key,
		output     [3:0]  led,         //4个LED灯
    
		//seg_led interface
		output    [5:0]  seg_sel  ,       // 数码管位选信号
		output    [7:0]  seg_led  ,        // 数码管段选信号

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

	
	//assign seg_sel = 6'b111111;

	wire [7:0] seg_data0;
	wire [7:0] seg_data1;
	wire [7:0] seg_data2;

	wire seg_en;
	assign seg_en = 1'b1;
	//数码管动态显示模块
	seg_led_hex(
			.clk           (sys_clk  ),       // 时钟信号
			.rst_n         (sys_rst_n),       // 复位信号
			.en            (seg_en       ),       // 数码管使能信号

			.data0          (seg_data0     ),       // 显示的数值
			.data1          (seg_data1     ),       // 显示的数值
			.data2          (seg_data2     ),       // 显示的数值

			.seg_sel       (seg_sel  ),       // 位选
			.seg_led       (seg_led  )        // 段选
		);

	assign seg_data0 = out_pin_a;
	assign seg_data1 = out_pin_b;
	assign seg_data2 = out_pin_c;
	assign led = out_pin_d[3:0];
	assign in_pin_a[3:0] = key;
	wire out_clk;
	wire out_rst;
	wire [7:0] in_pin_a;
	wire [7:0] in_pin_b;
	wire [7:0] in_pin_c;
	wire [7:0] in_pin_d;
	wire [7:0] out_pin_a;
	wire [7:0] out_pin_b;
	wire [7:0] out_pin_c;
	wire [7:0] out_pin_d;
	wire [7:0] out_pin_e;
	uart_mcu(
			.sys_clk           (sys_clk  ),       // 时钟信号
			.sys_rst_n         (sys_rst_n),       // 复位信号
			.uart_rxd(uart_rxd),
			.uart_txd(uart_txd),
      
			.out_clk    (out_clk),
			.out_rst (out_rst),
      
			.in_pin_a          ( in_pin_a ),
			.in_pin_b          ( in_pin_b ),
			.in_pin_c          ( in_pin_c ),
			.in_pin_d          ( in_pin_d ),

			.out_pin_a          ( out_pin_a  ),
			.out_pin_b          ( out_pin_b  ),
			.out_pin_c          ( out_pin_c  ),
			.out_pin_d          ( out_pin_d  ),
			.out_pin_e          ( out_pin_e  ) 
		);

    
endmodule 















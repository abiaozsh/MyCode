
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
/*
	reg [7:0] seg_char0;
	reg [7:0] seg_char1;
	reg [7:0] seg_char2;
	reg [7:0] seg_char3;
	reg [7:0] seg_char4;
	reg [7:0] seg_char5;

	reg uart_en_r_old;
  
	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			uart_en_r_old <= uart_en_r;
		end else begin
			uart_en_r_old <= uart_en_r;
		end
	end

	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			seg_char0<=0;
			seg_char1<=0;
			seg_char2<=0;
			seg_char3<=0;
			seg_char4<=0;
			seg_char5<=0;
		end else begin
			if(uart_en_r==1 && uart_en_r_old != uart_en_r)begin
				seg_char0<=uart_data_r;
				seg_char1<=seg_char0;
				seg_char2<=seg_char1;
				seg_char3<=seg_char2;
				seg_char4<=seg_char3;
				seg_char5<=seg_char4;
			end else begin
				seg_char0<=seg_char0;
				seg_char1<=seg_char1;
				seg_char2<=seg_char2;
				seg_char3<=seg_char3;
				seg_char4<=seg_char4;
				seg_char5<=seg_char5;
			end
		end
	end
  
	wire seg_en;
	assign seg_en = 1'b1;
	//数码管动态显示模块
	seg_led_ascii(
			.clk           (sys_clk  ),       // 时钟信号
			.rst_n         (sys_rst_n),       // 复位信号
			.en            (seg_en       ),       // 数码管使能信号

			.char0          (seg_char0     ),       // 显示的数值
			.char1          (seg_char1     ),       // 显示的数值
			.char2          (seg_char2     ),       // 显示的数值
			.char3          (seg_char3     ),       // 显示的数值
			.char4          (seg_char4     ),       // 显示的数值
			.char5          (seg_char5     ),       // 显示的数值

			.seg_sel       (seg_sel  ),       // 位选
			.seg_led       (seg_led  )        // 段选
		);
*/

	reg [7:0] seg_data0;
	reg [7:0] seg_data1;
	reg [7:0] seg_data2;

	reg uart_en_r_old;
  
	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			uart_en_r_old <= uart_en_r;
		end else begin
			uart_en_r_old <= uart_en_r;
		end
	end

	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			seg_data0<=0;
			seg_data1<=0;
			seg_data2<=0;
		end else begin
			if(uart_en_r==1 && uart_en_r_old != uart_en_r)begin
				seg_data0<=uart_data_r;
				seg_data1<=seg_data0;
				seg_data2<=seg_data1;
			end else begin
				seg_data0<=seg_data0;
				seg_data1<=seg_data1;
				seg_data2<=seg_data2;
			end
		end
	end
  
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


	wire       uart_en_r;
	wire [7:0] uart_data_r;
	wire uart_en_w;
	wire [7:0] uart_data_w;
    	uart_hs (                 
			.sys_clk        (sys_clk), 
			.sys_rst_n      (sys_rst_n),
			.uart_rxd       (uart_rxd),
			.uart_txd       (uart_txd),
			
			.uart_done      (uart_en_r),
			.uart_data_out      (uart_data_r),

			.uart_en        (uart_en_w),
			.uart_data_in       (uart_data_w)
		);
		
	ttl_in_status_matchin (                 
			.sys_clk        (sys_clk), 
			.sys_rst_n      (sys_rst_n),
    
			.uart_done       (uart_en_r),
			.uart_data      (uart_data_r),
			.outpin         (led)
		);


	ttl_out_status_matchin(                 
			.sys_clk        (sys_clk), 
			.sys_rst_n      (sys_rst_n),
    
			.key_in        (key),
      
			.uart_en        (uart_en_w),
			.uart_data       (uart_data_w)

		);
    
    
endmodule 



//a1 a2 a3 a4 点亮相应的灯 b1 b2 b3 b4 点亮相应的灯
module ttl_in_status_matchin(
		input               sys_clk  ,  
		input               sys_rst_n, 
    
		input       uart_done,
		input [7:0] uart_data,
    
		output    [3:0]  outpin       
		);
	reg uart_done_old;
	reg [1:0] command;//0~2
	reg [1:0] cmd_store;//0~2
	reg [3:0] mask;//0~15
	reg [3:0] outreg;

	assign outpin = outreg;

	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			uart_done_old <= uart_done;
		end else begin
			uart_done_old <= uart_done;
		end
	end
	
	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			command <= 0;
			cmd_store <= 0;
			mask <= 0;
		end else begin
			if (uart_done_old != uart_done && uart_done==1) begin //串口数据到达  
				if(cmd_store == 0) begin
					mask <= 0;
					command <= 0;
					if(uart_data == 97) begin // a 8'b01100001
						cmd_store <= 1;//打开
					end else if(uart_data == 98) begin // b 8'b01100010
						cmd_store <= 2;//关闭
					end else begin
						cmd_store <= 0;
					end
				end else begin
					cmd_store <= 0;
					command <= cmd_store;
					if(uart_data == 49) begin // 1
						mask <= 1;
					end else if(uart_data == 50) begin // 2
						mask <= 2;
					end else if(uart_data == 51) begin // 3
						mask <= 4;
					end else if(uart_data == 52) begin // 4
						mask <= 8;
					end else begin
						mask <= 0;//单周期执行型
					end
				end
			end else begin
				cmd_store <= cmd_store;
				command <= command;
				mask <= mask;
			end
		end
	end
	
	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			outreg <= 0;
		end else begin
			if (command == 1) begin //串口数据到达
				outreg <= outreg | mask;
			end else if (command == 2) begin
				outreg <= outreg & ~mask;
			end else begin
				outreg <= outreg;
			end
		end
	end
	
endmodule



module ttl_out_status_matchin(
		input               sys_clk  ,  
		input               sys_rst_n,
    
		input 		[3:0]		key_in,
    
		output reg  uart_en,
		output reg [7:0] uart_data
		);

	reg [2:0] status;
	reg [3:0] key_in_old;//捕获上升沿

	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			key_in_old <= key_in;
		end else begin
			key_in_old <= key_in;
		end
	end

	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			status<=0;
		end else begin
			if         (key_in_old[0] != key_in[0] && key_in[0]==0) begin
				status<=1;
			end else if(key_in_old[1] != key_in[1] && key_in[1]==0) begin
				status<=2;
			end else if(key_in_old[2] != key_in[2] && key_in[2]==0) begin
				status<=3;
			end else if(key_in_old[3] != key_in[3] && key_in[3]==0) begin
				status<=4;
			end else begin
				status<=0;
			end
		end
	end
	
	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			uart_en<=0;
			uart_data<=0;
		end else begin
			if(status==1) begin
				uart_en<=1;
				uart_data<=97;
			end else if(status==2) begin
				uart_en<=1;
				uart_data<=98;
			end else if(status==3) begin
				uart_en<=1;
				uart_data<=99;
			end else if(status==4) begin
				uart_en<=1;
				uart_data<=100;
			end else begin
				uart_en<=0;
				//uart_data<=0;保留数据
				uart_data<=uart_data;
			end
		end
	end


endmodule


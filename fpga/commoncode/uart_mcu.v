module uart_mcu(
		input               sys_clk  ,  
		input               sys_rst_n, 
    
		input  uart_rxd,
		output uart_txd,
    
		output reg out_clk,
		output reg out_rst,

		input [7:0] in_pin0,
		input [7:0] in_pin1,
		input [7:0] in_pin2,
		input [7:0] in_pin3,
		input [7:0] in_pin4,
		input [7:0] in_pin5,
		input [7:0] in_pin6,
		input [7:0] in_pin7,

		output reg [7:0] out_pin0,
		output reg [7:0] out_pin1,
		output reg [7:0] out_pin2,
		output reg [7:0] out_pin3,
		output reg [7:0] out_pin4,
		output reg [7:0] out_pin5,
		output reg [7:0] out_pin6,
		output reg [7:0] out_pin7

		);
	reg uart_done_old;
	reg [7:0] command;
	reg [7:0] cmd_temp;
	reg [7:0] data;

	wire uart_done;
	wire [7:0] uart_data_r;
	reg uart_send;
	reg [7:0] uart_data_w;
	
	reg [7:0] reg0;
	reg [7:0] reg1;
	reg [7:0] reg2;
	reg [7:0] reg3;
	reg [7:0] reg4;
	reg [7:0] reg5;
	reg [7:0] reg6;
	reg [7:0] reg7;
  
	///////////
	reg [7:0] status;
	reg [7:0] timer8;
	reg [31:0] timer32;
  
  
	///////////
	uart_hs (
			.sys_clk        (sys_clk), 
			.sys_rst_n      (sys_rst_n),
			.uart_rxd       (uart_rxd),
			.uart_txd       (uart_txd),
			
			.uart_done      (uart_done),
			.uart_data_out  (uart_data_r),
			.uart_en        (uart_send),
			.uart_data_in   (uart_data_w)
		);
  

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
			cmd_temp <= 0;
			data <= 0;
		end else begin
			if (uart_done_old != uart_done && uart_done==1) begin //串口数据到达  
				if(cmd_temp == 0) begin
					command <= 0;
					cmd_temp <= uart_data_r;
					data <= 0;
				end else begin
					command <= cmd_temp;
					cmd_temp <= 0;
					data <= uart_data_r;
				end
			end else begin
				command <= 0;
				cmd_temp <= cmd_temp;
				data <= 0;
			end
		end
	end
	
	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			out_pin0<=0;
			out_pin1<=0;
			out_pin2<=0;
			out_pin3<=0;
			out_pin4<=0;
			out_pin5<=0;
			out_pin6<=0;
			out_pin7<=0;
			uart_send<=0;
			uart_data_w<=0;
			out_clk<=0;
			out_rst<=0;
			status<=0;
		end else begin
			if           (command == 8'h10) begin out_clk<=1;
			end else if (command == 8'h11) begin out_clk<=0;
			end else if (command == 8'h12) begin out_rst<=1;
			end else if (command == 8'h13) begin out_rst<=0;

			end else if (command == 8'h20) begin uart_send<=1; uart_data_w<=in_pin0;
			end else if (command == 8'h21) begin uart_send<=1; uart_data_w<=in_pin1;
			end else if (command == 8'h22) begin uart_send<=1; uart_data_w<=in_pin2;
			end else if (command == 8'h23) begin uart_send<=1; uart_data_w<=in_pin3;
			end else if (command == 8'h24) begin uart_send<=1; uart_data_w<=in_pin4;
			end else if (command == 8'h25) begin uart_send<=1; uart_data_w<=in_pin5;
			end else if (command == 8'h26) begin uart_send<=1; uart_data_w<=in_pin6;
			end else if (command == 8'h27) begin uart_send<=1; uart_data_w<=in_pin7;

			end else if (command == 8'h30) begin out_pin0<=data;
			end else if (command == 8'h31) begin out_pin1<=data;
			end else if (command == 8'h32) begin out_pin2<=data;
			end else if (command == 8'h33) begin out_pin3<=data;
			end else if (command == 8'h34) begin out_pin4<=data;
			end else if (command == 8'h35) begin out_pin5<=data;
			end else if (command == 8'h36) begin out_pin6<=data;
			end else if (command == 8'h37) begin out_pin7<=data;

			end else if (command == 8'h40) begin reg0<=data;
			end else if (command == 8'h41) begin reg1<=data;
			end else if (command == 8'h42) begin reg2<=data;
			end else if (command == 8'h43) begin reg3<=data;
			end else if (command == 8'h44) begin reg4<=data;
			end else if (command == 8'h45) begin reg5<=data;
			end else if (command == 8'h46) begin reg6<=data;
			end else if (command == 8'h47) begin reg7<=data;

			end else if (command == 8'h50) begin uart_send<=1; uart_data_w<=reg0;
			end else if (command == 8'h51) begin uart_send<=1; uart_data_w<=reg1;
			end else if (command == 8'h52) begin uart_send<=1; uart_data_w<=reg2;
			end else if (command == 8'h53) begin uart_send<=1; uart_data_w<=reg3;
			end else if (command == 8'h54) begin uart_send<=1; uart_data_w<=reg4;
			end else if (command == 8'h55) begin uart_send<=1; uart_data_w<=reg5;
			end else if (command == 8'h56) begin uart_send<=1; uart_data_w<=reg6;
			end else if (command == 8'h57) begin uart_send<=1; uart_data_w<=reg7;
      
			end else if (command == 8'hA0) begin //write sdram
				status <= 1;
				timer8 <= 0;
				out_pin5[0] <= 0;
			end else if (command == 8'hA1) begin //read sdram
				status <= 2;
				timer8 <= 0;
			end else begin
				uart_send<=0;
				/*
pr000
pr100
pr212
pr334
scA0
gr4

pr001
pr100
pr256
pr378
scA0

pr000
pr100
scA1
gr2
gr3
gr4

pr001
pr100
scA1
gr2
gr3


				 */
        
				if         (status == 1) begin
					timer8 <= timer8+1;
					if         (timer8 == 0) begin
						out_pin0 <= reg0;//addr low  sdram_wr_addr = {8'h0,out_pin1,out_pin0};
						out_pin1 <= reg1;//addr hi
						//sdram_rd_addr = {8'h0,out_pin1,out_pin0};
						out_pin2 <= reg2;//data low assign sdram_din = {out_pin3,out_pin2};
						out_pin3 <= reg3;//data hi
					end else if(timer8 == 1) begin
						//sdram_wr_req = out_pin4[0];
						out_pin4[0] = 1;
					end else begin
						//in_pin2[0] = sdram_wr_ack;
						if(in_pin2[0])begin
							reg4 <= timer8;
							out_pin5[0] <= 0;//led1
							status <= 0;
							//sdram_wr_req = out_pin4[0];
							out_pin4[0] = 0;
						end
					end
				end else if(status == 2) begin
					timer8 <= timer8+1;
					if         (timer8 == 0) begin
						out_pin0 <= reg0;//addr low  sdram_wr_addr = {8'h0,out_pin1,out_pin0};
						out_pin1 <= reg1;//addr hi
					end else if(timer8 == 1) begin
						//sdram_rd_req = out_pin4[1];
						out_pin4[1] = 1;
					end else begin
						//in_pin2[1] = sdram_rd_ack;
						if(in_pin2[1])begin
							reg2 <= in_pin0;//data low  in_pin0 = sdram_dout[7:0];
							reg3 <= in_pin1;//data hi   in_pin1 = sdram_dout[15:8];
							reg4 <= timer8;
							out_pin5[1] <= 0;//led2
							status <= 0;
							//sdram_rd_req = out_pin4[1];
							out_pin4[1] = 0;
						end
					end
				end
			end
		end
	end
endmodule


module flow_led(
		input               sys_clk  ,  //系统时钟
		input               sys_rst_n,  //系统复位，低电平有效
		input 		[3:0]			key,
		output    [3:0]  led         //4个LED灯
		);
	 
	wire  [3:0]  key_stable;
	wire slow_clock;
	wire curr_clock;
	wire  [3:0]  ledreg;
	
	assign curr_clock = slow_clock;//slow_clock;//sys_clk;
	assign led = ledreg;
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

	slow_clock(
					.sys_clk        (sys_clk),
					.sys_rst_n      (sys_rst_n),
					.slow_clock     (slow_clock)
				);
	
	
	status_matchin(
					.clk        (curr_clock),
					.rst_n      (sys_rst_n),
					.inpin     (key_stable),
					.outpin     (ledreg)
				);
	/*
	assign led[3] = curr_clock;
	assign led[2:0] = ledreg[2:0];
	always @(posedge curr_clock or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			ledreg[2:0]<= 4'b0;
		end
		else
		begin
			ledreg[2:0] <= key_stable[2:0];
		end
	end 
	*/
endmodule 



module status_matchin(
		input               clk  ,  
		input               rst_n, 
		input 		[3:0]		inpin,
		output  reg  [3:0]  outpin       
);

reg [1:0] status;
reg [3:0] inpinold;//捕获上升沿

	always @(posedge clk or negedge rst_n) begin
		if (!rst_n) begin
			inpinold <= inpin;
		end else begin
			inpinold <= inpin;
		end
	end

	always @(posedge clk or negedge rst_n) begin
		if (!rst_n) begin
			status<=2'b0;
		end else begin
			if         (inpinold[0] != inpin[0] && inpin[0]==0) begin
				status<=0;
			end else if(inpinold[1] != inpin[1] && inpin[1]==0) begin
				status<=1;
			end else if(inpinold[2] != inpin[2] && inpin[2]==0) begin
				status<=2;
			end else if(inpinold[3] != inpin[3] && inpin[3]==0) begin
				status<=3;
			end else begin
				status<=status;
			end
		end
	end
	
	always @(posedge clk or negedge rst_n) begin
		if (!rst_n) begin
			outpin<=0;
		end else begin
			if(status==0) begin
				outpin<=1;
			end else if(status==1) begin
				outpin<=outpin;
			end else if(status==2) begin
				outpin<={outpin[2:0],outpin[3]};
			end else begin
				outpin<={outpin[0],outpin[3:1]};
			end
		end
	end

endmodule

module slow_clock(
		input            sys_clk,
		input            sys_rst_n,
		output reg       slow_clock
		);
	//reg [24:0] counter;//32M
	//localparam zero =25'd0;
	//localparam top =25'd25000000;

	reg [24:0] counter;//32M
	localparam zero =25'd0;
	localparam top =25'd2500000;

	//reg [24:0] counter;//32M
	//localparam zero =25'd0;
	//localparam top =25'd25;
	
	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			counter <= zero;
			slow_clock<= 0;
		end
		else if (counter == top)//0.5s 翻转
		begin
			counter <= zero;
			slow_clock <= !slow_clock;
		end
		else
		begin
			counter <= counter + 1;
			slow_clock <= slow_clock;
		end
	end
endmodule


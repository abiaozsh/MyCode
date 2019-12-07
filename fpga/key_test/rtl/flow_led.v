
module flow_led(
    input               sys_clk  ,  //系统时钟
    input               sys_rst_n,  //系统复位，低电平有效
 input 		[3:0]			key,
    output  reg  [3:0]  led         //4个LED灯
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


always @(posedge sys_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        led<= 4'b0;
		  end
    else
			begin
      led <= key_stable;
		  end

end

endmodule 





module key_debounce(
		input            sys_clk,          //外部50M时钟
		input            sys_rst_n,        //外部复位信号，低有效
		input            key,              //外部按键输入
		output reg       key_value         //按键消抖后的数据  
		);

	//reg define    
	//reg [31:0] counter;
	//localparam zero =32'd0;
	//localparam top =32'd50000000;
	
	reg [19:0] counter;
	localparam zero =20'd0;
	localparam top =20'd1000000;  //给延时计数器重新装载初始值（计数时间为20ms）
	
	always @(posedge sys_clk or negedge sys_rst_n) begin 
		if (!sys_rst_n) begin 
			key_value <= key;
			counter <= zero;
		end else begin
			if(counter==0)begin
				if(key_value!=key) begin
					key_value <= key;
					counter <= counter+1'b1;
				end else begin
					key_value <= key;
					counter<= counter;
				end
			end else if(counter==top)begin
				key_value <= key;
				counter <= zero;
			end else begin
				key_value <= key_value;
				counter <= counter+1'b1;
			end
		end
	end
endmodule 

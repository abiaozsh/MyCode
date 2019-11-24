
module key_debounce(
		input            sys_clk,     
		input            sys_rst_n,   
		input            key,         
		output reg       key_value 
		);

	//reg define    
	//reg [31:0] counter;
	//localparam zero =32'd0;
	//localparam top =32'd50000000;
	
	
	
	reg [19:0] counter;
	localparam zero =20'd0;
	localparam top =20'd1000000;
	
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


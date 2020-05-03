module mytimer (
		input          clk,                // clock.clk
		input          clk_50M,
		input          reset_n,               // reset.reset
    
		input    [1:0] avs_s0_address,     //    s0.address
		input          avs_s0_read,        //      .read
		input          avs_s0_write,       //      .write
		output  [31:0] avs_s0_readdata,    //      .readdata
		input   [31:0] avs_s0_writedata,   //      .writedata
		output         avs_s0_waitrequest, //      .waitrequest
		input   [3:0]  avs_s0_byteenable,    //      .readdata
    input dummy
	);

	// TODO: Auto-generated HDL template

	assign avs_s0_waitrequest = 1'b0;
	
	reg [31:0] timer[4];
	
	assign avs_s0_readdata = timer[avs_s0_address];
	
	reg [5:0] tick;
	reg ack;
	always @ (posedge clk_50M or negedge reset_n) begin
		if (!reset_n) begin
			tick <= 0;
			timer[0] <= 0;
			timer[1] <= 0;
			timer[2] <= 0;
			timer[3] <= 0;
			ack <= 0;
		end else begin
			tick <= tick + 1'b1;
			if(tick==50) begin
				tick<=0;
			end
			if(tick==0)begin
				timer[0]<=timer[0]+1'b1;
				timer[1]<=timer[1]+1'b1;
				timer[2]<=timer[2]+1'b1;
				timer[3]<=timer[3]+1'b1;
			end
			if(req && !ack) begin
				timer[set]<=val;
				ack <= 1;
			end
			if(!req && ack) begin
				ack <= 0;
			end
			
		end
	end

	reg [1:0] set;
	reg [31:0] val;
	reg        req;
	always @ (posedge clk or negedge reset_n) begin
		if (!reset_n) begin
			set <= 0;
			val <= 0;
			req <= 0;
		end else begin
			if(avs_s0_write) begin
				set <= avs_s0_address[1:0];
				val <= avs_s0_writedata;
				req <= 1;
			end
			
			if(ack)begin
				req <= 0;
			end
		end
	end

	
endmodule

module mytimer (
		input          clk,                // clock.clk
		input          reset_n,               // reset.reset
    
		input          avs_s0_address,     //    s0.address
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
	
	reg [31:0] timer;
	
	assign avs_s0_readdata = timer;
	
	reg [5:0] tick;
	reg flg;
	always @ (posedge clk or negedge reset_n) begin
		if (!reset_n) begin
			tick <= 0;
		end else begin
			tick <= tick + 1'b1;
			if(tick==50) begin
				tick<=0;
			end
		end
	end

	always @ (posedge clk or negedge reset_n) begin
		if (!reset_n) begin
			timer <= 0;
		end else begin
			if(avs_s0_write) begin
				timer<=avs_s0_writedata;
			end else begin
				if(tick==0)begin
					timer<=timer+1'b1;
				end
			end
		end
	end

	
endmodule

// mytimer.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module mytimer (
		input  wire        clk,                // clock.clk
		input  wire        reset_n,               // reset.reset
		input  wire        avs_s0_address,     //    s0.address
		input  wire        avs_s0_read,        //      .read
		output wire [31:0] avs_s0_readdata,    //      .readdata
		input  wire        avs_s0_write,       //      .write
		input  wire [31:0] avs_s0_writedata,   //      .writedata
		output wire        avs_s0_waitrequest //      .waitrequest
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

// softspi.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module softspi (
		input  wire        clk,                // clock.clk
		input  wire        reset_n,               // reset.reset
		input  wire [2:0]  avs_s0_address,     //    s0.address
		input  wire        avs_s0_read,        //      .read
		output wire [31:0] avs_s0_readdata,    //      .readdata
		input  wire        avs_s0_write,       //      .write
		input  wire [31:0] avs_s0_writedata,   //      .writedata
		output wire        avs_s0_waitrequest, //      .waitrequest
    input        MISO,
    output   reg MOSI,
    output   reg SCLK,
    output reg [2:0]  SS_n
		
	);

	// TODO: Auto-generated HDL template

	assign avs_s0_waitrequest = 1'b0;
	
	assign avs_s0_readdata[0] = MISO;
	
	always @ (posedge clk or negedge reset_n) begin
		if (!reset_n) begin
			MOSI <= 1;
			SCLK <= 1;
			SS_n <= 3'b111;
		end else begin
		
			if(avs_s0_write) begin
			
				if(avs_s0_address==1) begin
					MOSI<=avs_s0_writedata[0];
				end
				
				if(avs_s0_address==2) begin
					SCLK<=avs_s0_writedata[0];
				end
				
				if(avs_s0_address==3) begin
					SS_n<=avs_s0_writedata[2:0];
				end
			end

		end
	end

endmodule

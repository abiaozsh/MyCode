// mysdram.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module mysdram (
		input  wire [23:0] avs_s0_address,     //    s0.address
		input  wire        avs_s0_read,        //      .read
		output wire [15:0] avs_s0_readdata,    //      .readdata
		input  wire        avs_s0_write,       //      .write
		input  wire [15:0] avs_s0_writedata,   //      .writedata
		output reg         avs_s0_waitrequest, //      .waitrequest
		input  wire        clk,                // clock.clk
		input  wire        reset_n,               // reset.reset
		output reg 			   reset_n_out,
		input wire 			uart_rxd,
		output wire			uart_txd,
    output wire [7:0] debug,
		output wire [15:0] debug2
	);

	// TODO: Auto-generated HDL template

assign avs_s0_readdata = sramtemp_q;


reg control_by_me;
reg [24:0] my_address;
reg [15:0] my_write_data;
reg 			 my_write;

wire [11:0] sramtemp_address;
wire [15:0] sramtemp_data;
wire [15:0] sramtemp_q;
wire        sramtemp_wren;
assign sramtemp_address = control_by_me ? my_address[11:0] : avs_s0_address[11:0];
assign sramtemp_data =    control_by_me ? my_write_data : avs_s0_writedata;     
assign sramtemp_wren =    control_by_me ? my_write : avs_s0_write;              



ram1p	ram1p_inst (
	.address ( sramtemp_address ),
	.clock ( clk ),
	.data ( sramtemp_data ),
	.wren ( sramtemp_wren ),
	.q ( sramtemp_q )
	);

	
//assign debug2[7:0] = sramtemp_q[7:0];
//assign debug2[15:8] = sramtemp_address[7:0];
	
	

wire uart_rec;
wire [7:0] uart_data_out;
reg uart_send;
reg [7:0] uart_data_in;
uart_hs (
	.sys_clk(clk), 
	.sys_rst_n(reset_n),

	.uart_txd(uart_txd),
	.uart_rxd(uart_rxd),
	
	.uart_rec(uart_rec),
	.uart_data_out(uart_data_out),

	.uart_send(uart_send),
	.uart_data_in(uart_data_in)
);



reg [7:0] command;
reg [7:0] data;
reg [7:0] command_temp;
reg data_cmd;//1:data 0:cmd
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin
    command <= 0;
		command_temp <= 0;
    data <= 0;
		data_cmd <= 0;
  end else begin
    if (uart_rec) begin
      if(data_cmd==0)begin
        if(uart_data_out!=0)begin
          data_cmd<=1;
          command_temp<=uart_data_out;
        end
      end else begin
        data_cmd <= 0;
        command <= command_temp;
        data <= uart_data_out;
      end

    end else begin
      if(command_done)begin
        command <= 0;
      end
    end
  end
end

reg [23:0] addr_buff;
reg [15:0] data_buff_to_pc;
reg [15:0] data_buff_from_pc;

assign debug[0] = command!=0;
assign debug[1] = command_done;
assign debug[2] = avs_s0_read;
assign debug[3] = avs_s0_write;
assign debug[4] = reset_n;
assign debug[5] = my_write;
assign debug[6] = avs_s0_read_flip;
assign debug[7] = avs_s0_write_flip;

reg avs_s0_read_flip;
always @(posedge avs_s0_read) begin
	avs_s0_read_flip <= !avs_s0_read_flip;
end
reg avs_s0_write_flip;
always @(posedge avs_s0_write) begin
	avs_s0_write_flip <= !avs_s0_write_flip;
end

reg command_done;
reg [7:0] timer;
//avm_m0_address = 32'b00000000000000000000000000000000;
//avm_m0_writedata = 32'b00000000000000000000000000000000;
//avm_m0_write = 1'b0;
//avm_m0_read = 1'b0;
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin
    control_by_me <= 0;
		avs_s0_waitrequest <= 0;
		my_address <= 0;
		my_write_data <= 0;
		my_write <= 0;

    addr_buff<=0;
    data_buff_from_pc<=0;
    data_buff_to_pc<=0;

    command_done <= 0;
    timer<=0;
		reset_n_out<=1;
  end else begin
    uart_send<=0;
    
    if(command_done)begin
      if          (command == 8'h00) begin 
        command_done<=0;
      end
    end else begin//command_done==0
      if          (command == 8'h00) begin

      end else if (command == 8'h10) begin addr_buff[ 7: 0]<=data; command_done<=1;
      end else if (command == 8'h11) begin addr_buff[15: 8]<=data; command_done<=1;
      end else if (command == 8'h12) begin addr_buff[23:16]<=data; command_done<=1;

      end else if (command == 8'h14) begin data_buff_from_pc[ 7: 0]<=data; command_done<=1;
      end else if (command == 8'h15) begin data_buff_from_pc[15: 8]<=data; command_done<=1;

      end else if (command == 8'h20) begin uart_send<=1; uart_data_in<=data_buff_to_pc[ 7: 0]; command_done<=1;
      end else if (command == 8'h21) begin uart_send<=1; uart_data_in<=data_buff_to_pc[15: 8]; command_done<=1;

      end else if (command == 8'h40) begin reset_n_out<=1;control_by_me <= 0; command_done<=1;
      end else if (command == 8'h41) begin reset_n_out<=0;control_by_me <= 1; command_done<=1;

      end else if (command == 8'h30) begin//bus read
				timer<=timer+1'b1;
				if(timer==0)begin
					
					avs_s0_waitrequest <= 1;
					my_address <= addr_buff;
				end else if(timer==2)begin
						data_buff_to_pc <= sramtemp_q;
						timer<=0;
						
						avs_s0_waitrequest <= 0;
						command_done<=1;
				end

      end else if (command == 8'h31) begin//bus write
				timer<=timer+1'b1;
				if(timer==0)begin
					//control_by_me <= 1;
					avs_s0_waitrequest <= 1;
					my_address <= addr_buff;
					my_write_data <= data_buff_from_pc;
					my_write <= 1;
				end else begin
						timer<=0;
						//control_by_me <= 0;
						avs_s0_waitrequest <= 0;
						my_write <= 0;
						command_done<=1;
				end

        
      end else begin
        command_done<=1;
        uart_send<=0;
      end
    end



  end
end





	
	
	
	
	
	
	
	
	
endmodule

module myuart (
		input         clk,                // clock.clk
		input         reset_n,              // reset.reset

		input         avs_s0_address,     //    s0.address 0 receive 1 send
		input         avs_s0_read,        //      .read
		input         avs_s0_write,       //      .write
		output [31:0] avs_s0_readdata,    //      .readdata
		input  [31:0] avs_s0_writedata,   //      .writedata
		output        avs_s0_waitrequest, //      .waitrequest
		input  [3:0]  avs_s0_byteenable,    //      .readdata
		output        ins_irq0_irq,       //  irq0.irq

		input     uart_rxd,
		output    uart_txd

	);

	// TODO: Auto-generated HDL template

	assign avs_s0_waitrequest = 1'b0;

	assign avs_s0_readdata = avs_s0_address == 0 ? ({23'b0,rec_flg,rec_data}) : ({23'b0,uart_busy,8'b0});

	assign ins_irq0_irq = 1'b0;

  reg rec_flg;
  reg [7:0] rec_data;
  always @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      rec_flg <= 0;
    end else begin
      if(rec_flg == 1 && avs_s0_read && avs_s0_address == 0)begin
        rec_flg <= 0;
      end
    
      if(uart_rec)begin
        rec_flg <= 1;
        rec_data <= uart_data_out;
      end
      
      uart_send <= 0;
      if(avs_s0_write && avs_s0_address == 1)begin
        uart_send <= 1;
        uart_data_in <= avs_s0_writedata[7:0];
      end
      
    end
  end

	wire uart_rec;
	wire [7:0] uart_data_out;
	reg uart_send;
	reg [7:0] uart_data_in;
  wire uart_busy;
	uart_hs uart_hs_inst (
	  .sys_clk(clk), 
	  .sys_rst_n(reset_n),

	  .uart_txd(uart_txd),
	  .uart_rxd(uart_rxd),
	  
	  .uart_rec(uart_rec),
	  .uart_data_out(uart_data_out),

	  .uart_send(uart_send),
	  .uart_data_in(uart_data_in),
    .busy(uart_busy)
	);
  
endmodule

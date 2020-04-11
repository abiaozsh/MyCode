module uart_mcu(
    input         clk,                // clock.clk
    input         reset_n,              // reset.reset
		
    input       uart_rxd,
    output      uart_txd,
    output [7:0] debug8,
    output [31:0] debug32,
	 
output reg c3_p0_cmd_en,
output reg cmd_rw,
output reg [5:0] c3_p0_cmd_bl,
output reg [29:0] c3_p0_cmd_byte_addr,

output reg c3_p0_wr_en,
output reg [3:0] c3_p0_wr_mask,
output reg [31:0] c3_p0_wr_data,

output reg c3_p0_rd_en,
input wire [31:0] c3_p0_rd_data,
input wire c3_p0_rd_empty

  );

wire uart_rec;
wire [7:0] uart_data_out;
reg uart_send;
reg [7:0] uart_data_in;
uart_hs uart_hs_inst (
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
    data <= 0;
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


reg command_done;

reg  [2:0] debug_readmem_step;
reg        debug_step;
reg  [31:0] debug_data;


reg halt_uart;
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin

    c3_p0_cmd_en <= 0;
    cmd_rw <= 1;
    c3_p0_cmd_bl <= 0;//[5:0]
    c3_p0_cmd_byte_addr <= 0;//[29:0]

    c3_p0_wr_en <= 0;
    c3_p0_wr_mask <= 0;//[3:0] 
    c3_p0_wr_data <= 0;//[31:0] 
    c3_p0_rd_en <= 0;

    command_done <= 0;
    debug_step<=0;
    debug_readmem_step<=0;

  end else begin
    uart_send<=0;
    
    if(command_done)begin
      if          (command == 8'h00) begin 
        command_done<=0;
      end
    end else begin//command_done==0
      if          (command == 8'h00) begin

      //end else if (command == 8'h01) begin debug8 <= data; command_done<=1;

      end else if (command == 8'h10) begin uart_send<=1; uart_data_in<=debug_data[ 7: 0]; command_done<=1;
      end else if (command == 8'h11) begin uart_send<=1; uart_data_in<=debug_data[15: 8]; command_done<=1;
      end else if (command == 8'h12) begin uart_send<=1; uart_data_in<=debug_data[23:16]; command_done<=1;
      end else if (command == 8'h13) begin uart_send<=1; uart_data_in<=debug_data[31:24]; command_done<=1;
      
      end else if (command == 8'h20) begin c3_p0_cmd_byte_addr[ 7: 0] <= data; command_done<=1;
      end else if (command == 8'h21) begin c3_p0_cmd_byte_addr[15: 8] <= data; command_done<=1;
      end else if (command == 8'h22) begin c3_p0_cmd_byte_addr[23:16] <= data; command_done<=1;
      end else if (command == 8'h23) begin c3_p0_cmd_byte_addr[29:24] <= data; command_done<=1;
		
      end else if (command == 8'h24) begin c3_p0_wr_data[ 7: 0] <= data; command_done<=1;
      end else if (command == 8'h25) begin c3_p0_wr_data[15: 8] <= data; command_done<=1;
      end else if (command == 8'h26) begin c3_p0_wr_data[23:16] <= data; command_done<=1;
      end else if (command == 8'h27) begin c3_p0_wr_data[31:24] <= data; command_done<=1;

      end else if (command == 8'h30) begin c3_p0_cmd_bl <= data[5:0]; command_done<=1;
      end else if (command == 8'h31) begin c3_p0_wr_mask <= data[3:0]; command_done<=1;

      end else if (command == 8'h50) begin
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          c3_p0_cmd_en <= 1;
          cmd_rw <= 1;
        end else if(debug_readmem_step==1)begin
          c3_p0_cmd_en <= 0;
          if(!c3_p0_rd_empty)begin
				c3_p0_rd_en <= 1;
            debug_data <= c3_p0_rd_data;
            debug_readmem_step <= 2;
          end
        end else if(debug_readmem_step==2)begin
		    debug_readmem_step <= 0;
          c3_p0_rd_en <= 0;
          command_done <= 1;
        end
		  
      end else if (command == 8'h51) begin
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          c3_p0_wr_en <= 1;
			 //c3_p0_wr_data = uartdata
        end else if(debug_readmem_step==1)begin
          debug_readmem_step <= 2;
          c3_p0_wr_en <= 0;
        end else if(debug_readmem_step==2)begin
          debug_readmem_step <= 3;
			 c3_p0_cmd_en <= 1;
			 cmd_rw <= 0;
        end else if(debug_readmem_step==3)begin
          debug_readmem_step <= 0;
          c3_p0_cmd_en <= 0;
			 command_done <= 1;
        end

      end else begin
        command_done<=1;
      end
    end


  end
end



endmodule

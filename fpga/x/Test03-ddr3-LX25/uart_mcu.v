module uart_mcu(
    input         clk,                // clock.clk
    input         reset_n,              // reset.reset

    input       uart_rxd,
    output      uart_txd,
    output [7:0] debug8,
    output [31:0] debug32,

    output reg c3_p0_cmd_en,
    input      c3_p0_cmd_full,
    output reg c3_p0_cmd_rw,
    output reg [5:0] c3_p0_cmd_bl,
    output reg [29:0] c3_p0_cmd_byte_addr,

    output reg c3_p0_wr_en,
    input      c3_p0_wr_full,
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

reg [31:0] addr;
reg [31:0] dataFromPC;
reg [31:0] dataToPC;

reg halt_uart;
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin

    c3_p0_cmd_en <= 0;
    c3_p0_cmd_rw <= 1;
    c3_p0_cmd_bl <= 0;//[5:0]
    c3_p0_cmd_byte_addr <= 0;//[29:0]

    c3_p0_wr_en <= 0;
    c3_p0_wr_mask <= 0;//[3:0] 
    c3_p0_wr_data <= 0;//[31:0] 
    c3_p0_rd_en <= 0;

    command_done <= 0;
    debug_step<=0;
    debug_readmem_step<=0;

    addr <= 0;
    dataFromPC <= 0;
    dataToPC <= 0;

  end else begin
    uart_send<=0;
    
    if(command_done)begin
      if          (command == 8'h00) begin 
        command_done<=0;
      end
    end else begin//command_done==0
      if          (command == 8'h00) begin

      //end else if (command == 8'h01) begin debug8 <= data; command_done<=1;

      end else if (command == 8'h10) begin uart_send<=1; uart_data_in<=dataToPC[ 7: 0]; command_done<=1;
      end else if (command == 8'h11) begin uart_send<=1; uart_data_in<=dataToPC[15: 8]; command_done<=1;
      end else if (command == 8'h12) begin uart_send<=1; uart_data_in<=dataToPC[23:16]; command_done<=1;
      end else if (command == 8'h13) begin uart_send<=1; uart_data_in<=dataToPC[31:24]; command_done<=1;

      end else if (command == 8'h20) begin addr[ 7: 0] <= data; command_done<=1;
      end else if (command == 8'h21) begin addr[15: 8] <= data; command_done<=1;
      end else if (command == 8'h22) begin addr[23:16] <= data; command_done<=1;
      end else if (command == 8'h23) begin addr[29:24] <= data; command_done<=1;

      end else if (command == 8'h24) begin dataFromPC[ 7: 0] <= data; command_done<=1;
      end else if (command == 8'h25) begin dataFromPC[15: 8] <= data; command_done<=1;
      end else if (command == 8'h26) begin dataFromPC[23:16] <= data; command_done<=1;
      end else if (command == 8'h27) begin dataFromPC[31:24] <= data; command_done<=1;

      end else if (command == 8'h30) begin c3_p0_cmd_bl <= data[5:0]; command_done<=1;
      end else if (command == 8'h31) begin c3_p0_wr_mask <= data[3:0]; command_done<=1;//c3_p0_wr_mask 1:block data
		
      end else if (command == 8'h32) begin uart_send<=1; uart_data_in<=c3_p0_cmd_full; command_done<=1;
      end else if (command == 8'h33) begin uart_send<=1; uart_data_in<=c3_p0_wr_full; command_done<=1;
      end else if (command == 8'h34) begin uart_send<=1; uart_data_in<=c3_p0_cmd_full; command_done<=1;
      end else if (command == 8'h35) begin uart_send<=1; uart_data_in<=c3_p0_cmd_full; command_done<=1;
      end else if (command == 8'h36) begin uart_send<=1; uart_data_in<=c3_p0_cmd_full; command_done<=1;
      end else if (command == 8'h37) begin uart_send<=1; uart_data_in<=c3_p0_cmd_full; command_done<=1;

      //ddr read
      end else if (command == 8'h50) begin

        if         (debug_readmem_step==0)begin
          if(!c3_p0_cmd_full)begin
            debug_readmem_step <= 1;
            c3_p0_cmd_byte_addr <= addr;
            c3_p0_cmd_en <= 1;
            c3_p0_cmd_rw <= 1;
          end
        end else if(debug_readmem_step==1)begin
          c3_p0_cmd_en <= 0;
          if(!c3_p0_rd_empty)begin
            c3_p0_rd_en <= 1;
            dataToPC <= c3_p0_rd_data;
            debug_readmem_step <= 2;
          end
        end else if(debug_readmem_step==2)begin
          debug_readmem_step <= 0;
          c3_p0_rd_en <= 0;
          command_done <= 1;
        end
		/*
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
			 c3_p0_cmd_byte_addr <= addr;
          c3_p0_cmd_en <= 1;
          c3_p0_cmd_rw <= 1;
        end else if(debug_readmem_step==1)begin
          c3_p0_cmd_en <= 0;
          if(!c3_p0_rd_empty)begin
				c3_p0_rd_en <= 1;
            dataToPC <= c3_p0_rd_data;
            debug_readmem_step <= 2;
          end
        end else if(debug_readmem_step==2)begin
		    debug_readmem_step <= 0;
          c3_p0_rd_en <= 0;
          command_done <= 1;
        end
*/
      //ddr write
      end else if (command == 8'h51) begin
		
        if         (debug_readmem_step==0)begin
          if(!c3_p0_cmd_full && !c3_p0_wr_full)begin
            debug_readmem_step <= 1;
            c3_p0_wr_data <= dataFromPC;
            c3_p0_cmd_byte_addr <= addr;
            c3_p0_wr_en <= 1;
            c3_p0_cmd_rw <= 0;
            c3_p0_cmd_en <= 1;
          end
        end else if(debug_readmem_step==1)begin
          debug_readmem_step <= 0;
          c3_p0_wr_en <= 0;
          c3_p0_cmd_en <= 0;
          command_done <= 1;
        end
		  /*
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          c3_p0_wr_data <= dataFromPC;
          c3_p0_wr_en <= 1;
			 //c3_p0_wr_data = uartdata
        end else if(debug_readmem_step==1)begin
          debug_readmem_step <= 2;
          c3_p0_wr_en <= 0;
        end else if(debug_readmem_step==2)begin
          debug_readmem_step <= 3;
          c3_p0_cmd_byte_addr <= addr;
			 c3_p0_cmd_en <= 1;
			 c3_p0_cmd_rw <= 0;
        end else if(debug_readmem_step==3)begin
          debug_readmem_step <= 0;
          c3_p0_cmd_en <= 0;
			 command_done <= 1;
        end
*/


      //sram read
      end else if (command == 8'h52) begin
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          addra <= addr[7:0];
        end else if(debug_readmem_step==1)begin
          debug_readmem_step <= 2;
        end else if(debug_readmem_step==2)begin
          dataToPC <= {16'b0,douta};
          debug_readmem_step <= 0;
          command_done <= 1;
        end

      //sram write
      end else if (command == 8'h53) begin
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          dina <= dataFromPC[15:0];
          addra <= addr[7:0];
          wea <= 2'b11;
        end else if(debug_readmem_step==1)begin
          debug_readmem_step <= 0;
          c3_p0_wr_en <= 0;
          c3_p0_cmd_en <= 0;
          wea <= 2'b0;
          command_done <= 1;
        end









      end else begin
        command_done<=1;
      end
    end


  end
end





wire clka = clk;
reg [1 : 0] wea;
reg [9 : 0] addra;
reg [15 : 0] dina;
wire [15 : 0] douta;

sram(
  .clka(clka),
  .wea(wea),
  .addra(addra),
  .dina(dina),
  .douta(douta)
);



endmodule

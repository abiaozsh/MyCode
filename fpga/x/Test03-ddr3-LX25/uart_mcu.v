module uart_mcu(
    input         clk,                // clock.clk
    input         reset_n,              // reset.reset

    input       uart_rxd,
    output      uart_txd,
    output [7:0] debug8,
    output [31:0] debug32,
    input [31:0] debugin32,
    
    output reg c3_p0_cmd_en,
    input      c3_p0_cmd_full,
    input      c3_p0_wr_empty,
    output reg c3_p0_cmd_rw,
    output reg [5:0] c3_p0_cmd_bl,
    output reg [29:0] c3_p0_cmd_byte_addr,

    output reg c3_p0_wr_en,
    input      c3_p0_wr_full,
    output reg [3:0] c3_p0_wr_mask,
    output reg [31:0] c3_p0_wr_data,

    output reg c3_p0_rd_en,
    input wire [31:0] c3_p0_rd_data,
    input wire c3_p0_rd_empty,
    input wire c3_p0_rd_full,
    input wire[6:0]		  c3_p0_rd_count,
    input wire				  c3_p0_rd_overflow,
    input wire				  c3_p0_rd_error


  );

reg uart_send;
wire uart_rec_req;
reg  uart_rec_ack;
wire [7:0] uart_data_out;
wire uart_send_req = uart_send;
reg [7:0] uart_data_in;
uart_hs uart_hs_inst (
	.sys_clk(clk),//clk_50M
	.sys_rst_n(reset_n),

	.uart_txd(uart_txd),
	.uart_rxd(uart_rxd),
	
	.uart_rec_req(uart_rec_req),
	.uart_rec_ack(uart_rec_ack),
	.uart_data_out(uart_data_out),

	.uart_send_req(uart_send_req),
	.uart_send_ack(uart_send_ack),
	.uart_data_in(uart_data_in)
);

reg [7:0] command;
reg [7:0] data;
reg [7:0] command_temp;
reg data_cmd;//1:data 0:cmd
reg uart_rec_req_buff;
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin
    command <= 0;
    data <= 0;
		uart_rec_req_buff<=0;
		uart_rec_ack<=0;
  end else begin
		uart_rec_req_buff <= uart_rec_req;
	
    if (uart_rec_req_buff && !uart_rec_ack) begin 
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
			uart_rec_ack <= 1;
    end
		
    if (!uart_rec_req_buff && uart_rec_ack) begin 
			uart_rec_ack <= 0;
    end
		
		if(command_done)begin
			command <= 0;
		end
  end
end


reg command_done;

reg  [2:0] debug_readmem_step;
reg        debug_step;

reg [31:0] addr;
reg [31:0] dataFromPC;
reg [31:0] dataToPC;

reg [31:0] count;
reg halt_uart;
reg uart_send_ack_buff;
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
    
		uart_send_ack_buff<=0;
    count <= 0;
  end else begin
    uart_send_ack_buff <= uart_send_ack;

		if(uart_send_ack_buff)begin
			uart_send<=0;
		end
    
    if(command_done)begin
      if          (command == 8'h00) begin 
        command_done<=0;
      end
    end else begin//command_done==0
      if          (command == 8'h00) begin

      end else if (command == 8'h01) begin uart_send<=1; uart_data_in<=123; command_done<=1;

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

      end else if (command == 8'h28) begin c3_p0_cmd_bl <= data[5:0]; command_done<=1;
      end else if (command == 8'h29) begin c3_p0_wr_mask <= data[3:0]; command_done<=1;//c3_p0_wr_mask 1:block data
		
      end else if (command == 8'h32) begin uart_send<=1; uart_data_in<=c3_p0_cmd_full   ; command_done<=1;
      end else if (command == 8'h33) begin uart_send<=1; uart_data_in<=c3_p0_wr_full    ; command_done<=1;
      end else if (command == 8'h34) begin uart_send<=1; uart_data_in<=c3_p0_rd_full    ; command_done<=1;
      end else if (command == 8'h35) begin uart_send<=1; uart_data_in<=c3_p0_rd_empty   ; command_done<=1;
      end else if (command == 8'h36) begin uart_send<=1; uart_data_in<=c3_p0_rd_count   ; command_done<=1;
      end else if (command == 8'h37) begin uart_send<=1; uart_data_in<=c3_p0_rd_overflow; command_done<=1;
      end else if (command == 8'h38) begin uart_send<=1; uart_data_in<=c3_p0_rd_error   ; command_done<=1;

      end else if (command == 8'h39) begin dataToPC<=debugin32; command_done<=1;

      

      //ddr read
      end else if (command == 8'h30) begin

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
          uart_send<=1; uart_data_in<=123;
        end

      //ddr write
      end else if (command == 8'h31) begin
		
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
          uart_send<=1; uart_data_in<=123;
        end

        
      end else if (command == 8'h60) begin

        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          c3_p0_cmd_byte_addr <= addr;
          c3_p0_cmd_en <= 1;
          c3_p0_cmd_rw <= 1;
        end else if(debug_readmem_step==1)begin
          c3_p0_cmd_en <= 0;
          debug_readmem_step <= 0;
          command_done <= 1;
        end
        
      end else if (command == 8'h61) begin
        if         (debug_readmem_step==0)begin
          c3_p0_rd_en <= 1;
          dataToPC <= c3_p0_rd_data;
          debug_readmem_step <= 1;
        end else if(debug_readmem_step==1)begin
          debug_readmem_step <= 0;
          c3_p0_rd_en <= 0;
          command_done <= 1;
        end



      //sram read
      end else if (command == 8'h62) begin
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
      end else if (command == 8'h63) begin
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



      //连续读取测试

      //ddr read
      end else if (command == 8'h64) begin

        if         (debug_readmem_step==0)begin
          if(!c3_p0_cmd_full)begin
            debug_readmem_step <= 1;
            c3_p0_cmd_byte_addr <= {count[19:0],count[31:20]};//addr;
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
          if(count==65536)begin
            uart_send<=1; uart_data_in<=123;
            command_done <= 1;
            count <= 0;
          end else begin
            count<=count+1'b1;
          end
        end

      //ddr write
      end else if (command == 8'h65) begin
		
        if         (debug_readmem_step==0)begin
          if(!c3_p0_cmd_full && !c3_p0_wr_full)begin
            debug_readmem_step <= 1;
            c3_p0_wr_data <= dataFromPC;
            c3_p0_cmd_byte_addr <= count;
            c3_p0_wr_en <= 1;
            c3_p0_cmd_rw <= 0;
            c3_p0_cmd_en <= 1;
          end
        end else if(debug_readmem_step==1)begin
          c3_p0_wr_en <= 0;
          c3_p0_cmd_en <= 0;
          if(c3_p0_wr_empty)begin
            debug_readmem_step <= 0;
            if(count==65536)begin
              uart_send<=1; uart_data_in<=123;
              command_done <= 1;
              count <= 0;
            end else begin
              count<=count+1'b1;
            end
          end
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

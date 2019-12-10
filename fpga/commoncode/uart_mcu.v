module uart_mcu(
		input  sys_clk  ,  
		input  sys_rst_n, 
    
		input  uart_rxd,
		output uart_txd,
    
		output reg out_clk,
		output reg out_rst,

		input [7:0] in_pin0,
		input [7:0] in_pin1,
		input [7:0] in_pin2,
		input [7:0] in_pin3,
		input [7:0] in_pin4,
		input [7:0] in_pin5,
		input [7:0] in_pin6,
		input [7:0] in_pin7,

		output reg [7:0] out_pin0,
		output reg [7:0] out_pin1,
		output reg [7:0] out_pin2,
		output reg [7:0] out_pin3,
		output reg [7:0] out_pin4,
		output reg [7:0] out_pin5,
		output reg [7:0] out_pin6,
		output reg [7:0] out_pin7,

    input  sdram_clk,
    output reg        sdram_wr_req  ,
    output reg        sdram_rd_req  ,
    output reg [15:0] sdram_wr_addr ,
    output reg [15:0] sdram_rd_addr ,
    output reg [9:0]  sdram_wr_burst,
    output reg [9:0]  sdram_rd_burst,
    output reg [15:0] sdram_din     

		);
	reg uart_done_old;
	reg [7:0] command;
	reg [7:0] cmd_temp;
	reg [7:0] data;

	wire uart_done;
	wire [7:0] uart_data_r;
	reg uart_send;
	reg [7:0] uart_data_w;
	
	reg [7:0] uw_reg0;
	reg [7:0] uw_reg1;
	reg [7:0] uw_reg2;
	reg [7:0] uw_reg3;
	reg [7:0] uw_reg4;
	reg [7:0] uw_reg5;
	reg [7:0] uw_reg6;
	reg [7:0] uw_reg7;
  
	reg [7:0] ur_reg0;
	reg [7:0] ur_reg1;
	reg [7:0] ur_reg2;
	reg [7:0] ur_reg3;
	reg [7:0] ur_reg4;
	reg [7:0] ur_reg5;
	reg [7:0] ur_reg6;
	reg [7:0] ur_reg7;
  
	///////////
	reg [3:0]  sdram_cmd;
	reg        sdram_status;
	//reg        sdram_status_before;
  reg        sdram_status_rst;
	reg [7:0]  sdram_timer8;
	reg [31:0] sdram_timer32;
  reg [7:0]  sdram_write_count;
 
	///////////
	uart_hs (
			.sys_clk        (sys_clk), 
			.sys_rst_n      (sys_rst_n),
			.uart_rxd       (uart_rxd),
			.uart_txd       (uart_txd),
			
			.uart_done      (uart_done),
			.uart_data_out  (uart_data_r),
			.uart_en        (uart_send),
			.uart_data_in   (uart_data_w)
		);
  

	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			uart_done_old <= uart_done;
		end else begin
			uart_done_old <= uart_done;
		end
	end
	
	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			command <= 0;
			cmd_temp <= 0;
			data <= 0;
		end else begin
			if (uart_done_old != uart_done && uart_done==1) begin //串口数据到达  
				if(cmd_temp == 0) begin
					command <= 0;
					cmd_temp <= uart_data_r;
					data <= 0;
				end else begin
					command <= cmd_temp;
					cmd_temp <= 0;
					data <= uart_data_r;
				end
			end else begin
				command <= 0;
				cmd_temp <= cmd_temp;
				data <= 0;
			end
		end
	end
	
	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			out_pin0<=0;
			out_pin1<=0;
			out_pin2<=0;
			out_pin3<=0;
			out_pin4<=0;
			out_pin5<=0;
			out_pin6<=0;
			out_pin7<=0;
			uart_send<=0;
			uart_data_w<=0;
			out_clk<=0;
			out_rst<=0;
		end else begin
			if          (command == 8'h10) begin out_clk<=1;
			end else if (command == 8'h11) begin out_clk<=0;
			end else if (command == 8'h12) begin out_rst<=1;
			end else if (command == 8'h13) begin out_rst<=0;

			end else if (command == 8'h20) begin uart_send<=1; uart_data_w<=in_pin0;
			end else if (command == 8'h21) begin uart_send<=1; uart_data_w<=in_pin1;
			end else if (command == 8'h22) begin uart_send<=1; uart_data_w<=in_pin2;
			end else if (command == 8'h23) begin uart_send<=1; uart_data_w<=in_pin3;
			end else if (command == 8'h24) begin uart_send<=1; uart_data_w<=in_pin4;
			end else if (command == 8'h25) begin uart_send<=1; uart_data_w<=in_pin5;
			end else if (command == 8'h26) begin uart_send<=1; uart_data_w<=in_pin6;
			end else if (command == 8'h27) begin uart_send<=1; uart_data_w<=in_pin7;

			end else if (command == 8'h30) begin out_pin0<=data;
			end else if (command == 8'h31) begin out_pin1<=data;
			end else if (command == 8'h32) begin out_pin2<=data;
			end else if (command == 8'h33) begin out_pin3<=data;
			end else if (command == 8'h34) begin out_pin4<=data;
			end else if (command == 8'h35) begin out_pin5<=data;
			end else if (command == 8'h36) begin out_pin6<=data;
			end else if (command == 8'h37) begin out_pin7<=data;

			end else if (command == 8'h40) begin uw_reg0<=data;
			end else if (command == 8'h41) begin uw_reg1<=data;
			end else if (command == 8'h42) begin uw_reg2<=data;
			end else if (command == 8'h43) begin uw_reg3<=data;
			end else if (command == 8'h44) begin uw_reg4<=data;
			end else if (command == 8'h45) begin uw_reg5<=data;
			end else if (command == 8'h46) begin uw_reg6<=data;
			end else if (command == 8'h47) begin uw_reg7<=data;

			end else if (command == 8'h50) begin uart_send<=1; uart_data_w<=ur_reg0;
			end else if (command == 8'h51) begin uart_send<=1; uart_data_w<=ur_reg1;
			end else if (command == 8'h52) begin uart_send<=1; uart_data_w<=ur_reg2;
			end else if (command == 8'h53) begin uart_send<=1; uart_data_w<=ur_reg3;
			end else if (command == 8'h54) begin uart_send<=1; uart_data_w<=ur_reg4;
			end else if (command == 8'h55) begin uart_send<=1; uart_data_w<=ur_reg5;
			end else if (command == 8'h56) begin uart_send<=1; uart_data_w<=ur_reg6;
			end else if (command == 8'h57) begin uart_send<=1; uart_data_w<=ur_reg7;
      
			end else if (command == 8'hA0) begin //write sdram
        sdram_cmd <= 1;
        sdram_status <= 1;
			end else if (command == 8'hA1) begin //read sdram
        sdram_cmd <= 2;
        sdram_status <= 1;
			end else if (command == 8'hB0) begin //write 2word sdram
        sdram_cmd <= 10;
        sdram_status <= 1;
			end else begin
				uart_send<=0;
        if(sdram_status_rst)begin
          sdram_status <= 0;
        end
			end
		end
	end
  
  

//always@(posedge sdram_clk or negedge rst_n) begin
//	if(!rst_n) begin
//    sdram_status_before <= 0;
//	end else begin
//    sdram_status_before <= sdram_status;
//  end
//end


/*
pr000
pr100
pr212
pr334
scA0
gr4

pr001
pr100
pr256
pr378
scA0

pr000
pr100
scA1
gr2
gr3
gr4

pr001
pr100
scA1
gr2
gr3

03  1234  0e  5678

//////////////////////////////////////////////////////////////////////////////////

pr000
pr100
pr412
pr534
pr656
pr778
scB0
gr4//19

pr000
pr100
scA1
gr2//78
gr3//00

pr001
pr100
scA1
gr2//78
gr3//00

pr002
pr100
scA1
gr2//2a
gr3//3f

pr003
pr100
scA1
gr2//3f
gr3//3f
gr4//0e

0734005600780078000a

19 78 00 78 00 2a 3f 3f 3f 0e

*/

//sdram 读写请求信号产生模块
always@(posedge sdram_clk or negedge sys_rst_n) begin
	if(!sys_rst_n) begin
    sdram_timer8 = 0;
	end else begin
    if(sdram_status)begin
      if(sdram_status_rst)begin
        //执行完 已通知uart
        sdram_timer8 <= 0;
      end else begin
        //执行中
        sdram_timer8 <= sdram_timer8+1;
      
        //write
        if         (sdram_cmd == 1) begin
					if         (sdram_timer8 == 0) begin
						sdram_wr_addr <= {8'h0,uw_reg1,uw_reg0};
            sdram_din     <= {uw_reg3,uw_reg2};
            sdram_wr_burst <= 1;
					end else if(sdram_timer8 == 1) begin
						sdram_wr_req <= 1;
					end else begin
						if(in_pin2[0])begin//in_pin2[0] = sdram_wr_ack;
							ur_reg4 <= sdram_timer8;
							sdram_wr_req <= 0;
							sdram_status_rst <= 1;
						end
					end
        //read
				end else if(sdram_cmd == 2) begin
					if         (sdram_timer8 == 0) begin
            sdram_rd_addr <= {8'h0,uw_reg1,uw_reg0};
            sdram_rd_burst <= 1;
					end else if(sdram_timer8 == 1) begin
						sdram_rd_req = 1;
					end else begin
						if(in_pin2[1])begin//in_pin2[1] = sdram_rd_ack;
							ur_reg2 <= in_pin0;//data low  in_pin0 = sdram_dout[7:0];
							ur_reg3 <= in_pin1;//data hi   in_pin1 = sdram_dout[15:8];
							ur_reg4 <= sdram_timer8;
							sdram_rd_req = 0;
							sdram_status_rst <= 1;
						end
					end
          
        //write 2word
				end else if(sdram_cmd == 10) begin
					if         (sdram_timer8 == 0) begin
						sdram_wr_addr <= {8'h0,uw_reg1,uw_reg0};
            sdram_din     <= {8'h0,uw_reg4};
            sdram_wr_burst <= 4;//sdram_wr_burst
            sdram_write_count <= 0;
					end else if(sdram_timer8 == 1) begin
						sdram_wr_req <= 1;
					end else begin
						if(in_pin2[0])begin//in_pin2[0] = sdram_wr_ack;
              sdram_write_count <= sdram_write_count + 1;
              if         (sdram_write_count==0)begin
                sdram_din     <= {8'h0,uw_reg5};
              end else if(sdram_write_count==1)begin
                sdram_din     <= {8'h0,uw_reg6};
              end else if(sdram_write_count==2)begin
                sdram_din     <= {8'h0,uw_reg7};
              end else if(sdram_write_count==3)begin
                ur_reg4 <= sdram_timer8;
                sdram_wr_req <= 0;
                sdram_status_rst <= 1;
              end else begin
              end
						end
					end

				end else begin


        end

        
        
      end
    end else begin
      if(sdram_status_rst)begin
        //已通知到uart
        sdram_status_rst = 0;
        sdram_timer8 <= 0;
      end else begin
        //待机
        sdram_timer8 <= 0;
      end
    end





          
          
        
          
  
  end
end

endmodule

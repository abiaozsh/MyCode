
module uart_hs(
    input sys_clk,
    input sys_rst_n,
    
    input  uart_rxd,                 //UART接收端口
    output uart_txd,                  //UART发送端口
    
    output            uart_rec,                //接收一帧数据完成标志信号 上升沿表示接收到
    output      [7:0] uart_data_out,                 //接收的数据
    input             uart_send,                  //发送使能信号
    input       [7:0] uart_data_in                 //待发送数据
);
	//parameter define
	localparam BPS_CNT  = 25;        //2000000 bps  50000000/2000000
	localparam BPS_CNT_HALF  = 12;        //2000000 bps  50000000/2000000

/*

	//reg define
	reg        uart_send_d0; 
	reg        uart_send_d1;  
	reg [ 4:0] clk_cnt;                         //系统时钟计数器
	reg [ 3:0] tx_cnt;                          //发送数据计数器
	reg        tx_flag;                         //发送过程标志信号
	reg [ 7:0] tx_data;                         //寄存发送数据

	//wire define
	wire       en_flag;

	assign en_flag = (~uart_send_d1) & uart_send_d0;
                                                 
	//对发送使能信号uart_send延迟两个时钟周期
	always @(posedge sys_clk or negedge sys_rst_n) begin         
		if (!sys_rst_n) begin
			uart_send_d0 <= 0;                                  
			uart_send_d1 <= 0;
		end                                                      
		else begin                                               
			uart_send_d0 <= uart_send;                               
			uart_send_d1 <= uart_send_d0;                            
		end
	end

	//当脉冲信号en_flag到达时,寄存待发送的数据，并进入发送过程          
	always @(posedge sys_clk or negedge sys_rst_n) begin         
		if (!sys_rst_n) begin                                  
			tx_flag <= 1'b0;
			tx_data <= 8'd0;
		end 
		else if (en_flag) begin                 //检测到发送使能上升沿                      
			tx_flag <= 1'b1;                //进入发送过程，标志位tx_flag拉高
			tx_data <= uart_data_in;            //寄存待发送的数据
		end
		else 
		if ((tx_cnt == 4'd9)&&(clk_cnt == BPS_CNT_HALF))
		begin                               //计数到停止位中间时，停止发送过程
			tx_flag <= 1'b0;                //发送过程结束，标志位tx_flag拉低
			tx_data <= 8'd0;
		end
	end

	//进入发送过程后，启动系统时钟计数器与发送数据计数器
	always @(posedge sys_clk or negedge sys_rst_n) begin         
		if (!sys_rst_n) begin                             
			clk_cnt <= 5'd0;                                  
			tx_cnt  <= 4'd0;
		end                                                      
		else if (tx_flag) begin                 //处于发送过程
			if (clk_cnt < BPS_CNT - 1) begin
				clk_cnt <= clk_cnt + 1'b1;
			end
			else begin
				clk_cnt <= 5'd0;               //对系统时钟计数达一个波特率周期后清零
				tx_cnt  <= tx_cnt + 1'b1;       //此时发送数据计数器加1
			end
		end
		else begin                              //发送过程结束
			clk_cnt <= 5'd0;
			tx_cnt  <= 4'd0;
		end
	end

  assign uart_txd = reg_uart_txd;
  reg reg_uart_txd;
	//根据发送数据计数器来给uart发送端口赋值
	always @(posedge sys_clk or negedge sys_rst_n) begin        
		if (!sys_rst_n)  
			reg_uart_txd <= 1'b1;        
		else if (tx_flag)
			case(tx_cnt)
				4'd0: reg_uart_txd <= 1'b0;         //起始位 
				4'd1: reg_uart_txd <= tx_data[0];   //数据位最低位
				4'd2: reg_uart_txd <= tx_data[1];
				4'd3: reg_uart_txd <= tx_data[2];
				4'd4: reg_uart_txd <= tx_data[3];
				4'd5: reg_uart_txd <= tx_data[4];
				4'd6: reg_uart_txd <= tx_data[5];
				4'd7: reg_uart_txd <= tx_data[6];
				4'd8: reg_uart_txd <= tx_data[7];   //数据位最高位
				4'd9: reg_uart_txd <= 1'b1;         //停止位
				default: ;
			endcase
		else 
			reg_uart_txd <= 1'b1;                   //空闲时发送端口为高电平
	end
*/
  uart_send_hs(
    .sys_clk (sys_clk),
    .sys_rst_n (sys_rst_n),

    .uart_txd (uart_txd),
    
    .uart_send (uart_send),
    .uart_data_in (uart_data_in)
  );

	//需要对系统时钟计数BPS_CNT次
	//reg define
	reg        uart_rxd_d0;
	reg        uart_rxd_d1;
	reg [ 4:0] rx_clk_cnt;                             //系统时钟计数器
	reg [ 3:0] rx_cnt;                              //接收数据计数器
	reg        rx_flag;                             //接收过程标志信号
	reg [ 7:0] rxdata;                              //接收数据寄存器

	//wire define
	wire       start_flag;

	//捕获接收端口下降沿(起始位)，得到一个时钟周期的脉冲信号
	assign  start_flag = uart_rxd_d1 & (~uart_rxd_d0);    

	//对UART接收端口的数据延迟两个时钟周期
	always @(posedge sys_clk or negedge sys_rst_n) begin 
		if (!sys_rst_n) begin 
			uart_rxd_d0 <= 1'b0;
			uart_rxd_d1 <= 1'b0;          
		end
		else begin
			uart_rxd_d0  <= uart_rxd;                   
			uart_rxd_d1  <= uart_rxd_d0;
		end   
	end

	//当脉冲信号start_flag到达时，进入接收过程           
	always @(posedge sys_clk or negedge sys_rst_n) begin         
		if (!sys_rst_n)                                  
			rx_flag <= 1'b0;
		else begin
			if(start_flag)                          //检测到起始位
				rx_flag <= 1'b1;                    //进入接收过程，标志位rx_flag拉高
			else if((rx_cnt == 4'd9)&&(rx_clk_cnt == BPS_CNT_HALF))
				rx_flag <= 1'b0;                    //计数到停止位中间时，停止接收过程
			else
				rx_flag <= rx_flag;
		end
	end

	//进入接收过程后，启动系统时钟计数器与接收数据计数器
	always @(posedge sys_clk or negedge sys_rst_n) begin         
		if (!sys_rst_n) begin                             
			rx_clk_cnt <= 5'd0;                                  
			rx_cnt  <= 4'd0;
		end else if ( rx_flag ) begin                   //处于接收过程
			if (rx_clk_cnt < BPS_CNT - 1) begin
				rx_clk_cnt <= rx_clk_cnt + 1'b1;
				rx_cnt  <= rx_cnt;
			end else begin
				rx_clk_cnt <= 5'd0;               //对系统时钟计数达一个波特率周期后清零
				rx_cnt  <= rx_cnt + 1'b1;       //此时接收数据计数器加1
			end
		end else begin                              //接收过程结束，计数器清零
			rx_clk_cnt <= 5'd0;
			rx_cnt  <= 4'd0;
		end
	end

	//根据接收数据计数器来寄存uart接收端口数据
	always @(posedge sys_clk or negedge sys_rst_n) begin 
		if ( !sys_rst_n) begin
			rxdata <= 8'd0;                                     
		end else if(rx_flag) begin                            //系统处于接收过程
			if (rx_clk_cnt == BPS_CNT_HALF) begin         //判断系统时钟计数器计数到数据位中间
				case ( rx_cnt )
					4'd1 : rxdata[0] <= uart_rxd_d1;   //寄存数据位最低位
					4'd2 : rxdata[1] <= uart_rxd_d1;
					4'd3 : rxdata[2] <= uart_rxd_d1;
					4'd4 : rxdata[3] <= uart_rxd_d1;
					4'd5 : rxdata[4] <= uart_rxd_d1;
					4'd6 : rxdata[5] <= uart_rxd_d1;
					4'd7 : rxdata[6] <= uart_rxd_d1;
					4'd8 : rxdata[7] <= uart_rxd_d1;   //寄存数据位最高位
					default:;                                    
				endcase
			end else begin
				rxdata <= rxdata;
      end
		end else begin
			rxdata <= 8'd0;
    end
	end
  
assign uart_rec = reg_uart_rec && !reg_uart_rec_last;
assign uart_data_out = reg_uart_data_out;
  reg reg_uart_rec;
  reg reg_uart_rec_last;
  reg [7:0] reg_uart_data_out;
	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (!sys_rst_n) begin
			reg_uart_rec_last <= 0;
		end else begin
  	  reg_uart_rec_last <= reg_uart_rec;
		end
	end

  
	//数据接收完毕后给出标志信号并寄存输出接收到的数据
	always @(posedge sys_clk or negedge sys_rst_n) begin        
		if (!sys_rst_n) begin
			reg_uart_data_out <= 8'd0;                               
			reg_uart_rec <= 1'b0;
		end else begin
		  if(rx_cnt == 4'd9) begin               //接收数据计数器计数到停止位时           
		  	  reg_uart_data_out <= rxdata;                    //寄存输出接收到的数据
			  reg_uart_rec <= 1'b1;                      //并将接收完成标志位拉高
		  end else begin
			  reg_uart_data_out <= 8'd0;                                   
			  reg_uart_rec <= 1'b0; 
		  end
		end
	end
/*
uart_recv_hs(
    .sys_clk (sys_clk),
    .sys_rst_n (sys_rst_n),

    .uart_rxd (uart_rxd),
    
    .uart_rec (uart_rec),
    .uart_data_out (uart_data_out)
  );*/
endmodule

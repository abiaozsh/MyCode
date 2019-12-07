
module flow_led(
		input           sys_clk,          //外部50M时钟
		input           sys_rst_n,        //外部复位信号，低有效
		input 		[3:0]			key,
		output  reg  [3:0]  led,         //4个LED灯
		//uart接口
		input           uart_rxd,         //UART接收端口
		output          uart_txd          //UART发送端口
		);
 
	wire  [3:0]  key_stable;

	genvar i;
	generate
		for(i=0; i<4; i=i+1) begin:BLOCK1
			key_debounce(
					.sys_clk        (sys_clk),
					.sys_rst_n      (sys_rst_n),
    
					.key            (key[i]),
					.key_value      (key_stable[i])
				);
		end
	endgenerate

	uart_recvhs (                 
			.sys_clk        (sys_clk), 
			.sys_rst_n      (sys_rst_n),
    
			.uart_rxd       (uart_rxd),
			.uart_done      (uart_en_r),
			.uart_data      (uart_data_r)
		);
    
	uart_sendhs (                 
			.sys_clk        (sys_clk),
			.sys_rst_n      (sys_rst_n),
     
			.uart_en        (uart_en_w),
			.uart_din       (uart_data_w),
			.uart_txd       (uart_txd)
		);

    
	//wire define   
	wire       uart_en_r;                 //UART发送使能
	reg       uart_en_w;                 //UART发送使能
	wire [7:0] uart_data_r; 
	reg [7:0] uart_data_lock;  
	reg [7:0] uart_data_w;    

	//*****************************************************
	//**                    main code
	//*****************************************************

	always @(posedge sys_clk or negedge sys_rst_n) begin         
		if (!sys_rst_n) begin
			uart_en_w<=1'b0;
			uart_data_w <= 8'b0;
		end                                                      
		else begin  
			if(!key_stable[0])
			begin
				uart_en_w <= !key_stable[0]; 
				uart_data_w[0]<=	key_stable[1];	  
				uart_data_w[1]<=	key_stable[2];	  
				uart_data_w[2]<=	key_stable[3];	  
				uart_data_w[3]<=	uart_data_lock[3];	  
				uart_data_w[4]<=	uart_data_lock[4];	  
				uart_data_w[5]<=	uart_data_lock[5];	  
				uart_data_w[6]<=	uart_data_lock[6];	  
				uart_data_w[7]<=	uart_data_lock[7];
			end
			else
			begin
				uart_en_w <=1'b0;
				uart_data_w <=uart_data_w;
			end
		end
	end

	always @(posedge sys_clk or negedge sys_rst_n) 
	begin         
		if (!sys_rst_n) begin
			led <= 4'b0;              
			uart_data_lock		= 8'b0;
		end                                                      
		else begin      
			if( uart_en_r) 
			begin
				led[0] <=  uart_data_r[0];
				led[1] <=  uart_data_r[1];
				led[2] <=  uart_data_r[2];
				led[3] <=  uart_data_r[3];
				uart_data_lock = uart_data_r;
			end
			else	
			begin
				led<=led;
				uart_data_lock = uart_data_lock;
			end
		end
	end

endmodule 









module uart_recvhs(
		input			  sys_clk,                  //系统时钟
		input             sys_rst_n,                //系统复位，低电平有效
		input             uart_rxd,                 //UART接收端口
		output  reg       uart_done,                //接收一帧数据完成标志信号
		output  reg [7:0] uart_data                 //接收的数据
		);
    
	//parameter define
	localparam BPS_CNT  = 25;        //2000000 bps  50000000/2000000
	//需要对系统时钟计数BPS_CNT次
	//reg define
	reg        uart_rxd_d0;
	reg        uart_rxd_d1;
	reg [ 4:0] clk_cnt;                             //系统时钟计数器
	reg [ 3:0] rx_cnt;                              //接收数据计数器
	reg        rx_flag;                             //接收过程标志信号
	reg [ 7:0] rxdata;                              //接收数据寄存器

	//wire define
	wire       start_flag;

	//*****************************************************
	//**                    main code
	//*****************************************************
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
			else if((rx_cnt == 4'd9)&&(clk_cnt == BPS_CNT/2))
				rx_flag <= 1'b0;                    //计数到停止位中间时，停止接收过程
			else
				rx_flag <= rx_flag;
		end
	end

	//进入接收过程后，启动系统时钟计数器与接收数据计数器
	always @(posedge sys_clk or negedge sys_rst_n) begin         
		if (!sys_rst_n) begin                             
			clk_cnt <= 5'd0;                                  
			rx_cnt  <= 4'd0;
		end                                                      
		else if ( rx_flag ) begin                   //处于接收过程
			if (clk_cnt < BPS_CNT - 1) begin
				clk_cnt <= clk_cnt + 1'b1;
				rx_cnt  <= rx_cnt;
			end
			else begin
				clk_cnt <= 5'd0;               //对系统时钟计数达一个波特率周期后清零
				rx_cnt  <= rx_cnt + 1'b1;       //此时接收数据计数器加1
			end
		end
		else begin                              //接收过程结束，计数器清零
			clk_cnt <= 5'd0;
			rx_cnt  <= 4'd0;
		end
	end

	//根据接收数据计数器来寄存uart接收端口数据
	always @(posedge sys_clk or negedge sys_rst_n) begin 
		if ( !sys_rst_n)  
			rxdata <= 8'd0;                                     
		else if(rx_flag)                            //系统处于接收过程
			if (clk_cnt == BPS_CNT/2) begin         //判断系统时钟计数器计数到数据位中间
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
			end
			else 
				rxdata <= rxdata;
		else
			rxdata <= 8'd0;
	end

	//数据接收完毕后给出标志信号并寄存输出接收到的数据
	always @(posedge sys_clk or negedge sys_rst_n) begin        
		if (!sys_rst_n) begin
			uart_data <= 8'd0;                               
			uart_done <= 1'b0;
		end
		else if(rx_cnt == 4'd9) begin               //接收数据计数器计数到停止位时           
			uart_data <= rxdata;                    //寄存输出接收到的数据
			uart_done <= 1'b1;                      //并将接收完成标志位拉高
		end
		else begin
			uart_data <= 8'd0;                                   
			uart_done <= 1'b0; 
		end    
	end

endmodule	






module uart_sendhs(
		input	      sys_clk,                  //系统时钟
		input         sys_rst_n,                //系统复位，低电平有效
    
		input         uart_en,                  //发送使能信号
		input  [7:0]  uart_din,                 //待发送数据
		output  reg   uart_txd                  //UART发送端口
		);
    
	//parameter define
	localparam BPS_CNT  = 25;        //2000000 bps  50000000/2000000
   
	//reg define
	reg        uart_en_d0; 
	reg        uart_en_d1;  
	reg [ 4:0] clk_cnt;                         //系统时钟计数器
	reg [ 3:0] tx_cnt;                          //发送数据计数器
	reg        tx_flag;                         //发送过程标志信号
	reg [ 7:0] tx_data;                         //寄存发送数据

	//wire define
	wire       en_flag;

	//*****************************************************
	//**                    main code
	//*****************************************************
	//捕获uart_en上升沿，得到一个时钟周期的脉冲信号
	assign en_flag = (~uart_en_d1) & uart_en_d0;
                                                 
	//对发送使能信号uart_en延迟两个时钟周期
	always @(posedge sys_clk or negedge sys_rst_n) begin         
		if (!sys_rst_n) begin
			uart_en_d0 <= 1'b0;                                  
			uart_en_d1 <= 1'b0;
		end                                                      
		else begin                                               
			uart_en_d0 <= uart_en;                               
			uart_en_d1 <= uart_en_d0;                            
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
			tx_data <= uart_din;            //寄存待发送的数据
		end
		else 
		if ((tx_cnt == 4'd9)&&(clk_cnt == BPS_CNT/2))
		begin                               //计数到停止位中间时，停止发送过程
			tx_flag <= 1'b0;                //发送过程结束，标志位tx_flag拉低
			tx_data <= 8'd0;
		end
		else begin
			tx_flag <= tx_flag;
			tx_data <= tx_data;
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
				tx_cnt  <= tx_cnt;
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

	//根据发送数据计数器来给uart发送端口赋值
	always @(posedge sys_clk or negedge sys_rst_n) begin        
		if (!sys_rst_n)  
			uart_txd <= 1'b1;        
		else if (tx_flag)
			case(tx_cnt)
				4'd0: uart_txd <= 1'b0;         //起始位 
				4'd1: uart_txd <= tx_data[0];   //数据位最低位
				4'd2: uart_txd <= tx_data[1];
				4'd3: uart_txd <= tx_data[2];
				4'd4: uart_txd <= tx_data[3];
				4'd5: uart_txd <= tx_data[4];
				4'd6: uart_txd <= tx_data[5];
				4'd7: uart_txd <= tx_data[6];
				4'd8: uart_txd <= tx_data[7];   //数据位最高位
				4'd9: uart_txd <= 1'b1;         //停止位
				default: ;
			endcase
		else 
			uart_txd <= 1'b1;                   //空闲时发送端口为高电平
	end

endmodule	          











module key_debounce(
		input            sys_clk,          //外部50M时钟
		input            sys_rst_n,        //外部复位信号，低有效
		input            key,              //外部按键输入
		output reg       key_value         //按键消抖后的数据  
		);

	//reg define    
	reg [19:0] delay_cnt;
	reg        key_reg;

	//*****************************************************
	//**                    main code
	//*****************************************************
	always @(posedge sys_clk or negedge sys_rst_n) begin 
		if (!sys_rst_n) begin 
			key_reg   <= 1'b1;
			delay_cnt <= 20'd0;
		end
		else begin
			key_reg <= key;
			if(key_reg != key)             //一旦检测到按键状态发生变化(有按键被按下或释放)
				delay_cnt <= 20'd1000000;  //给延时计数器重新装载初始值（计数时间为20ms）
			else if(key_reg == key) begin  //在按键状态稳定时，计数器递减，开始20ms倒计时
				if(delay_cnt > 20'd0)
					delay_cnt <= delay_cnt - 1'b1;
				else
					delay_cnt <= delay_cnt;
			end           
		end   
	end

	always @(posedge sys_clk or negedge sys_rst_n) begin 
		if (!sys_rst_n) begin 
			key_value <= 1'b1;          
		end
		else begin
			if(delay_cnt == 20'd1) begin   //当计数器递减到1时，说明按键稳定状态维持了20ms
				key_value <= key;          //并寄存此时按键的值
			end
			else begin
				key_value <= key_value; 
			end  
		end   
	end
    
endmodule 


module flow_led(
		input           sys_clk,          //外部50M时钟
		input           rst_n,        //外部复位信号，低有效
		input 		[3:0]			key,
		output     [3:0]  led,         //4个LED灯

    //SDRAM 芯片接口
    output        sdram_clk,                //SDRAM 芯片时钟
    output        sdram_cke,                //SDRAM 时钟有效
    output        sdram_cs_n,               //SDRAM 片选
    output        sdram_ras_n,              //SDRAM 行有效
    output        sdram_cas_n,              //SDRAM 列有效
    output        sdram_we_n,               //SDRAM 写有效
    output [ 1:0] sdram_ba,                 //SDRAM Bank地址
    output [12:0] sdram_addr,               //SDRAM 行/列地址
    inout  [15:0] sdram_data,               //SDRAM 数据
    output [ 1:0] sdram_dqm,                //SDRAM 数据掩码

		//uart接口
		input           uart_rxd,         //UART接收端口
		output          uart_txd          //UART发送端口
		);
 
//待PLL输出稳定之后，停止系统复位
assign sys_rst_n = rst_n & locked;

//例化PLL, 产生各模块所需要的时钟
pll_clk u_pll_clk(
    .inclk0             (sys_clk),
    .areset             (~rst_n),
    
    .c0                 (clk_50m),
    .c1                 (clk_100m),
    .c2                 (clk_100m_shift),
    .locked             (locked)
    );

  wire ref_clk;
  assign ref_clk = clk_100m;
assign	sdram_clk = clk_100m_shift;//out_clk;                //将相位偏移时钟输出给sdram芯片
assign	sdram_dqm = 2'b00;                  //读写过程中均不屏蔽数据线







	/*
    input         clk,		        //SDRAM控制器时钟，100MHz
    input         rst_n,	        //系统复位信号，低电平有效
    
	//SDRAM 控制器写端口	
    input         sdram_wr_req,		//写SDRAM请求信号
    output        sdram_wr_ack,		//写SDRAM响应信号
    input  [23:0] sdram_wr_addr,	//SDRAM写操作的地址
    input  [ 9:0] sdram_wr_burst,   //写sdram时数据突发长度
    input  [15:0] sdram_din,	    //写入SDRAM的数据
    
	//SDRAM 控制器读端口	
    input         sdram_rd_req,		//读SDRAM请求信号
    output        sdram_rd_ack,		//读SDRAM响应信号
    input  [23:0] sdram_rd_addr,	//SDRAM写操作的地址
    input  [ 9:0] sdram_rd_burst,   //读sdram时数据突发长度
    output [15:0] sdram_dout,	    //从SDRAM读出的数据
    
    output	      sdram_init_done,  //SDRAM 初始化完成标志
                                     
	// FPGA与SDRAM硬件接口
    output        sdram_cke,		// SDRAM 时钟有效信号
    output        sdram_cs_n,		// SDRAM 片选信号
    output        sdram_ras_n,		// SDRAM 行地址选通脉冲
    output        sdram_cas_n,		// SDRAM 列地址选通脉冲
    output        sdram_we_n,		// SDRAM 写允许位
    output [ 1:0] sdram_ba,		    // SDRAM L-Bank地址线
    output [12:0] sdram_addr,	    // SDRAM 地址总线
    inout  [15:0] sdram_data		// SDRAM 数据总线
    */
    wire        sdram_wr_req    ;		//写SDRAM请求信号              input 
    wire        sdram_wr_ack    ;		//写SDRAM响应信号              output
    wire [23:0] sdram_wr_addr   ;	//SDRAM写操作的地址            input 
    wire [ 9:0] sdram_wr_burst  ;   //写sdram时数据突发长度      input 
    wire [15:0] sdram_din       ;	    //写入SDRAM的数据              input 
    wire        sdram_rd_req    ;		//读SDRAM请求信号              input 
    wire        sdram_rd_ack    ;		//读SDRAM响应信号              output
    wire [23:0] sdram_rd_addr   ;	//SDRAM写操作的地址            input 
    wire [ 9:0] sdram_rd_burst  ;   //读sdram时数据突发长度      input 
    wire [15:0] sdram_dout      ;	    //从SDRAM读出的数据            output
    wire	      sdram_init_done ;  //SDRAM 初始化完成标志       output
//SDRAM控制器
sdram_controller(
	.clk				(ref_clk),			//sdram 控制器时钟
	.rst_n				(rst_n),			//系统复位
    
	//SDRAM 控制器写端口	
	.sdram_wr_req		(sdram_wr_req), 	//sdram 写请求
	.sdram_wr_ack		(sdram_wr_ack), 	//sdram 写响应
	.sdram_wr_addr		(sdram_wr_addr), 	//sdram 写地址
	.sdram_wr_burst		(sdram_wr_burst),		    //写sdram时数据突发长度
	.sdram_din  		(sdram_din),    	//写入sdram中的数据
    
    //SDRAM 控制器读端口
	.sdram_rd_req		(sdram_rd_req), 	//sdram 读请求
	.sdram_rd_ack		(sdram_rd_ack),		//sdram 读响应
	.sdram_rd_addr		(sdram_rd_addr), 	//sdram 读地址
	.sdram_rd_burst		(sdram_rd_burst),		    //读sdram时数据突发长度
	.sdram_dout		    (sdram_dout),   	//从sdram中读出的数据
    
	.sdram_init_done	(sdram_init_done),	//sdram 初始化完成标志

	//SDRAM 芯片接口
	.sdram_cke			(sdram_cke),		//SDRAM 时钟有效
	.sdram_cs_n			(sdram_cs_n),		//SDRAM 片选
	.sdram_ras_n		(sdram_ras_n),		//SDRAM 行有效	
	.sdram_cas_n		(sdram_cas_n),		//SDRAM 列有效
	.sdram_we_n			(sdram_we_n),		//SDRAM 写有效
	.sdram_ba			(sdram_ba),			//SDRAM Bank地址
	.sdram_addr			(sdram_addr),		//SDRAM 行/列地址
	.sdram_data			(sdram_data)		//SDRAM 数据	
    );

  assign led[0] = sdram_init_done;
  assign led[1] = out_pin5[0];//sdram_wr_ack;
  assign led[2] = out_pin5[1];//sdram_rd_ack;
  
  assign sdram_wr_req = out_pin4[0];
  assign sdram_rd_req = out_pin4[1];
  assign in_pin2[0] = sdram_wr_ack;
  assign in_pin2[1] = sdram_rd_ack;
  assign sdram_wr_addr = {8'h0,out_pin1,out_pin0};
  assign sdram_rd_addr = {8'h0,out_pin1,out_pin0};
  assign sdram_wr_burst = 1;
  assign sdram_rd_burst = 1;
  
  assign sdram_din = {out_pin3,out_pin2};
  assign in_pin0 = sdram_dout[7:0];
  assign in_pin1 = sdram_dout[15:8];
  
	wire out_clk;
	wire out_rst;
	wire [7:0] in_pin0;//数据低
	wire [7:0] in_pin1;//数据高
	wire [7:0] in_pin2;
	wire [7:0] in_pin3;
	wire [7:0] out_pin0;//地址低
	wire [7:0] out_pin1;//地址高
	wire [7:0] out_pin2;//数据低
	wire [7:0] out_pin3;//数据高
	wire [7:0] out_pin4;//sdram_wr_req  sdram_rd_req
	wire [7:0] out_pin5;//led
	uart_mcu(
			.sys_clk           (sys_clk  ),       // 时钟信号
			.sys_rst_n         (sys_rst_n),       // 复位信号
			.uart_rxd(uart_rxd),
			.uart_txd(uart_txd),
      
			.out_clk    (out_clk),
			.out_rst (out_rst),
      
			.in_pin0          ( in_pin0 ),
			.in_pin1          ( in_pin1 ),
			.in_pin2          ( in_pin2 ),
			.in_pin3          ( in_pin3 ),

			.out_pin0          ( out_pin0  ),
			.out_pin1          ( out_pin1  ),
			.out_pin2          ( out_pin2  ),
			.out_pin3          ( out_pin3  ),
			.out_pin4          ( out_pin4  ),
			.out_pin5          ( out_pin5  ) 
		);

    
endmodule 















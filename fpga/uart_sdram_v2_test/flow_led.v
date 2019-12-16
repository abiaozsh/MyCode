
module flow_led(
		input           sys_clk,          //外部50M时钟
		input           sys_rst_n,        //外部复位信号，低有效
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
 




 
 
/*
module sdram(
		input  sys_clk  ,
		input  sys_rst_n,

    //SDRAM 芯片接口
    output        sdram_clk_out,            //SDRAM 芯片时钟
    output        sdram_cke,                //SDRAM 时钟有效
    output        sdram_cs_n,               //SDRAM 片选
    output        sdram_ras_n,              //SDRAM 行有效
    output        sdram_cas_n,              //SDRAM 列有效
    output        sdram_we_n,               //SDRAM 写有效
    output [ 1:0] sdram_ba,                 //SDRAM Bank地址
    output [12:0] sdram_addr,               //SDRAM 行/列地址
    inout  [15:0] sdram_data,               //SDRAM 数据
    output [ 1:0] sdram_dqm,                //SDRAM 数据掩码

    //共用
    input clk,//上升沿拉取地址数据
    input  [23:0] address,
    input  [15:0] data_in,
    output [15:0] data_out,
    
    //read_req write_req write_en 不能同时为高
    
    //读取端口
    input  read_req,//读取1*4字缓存
    output reg read_ack,//ack响应之前，address和read_req要保持

    input  write_req,//单字写
    output reg write_ack,//ack响应之前，address和write_req要保持

    
    //连续写入端口 //2*8字缓存，触及边界后刷入
    //上升沿锁存地址，之后每次加一
    //写完8个字后，要保持一个周期，以写入sdram
    input write_en,//写入过程中保持高,要从8字前边界开始写，地址0x00,0x08,0x10...,否则会覆盖原有数据
    
    
    //vga输出端口
    //vga输出优先级最高
    output vga
    );
    */

wire sdram_c_clk;
wire sdram_c_address;
wire [15:0] sdram_c_data_in;
wire [15:0] sdram_c_data_out;
wire  sdram_c_read_req;
wire  sdram_c_read_ack;
wire  sdram_c_write_req;
wire  sdram_c_write_ack;
wire sdram_c_write_en;
wire sdram_c_vga;
 sdram(
  .sys_clk    (sys_clk  ),       // 时钟信号
  .sys_rst_n  (sys_rst_n),       // 复位信号
  //SDRAM 芯片接口
  .sdram_cke			(sdram_cke),		//SDRAM 时钟有效
  .sdram_cs_n			(sdram_cs_n),		//SDRAM 片选
  .sdram_ras_n		(sdram_ras_n),		//SDRAM 行有效	
  .sdram_cas_n		(sdram_cas_n),		//SDRAM 列有效
  .sdram_we_n			(sdram_we_n),		//SDRAM 写有效
  .sdram_ba			  (sdram_ba),			//SDRAM Bank地址
  .sdram_addr			(sdram_addr),		//SDRAM 行/列地址
  .sdram_data			(sdram_data),		//SDRAM 数据	
  
  .clk        (sdram_c_clk),//in
  .address    (sdram_c_address),//in
  .data_in    (sdram_c_data_in),//in
  .data_out   (sdram_c_data_out),//out
  .read_req   (sdram_c_read_req),//in
  .read_ack   (sdram_c_read_ack),//out
  .write_req  (sdram_c_write_req),//in
  .write_ack  (sdram_c_write_ack),//out
  .write_en   (sdram_c_write_en),//in
  .vga        (sdram_c_vga)//out

 );
 
assign sdram_c_clk = out_clk;
assign sdram_c_data_in = {out_pin0,out_pin1};
assign sdram_c_address = {8'b0,out_pin3,out_pin2};
assign sdram_c_read_req = out_pin4[0];
assign sdram_c_write_req = out_pin5[0];
assign sdram_c_write_en = out_pin6[0];
  
assign in_pin0 = sdram_c_data_out[7:0];
assign in_pin1 = sdram_c_data_out[15:8];
assign in_pin2 = {7'b0,sdram_c_read_ack};
assign in_pin3 = {7'b0,sdram_c_write_ack};


/*
cl
ou012//data
ou134//data
ou200//addr
ou300//addr
ou501//write_req
ch
cl
in3//write_ack
ou500//write_req
ch
cl

ou200//addr
ou300//addr
ou401//read_req
ch
cl
in2//read_ack
in0//data
in1//data

00000000

*/

  
	wire out_clk;
	wire out_rst;
	wire [7:0] in_pin0;//数据低
	wire [7:0] in_pin1;//数据高
	wire [7:0] in_pin2;
	wire [7:0] in_pin3;
	wire [7:0] out_pin0;//数据低
	wire [7:0] out_pin1;//数据高
	wire [7:0] out_pin2;//地址低
	wire [7:0] out_pin3;//地址高
	wire [7:0] out_pin4;//sdram_wr_req  sdram_rd_req
	wire [7:0] out_pin5;//led
	wire [7:0] out_pin6;//sdram_wr_burst
	wire [7:0] out_pin7;//sdram_rd_burst
	uart_mcu(
			.sys_clk    (sys_clk  ),       // 时钟信号
			.sys_rst_n  (sys_rst_n),       // 复位信号
			.uart_rxd  (uart_rxd),
			.uart_txd  (uart_txd),
      
			.out_clk (out_clk),
			.out_rst (out_rst),
      
			.in_pin0          ( in_pin0 ),//数据低
			.in_pin1          ( in_pin1 ),//数据高
			.in_pin2          ( in_pin2 ),//sdram_wr_ack,sdram_rd_ack
			.in_pin3          ( in_pin3 ),

			.out_pin0          ( out_pin0  ),
			.out_pin1          ( out_pin1  ),
			.out_pin2          ( out_pin2  ),
			.out_pin3          ( out_pin3  ),
			.out_pin4          ( out_pin4  ),
			.out_pin5          ( out_pin5  ),//led
			.out_pin6          ( out_pin6  ),
			.out_pin7          ( out_pin7  )

		);

    
endmodule 













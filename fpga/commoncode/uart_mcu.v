module uart_mcu(
		input  sys_clk  ,
		input  sys_rst_n,
    
		input  uart_rxd,
		output uart_txd,
    
/*
  .sdram_clk_out     (sdram_clk_out),
  .sdram_cke			(sdram_cke),		//SDRAM 时钟有效
  .sdram_cs_n			(sdram_cs_n),		//SDRAM 片选
  .sdram_ras_n		(sdram_ras_n),		//SDRAM 行有效	
  .sdram_cas_n		(sdram_cas_n),		//SDRAM 列有效
  .sdram_we_n			(sdram_we_n),		//SDRAM 写有效
  .sdram_ba			  (sdram_ba),			//SDRAM Bank地址
  .sdram_addr			(sdram_addr),		//SDRAM 行/列地址
  .sdram_data			(sdram_data),		//SDRAM 数据	
  .sdram_dqm		(sdram_dqm),
*/
    
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
		output reg [7:0] out_pin7

		);
reg uart_rec_old;
reg [7:0] cmd_temp;
reg [7:0] command;
reg command_done;
reg [7:0] data;

wire uart_rec;
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
uart_hs (
    .sys_clk        (sys_clk), 
    .sys_rst_n      (sys_rst_n),
    .uart_rxd       (uart_rxd),
    .uart_txd       (uart_txd),

    .uart_rec       (uart_rec),
    .uart_data_out  (uart_data_r),
    .uart_send      (uart_send),
    .uart_data_in   (uart_data_w)
  );


always @(posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    uart_rec_old <= uart_rec;
  end else begin
    uart_rec_old <= uart_rec;
  end
end

always @(posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    command <= 0;
    cmd_temp <= 0;
    data <= 0;
  end else begin
    if (uart_rec_old != uart_rec && uart_rec==1) begin //串口数据到达  
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
      if(command_done)begin
        command <= 0;
        data <= 0;
      end
    end
  end
end

reg [7:0] timer;
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
    command_done <= 0;
    timer<=0;
    
    sdram_c_address <= 0;
    sdram_c_data_in <= 0;
    sdram_c_read_req <= 0;
    sdram_c_write_req <= 0;
    sdram_c_write_en <= 0;
  end else begin
    if          (command == 0) begin 
      command_done<=0;
      timer<=0;
    end else if (command == 8'h10 && !command_done) begin out_clk<=1; command_done<=1;
    end else if (command == 8'h11 && !command_done) begin out_clk<=0; command_done<=1;
    end else if (command == 8'h12 && !command_done) begin out_rst<=1; command_done<=1;
    end else if (command == 8'h13 && !command_done) begin out_rst<=0; command_done<=1;

    end else if (command == 8'h20 && !command_done) begin uart_send<=1; uart_data_w<=in_pin0; command_done<=1;
    end else if (command == 8'h21 && !command_done) begin uart_send<=1; uart_data_w<=in_pin1; command_done<=1;
    end else if (command == 8'h22 && !command_done) begin uart_send<=1; uart_data_w<=in_pin2; command_done<=1;
    end else if (command == 8'h23 && !command_done) begin uart_send<=1; uart_data_w<=in_pin3; command_done<=1;
    end else if (command == 8'h24 && !command_done) begin uart_send<=1; uart_data_w<=in_pin4; command_done<=1;
    end else if (command == 8'h25 && !command_done) begin uart_send<=1; uart_data_w<=in_pin5; command_done<=1;
    end else if (command == 8'h26 && !command_done) begin uart_send<=1; uart_data_w<=in_pin6; command_done<=1;
    end else if (command == 8'h27 && !command_done) begin uart_send<=1; uart_data_w<=in_pin7; command_done<=1;

    end else if (command == 8'h30 && !command_done) begin out_pin0<=data; command_done<=1;
    end else if (command == 8'h31 && !command_done) begin out_pin1<=data; command_done<=1;
    end else if (command == 8'h32 && !command_done) begin out_pin2<=data; command_done<=1;
    end else if (command == 8'h33 && !command_done) begin out_pin3<=data; command_done<=1;
    end else if (command == 8'h34 && !command_done) begin out_pin4<=data; command_done<=1;
    end else if (command == 8'h35 && !command_done) begin out_pin5<=data; command_done<=1;
    end else if (command == 8'h36 && !command_done) begin out_pin6<=data; command_done<=1;
    end else if (command == 8'h37 && !command_done) begin out_pin7<=data; command_done<=1;

    end else if (command == 8'h40 && !command_done) begin uw_reg0<=data; command_done<=1;
    end else if (command == 8'h41 && !command_done) begin uw_reg1<=data; command_done<=1;
    end else if (command == 8'h42 && !command_done) begin uw_reg2<=data; command_done<=1;
    end else if (command == 8'h43 && !command_done) begin uw_reg3<=data; command_done<=1;
    end else if (command == 8'h44 && !command_done) begin uw_reg4<=data; command_done<=1;
    end else if (command == 8'h45 && !command_done) begin uw_reg5<=data; command_done<=1;
    end else if (command == 8'h46 && !command_done) begin uw_reg6<=data; command_done<=1;
    end else if (command == 8'h47 && !command_done) begin uw_reg7<=data; command_done<=1;

    end else if (command == 8'h50 && !command_done) begin uart_send<=1; uart_data_w<=ur_reg0; command_done<=1;
    end else if (command == 8'h51 && !command_done) begin uart_send<=1; uart_data_w<=ur_reg1; command_done<=1;
    end else if (command == 8'h52 && !command_done) begin uart_send<=1; uart_data_w<=ur_reg2; command_done<=1;
    end else if (command == 8'h53 && !command_done) begin uart_send<=1; uart_data_w<=ur_reg3; command_done<=1;
    end else if (command == 8'h54 && !command_done) begin uart_send<=1; uart_data_w<=ur_reg4; command_done<=1;
    end else if (command == 8'h55 && !command_done) begin uart_send<=1; uart_data_w<=ur_reg5; command_done<=1;
    end else if (command == 8'h56 && !command_done) begin uart_send<=1; uart_data_w<=ur_reg6; command_done<=1;
    end else if (command == 8'h57 && !command_done) begin uart_send<=1; uart_data_w<=ur_reg7; command_done<=1;

    end else if (command == 8'hA0 && !command_done) begin//sdram write
      timer<=timer+1'b1;
      if         (timer==0)begin
        sdram_c_address <= {8'b0,uw_reg3,uw_reg2};
        sdram_c_data_in <= {uw_reg1,uw_reg0};
      end else if(timer==1)begin
        sdram_c_write_req<=1;
      end else begin
        if(sdram_c_write_ack)begin
          ur_reg7<=timer;
          sdram_c_write_req<=0;
          command_done<=1;
        end
      end
      
    end else if (command == 8'hA1 && !command_done) begin//sdram read
      timer<=timer+1'b1;
      if         (timer==0)begin
        sdram_c_address <= {8'b0,uw_reg3,uw_reg2};
      end else if(timer==1)begin
        sdram_c_read_req<=1;
      end else begin
        if(sdram_c_read_ack)begin
          ur_reg7<=timer;
          ur_reg0<=sdram_c_data_out[7:0];
          ur_reg1<=sdram_c_data_out[15:8];
          sdram_c_read_req<=0;
          command_done<=1;
        end
      end
    end else if (command == 8'hB0 && !command_done) begin //get probe
      ur_reg0 <= probe_timer8;
      ur_reg1 <= probe_locked_time;
      ur_reg2 <= probe_sdram_init_done_timer;
		ur_reg3 <= probe_readBuffer0;
      command_done<=1;

    end else begin
      uart_send<=0;
    end
  end
end



/*
pr080//data
pr144//data
pr200//addr
pr300//addr
scA0//sdram write
gr7//timer
#br

pr200//addr
pr300//addr
scA1//sdram read
gr0//data
gr1//data
gr7//timer
#br

06
3f440a




pr070//data
pr184//data
pr200//addr
pr300//addr
scA0//sdram write
gr7//timer
#br
06


pr200//addr
pr300//addr
scA1//sdram read
gr0//data
gr1//data
gr7//timer
#br
//703f0a



*/


















 
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

reg [23:0] sdram_c_address;
reg [15:0] sdram_c_data_in;
wire [15:0] sdram_c_data_out;
reg  sdram_c_read_req;
wire  sdram_c_read_ack;
reg  sdram_c_write_req;
wire  sdram_c_write_ack;
reg sdram_c_write_en;
wire [7:0] probe_timer8;
wire [7:0] probe_locked_time;
wire [7:0] probe_sdram_init_done_timer;
wire [7:0] probe_readBuffer0;
wire sdram_c_vga;
 sdram(
  .sys_clk    (sys_clk  ),       // 时钟信号
  .sys_rst_n  (sys_rst_n),       // 复位信号
  
  //SDRAM 芯片接口
  .sdram_clk_out     (sdram_clk_out),
  .sdram_cke			(sdram_cke),		//SDRAM 时钟有效
  .sdram_cs_n			(sdram_cs_n),		//SDRAM 片选
  .sdram_ras_n		(sdram_ras_n),		//SDRAM 行有效	
  .sdram_cas_n		(sdram_cas_n),		//SDRAM 列有效
  .sdram_we_n			(sdram_we_n),		//SDRAM 写有效
  .sdram_ba			  (sdram_ba),			//SDRAM Bank地址
  .sdram_addr			(sdram_addr),		//SDRAM 行/列地址
  .sdram_data			(sdram_data),		//SDRAM 数据	
  .sdram_dqm		(sdram_dqm),
  
  .clk        (sys_clk),//in
  .address    (sdram_c_address),//in
  .data_in    (sdram_c_data_in),//in
  .data_out   (sdram_c_data_out),//out
  .read_req   (sdram_c_read_req),//in
  .read_ack   (sdram_c_read_ack),//out
  .write_req  (sdram_c_write_req),//in
  .write_ack  (sdram_c_write_ack),//out
  .write_en   (sdram_c_write_en),//in
  
  .probe_timer8 (probe_timer8),
  .probe_locked_time (probe_locked_time),
  .probe_sdram_init_done_timer (probe_sdram_init_done_timer),
    .probe_readBuffer0 (probe_readBuffer0),
  .vga        (sdram_c_vga)//out

 );
 


endmodule

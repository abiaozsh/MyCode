module sdram(
		input  sys_clk  ,
		input  sys_rst_n,
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
    
    //output        sdram_prob_refresh,       //SDRAM 刷新指示
    //output reg [7:0] debug_port0,
    //output reg [7:0] debug_port1,
    //output reg [7:0] debug_port2,
    //output reg debug_pin0,
    //output reg debug_pin1,
    //output reg debug_pin2,
    //output reg debug_pin3,
    //output reg debug_pin6,
    //output reg debug_pin7,

    
    
    //共用
    input clk,//上升沿拉取地址数据
    input  [23:0] address,
    input  [15:0] data_in,
    output reg [15:0] data_out,
    
    //read_req write_req write_en 不能同时为高
    
    //读取端口
    input  read_req,//读取1*4字缓存
    output reg read_ack,//ack响应之前，address和read_req要保持

    input  write_req,//单字写
    output reg write_ack,//ack响应之前，address和write_req要保持

    //连续写入端口 //2*8字缓存，触及边界后刷入
    //上升沿锁存地址，之后每次加一
    input write_latch_address,
    input write_en,//写入过程中保持高,要从8字前边界开始写，地址0x00,0x08,0x10...,否则会覆盖原有数据
    output [23:0] write_address,

    //vga输出端口
    //vga输出优先级最高
    output vga
    );
    
wire rst_n;
wire clk_50m;
wire clk_100m;
wire clk_100m_shift;
wire locked;
//待PLL输出稳定之后，停止系统复位
assign rst_n = sys_rst_n & locked;

//例化PLL, 产生各模块所需要的时钟
pll_clk u_pll_clk(
  .inclk0             (sys_clk),
  .areset             (~sys_rst_n),
  
  .c0                 (clk_50m),
  .c1                 (clk_100m),
  .c2                 (clk_100m_shift),
  .locked             (locked)
);


wire sdram_clk;
assign sdram_clk = clk_100m;
assign	sdram_clk_out = clk_100m_shift;//out_clk;                //将相位偏移时钟输出给sdram芯片
assign	sdram_dqm = 2'b00;                  //读写过程中均不屏蔽数据线


reg        sdram_wr_req    ;		//写SDRAM请求信号              input 
wire        sdram_wr_ack    ;		//写SDRAM响应信号              output
reg [23:0] sdram_wr_addr   ;	//SDRAM写操作的地址            input 
reg [ 9:0] sdram_wr_burst  ;   //写sdram时数据突发长度      input 
reg [15:0] sdram_din       ;	    //写入SDRAM的数据              input 
reg        sdram_rd_req    ;		//读SDRAM请求信号              input 
wire        sdram_rd_ack    ;		//读SDRAM响应信号              output
reg [23:0] sdram_rd_addr   ;	//SDRAM写操作的地址            input 
reg [ 9:0] sdram_rd_burst  ;   //读sdram时数据突发长度      input 
wire [15:0] sdram_dout      ;	    //从SDRAM读出的数据            output
wire	      sdram_init_done ;  //SDRAM 初始化完成标志       output

//SDRAM控制器
sdram_controller ins_sdram_controller(
	.clk				(sdram_clk),			//sdram 控制器时钟
	.rst_n				(rst_n),			//系统复位

	//SDRAM 芯片接口
	.sdram_cke			(sdram_cke),		//SDRAM 时钟有效
	.sdram_cs_n			(sdram_cs_n),		//SDRAM 片选
	.sdram_ras_n		(sdram_ras_n),		//SDRAM 行有效	
	.sdram_cas_n		(sdram_cas_n),		//SDRAM 列有效
	.sdram_we_n			(sdram_we_n),		//SDRAM 写有效
	.sdram_ba			  (sdram_ba),			//SDRAM Bank地址
	.sdram_addr			(sdram_addr),		//SDRAM 行/列地址
	.sdram_data			(sdram_data),		//SDRAM 数据	
  
	//SDRAM 控制器端口	
	.sdram_wr_req		  (sdram_wr_req), 	//sdram 写请求
	.sdram_wr_ack		  (sdram_wr_ack), 	//sdram 写响应
	.sdram_wr_addr		(sdram_wr_addr), 	//sdram 写地址
	.sdram_wr_burst		(sdram_wr_burst),		    //写sdram时数据突发长度
	.sdram_din  		  (sdram_din),    	//写入sdram中的数据
	.sdram_rd_req		  (sdram_rd_req), 	//sdram 读请求
	.sdram_rd_ack		  (sdram_rd_ack),		//sdram 读响应
	.sdram_rd_addr		(sdram_rd_addr), 	//sdram 读地址
	.sdram_rd_burst		(sdram_rd_burst),		    //读sdram时数据突发长度
	.sdram_dout		    (sdram_dout),   	//从sdram中读出的数据
  
  .block_auto_refresh (write_en),
  
	.sdram_init_done	(sdram_init_done)	//sdram 初始化完成标志

);

wire read_vga_sdram_req;
assign read_vga_sdram_req = 0;

reg read_req_last;//用户接口 读请求 上升沿
always@(posedge clk or negedge sys_rst_n) begin
	if(!sys_rst_n) begin
    read_req_last <= 0;
	end else begin
    read_req_last <= read_req;
  end
end

reg readBufferAddressValid;
reg [21:0] readBufferAddress;
reg read_sdram_req;
//read_ack data_out
always@(posedge clk or negedge sys_rst_n) begin
	if(!sys_rst_n) begin
    read_ack <= 0;
    data_out <= 0;
    readBufferAddress <= 0;
    readBufferAddressValid <= 0;
	end else begin
    if(read_req && !read_req_last)begin
      if((readBufferAddressValid && readBufferAddress==address[23:2]))begin
        read_ack <= 1;
        if          (address[1:0]==0)begin data_out <= readBuffer0;
        end else if (address[1:0]==1)begin data_out <= readBuffer1;
        end else if (address[1:0]==2)begin data_out <= readBuffer2;
        end else if (address[1:0]==3)begin data_out <= readBuffer3;
        end
      end else begin
        readBufferAddressValid = 0;
        read_sdram_req <= 1;
      end
    end else begin
      if(read_req && !read_ack)begin
        if(read_sdram_ack)begin
          readBufferAddressValid = 1;
          readBufferAddress <= address[23:2];
          read_sdram_req <= 0;
          read_ack <= 1;
          if          (address[1:0]==0)begin data_out <= readBuffer0;
          end else if (address[1:0]==1)begin data_out <= readBuffer1;
          end else if (address[1:0]==2)begin data_out <= readBuffer2;
          end else if (address[1:0]==3)begin data_out <= readBuffer3;
          end
        end
      end
      if(!read_req)begin
        if(write_req || write_en)begin
          readBufferAddressValid <= 0;
        end
        read_ack <= 0;
        data_out <= 0;
      end
    end
  end
end

reg write_req_last;//用户接口 写请求 上升沿
always@(posedge clk or negedge sys_rst_n) begin
	if(!sys_rst_n) begin
    write_req_last <= 0;
	end else begin
    write_req_last <= write_req;
  end
end

reg write_single_sdram_req;//sdram 单次写请求
//write_ack
always@(posedge clk or negedge sys_rst_n) begin
  if(!sys_rst_n) begin
    write_single_sdram_req <= 0;
    write_ack <= 0;
  end else begin
    write_ack <= 0;
    if(write_req&&!write_req_last)begin//用户接口 写请求 上升沿
      write_single_sdram_req <= 1;
    end else begin
      if(write_single_sdram_ack)begin
        write_single_sdram_req <= 0;
        write_ack <= 1;
      end
    end
  end
end

assign write_address = writeAddressDataIn;

wire [23:0] writeAddressDataInCurr;//连续写 地址
assign writeAddressDataInCurr = (write_latch_address) ? address : writeAddressDataIn;

reg [23:0] writeAddressDataIn;
always@(posedge clk or negedge sys_rst_n) begin//地址递增
  if(!sys_rst_n) begin
    writeAddressDataIn <= 0;
  end else begin
    if(write_en)begin
      writeAddressDataIn <= writeAddressDataInCurr+1'b1;
    end
  end
end

reg write_sdram_req;
reg [19:0] writeAddressSdram;//20bit (24-4)

reg [15:0] writeBufferFront0;
reg [15:0] writeBufferFront1;
reg [15:0] writeBufferFront2;
reg [15:0] writeBufferFront3;
reg [15:0] writeBufferFront4;
reg [15:0] writeBufferFront5;
reg [15:0] writeBufferFront6;
reg [15:0] writeBufferFront7;
reg [15:0] writeBufferFront8;
reg [15:0] writeBufferFront9;
reg [15:0] writeBufferFrontA;
reg [15:0] writeBufferFrontB;
reg [15:0] writeBufferFrontC;
reg [15:0] writeBufferFrontD;
reg [15:0] writeBufferFrontE;
//reg [15:0] writeBufferFrontF;

reg [15:0] writeBufferBack0;
reg [15:0] writeBufferBack1;
reg [15:0] writeBufferBack2;
reg [15:0] writeBufferBack3;
reg [15:0] writeBufferBack4;
reg [15:0] writeBufferBack5;
reg [15:0] writeBufferBack6;
reg [15:0] writeBufferBack7;
reg [15:0] writeBufferBack8;
reg [15:0] writeBufferBack9;
reg [15:0] writeBufferBackA;
reg [15:0] writeBufferBackB;
reg [15:0] writeBufferBackC;
reg [15:0] writeBufferBackD;
reg [15:0] writeBufferBackE;
reg [15:0] writeBufferBackF;
always@(posedge clk or negedge sys_rst_n) begin // 注入连续写缓存
  if(!sys_rst_n) begin
    write_sdram_req <= 0;
    writeAddressSdram <= 0;
  end else begin
    if(write_en)begin
      if         (writeAddressDataInCurr[3:0]== 0)begin writeBufferFront0 <= data_in;
      end else if(writeAddressDataInCurr[3:0]== 1)begin writeBufferFront1 <= data_in;
      end else if(writeAddressDataInCurr[3:0]== 2)begin writeBufferFront2 <= data_in;
      end else if(writeAddressDataInCurr[3:0]== 3)begin writeBufferFront3 <= data_in;
      end else if(writeAddressDataInCurr[3:0]== 4)begin writeBufferFront4 <= data_in;
      end else if(writeAddressDataInCurr[3:0]== 5)begin writeBufferFront5 <= data_in;
      end else if(writeAddressDataInCurr[3:0]== 6)begin writeBufferFront6 <= data_in;
      end else if(writeAddressDataInCurr[3:0]== 7)begin writeBufferFront7 <= data_in;
      end else if(writeAddressDataInCurr[3:0]== 8)begin writeBufferFront8 <= data_in;
      end else if(writeAddressDataInCurr[3:0]== 9)begin writeBufferFront9 <= data_in;
      end else if(writeAddressDataInCurr[3:0]==10)begin writeBufferFrontA <= data_in;
      end else if(writeAddressDataInCurr[3:0]==11)begin writeBufferFrontB <= data_in;
      end else if(writeAddressDataInCurr[3:0]==12)begin writeBufferFrontC <= data_in;
      end else if(writeAddressDataInCurr[3:0]==13)begin writeBufferFrontD <= data_in;
      end else if(writeAddressDataInCurr[3:0]==14)begin writeBufferFrontE <= data_in;
      end else if(writeAddressDataInCurr[3:0]==15)begin
        //发起sdram写入
        write_sdram_req <= 1;
        writeAddressSdram <= writeAddressDataInCurr[23:4];
        writeBufferBack0 <= writeBufferFront0;
        writeBufferBack1 <= writeBufferFront1;
        writeBufferBack2 <= writeBufferFront2;
        writeBufferBack3 <= writeBufferFront3;
        writeBufferBack4 <= writeBufferFront4;
        writeBufferBack5 <= writeBufferFront5;
        writeBufferBack6 <= writeBufferFront6;
        writeBufferBack7 <= writeBufferFront7;
        writeBufferBack8 <= writeBufferFront8;
        writeBufferBack9 <= writeBufferFront9;
        writeBufferBackA <= writeBufferFrontA;
        writeBufferBackB <= writeBufferFrontB;
        writeBufferBackC <= writeBufferFrontC;
        writeBufferBackD <= writeBufferFrontD;
        writeBufferBackE <= writeBufferFrontE;
        writeBufferBackF <= data_in;//writeBufferBack7 <= writeBufferFront7;
      end
    end
    if(write_sdram_ack)begin
      write_sdram_req <= 0;
    end
  end
end



reg read_vga_sdram_req_last;
reg read_sdram_req_last;
reg write_single_sdram_req_last;
reg write_sdram_req_last;

reg read_vga_sdram_ack;
reg read_sdram_ack;
reg write_single_sdram_ack;
reg write_sdram_ack;

reg [7:0]  sdram_timer8;
reg [15:0] readBuffer0;
reg [15:0] readBuffer1;
reg [15:0] readBuffer2;
reg [15:0] readBuffer3;
//sdram_rd_req sdram_rd_burst sdram_rd_addr
//
always@(posedge sdram_clk or negedge sys_rst_n) begin // sdram 主控
  if(!sys_rst_n) begin
    sdram_timer8 <= 0;
    
    read_vga_sdram_req_last <= 0;
    read_sdram_req_last <= 0;
    write_single_sdram_req_last <= 0;
    write_sdram_req_last <= 0;
    
    read_vga_sdram_ack <= 0;
    read_sdram_ack <= 0;
    write_single_sdram_ack <= 0;
    write_sdram_ack <= 0;
    
    sdram_rd_req <= 0;
    sdram_rd_burst <= 0;
    
  end else begin
    read_vga_sdram_req_last <= read_vga_sdram_req;
    read_sdram_req_last <= read_sdram_req;
    write_single_sdram_req_last <= write_single_sdram_req;
    write_sdram_req_last <= write_sdram_req;
    
    if          (read_vga_sdram_req && !read_vga_sdram_ack)begin
      //read vga
      
    end else if (read_sdram_req && !read_sdram_ack)begin
      if(!read_sdram_req_last)begin
        sdram_rd_addr <= {address[23:2],2'b0};
        sdram_rd_burst <= 4;
        sdram_timer8 <= 0;
        sdram_rd_req = 1;//只需要置高一个周期就可以了
      end else begin
        if(sdram_rd_ack)begin
          sdram_timer8 <= sdram_timer8 + 1'b1;
          if         (sdram_timer8==0)begin readBuffer0 <= sdram_dout;
          end else if(sdram_timer8==1)begin readBuffer1 <= sdram_dout;
          end else if(sdram_timer8==2)begin readBuffer2 <= sdram_dout;
          end else if(sdram_timer8==3)begin readBuffer3 <= sdram_dout;
            sdram_rd_req <= 0;
            read_sdram_ack <= 1;
          end else begin
          end
        end
      end
    end else if (write_single_sdram_req && !write_single_sdram_ack)begin
      if(!write_single_sdram_req_last)begin
        sdram_wr_addr <= address;
        sdram_wr_burst <= 1;
        sdram_timer8 <= 0;
        sdram_wr_req <= 1;//只需要置高一个周期就可以了
      end else begin
        if(sdram_wr_ack)begin
          sdram_timer8 <= sdram_timer8 + 1'b1;
          if         (sdram_timer8==0)begin sdram_din <= data_in;
          end else if(sdram_timer8==1)begin
            sdram_wr_req <= 0;
            write_single_sdram_ack <= 1;
          end
        end
      end
    end else if(write_sdram_req && !write_sdram_ack)begin
      if(!write_sdram_req_last) begin
        sdram_wr_addr <= {writeAddressSdram,4'b0};//20bit+4bit
        sdram_wr_burst <= 16;
        sdram_timer8 <= 0;
        sdram_wr_req <= 1;//只需要置高一个周期就可以了
      end else begin
        if(sdram_wr_ack)begin
          sdram_timer8 <= sdram_timer8 + 1'b1;
          if         (sdram_timer8== 0)begin sdram_din <= writeBufferBack0;
          end else if(sdram_timer8== 1)begin sdram_din <= writeBufferBack1;
          end else if(sdram_timer8== 2)begin sdram_din <= writeBufferBack2;
          end else if(sdram_timer8== 3)begin sdram_din <= writeBufferBack3;
          end else if(sdram_timer8== 4)begin sdram_din <= writeBufferBack4;
          end else if(sdram_timer8== 5)begin sdram_din <= writeBufferBack5;
          end else if(sdram_timer8== 6)begin sdram_din <= writeBufferBack6;
          end else if(sdram_timer8== 7)begin sdram_din <= writeBufferBack7;
          end else if(sdram_timer8== 8)begin sdram_din <= writeBufferBack8;
          end else if(sdram_timer8== 9)begin sdram_din <= writeBufferBack9;
          end else if(sdram_timer8==10)begin sdram_din <= writeBufferBackA;
          end else if(sdram_timer8==11)begin sdram_din <= writeBufferBackB;
          end else if(sdram_timer8==12)begin sdram_din <= writeBufferBackC;
          end else if(sdram_timer8==13)begin sdram_din <= writeBufferBackD;
          end else if(sdram_timer8==14)begin sdram_din <= writeBufferBackE;
          end else if(sdram_timer8==15)begin sdram_din <= writeBufferBackF;
          end else if(sdram_timer8==16)begin
            sdram_wr_req <= 0;
            write_sdram_ack <= 1;
          end else begin
          end
        end
      end
      
    end else begin
      if(!read_vga_sdram_req && read_vga_sdram_ack)begin
        read_vga_sdram_ack <= 0;
      end
      if(!read_sdram_req && read_sdram_ack)begin
        read_sdram_ack <= 0;
      end
      if(!write_single_sdram_req && write_single_sdram_ack)begin
        write_single_sdram_ack <= 0;
      end
      if(!write_sdram_req && write_sdram_ack)begin
        write_sdram_ack <= 0;
      end
    end
  end
end

endmodule 
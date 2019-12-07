
module flow_led(
		input           sys_clk,          //外部50M时钟
		input           sys_rst_n,        //外部复位信号，低有效
		input 		[3:0]			key,
		output  reg  [3:0]  led,         //4个LED灯
		
    //SDRAM接口
    output                sdram_clk   ,  //SDRAM 时钟
    output                sdram_cke   ,  //SDRAM 时钟有效
    output                sdram_cs_n  ,  //SDRAM 片选
    output                sdram_ras_n ,  //SDRAM 行有效
    output                sdram_cas_n ,  //SDRAM 列有效
    output                sdram_we_n  ,  //SDRAM 写有效
    output       [1:0]    sdram_ba    ,  //SDRAM Bank地址
    output       [1:0]    sdram_dqm   ,  //SDRAM 数据掩码
    output       [12:0]   sdram_addr  ,  //SDRAM 地址
    inout        [15:0]   sdram_data  ,  //SDRAM 数据    

	 //uart接口
		input           uart_rxd,         //UART接收端口
		output          uart_txd          //UART发送端口
		);
 
	wire  [3:0]  key_stable;

assign  rst_n = sys_rst_n & locked;
	
//锁相环
pll_clk u_pll_clk(
    .areset       (1'b0           ),
    .inclk0       (sys_clk        ),
    .c0           (clk_100m       ),
    .c1           (clk_100m_shift ),
    .c2           (clk_50m        ),
    .c3           (clk_50m_180deg ),
    .c4           (clk_25m        ),
    .locked       (locked         )
    );

//SDRAM 控制器顶层模块,封装成FIFO接口
//SDRAM 控制器地址组成: {bank_addr[1:0],row_addr[12:0],col_addr[8:0]}
sdram_top u_sdram_top(
 .ref_clk      (clk_100m),                   //sdram 控制器参考时钟
 .out_clk      (clk_100m_shift),             //用于输出的相位偏移时钟
 .rst_n        (rst_n),                      //系统复位
                                             
  //用户写端口                                   
 .wr_clk       (clk_50m),                    //写端口FIFO: 写时钟
 .wr_en        (wr_en),                      //写端口FIFO: 写使能
 .wr_data      (wr_data),                    //写端口FIFO: 写数据
 .wr_min_addr  (24'd0),                      //写SDRAM的起始地址
 .wr_max_addr  (PHOTO_H_PIXEL*PHOTO_V_PIXEL),//写SDRAM的结束地址
 .wr_len       (10'd512),                    //写SDRAM时的数据突发长度
 .wr_load      (~rst_n),                     //写端口复位: 复位写地址,清空写FIFO
                                             
  //用户读端口                                  
 .rd_clk       (clk_25m),                    //读端口FIFO: 读时钟
 .rd_en        (rd_en),                      //读端口FIFO: 读使能
 .rd_data      (rd_data),                    //读端口FIFO: 读数据
 .rd_min_addr  (24'd0),                      //读SDRAM的起始地址
 .rd_max_addr  (PHOTO_H_PIXEL*PHOTO_V_PIXEL),//读SDRAM的结束地址
 .rd_len       (10'd512),                    //从SDRAM中读数据时的突发长度
 .rd_load      (~rst_n),                     //读端口复位: 复位读地址,清空读FIFO
                                             
 //用户控制端口                                
 .sdram_read_valid  (1'b1),                  //SDRAM 读使能
 .sdram_pingpang_en (1'b0),                  //SDRAM 乒乓操作使能
 .sdram_init_done (sdram_init_done),         //SDRAM 初始化完成标志
                                             
 //SDRAM 芯片接口                                
 .sdram_clk    (sdram_clk),                  //SDRAM 芯片时钟
 .sdram_cke    (sdram_cke),                  //SDRAM 时钟有效
 .sdram_cs_n   (sdram_cs_n),                 //SDRAM 片选
 .sdram_ras_n  (sdram_ras_n),                //SDRAM 行有效
 .sdram_cas_n  (sdram_cas_n),                //SDRAM 列有效
 .sdram_we_n   (sdram_we_n),                 //SDRAM 写有效
 .sdram_ba     (sdram_ba),                   //SDRAM Bank地址
 .sdram_addr   (sdram_addr),                 //SDRAM 行/列地址
 .sdram_data   (sdram_data),                 //SDRAM 数据
 .sdram_dqm    (sdram_dqm)                   //SDRAM 数据掩码
    );

	
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


















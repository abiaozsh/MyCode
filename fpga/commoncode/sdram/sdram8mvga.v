module sdram8mvga(
    input  sys_clk,
    input  sys_rst_n,

    //SDRAM 芯片接口
    output        sdram_clk_out,            //SDRAM 芯片时钟
    output        sdram_cke,                //SDRAM 时钟有效
    output        sdram_cs_n,               //SDRAM 片选
    output        sdram_ras_n,              //SDRAM 行有效
    output        sdram_cas_n,              //SDRAM 列有效
    output        sdram_we_n,               //SDRAM 写有效
    output  [1:0] sdram_ba,                 //SDRAM Bank地址 **
    output [11:0] sdram_addr,               //SDRAM 行/列地址 **
    inout  [15:0] sdram_data,               //SDRAM 数据

    input        buffDMAwrite_req   ,
    input [13:0] buffDMAwrite_addr  ,//14+8 **
    input        buffDMAwrite_A_B   ,
    output reg   buffDMAwrite_ack   ,
    input        buffDMAWrite_clk   ,
    input [15:0] buffDMAWriteAB_data,
    input  [7:0] buffDMAWriteAB_addr,//8bit 256 word sub page
    input        buffDMAWriteA_en   ,
    input        buffDMAWriteB_en   ,

    input         read_line_req  ,
    input         read_line_A_B  ,
    input  [11:0] read_line_addr ,//line address -> 1024 * 1024 word * 4 frame = 4Mword  **

    output [15:0] read_pixelA_data,
    output [15:0] read_pixelB_data,
    input   [9:0] read_pixel_addr ,//10bit 1024word per line
    input         read_pixel_clk  ,

    input         isDMA,
    
    output reg busy
);


wire  [7:0] buffDMAWrite_address;
assign buffDMAWrite_address = sdram_timer2[7:0];

reg [15:0] buffAB_wrdata;
reg [9:0]  buffAB_wraddress;
reg        buffA_wren;
reg        buffB_wren;

wire [15:0] data      ;
wire  [9:0] wraddress ;
wire  [9:0] rdaddress ;
wire        wrclock   ;
wire        rdclock   ;
wire        wrenA     ;
wire        wrenB     ;

assign data       = isDMA ? buffDMAWriteAB_data          : buffAB_wrdata    ;
assign wraddress  = isDMA ? {2'b00,buffDMAWriteAB_addr}  : buffAB_wraddress ;
assign rdaddress  = isDMA ? {2'b00,buffDMAWrite_address} : read_pixel_addr  ;
assign wrclock    = isDMA ? buffDMAWrite_clk             : sdram_clk        ;
assign rdclock    = isDMA ? sdram_clk                    : read_pixel_clk   ;
assign wrenA      = isDMA ? buffDMAWriteA_en             : buffA_wren       ;
assign wrenB      = isDMA ? buffDMAWriteB_en             : buffB_wren       ;

wire [15:0] buffDMAWriteA_q;
wire [15:0] buffDMAWriteB_q;

assign read_pixelA_data = qA;
assign buffDMAWriteA_q  = qA;
wire [15:0] qA;
buff1024x16  buffReadA (
  .data      ( data             ),
  .wraddress ( wraddress        ),
  .wrclock   ( wrclock          ),
  .wren      ( wrenA            ),
  .rdaddress ( rdaddress        ),
  .rdclock   ( rdclock          ),
  .q         ( qA               )
);
assign read_pixelB_data = qB;
assign buffDMAWriteB_q  = qB;
wire [15:0] qB;
buff1024x16  buffReadB (
  .data      ( data             ),
  .wraddress ( wraddress        ),
  .wrclock   ( wrclock          ),
  .wren      ( wrenB            ),
  .rdaddress ( rdaddress        ),
  .rdclock   ( rdclock          ),
  .q         ( qB               )
);

wire rst_n;
wire clk_50m;
wire clk_100m;
wire clk_100m_shift;
wire locked;
//待PLL输出稳定之后，停止系统复位
assign rst_n = sys_rst_n & locked;

//例化PLL, 产生各模块所需要的时钟
pll_clk ins_pll_clk(
//pll_2m(
  .inclk0             (sys_clk),
  .areset             (~sys_rst_n),
  
  .c0                 (clk_50m),
  .c1                 (clk_100m),
  .c2                 (clk_100m_shift),
  .locked             (locked)
);


wire sdram_clk;
assign sdram_clk = clk_100m;
assign  sdram_clk_out = clk_100m_shift;//out_clk;                //将相位偏移时钟输出给sdram芯片


reg         sdram_wr_req   ; //写SDRAM请求信号              input 
wire        sdram_wr_ack   ; //写SDRAM响应信号              output
reg  [21:0] sdram_rw_addr  ; //SDRAM写操作的地址            input  22bit 4M Word **
reg  [ 9:0] sdram_wr_burst ; //写sdram时数据突发长度        input 
reg  [15:0] sdram_din      ; //写入SDRAM的数据              input 
reg         sdram_rd_req   ; //读SDRAM请求信号              input 
wire        sdram_rd_ack   ; //读SDRAM响应信号              output
reg  [ 9:0] sdram_rd_burst ; //读sdram时数据突发长度        input 
wire [15:0] sdram_dout     ; //从SDRAM读出的数据            output
wire        sdram_init_done; //SDRAM 初始化完成标志         output

//SDRAM控制器
sdram8m_controller ins_sdram8m_controller(
  .clk        (sdram_clk),      //sdram 控制器时钟
  .rst_n      (rst_n),      //系统复位

  //SDRAM 芯片接口
  .sdram_cke       (sdram_cke),     //SDRAM 时钟有效
  .sdram_cs_n      (sdram_cs_n),    //SDRAM 片选
  .sdram_ras_n     (sdram_ras_n),   //SDRAM 行有效  
  .sdram_cas_n     (sdram_cas_n),   //SDRAM 列有效
  .sdram_we_n      (sdram_we_n),    //SDRAM 写有效
  .sdram_ba        (sdram_ba),      //SDRAM Bank地址
  .sdram_addr      (sdram_addr),    //SDRAM 行/列地址
  .sdram_data      (sdram_data),    //SDRAM 数据  
  
  //SDRAM 控制器端口  
  .sdram_rw_addr     (sdram_rw_addr),   //sdram 写地址
  
  .sdram_wr_req      (sdram_wr_req),   //sdram 写请求
  .sdram_wr_ack      (sdram_wr_ack),   //sdram 写响应
  .sdram_wr_burst    (sdram_wr_burst),        //写sdram时数据突发长度
  .sdram_din         (sdram_din),      //写入sdram中的数据
  .sdram_rd_req      (sdram_rd_req),   //sdram 读请求
  .sdram_rd_ack      (sdram_rd_ack),    //sdram 读响应
  .sdram_rd_burst    (sdram_rd_burst),        //读sdram时数据突发长度
  .sdram_dout        (sdram_dout),     //从sdram中读出的数据
  
  .block_auto_refresh (0),
  
  .sdram_init_done  (sdram_init_done)  //sdram 初始化完成标志

);


reg read_line_req_buff;
reg buffDMAwrite_req_buff;
reg read_line_ack;

reg  [2:0] sdram_timer1;
reg  [1:0] sram_add_high;
reg  [8:0] sdram_timer2;
reg        sdram_page_delay;
reg        sdram_timer0;
//sdram_rd_req sdram_rd_burst sdram_rd_addr
always@(posedge sdram_clk or negedge sys_rst_n) begin // sdram 主控
  if(!sys_rst_n) begin
    sdram_timer0 <= 0;
    
    read_line_req_buff <= 0;
    buffDMAwrite_req_buff <= 0;

    read_line_ack <= 0;
		buffDMAwrite_ack <= 0;

    sdram_rd_req <= 0;
    sdram_rd_burst <= 0;
    
  end else begin
    read_line_req_buff <= read_line_req;
    buffDMAwrite_req_buff <= buffDMAwrite_req;
    
    busy<=0;
    buffA_wren<=0;
    buffB_wren<=0;
    
    //vga line read
    if          (read_line_req_buff && !read_line_ack)begin
      busy<=1;
      sdram_timer0 <= 1;
      if(sdram_timer0 == 0)begin
        if         (sdram_timer1 == 0) begin sram_add_high <= 0; sdram_timer1 <= 1; sdram_rw_addr <= {read_line_addr,2'b00,8'b0}; //12+2+8  **
        end else if(sdram_timer1 == 1) begin sram_add_high <= 1; sdram_timer1 <= 2; sdram_rw_addr <= {read_line_addr,2'b01,8'b0}; //12+2+8  **
        end else if(sdram_timer1 == 2) begin sram_add_high <= 2; sdram_timer1 <= 3; sdram_rw_addr <= {read_line_addr,2'b10,8'b0}; //12+2+8  **
        end else if(sdram_timer1 == 3) begin sram_add_high <= 3; sdram_timer1 <= 4; sdram_rw_addr <= {read_line_addr,2'b11,8'b0}; //12+2+8  **
        end
        sdram_rd_burst <= 256;
        sdram_timer2 <= 0;
        sdram_page_delay <= 0;
        sdram_rd_req = 1;//只需要置高一个周期就可以了
      end else begin
        if(sdram_rd_ack || sdram_page_delay)begin
          sdram_timer2 <= sdram_timer2 + 1'b1;
          if(!sdram_page_delay)begin
            buffAB_wrdata <= sdram_dout;
            buffAB_wraddress <= {sram_add_high,sdram_timer2[7:0]};
            if(read_line_A_B)begin
              buffA_wren <= 1;
            end else begin
              buffB_wren <= 1;
            end
          end
          if(sdram_timer2==255)begin 
            sdram_page_delay <= 1;
            sdram_rd_req <= 0;
          end else if(sdram_timer2==256)begin //263 TODO reduce
            sdram_timer0 <= 0;
            if(sdram_timer1 == 4)begin
              sdram_timer1 <= 0;
              read_line_ack <= 1;
            end
          end else begin
          end
        end
      end
      
    end else if(buffDMAwrite_req_buff && !buffDMAwrite_ack)begin
      busy<=1;
      sdram_timer0 <= 1;
      if(sdram_timer0 == 0) begin
        sdram_rw_addr <= {buffDMAwrite_addr,8'b0};//14+8 **
        sdram_wr_burst <= 256;
        sdram_timer2 <= 0;
        sdram_page_delay <= 0;
        sdram_wr_req <= 1;//只需要置高一个周期就可以了
      end else begin
        if(sdram_wr_ack || sdram_page_delay)begin
          sdram_timer2 <= sdram_timer2 + 1'b1;
          if(!sdram_page_delay)begin
            if(buffDMAwrite_A_B)begin
              sdram_din <= buffDMAWriteA_q;
            end else begin
              sdram_din <= buffDMAWriteB_q;
            end
          end
          if(sdram_timer2==255)begin
            sdram_page_delay <= 1;
            sdram_wr_req <= 0;
          end else if(sdram_timer2==256)begin //263 TODO reduce
            sdram_timer0 <= 0;
            buffDMAwrite_ack <= 1;
          end
        end
      end

    end else begin
    
      if(!read_line_req_buff && read_line_ack)begin
        read_line_ack <= 0;
      end
      if(!buffDMAwrite_req_buff && buffDMAwrite_ack)begin
        buffDMAwrite_ack <= 0;
      end

    end
  end
end

endmodule 
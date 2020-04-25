module sdrambusvga(
    input          clk,                // clock.clk
    input          reset_n,               // reset.reset

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

    
    input   [22:0]     avs_s0_address,     //    s0.address
    input              avs_s0_read,        //      .read
    input              avs_s0_write,       //      .write
    output  reg [31:0] avs_s0_readdata,    //      .readdata
    input   [31:0]     avs_s0_writedata,   //      .writedata
    output             avs_s0_waitrequest, //      .waitrequest
    input   [3:0]      avs_s0_byteenable,    //      .readdata

    input              flush_cache,
    input   [14:0]     flush_page,

    input         read_line_req  ,
    input         read_line_A_B  ,
    input  [15:0] read_line_addr ,//line address -> 1024 * 1024 word * 4 frame = 4Mword  **

    output [15:0] read_pixelA_data,
    output [15:0] read_pixelB_data,
    input   [9:0] read_pixel_addr ,//10bit 1024word per line
    input         read_pixel_clk  ,
    
    output reg [7:0] debug8,
    output [15:0] cache_life0   ,
    output [15:0] cache_life1   ,
    output [15:0] cache_life2   ,
    output [15:0] cache_life3   ,
    output [15:0] cacheAddrHigh0,
    output [15:0] cacheAddrHigh1,
    output [15:0] cacheAddrHigh2,
    output [15:0] cacheAddrHigh3,
    output [31:0] debug32

);
wire sys_clk = clk;
wire sys_rst_n = reset_n;

`include "config.v"

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




reg [15:0] buffAB_wrdata;
reg [9:0]  buffAB_wraddress;
reg        buffA_wren;
reg        buffB_wren;

wire        wrclock    = sdram_clk        ;
wire        rdclock    = read_pixel_clk   ;
wire [15:0] data       = buffAB_wrdata    ;
wire  [9:0] wraddress  = buffAB_wraddress ;
wire  [9:0] rdaddress  = read_pixel_addr  ;
wire        wrenA      = buffA_wren       ;
wire        wrenB      = buffB_wren       ;

assign      read_pixelA_data = qA;
wire [15:0] qA;
`ifdef IS_ALTERA
buff1024x16  buffReadA (
  .data      ( data             ),
  .wraddress ( wraddress        ),
  .wrclock   ( wrclock          ),
  .wren      ( wrenA            ),
  .rdaddress ( rdaddress        ),
  .rdclock   ( rdclock          ),
  .q         ( qA               )
);
`endif
assign      read_pixelB_data = qB;
wire [15:0] qB;
`ifdef IS_ALTERA
buff1024x16  buffReadB (
  .data      ( data             ),
  .wraddress ( wraddress        ),
  .wrclock   ( wrclock          ),
  .wren      ( wrenB            ),
  .rdaddress ( rdaddress        ),
  .rdclock   ( rdclock          ),
  .q         ( qB               )
);
`endif










wire sdram_clk;
assign sdram_clk = clk_100m;
assign  sdram_clk_out = clk_100m_shift;//out_clk;                //将相位偏移时钟输出给sdram芯片


reg         sdram_wr_req    ;    //写SDRAM请求信号              input 
wire        sdram_wr_ack    ;    //写SDRAM响应信号              output
reg [23:0]  sdram_rw_addr   ;  //SDRAM写操作的地址            input 
reg [ 9:0]  sdram_wr_burst  ;   //写sdram时数据突发长度      input 
reg [15:0]  sdram_din       ;      //写入SDRAM的数据              input 
reg [1:0]   sdram_mask      ;
reg         sdram_rd_req    ;    //读SDRAM请求信号              input 
wire        sdram_rd_ack    ;    //读SDRAM响应信号              output
reg [ 9:0]  sdram_rd_burst  ;   //读sdram时数据突发长度      input 
wire [15:0] sdram_dout      ;      //从SDRAM读出的数据            output
wire        sdram_init_done ;  //SDRAM 初始化完成标志       output

//SDRAM控制器
sdram_controller ins_sdram_controller(
  .clk        (sdram_clk),      //sdram 控制器时钟
  .rst_n        (rst_n),      //系统复位

  //SDRAM 芯片接口
  .sdram_cke       (sdram_cke),    //SDRAM 时钟有效
  .sdram_cs_n      (sdram_cs_n),    //SDRAM 片选
  .sdram_ras_n     (sdram_ras_n),    //SDRAM 行有效  
  .sdram_cas_n     (sdram_cas_n),    //SDRAM 列有效
  .sdram_we_n      (sdram_we_n),    //SDRAM 写有效
  .sdram_ba        (sdram_ba),      //SDRAM Bank地址
  .sdram_addr      (sdram_addr),    //SDRAM 行/列地址
  .sdram_data      (sdram_data),    //SDRAM 数据
  .sdram_dqm       (sdram_dqm),    //SDRAM 数据
  
  //SDRAM 控制器端口  
  .sdram_rw_addr     (sdram_rw_addr),   //sdram 地址
  .sdram_wr_req      (sdram_wr_req),   //sdram 写请求
  .sdram_wr_ack      (sdram_wr_ack),   //sdram 写响应
  .sdram_wr_burst    (sdram_wr_burst),        //写sdram时数据突发长度
  .sdram_din         (sdram_din),      //写入sdram中的数据
  .sdram_mask        (sdram_mask),
  .sdram_rd_req      (sdram_rd_req),   //sdram 读请求
  .sdram_rd_ack      (sdram_rd_ack),    //sdram 读响应
  .sdram_rd_burst    (sdram_rd_burst),        //读sdram时数据突发长度
  .sdram_dout        (sdram_dout),     //从sdram中读出的数据
  
  .block_auto_refresh (0),//write_en
  
  .sdram_init_done  (sdram_init_done)  //sdram 初始化完成标志

);


  assign cache_life0    = cache_life[0]   ;
  assign cache_life1    = cache_life[1]   ;
  assign cache_life2    = cache_life[2]   ;
  assign cache_life3    = cache_life[3]   ;
  assign cacheAddrHigh0 = cacheAddrHigh[0];
  assign cacheAddrHigh1 = cacheAddrHigh[1];
  assign cacheAddrHigh2 = cacheAddrHigh[2];
  assign cacheAddrHigh3 = cacheAddrHigh[3];


parameter CACHE_COUNT = 4;

assign debug32 = avs_s0_address[22:8];

reg cacheAddrLow8_writeBack;
wire  [ 7:0] cacheAddrLow8 = cacheAddrLow8_writeBack ? write_back_count[7:0] : avs_s0_address[7:0];
reg   [35:0] cacheData;
wire         wren[CACHE_COUNT];
reg          write_enable;

reg  [15:0] cacheAddrHigh[CACHE_COUNT];//32Mbyte / 1024byte per cache slot = total 32k cache slot
wire [35:0] cacheq[CACHE_COUNT];
wire        cache_hit[CACHE_COUNT];
wire        cache_flush_hit[CACHE_COUNT];
reg  [15:0] cache_life[CACHE_COUNT];
reg  [15:0] cache_life_reset;
reg         adj_cache_life;//update when finish
reg   [2:0] set_cacheAddrHigh;
reg   [2:0] clr_cacheAddrHigh;
genvar i;
generate
  for(i=0; i<CACHE_COUNT; i=i+1) begin:BLOCK1
    cache256x36 (
      .address(cacheAddrLow8),//input	[7:0]  address;
      .clock  (clk          ),  //input	  clock;
      .data   (cacheData    ),   //input	[35:0]  data;
      .wren   (wren[i]      ),   //input	  wren;
      .q      (cacheq[i]    ));     //output	[35:0]  q;
    assign cache_hit[i]       = cacheAddrHigh[i][15] == 0 && avs_s0_address[22:8] == cacheAddrHigh[i][14:0];
    assign cache_flush_hit[i] = cacheAddrHigh[i][15] == 0 &&           flush_page == cacheAddrHigh[i][14:0];
    assign wren[i] = (current_slot == (i+1)) ? write_enable : 1'b0;
    always@(posedge clk or negedge sys_rst_n) begin
      if(!sys_rst_n) begin
        //cache_life[i]<=512;
        //cache_life[i]<=16;
        //cacheAddrHigh[i] <= i;
        cache_life[i]<=0;
        cacheAddrHigh[i] <= {1'b1,15'b0};
      end else begin
        if(adj_cache_life)begin//(i+1) == cache_hited
          //if(cache_hit[i])begin
          if((i+1)==current_slot)begin
            //if(cache_life[i]<(65535-CACHE_COUNT))begin
            if(cache_life[i]<(256-CACHE_COUNT))begin
              cache_life[i]<=cache_life[i]+CACHE_COUNT*2;
            end
          end else begin
            if(cache_life[i]!=0)begin
              cache_life[i]<=cache_life[i]-1'b1;
            end
          end
        end
        
        if((i+1)==set_cacheAddrHigh)begin
          cacheAddrHigh[i] <= avs_s0_address[22:8];
          //cache_life[i] <= 512;
          cache_life[i] <= 16;
        end
        if((i+1)==clr_cacheAddrHigh)begin
          cacheAddrHigh[i] <= {1'b1,15'b0};
          cache_life[i] <= 0;
        end
      end
    end
  end
endgenerate

reg  [2:0] current_slot;
wire [2:0] cache_hited       = /**/cache_hit[0]       ? 1 :
                               /**/cache_hit[1]       ? 2 :
                               /**/cache_hit[2]       ? 3 :
                               /**/cache_hit[3]       ? 4 : 0;
wire [2:0] cache_flush_hited = /**/cache_flush_hit[0] ? 1 :
                               /**/cache_flush_hit[1] ? 2 :
                               /**/cache_flush_hit[2] ? 3 :
                               /**/cache_flush_hit[3] ? 4 : 0;
wire [2:0] free_cache        = /**/cache_life[0] == 0 ? 1 :
                               /**/cache_life[1] == 0 ? 2 :
                               /**/cache_life[2] == 0 ? 3 :
                               /**/cache_life[3] == 0 ? 4 : 0;



parameter FLG_VALID = 35;//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,}
parameter FLG_DIRTY = 34;


wire [35:0] cache_hit_data        = current_slot == 0 ? 36'b0 : cacheq[current_slot-1];
wire [14:0] current_cacheAddrHigh = current_slot == 0 ? 15'b0 : cacheAddrHigh[current_slot-1][14:0];


reg [2:0]  interface_status;//0~8
parameter STATUS_INIT              = 0;
parameter STATUS_HITED1            = 1;
parameter STATUS_READ              = 2;
parameter STATUS_WRITE_BACK_DLY    = 3;
parameter STATUS_WRITE_BACK        = 4;
parameter STATUS_WRITE_BACK_EXEC   = 5;
parameter STATUS_WRITE_BACK_FINISH = 6;
parameter STATUS_WRITE             = 7;

`define RD_FINISH interface_status <= STATUS_INIT;avs_s0_read_ack  <= 1;
`define WR_FINISH interface_status <= STATUS_INIT;avs_s0_write_ack <= 1;

reg        read_sdram_req;
reg [23:0] rdwr_sdram_addr;
reg        write_single_sdram_req;
reg [31:0] write_single_sdram_data;
reg [3:0]  write_single_sdram_mask;
reg avs_s0_read_ack;
reg avs_s0_write_ack;
reg read_sdram_ack_buff;
reg write_single_sdram_ack_buff;
reg [10:0] write_back_count;
assign avs_s0_waitrequest = (avs_s0_read && !avs_s0_read_ack) || (avs_s0_write && !avs_s0_write_ack);
always@(posedge clk or negedge sys_rst_n) begin
  if(!sys_rst_n) begin
    avs_s0_read_ack <= 0;
    avs_s0_write_ack <= 0;
    avs_s0_readdata <= 0;
    interface_status <= 0;
    read_sdram_req <= 0;
    rdwr_sdram_addr <= 0;
    write_single_sdram_req <= 0;
    write_single_sdram_data <= 0;
    write_single_sdram_mask <= 4'b0000;
    read_sdram_ack_buff <= 0;
    write_single_sdram_ack_buff <= 0;
    current_slot <= 0;
    write_enable <= 0;
    adj_cache_life <= 0;
    debug8 <= 0;
    cacheAddrLow8_writeBack <= 0;
  end else begin
    read_sdram_ack_buff <= read_sdram_ack;
    write_single_sdram_ack_buff <= write_single_sdram_ack;
    
    write_enable <= 0;
    adj_cache_life <= 0;
    clr_cacheAddrHigh <= 0;
    set_cacheAddrHigh <= 0;
    //载入cache后，第一次读内容不对
    if(avs_s0_read && !avs_s0_read_ack)begin
      if         (interface_status==STATUS_INIT)begin//初始化
        debug8 <= 0;
        if(cache_hited != 0)begin
          current_slot <= cache_hited;
          interface_status <= STATUS_HITED1;//高地址命中等一个周期
        end else begin
          if(flush_cache && cache_flush_hited != 0)begin
            current_slot <= cache_flush_hited;
            write_back_count <= 0;
            cacheAddrLow8_writeBack <= 1;
            interface_status <= STATUS_WRITE_BACK_DLY;
          end else begin
            if(free_cache != 0)begin//找到空闲cache
              current_slot <= free_cache;
              if(cacheAddrHigh[free_cache-1][15])begin//invalid 无效 直接使用
                interface_status <= STATUS_HITED1; set_cacheAddrHigh <= free_cache;//当前地址写入缓存地址高
              end else begin
                write_back_count <= 0;
                cacheAddrLow8_writeBack <= 1;
                interface_status <= STATUS_WRITE_BACK_DLY;
              end
            end else begin//不经过cache
              current_slot <= 0;
              rdwr_sdram_addr <= {avs_s0_address,1'b0};
              read_sdram_req <= 1;
              interface_status <= STATUS_READ;
            end
          end
        end
      end else if(interface_status==STATUS_HITED1)begin//高地址命中
        if(cache_hit_data[FLG_VALID])begin
          avs_s0_readdata <= cache_hit_data[31:0];
          adj_cache_life <= 1;
          `RD_FINISH
        end else begin
          interface_status <= STATUS_READ;
          rdwr_sdram_addr <= {avs_s0_address,1'b0};
          read_sdram_req <= 1;
        end
      end else if(interface_status==STATUS_READ)begin//读取等待
        if(read_sdram_ack_buff)begin
          read_sdram_req <= 0;
          avs_s0_readdata <= readBuffer;
          if(current_slot != 0)begin
            write_enable <= 1;
            cacheData <= {4'b1000,readBuffer};//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
          end
          adj_cache_life <= 1;
          `RD_FINISH
        end
      end else if(interface_status==STATUS_WRITE_BACK_DLY)begin//sram读出延时
        if(write_back_count==256)begin
          cacheAddrLow8_writeBack <= 0;
          if(flush_cache && current_slot != 0)begin
            debug8[0] <= 1;//0  00000110
            clr_cacheAddrHigh <= current_slot;
            `RD_FINISH
          end else begin
            debug8[1] <= 1;//1  00000110
            interface_status <= STATUS_HITED1;set_cacheAddrHigh <= current_slot;//当前地址写入缓存地址高
          end
        end else begin
          interface_status <= STATUS_WRITE_BACK;
          debug8[2] <= 1;//1  00000110
          write_enable <= 1;//写回后要置空
          cacheData <= {36'b0};//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
        end
      end else if(interface_status==STATUS_WRITE_BACK)begin//dirty 的话，写回
        if(cache_hit_data[FLG_DIRTY])begin
          interface_status <= STATUS_WRITE_BACK_EXEC;
          rdwr_sdram_addr <= {current_cacheAddrHigh,write_back_count[7:0],1'b0};
          write_single_sdram_data <= cache_hit_data[31:0];
          write_single_sdram_mask <= 4'b0000;
          write_single_sdram_req <= 1;
        end else begin
          write_back_count <= write_back_count + 1'b1;
          interface_status <= STATUS_WRITE_BACK_DLY;
        end
      end else if(interface_status==STATUS_WRITE_BACK_EXEC)begin//写回执行
        if(write_single_sdram_ack_buff)begin
          write_single_sdram_req <= 0;
          write_back_count <= write_back_count + 1'b1;
          interface_status <= STATUS_WRITE_BACK_DLY;
        end
      end
    end

    if(!avs_s0_read && avs_s0_read_ack)begin
      avs_s0_read_ack <= 0;
    end



    
    if(avs_s0_write && !avs_s0_write_ack)begin
      if         (interface_status==STATUS_INIT)begin//初始化
        if(cache_hited)begin
          current_slot <= cache_hited;
          if(avs_s0_byteenable==4'b1111)begin//完整写入DW的话，不考虑FLG_VALID
            write_enable <= 1;
            cacheData <= {4'b1100,avs_s0_writedata};//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
            adj_cache_life <= 1;
            `WR_FINISH
          end else begin
            interface_status <= STATUS_HITED1;//高地址命中等一个周期
          end
        end else begin
          if(free_cache != 0)begin//有闲置cache
            current_slot <= free_cache;
            if(cacheAddrHigh[free_cache-1][15])begin//invalid 无效 直接使用
              set_cacheAddrHigh <= free_cache;//当前地址写入缓存地址高
              interface_status <= STATUS_WRITE_BACK_FINISH;
            end else begin
              write_back_count <= 0;
              cacheAddrLow8_writeBack <= 1;
              interface_status <= STATUS_WRITE_BACK_DLY;
            end
          end else begin
            //直接写入
            current_slot <= 0;
            interface_status <= STATUS_WRITE;//高地址命中等一个周期
            rdwr_sdram_addr <= {avs_s0_address,1'b0};
            write_single_sdram_data <= avs_s0_writedata;
            write_single_sdram_mask <= ~avs_s0_byteenable;
            write_single_sdram_req <= 1;
          end
        end
      end else if(interface_status==STATUS_HITED1)begin//高地址命中
        if(cache_hit_data[FLG_VALID])begin//有效
            write_enable <= 1;
            cacheData <= {4'b1100,
              avs_s0_byteenable[3] ? avs_s0_writedata[31:24] : cache_hit_data[31:24],
              avs_s0_byteenable[2] ? avs_s0_writedata[23:16] : cache_hit_data[23:16],
              avs_s0_byteenable[1] ? avs_s0_writedata[15: 8] : cache_hit_data[15: 8],
              avs_s0_byteenable[0] ? avs_s0_writedata[ 7: 0] : cache_hit_data[ 7: 0]
              };//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
            adj_cache_life <= 1;
            `WR_FINISH
        end else begin
          //读取后写入
            interface_status <= STATUS_READ;
            rdwr_sdram_addr <= {avs_s0_address,1'b0};
            read_sdram_req <= 1;
        end
      end else if(interface_status==STATUS_READ)begin//读取等待
        if(read_sdram_ack_buff)begin
          read_sdram_req <= 0;
          write_enable <= 1;
          cacheData <= {4'b1100,
            avs_s0_byteenable[3] ? avs_s0_writedata[31:24] : readBuffer[31:24],
            avs_s0_byteenable[2] ? avs_s0_writedata[23:16] : readBuffer[23:16],
            avs_s0_byteenable[1] ? avs_s0_writedata[15: 8] : readBuffer[15: 8],
            avs_s0_byteenable[0] ? avs_s0_writedata[ 7: 0] : readBuffer[ 7: 0]
            };//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
          adj_cache_life <= 1;
          `WR_FINISH
        end
      end else if(interface_status==STATUS_WRITE_BACK_DLY)begin//sram读出延时
        if(write_back_count==256)begin
          set_cacheAddrHigh <= current_slot;//当前地址写入缓存地址高
          cacheAddrLow8_writeBack <= 0;
          interface_status <= STATUS_WRITE_BACK_FINISH;
        end else begin
          interface_status <= STATUS_WRITE_BACK;
          write_enable <= 1;//写回后要置空
          cacheData <= {36'b0};//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
        end
      end else if(interface_status==STATUS_WRITE_BACK)begin//dirty 的话，写回
        if(cache_hit_data[FLG_DIRTY])begin
          interface_status <= STATUS_WRITE_BACK_EXEC;
          rdwr_sdram_addr <= {current_cacheAddrHigh,write_back_count[7:0],1'b0};
          write_single_sdram_data <= cache_hit_data[31:0];
          write_single_sdram_mask <= 4'b0000;
          write_single_sdram_req <= 1;
        end else begin
          write_back_count <= write_back_count + 1'b1;
          interface_status <= STATUS_WRITE_BACK_DLY;
        end
      end else if(interface_status==STATUS_WRITE_BACK_EXEC)begin//写回执行
        if(write_single_sdram_ack_buff)begin
          write_single_sdram_req <= 0;
          write_back_count <= write_back_count + 1'b1;
          interface_status <= STATUS_WRITE_BACK_DLY;
        end
      end else if(interface_status==STATUS_WRITE_BACK_FINISH)begin
        set_cacheAddrHigh <= 0;
        if(avs_s0_byteenable==4'b1111)begin//完整写入DW的话，不考虑FLG_VALID
          write_enable <= 1;
          cacheData <= {4'b1100,avs_s0_writedata};//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
          `WR_FINISH
        end else begin
          interface_status <= STATUS_HITED1;//高地址命中等一个周期
        end
      end else if(interface_status==STATUS_WRITE)begin
        if(write_single_sdram_ack_buff)begin
          write_single_sdram_req <= 0;
          `WR_FINISH
        end
      end

    end
    
    if(!avs_s0_write && avs_s0_write_ack)begin
      avs_s0_write_ack <= 0;
    end
    
  end
end

reg read_sdram_req_buff;
reg write_single_sdram_req_buff;
reg read_line_req_buff;

reg read_sdram_ack;
reg write_single_sdram_ack;
reg read_line_ack;

reg sdram_page_delay;
reg        sdram_timer0;
reg  [8:0] sdram_timer2;

reg  [2:0] sdram_timer1;
reg  [1:0] sram_add_high;

reg [1:0]  sdram_step;
reg [1:0] cmd;
reg [31:0] readBuffer;
//sdram_rd_req sdram_rd_burst sdram_rw_addr
always@(posedge sdram_clk or negedge sys_rst_n) begin // sdram 主控
  if(!sys_rst_n) begin
    sdram_timer0 <= 0;
    sdram_step <= 0;
    
    read_sdram_req_buff <= 0;
    write_single_sdram_req_buff <= 0;
    read_line_req_buff <= 0;
    
    read_sdram_ack <= 0;
    write_single_sdram_ack <= 0;
    read_line_ack <= 0;
    
    sdram_rd_req <= 0;
    sdram_wr_req <= 0;
    sdram_rd_burst <= 0;
    sdram_rw_addr <= 0;
  end else begin
    read_sdram_req_buff <= read_sdram_req;
    write_single_sdram_req_buff <= write_single_sdram_req;
    read_line_req_buff <= read_line_req;
        
    buffA_wren <= 0;
    buffB_wren <= 0;

    if(cmd==0)begin
      if         (read_line_req_buff)begin
        cmd <= 1;
      end else if(read_sdram_req_buff)begin
        cmd <= 2;
      end else if(write_single_sdram_req_buff)begin
        cmd <= 3;
      end
    end

    //vga line read
    if          (cmd == 1 && !read_line_ack)begin
      //step3
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
          end else if(sdram_timer2==263)begin //263 TODO reduce
            sdram_timer0 <= 0;
            if(sdram_timer1 == 4)begin
              sdram_timer1 <= 0;
              read_line_ack <= 1;
            end
          end else begin
          end
        end
      end
    end else if (cmd == 2 && !read_sdram_ack)begin
      if         (sdram_step == 0) begin
        sdram_step <= 1;
        sdram_rw_addr <= rdwr_sdram_addr;
        sdram_rd_burst <= 2;
        sdram_rd_req <= 1;
      end else if(sdram_step == 1) begin
        if(sdram_rd_ack)begin
          sdram_step <= 2;
          readBuffer[15: 0] <= sdram_dout;
        end
      end else if(sdram_step == 2) begin
        readBuffer[31:16] <= sdram_dout;
        sdram_rd_req <= 0;
        sdram_step <= 0;
        read_sdram_ack <= 1;
      end
    end else if (cmd == 3 && !write_single_sdram_ack)begin
      if         (sdram_step==0)begin
        sdram_step <= 1;
        sdram_rw_addr <= rdwr_sdram_addr;
        sdram_wr_burst <= 2;
        sdram_wr_req <= 1;
      end else if(sdram_step==1)begin
        if(sdram_wr_ack)begin
          sdram_step <= 2;
          sdram_din <= write_single_sdram_data[15:0];
          sdram_mask <= write_single_sdram_mask[1:0];
        end
      end else if(sdram_step==2)begin
        sdram_din <= write_single_sdram_data[31:16];
        sdram_mask <= write_single_sdram_mask[3:2];
        sdram_wr_req <= 0;
        sdram_step <= 0;
        write_single_sdram_ack <= 1;
      end
    end
    
    if(!read_line_req_buff && read_line_ack)begin
      cmd <= 0;
      read_line_ack <= 0;
    end

    if(!read_sdram_req_buff && read_sdram_ack)begin
      cmd <= 0;
      read_sdram_ack <= 0;
    end

    if(!write_single_sdram_req_buff && write_single_sdram_ack)begin
      cmd <= 0;
      write_single_sdram_ack <= 0;
    end
    
  end
end

endmodule 
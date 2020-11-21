module sdrambusvga(
    input          clk,                // clock.clk
    input          reset_n,               // reset.reset
    input          clk_100m,
    input          clk_100m_shift,

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
    
    input              nocache,
    input   [14:0]     nocache_page,

    input         read_line_req  ,
    input         read_line_A_B  ,
    input  [15:0] read_line_addr ,//line address -> 1024 * 1024 word * 4 frame = 4Mword  **

    output [15:0] read_pixelA_data,
    output [15:0] read_pixelB_data,
    input   [9:0] read_pixel_addr ,//10bit 1024word per line
    input         read_pixel_clk  ,
    
    output reg [7:0] debug8,
    input      [3:0] cache_life_addr ,
    output reg [15:0] cache_life_data   ,
    input      [3:0] cacheAddrHigh_addr,
    output reg [16:0] cacheAddrHigh_data,
    output     [31:0] debug32
    
);
//wire sys_clk = clk;
wire sys_rst_n = reset_n;

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
buff1024x16  buffReadA (
  .data      ( data             ),
  .wraddress ( wraddress        ),
  .wrclock   ( wrclock          ),
  .wren      ( wrenA            ),
  .rdaddress ( rdaddress        ),
  .rdclock   ( rdclock          ),
  .q         ( qA               )
);
assign      read_pixelB_data = qB;
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
  .rst_n        (reset_n),      //系统复位

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

always@(posedge clk or negedge sys_rst_n) begin
  if(!sys_rst_n) begin
    //cacheAddrHigh[i] <= i;
    cache_life_data <= 0;
    cacheAddrHigh_data <= 0;
  end else begin
    cache_life_data    <= cache_life[cache_life_addr];
    cacheAddrHigh_data <= {1'b0, cacheInvalid[cacheAddrHigh_addr], cacheAddrHigh[cacheAddrHigh_addr]};
  end
end


parameter CACHE_COUNT = 16;

assign debug32 = {flush_cache,1'b0,flush_page};//{interface_status,1'b0,free_cache,1'b0,current_slot};
    //input              flush_cache,
    //input   [14:0]     flush_page,


reg cacheAddrLow8_writeBack;
wire  [ 7:0] cacheAddrLow8 = cacheAddrLow8_writeBack ? write_back_count[7:0] : avs_s0_address[7:0];
wire [35:0] cacheq[CACHE_COUNT];
wire        cache_hit[CACHE_COUNT];
wire        cache_flush_hit[CACHE_COUNT];

wire        cacheInvalid[CACHE_COUNT];//32Mbyte / 1024byte per cache slot = total 32k cache slot
wire [14:0] cacheAddrHigh[CACHE_COUNT];//32Mbyte / 1024byte per cache slot = total 32k cache slot
wire [CACHE_COUNT-1:0] cache_life[CACHE_COUNT];

wire [CACHE_COUNT-1:0] cache_hited;
wire [CACHE_COUNT-1:0] cache_flush_hited;
wire [CACHE_COUNT-1:0] free_cache_temp;

genvar i;
generate
  for(i=0; i<CACHE_COUNT; i=i+1) begin:BLOCK1
    cacheEntity cacheEntity_inst(
      .clk                (clk                   ),
      .sys_rst_n          (sys_rst_n             ),
      .avs_s0_address     (avs_s0_address        ),
      .flush_page         (flush_page            ),
      .flush_cache        (flush_cache           ),

      .cacheAddrLow8      (cacheAddrLow8         ),
      .cacheData          (cacheData             ),
      .write_enable       (write_enable          ),
      .adj_cache_life     (adj_cache_life        ),
      .current_slot       (current_slot[i]       ),
      .set_cacheAddrHigh  (set_cacheAddrHigh[i]  ),
      .clr_cacheAddrHigh  (clr_cacheAddrHigh[i]  ),

      .cacheq             (cacheq[i]             ),
      .cache_hit          (cache_hit[i]          ),
      .cache_flush_hit    (cache_flush_hit[i]    ),
      
      .cacheInvalid       (cacheInvalid[i]       ),
      .cacheAddrHigh      (cacheAddrHigh[i]      ),
      .cache_life         (cache_life[i]         )
    );
    assign cache_hited[i] = cache_hit[i];
    assign cache_flush_hited[i] = cache_flush_hit[i];
    assign free_cache_temp[i] = cache_life[i] == 0;
  end
endgenerate

reg  [CACHE_COUNT-1:0] set_cacheAddrHigh;
reg  [CACHE_COUNT-1:0] clr_cacheAddrHigh;
reg  [CACHE_COUNT-1:0] current_slot;

wire [CACHE_COUNT-1:0] free_cache;

//assign singleOut = free_cache_temp;
//assign free_cache = singleIn;
//
singleOut ins_singleOut(
.in(free_cache_temp),
.out(free_cache)
);

wire freeCacheInvalid = (free_cache[ 0] ? cacheInvalid[ 0] : 1'b0) | //current_slot == 0 ? 15'b0 : cacheAddrHigh[current_slot-1][14:0];
                        (free_cache[ 1] ? cacheInvalid[ 1] : 1'b0) | 
                        (free_cache[ 2] ? cacheInvalid[ 2] : 1'b0) | 
                        (free_cache[ 3] ? cacheInvalid[ 3] : 1'b0) | 
                        (free_cache[ 4] ? cacheInvalid[ 4] : 1'b0) | 
                        (free_cache[ 5] ? cacheInvalid[ 5] : 1'b0) | 
                        (free_cache[ 6] ? cacheInvalid[ 6] : 1'b0) | 
                        (free_cache[ 7] ? cacheInvalid[ 7] : 1'b0) | 
                        (free_cache[ 8] ? cacheInvalid[ 8] : 1'b0) | 
                        (free_cache[ 9] ? cacheInvalid[ 9] : 1'b0) | 
                        (free_cache[10] ? cacheInvalid[10] : 1'b0) | 
                        (free_cache[11] ? cacheInvalid[11] : 1'b0) | 
                        (free_cache[12] ? cacheInvalid[12] : 1'b0) | 
                        (free_cache[13] ? cacheInvalid[13] : 1'b0) | 
                        (free_cache[14] ? cacheInvalid[14] : 1'b0) | 
                        (free_cache[15] ? cacheInvalid[15] : 1'b0);

parameter FLG_VALID = 35;//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,}
parameter FLG_DIRTY = 34;


wire [35:0] cache_hit_data = (current_slot[ 0] ? cacheq[ 0] : 36'b0) | //current_slot == 0 ? 36'b0 : cacheq[current_slot-1];
                             (current_slot[ 1] ? cacheq[ 1] : 36'b0) | 
                             (current_slot[ 2] ? cacheq[ 2] : 36'b0) | 
                             (current_slot[ 3] ? cacheq[ 3] : 36'b0) | 
                             (current_slot[ 4] ? cacheq[ 4] : 36'b0) | 
                             (current_slot[ 5] ? cacheq[ 5] : 36'b0) | 
                             (current_slot[ 6] ? cacheq[ 6] : 36'b0) | 
                             (current_slot[ 7] ? cacheq[ 7] : 36'b0) | 
                             (current_slot[ 8] ? cacheq[ 8] : 36'b0) | 
                             (current_slot[ 9] ? cacheq[ 9] : 36'b0) | 
                             (current_slot[10] ? cacheq[10] : 36'b0) | 
                             (current_slot[11] ? cacheq[11] : 36'b0) | 
                             (current_slot[12] ? cacheq[12] : 36'b0) | 
                             (current_slot[13] ? cacheq[13] : 36'b0) | 
                             (current_slot[14] ? cacheq[14] : 36'b0) | 
                             (current_slot[15] ? cacheq[15] : 36'b0);


wire [14:0] current_cacheAddrHigh = (current_slot[ 0] ? cacheAddrHigh[ 0] : 15'b0) | //current_slot == 0 ? 15'b0 : cacheAddrHigh[current_slot-1][14:0];
                                    (current_slot[ 1] ? cacheAddrHigh[ 1] : 15'b0) | 
                                    (current_slot[ 2] ? cacheAddrHigh[ 2] : 15'b0) | 
                                    (current_slot[ 3] ? cacheAddrHigh[ 3] : 15'b0) | 
                                    (current_slot[ 4] ? cacheAddrHigh[ 4] : 15'b0) | 
                                    (current_slot[ 5] ? cacheAddrHigh[ 5] : 15'b0) | 
                                    (current_slot[ 6] ? cacheAddrHigh[ 6] : 15'b0) | 
                                    (current_slot[ 7] ? cacheAddrHigh[ 7] : 15'b0) | 
                                    (current_slot[ 8] ? cacheAddrHigh[ 8] : 15'b0) | 
                                    (current_slot[ 9] ? cacheAddrHigh[ 9] : 15'b0) | 
                                    (current_slot[10] ? cacheAddrHigh[10] : 15'b0) | 
                                    (current_slot[11] ? cacheAddrHigh[11] : 15'b0) | 
                                    (current_slot[12] ? cacheAddrHigh[12] : 15'b0) | 
                                    (current_slot[13] ? cacheAddrHigh[13] : 15'b0) | 
                                    (current_slot[14] ? cacheAddrHigh[14] : 15'b0) | 
                                    (current_slot[15] ? cacheAddrHigh[15] : 15'b0);


reg [3:0]  interface_status;
reg        interface_status_rw;
parameter STATUS_INIT              = 0;
parameter STATUS_HITED1            = 1;
parameter STATUS_READ              = 2;
parameter STATUS_WRITE_BACK_1      = 3;
parameter STATUS_WRITE_BACK_2      = 4;
parameter STATUS_WRITE_BACK_3      = 5;
parameter STATUS_WRITE_BACK_4      = 6;
parameter STATUS_WRITE_SINGLE      = 7;

`define FINISH interface_status <= STATUS_INIT;if(interface_status_rw)begin avs_s0_write_ack <= 1;end else begin avs_s0_read_ack  <= 1;end

reg [31:0] flushCount;
always@(posedge clk or negedge sys_rst_n) begin
  if(!sys_rst_n) begin
    flushCount <= 0;
  end else begin
    if(flushCountPlus1)begin
      flushCount <= flushCount + 1'b1;
    end
  end
end

reg        flushCountPlus1;
reg [35:0] cacheData;
reg        write_enable;
reg        adj_cache_life;//update when finish
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
reg onFlush;
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
    flushCountPlus1 <= 0;
    onFlush <= 0;
    interface_status_rw <= 0;
  end else begin
    read_sdram_ack_buff <= read_sdram_ack;
    write_single_sdram_ack_buff <= write_single_sdram_ack;
    
    flushCountPlus1 <= 0;
    write_enable <= 0;
    adj_cache_life <= 0;
    clr_cacheAddrHigh <= 0;
    set_cacheAddrHigh <= 0;
    
    case (interface_status)
    STATUS_INIT : begin//初始化
      onFlush <= 0;
      current_slot <= 0;
      interface_status_rw <= 0;
      rdwr_sdram_addr <= {avs_s0_address,1'b0};
      write_back_count <= 0;
      if(avs_s0_read && !avs_s0_read_ack)begin
        if(flush_cache && (cache_flush_hited & cache_hited))begin
          onFlush <= 1;
          current_slot <= cache_flush_hited;
          cacheAddrLow8_writeBack <= 1;
          flushCountPlus1 <= 1;
          interface_status <= STATUS_WRITE_BACK_1;
        end else if(cache_hited)begin
          current_slot <= cache_hited;
          interface_status <= STATUS_HITED1;//高地址命中等一个周期
        end else if(free_cache)begin//找到空闲cache
          current_slot <= free_cache;
          if(freeCacheInvalid)begin//invalid 无效 直接使用
            set_cacheAddrHigh <= free_cache;//当前地址写入缓存地址高
            interface_status <= STATUS_HITED1; 
          end else begin
            cacheAddrLow8_writeBack <= 1;
            flushCountPlus1 <= 1;
            interface_status <= STATUS_WRITE_BACK_1;
          end
        end else begin//不经过cache
          read_sdram_req <= 1;
          interface_status <= STATUS_READ;
        end
      end
      if(avs_s0_write && !avs_s0_write_ack)begin
        interface_status_rw <= 1;
        write_single_sdram_data <= avs_s0_writedata;
        write_single_sdram_mask <= ~avs_s0_byteenable;
        cacheData <= {4'b1100,avs_s0_writedata};//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
        if(cache_hited)begin
          current_slot <= cache_hited;
          if(avs_s0_byteenable==4'b1111)begin//完整写入DW的话，不考虑FLG_VALID
            write_enable <= 1;
            adj_cache_life <= 1;
            interface_status <= STATUS_INIT; avs_s0_write_ack <= 1;//`WR_FINISH
          end else begin
            interface_status <= STATUS_HITED1;//高地址命中等一个周期
          end
        end else begin
          if(free_cache != 0)begin//有闲置cache
            current_slot <= free_cache;
            if(freeCacheInvalid)begin//invalid 无效 直接使用
              set_cacheAddrHigh <= free_cache;//当前地址写入缓存地址高
              interface_status <= STATUS_WRITE_BACK_4;
            end else begin
              cacheAddrLow8_writeBack <= 1;
              flushCountPlus1 <= 1;
              interface_status <= STATUS_WRITE_BACK_1;
            end
          end else begin
            //直接写入
            interface_status <= STATUS_WRITE_SINGLE;//高地址命中等一个周期
            write_single_sdram_req <= 1;
          end
        end
      end
    end
    
    STATUS_HITED1 : begin//高地址命中
      rdwr_sdram_addr <= {avs_s0_address,1'b0};//for not valid
      avs_s0_readdata <= cache_hit_data[31:0];//for read
      cacheData <= {4'b1100,
        avs_s0_byteenable[3] ? avs_s0_writedata[31:24] : cache_hit_data[31:24],
        avs_s0_byteenable[2] ? avs_s0_writedata[23:16] : cache_hit_data[23:16],
        avs_s0_byteenable[1] ? avs_s0_writedata[15: 8] : cache_hit_data[15: 8],
        avs_s0_byteenable[0] ? avs_s0_writedata[ 7: 0] : cache_hit_data[ 7: 0]
      };//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} for write

      if(onFlush)begin
        clr_cacheAddrHigh <= current_slot;
        `FINISH
      end else begin
        if(cache_hit_data[FLG_VALID])begin
          if(interface_status_rw)begin
            write_enable <= 1;
          end
          adj_cache_life <= 1;
          `FINISH
        end else begin
          interface_status <= STATUS_READ;
          read_sdram_req <= 1;
        end
      end
    end


    STATUS_READ : begin//读取等待
      avs_s0_readdata <= readBuffer;
      if(interface_status_rw)begin
        cacheData[35:32] <= 4'b1100;//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
        cacheData[31:0] <= {
          avs_s0_byteenable[3] ? avs_s0_writedata[31:24] : readBuffer[31:24],
          avs_s0_byteenable[2] ? avs_s0_writedata[23:16] : readBuffer[23:16],
          avs_s0_byteenable[1] ? avs_s0_writedata[15: 8] : readBuffer[15: 8],
          avs_s0_byteenable[0] ? avs_s0_writedata[ 7: 0] : readBuffer[ 7: 0]
        };//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
      end else begin
        cacheData[35:32] <= 4'b1000;//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
        cacheData[31:0] <= readBuffer;//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
      end
      if(read_sdram_ack_buff)begin //TODO 如果是低地址的话，再读取一个双字，注意如果后一个字是dirty的话，怎么处理  等待sdram响应的时候，读取邻字的dirty flag
        read_sdram_req <= 0;
        if(current_slot != 0)begin
          write_enable <= 1;
        end
        adj_cache_life <= 1;
        `FINISH
      end
    end

    STATUS_WRITE_BACK_1 : begin//sram读出延时
      cacheData[35:32] <= 4'b0000;//状态清零 {FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
      if(write_back_count[8])begin//write_back_count==256
        cacheAddrLow8_writeBack <= 0;
        set_cacheAddrHigh <= current_slot;//当前地址写入缓存地址高
        if(interface_status_rw)begin
          interface_status <= STATUS_WRITE_BACK_4;
        end else begin
          interface_status <= STATUS_HITED1;
        end
      end else begin
        interface_status <= STATUS_WRITE_BACK_2;
        write_enable <= 1;//写回后要置空
      end
    end
    

    STATUS_WRITE_BACK_2 : begin//dirty 的话，写回
      rdwr_sdram_addr <= {current_cacheAddrHigh,write_back_count[7:0],1'b0};
      write_single_sdram_data <= cache_hit_data[31:0];
      write_single_sdram_mask <= 4'b0000;
      if(cache_hit_data[FLG_DIRTY])begin
        interface_status <= STATUS_WRITE_BACK_3;
        write_single_sdram_req <= 1;
      end else begin
        write_back_count <= write_back_count + 1'b1;
        interface_status <= STATUS_WRITE_BACK_1;
      end
    end

    
    STATUS_WRITE_BACK_3 : begin//写回执行
      if(write_single_sdram_ack_buff)begin
        write_single_sdram_req <= 0;
        write_back_count <= write_back_count + 1'b1;
        interface_status <= STATUS_WRITE_BACK_1;
      end
    end

    STATUS_WRITE_BACK_4 : begin
      cacheData <= {4'b1100,avs_s0_writedata};//{FLG_VALID,FLG_DIRTY,1'b0,1'b0,} 置已缓存位
      if(avs_s0_byteenable==4'b1111)begin//完整写入DW的话，不考虑FLG_VALID
        write_enable <= 1;
        `FINISH
      end else begin
        interface_status <= STATUS_HITED1;//高地址命中等一个周期
      end
    end
    
    STATUS_WRITE_SINGLE : begin
      if(write_single_sdram_ack_buff)begin
        write_single_sdram_req <= 0;
        `FINISH
      end
    end


    endcase

    if(!avs_s0_read && avs_s0_read_ack)begin
      avs_s0_read_ack <= 0;
    end


    if(!avs_s0_write && avs_s0_write_ack)begin
      avs_s0_write_ack <= 0;
    end
    
  end
end


//fifo化


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
reg [31:0] readBuffer;

reg read_sdram_req_cmd;
reg write_single_sdram_req_cmd;
reg read_line_req_cmd;
//sdram_rd_req sdram_rd_burst sdram_rw_addr
always@(posedge sdram_clk or negedge sys_rst_n) begin // sdram 主控
  if(!sys_rst_n) begin
    sdram_timer0 <= 0;
    sdram_step <= 0;
    
    read_sdram_req_buff <= 0;
    write_single_sdram_req_buff <= 0;
    read_line_req_buff <= 0;
    
    read_sdram_req_cmd <= 0;
    write_single_sdram_req_cmd <= 0;
    read_line_req_cmd <= 0;

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

    
    if(read_line_req_cmd==0 && read_sdram_req_cmd==0 && write_single_sdram_req_cmd==0)begin
      read_sdram_req_cmd <= 0;
      write_single_sdram_req_cmd <= 0;
      read_line_req_cmd <= 0;
      if         (read_line_req_buff)begin
        read_line_req_cmd <= 1;
      end else if(read_sdram_req_buff)begin
        read_sdram_req_cmd <= 1;
      end else if(write_single_sdram_req_buff)begin
        write_single_sdram_req_cmd <= 1;
      end
    end
    
    buffAB_wrdata <= sdram_dout;
    buffAB_wraddress <= {sram_add_high,sdram_timer2[7:0]};

    //vga line read
    if (read_line_req_cmd && !read_line_ack)begin
      sdram_rd_burst <= 256;
      //step3
      sdram_timer0 <= 1;
      if(sdram_timer0 == 0)begin
        if         (sdram_timer1 == 0) begin sram_add_high <= 0; sdram_timer1 <= 1; sdram_rw_addr <= {read_line_addr,2'b00,8'b0}; //12+2+8  **
        end else if(sdram_timer1 == 1) begin sram_add_high <= 1; sdram_timer1 <= 2; sdram_rw_addr <= {read_line_addr,2'b01,8'b0}; //12+2+8  **
        end else if(sdram_timer1 == 2) begin sram_add_high <= 2; sdram_timer1 <= 3; sdram_rw_addr <= {read_line_addr,2'b10,8'b0}; //12+2+8  **
        end else if(sdram_timer1 == 3) begin sram_add_high <= 3; sdram_timer1 <= 4; sdram_rw_addr <= {read_line_addr,2'b11,8'b0}; //12+2+8  **
        end
        sdram_timer2 <= 0;
        sdram_page_delay <= 0;
        sdram_rd_req = 1;//只需要置高一个周期就可以了
      end else begin
        if(sdram_rd_ack || sdram_page_delay)begin
          sdram_timer2 <= sdram_timer2 + 1'b1;
          if(!sdram_page_delay)begin
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
    end
    
    if (read_sdram_req_cmd && !read_sdram_ack)begin
      sdram_rd_burst <= 2;
      sdram_rw_addr <= rdwr_sdram_addr;
      if         (sdram_step == 0) begin
        sdram_step <= 1;
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
    end

    if(sdram_step[0])begin
      sdram_din <= write_single_sdram_data[15:0];
      sdram_mask <= write_single_sdram_mask[1:0];
    end else begin
      sdram_din <= write_single_sdram_data[31:16];
      sdram_mask <= write_single_sdram_mask[3:2];
    end
    if (write_single_sdram_req_cmd && !write_single_sdram_ack)begin
      sdram_rw_addr <= rdwr_sdram_addr;
      sdram_wr_burst <= 2;
      if         (sdram_step==0)begin
        sdram_step <= 1;
        sdram_wr_req <= 1;
      end else if(sdram_step==1)begin
        if(sdram_wr_ack)begin
          sdram_step <= 2;
        end
      end else if(sdram_step==2)begin
        sdram_wr_req <= 0;
        sdram_step <= 0;
        write_single_sdram_ack <= 1;
      end
    end
    
    if(!read_line_req_buff && read_line_ack)begin
      read_line_req_cmd <= 0;
      read_line_ack <= 0;
    end

    if(!read_sdram_req_buff && read_sdram_ack)begin
      read_sdram_req_cmd <= 0;
      read_sdram_ack <= 0;
    end

    if(!write_single_sdram_req_buff && write_single_sdram_ack)begin
      write_single_sdram_req_cmd <= 0;
      write_single_sdram_ack <= 0;
    end
    
  end
end

endmodule 


module singleOut(
  input [15:0] in,
  output reg [15:0] out
);
always @(*) begin
  casez (in)
    16'b???????????????1: out <= 16'b0000000000000001;
    16'b??????????????10: out <= 16'b0000000000000010;
    16'b?????????????100: out <= 16'b0000000000000100;
    16'b????????????1000: out <= 16'b0000000000001000;
    16'b???????????10000: out <= 16'b0000000000010000;
    16'b??????????100000: out <= 16'b0000000000100000;
    16'b?????????1000000: out <= 16'b0000000001000000;
    16'b????????10000000: out <= 16'b0000000010000000;
    16'b???????100000000: out <= 16'b0000000100000000;
    16'b??????1000000000: out <= 16'b0000001000000000;
    16'b?????10000000000: out <= 16'b0000010000000000;
    16'b????100000000000: out <= 16'b0000100000000000;
    16'b???1000000000000: out <= 16'b0001000000000000;
    16'b??10000000000000: out <= 16'b0010000000000000;
    16'b?100000000000000: out <= 16'b0100000000000000;
    16'b1000000000000000: out <= 16'b1000000000000000;
    default:              out <= 16'b0000000000000000;
  endcase
end

endmodule 



module cacheEntity(
  input              clk              ,
  input              sys_rst_n        ,
  input      [ 7:0]  cacheAddrLow8    ,
  input      [35:0]  cacheData        ,
  input              current_slot     ,
  input              set_cacheAddrHigh,
  input              clr_cacheAddrHigh,
  input      [22:0]  avs_s0_address   ,
  input              write_enable     ,
  input      [14:0]  flush_page       ,
  input              flush_cache      ,
  input              adj_cache_life   ,
  
  output     [35:0]  cacheq           ,
  output             cache_hit        ,
  output             cache_flush_hit  ,
  
  output reg         cacheInvalid     ,
  output reg  [14:0] cacheAddrHigh    ,
  output reg  [15:0] cache_life       
);
parameter CACHE_COUNTX2 = 6'd32;

cache256x36 cache256x36_inst (
  .address(cacheAddrLow8),//input	[7:0]  address;
  .clock  (clk          ),  //input	  clock;
  .data   (cacheData    ),   //input	[35:0]  data;
  .wren   (wren         ),   //input	  wren;
  .q      (cacheq       )     //output	[35:0]  q;
);

assign cache_hit       = cacheInvalid == 0 && avs_s0_address[22:8] == cacheAddrHigh[14:0];
assign cache_flush_hit = cacheInvalid == 0 &&           flush_page == cacheAddrHigh[14:0];
wire wren = current_slot ? write_enable : 1'b0;
always@(posedge clk or negedge sys_rst_n) begin
  if(!sys_rst_n) begin
    cache_life <= 0;
    cacheAddrHigh <= 15'b0;
    cacheInvalid <= 1'b1;
  end else begin
    if(adj_cache_life)begin
      if(current_slot)begin
        if(flush_cache && cache_flush_hit)begin
          cache_life<=0;
        end else begin
          if(!cache_life[15])begin
            cache_life<=cache_life+CACHE_COUNTX2;
          end
        end
      end else begin
        if(cache_life!=0)begin
          cache_life<=cache_life-1'b1;
        end
      end
    end
    
    if(set_cacheAddrHigh)begin
      cacheAddrHigh <= avs_s0_address[22:8];
      cacheInvalid <= 1'b0;
      cache_life <= 512;
    end
    if(clr_cacheAddrHigh)begin
      //cacheAddrHigh <= 15'b0;
      cacheInvalid <= 1'b1;
      cache_life <= 0;
    end
  end
end

endmodule 

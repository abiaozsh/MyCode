module system (
    input  wire        clk,  //   clk50.clk
    input  wire        reset_n,  //   reset.reset_n
		input clk_50M,
		input vga_clk_25M,
		input vga_clk_65M,
		input clk_100m,
		input clk_100m_shift,

    output        mycpu_uart_txd, //   mycpu.uart_txd
    input         mycpu_uart_rxd, //        .uart_rxd
    output [7:0]  debug8,   //        .debug8
    output [31:0] debug32,   //        .debug32

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

    input          softspi_MISO,   // softspi.MISO
    output         softspi_MOSI,   //        .MOSI
    output         softspi_SCLK,   //        .SCLK
    output  [2:0]  softspi_SS_n,   //        .SS_n
    
    input          myuart_rxd,     //  myuart.rxd
    output         myuart_txd,     //        .txd
    
    inout key_data,
    inout key_clk,
    inout mouse_data,
    inout mouse_clk,

     //SDRAM2m 芯片接口
    //output        sdram2m_clk_out,            //SDRAM 芯片时钟
    //output        sdram2m_cke,                //SDRAM 时钟有效
    //output        sdram2m_cs_n,               //SDRAM 片选
    //output        sdram2m_ras_n,              //SDRAM 行有效
    //output        sdram2m_cas_n,              //SDRAM 列有效
    //output        sdram2m_we_n,               //SDRAM 写有效
    //output        sdram2m_ba,                 //SDRAM Bank地址
    //output [10:0] sdram2m_addr,               //SDRAM 行/列地址
    //inout  [15:0] sdram2m_data,               //SDRAM 数据

/*
    output cyok,
    output reg [7:0] cy_cmd,
    output reg [7:0] cy_dat,
    output reg [7:0] cy_snd_data0,
    output reg [7:0] cy_snd_data1,
     //input cy_SCL,
  //input cy_SDA,
  inout [7:0] cy_D,
  inout [7:0] cy_B,
  output cy_IFCLK_out                     ,
  input cy_to_fpga_CTL0_FLAGA        ,
  input cy_to_fpga_CTL2_FLAGC        ,
  input cy_to_fpga_CTL1_FLAGB        ,
  input cy_to_fpga_A7_FLAGD          ,
  output  cy_from_fpga_RDY1_SLWR       ,//output
  output  cy_from_fpga_RDY0_SLRD       ,//output
  input cy_A0_INT0                   ,
  output cy_A1_INT1                   ,
  output  cy_from_fpga_A2_SLOE         ,//output
  input cy_A3_WU2                    ,
  //output  cy_from_fpga_A4_FIFOADR0     ,//output
  output  cy_from_fpga_A5_FIFOADR1     ,//output
  //output  cy_from_fpga_A6_PKTEND       ,//output
*/
    //VGA接口                          
    output          vga_hs,         //行同步信号
    output          vga_vs,         //场同步信号
    output  [15:0]  vga_rgb,         //红绿蓝三原色输出 
     
    
    
    input dummy
  );
  
  
  
  wire sdrambus_debug8;
  assign debug32 = mycpu_debug32;
  wire [31:0] sdrambus_debug32;

  wire [7:0]  mycpu_debug8;
  wire [31:0]  mycpu_debug32;
  wire [31:0] avm_m0_address;
  wire        avm_m0_read;
  wire        avm_m0_write;
  wire [31:0] avm_m0_writedata;
  wire [ 3:0] avm_m0_byteenable;
  mycpu mycpu_0
    (
    .clk                (clk),                       //       clock.clk
    .clk_50M            (clk_50M),

    .reset_n            (reset_n), //       reset.reset_n
    .avm_m0_address     (avm_m0_address),              //          m0.address
    .avm_m0_read        (avm_m0_read),                 //            .read
    .avm_m0_write       (avm_m0_write),                //            .write
    .avm_m0_readdata    (avm_m0_readdata),             //            .readdata
    .avm_m0_writedata   (avm_m0_writedata),            //            .writedata
    .avm_m0_byteenable  (avm_m0_byteenable),           //            .byteenable
    .avm_m0_waitrequest (avm_m0_waitrequest),          //            .waitrequest
    .irq_req            (irq_req),                //        irq0.irq
    
    .uart_txd           (mycpu_uart_txd),                  // conduit_end.export
    .uart_rxd           (mycpu_uart_rxd),                  //            .export
    .debug8             (mycpu_debug8),                    //            .export
    .debug32            (mycpu_debug32),                    //            .export
      
    .debugin8           (spidebug8    ),
    .cache_life0    (cache_life0   ),
    .cache_life1    (cache_life1   ),
    .cache_life2    (cache_life2   ),
    .cache_life3    (cache_life3   ),
    .cacheAddrHigh0 (cacheAddrHigh0),
    .cacheAddrHigh1 (cacheAddrHigh1),
    .cacheAddrHigh2 (cacheAddrHigh2),
    .cacheAddrHigh3 (cacheAddrHigh3),
    .debugin32  (sdrambus_debug32)
      
  );
  reg [31:0] avm_m0_readdata;
  reg        avm_m0_waitrequest;
  always @ (*) begin
    if         (sdrambus_cs )begin avm_m0_waitrequest <= sdrambus_waitrequest ; avm_m0_readdata <= sdrambus_readdata ;//TODO
    end else if(mainSRAM_cs )begin avm_m0_waitrequest <= mainSRAM_waitrequest ; avm_m0_readdata <= mainSRAM_readdata ;
    end else if(cacheCtrl_cs)begin avm_m0_waitrequest <= 0                    ; avm_m0_readdata <= 0                 ;
    end else if(irq_Ctrl_cs )begin avm_m0_waitrequest <= 0                    ; avm_m0_readdata <= irq_Ctrl_readdata ;
    end else if(mytimer_cs  )begin avm_m0_waitrequest <= mytimer_waitrequest  ; avm_m0_readdata <= mytimer_readdata  ;
    end else if(myuart_cs   )begin avm_m0_waitrequest <= myuart_waitrequest   ; avm_m0_readdata <= myuart_readdata   ;
    end else if(softspi_cs  )begin avm_m0_waitrequest <= softspi_waitrequest  ; avm_m0_readdata <= softspi_readdata  ;
    end else if(vga_cs      )begin avm_m0_waitrequest <= vga_waitrequest      ; avm_m0_readdata <= 0                 ;
    end else if(mykeyb_cs   )begin avm_m0_waitrequest <= 0                    ; avm_m0_readdata <= mykeyb_readdata   ;
    end else if(mymouse_cs  )begin avm_m0_waitrequest <= 0                    ; avm_m0_readdata <= mymouse_readdata  ;
    
    
    end else                 begin avm_m0_waitrequest <= 0;                     avm_m0_readdata <= 0;
    end
  end
  
  
  
  
  
  
  
  
  
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  
  

  wire        read_line_req         ;//input read_buffA_req,
  wire        read_line_A_B         ;//input read_buffB_req,
  wire [15:0] read_line_addr        ;//input [9:0] read_buff_addr,
  wire [15:0] read_pixelA_data      ;//output [15:0] buff_readA_data,
  wire [15:0] read_pixelB_data      ;//output [15:0] buff_readB_data,
  wire  [9:0] read_pixel_addr       ;//input [9:0]   buff_readB_addr,
  wire        read_pixel_clk        ;//input         buff_readB_clk,

  wire  [15:0] cache_life0   ;
  wire  [15:0] cache_life1   ;
  wire  [15:0] cache_life2   ;
  wire  [15:0] cache_life3   ;
  wire  [15:0] cacheAddrHigh0;
  wire  [15:0] cacheAddrHigh1;
  wire  [15:0] cacheAddrHigh2;
  wire  [15:0] cacheAddrHigh3;


  
  wire sdrambus_cs = avm_m0_address[31:25] == 7'h0;// 0000:0000 ~ 01FF:FFFF 32M byte

  wire [31:0] sdrambus_readdata;
  wire sdrambus_waitrequest;
  sdrambusvga sdrambusvga_inst (
    .clk                (clk),                       //       clock.clk
    .clk_100m           (clk_100m),
    .clk_100m_shift     (clk_100m_shift),
    .reset_n            (reset_n), //       reset.reset_n
    .avs_s0_address     (sdrambus_address ),
    .avs_s0_read        (sdrambus_read ),
    .avs_s0_write       (sdrambus_write ),
    .avs_s0_readdata    (sdrambus_readdata ),
    .avs_s0_writedata   (avm_m0_writedata ),
    .avs_s0_waitrequest (sdrambus_waitrequest ),
    .avs_s0_byteenable  (avm_m0_byteenable ),
    
    //SDRAM 芯片接口
    .sdram_clk_out      (sdram_clk_out),
    .sdram_cke          (sdram_cke    ),
    .sdram_cs_n         (sdram_cs_n   ),
    .sdram_ras_n        (sdram_ras_n  ),
    .sdram_cas_n        (sdram_cas_n  ),
    .sdram_we_n         (sdram_we_n   ),
    .sdram_ba           (sdram_ba     ),
    .sdram_addr         (sdram_addr   ),
    .sdram_data         (sdram_data   ),
    .sdram_dqm          (sdram_dqm    ),
      
    .read_line_req   (read_line_req       ),//input read_buffA_req,
    .read_line_A_B   (read_line_A_B       ),//input read_buffB_req,
    .read_line_addr  (read_line_addr      ),//input [9:0] read_buff_addr,
    .read_pixelA_data(read_pixelA_data    ),//output [15:0] buff_readA_data,
    .read_pixelB_data(read_pixelB_data    ),//output [15:0] buff_readB_data,
    .read_pixel_addr (read_pixel_addr     ),//input [9:0]   buff_readB_addr,
    .read_pixel_clk  (read_pixel_clk      ),//input         buff_readB_clk,

    .flush_cache    (flush_cache),
    .flush_page     (flush_page ),
    
    .debug8         (sdrambus_debug8 ),
    .cache_life0    (cache_life0     ),
    .cache_life1    (cache_life1     ),
    .cache_life2    (cache_life2     ),
    .cache_life3    (cache_life3     ),
    .cacheAddrHigh0 (cacheAddrHigh0  ),
    .cacheAddrHigh1 (cacheAddrHigh1  ),
    .cacheAddrHigh2 (cacheAddrHigh2  ),
    .cacheAddrHigh3 (cacheAddrHigh3  ),
    .debug32        (sdrambus_debug32)

  );
  wire [22:0] sdrambus_address;
  assign      sdrambus_address = avm_m0_address[24:2];//~[0]
  
  wire   sdrambus_read  = sdrambus_cs ? avm_m0_read  : 1'b0;
  wire   sdrambus_write = sdrambus_cs ? avm_m0_write : 1'b0;
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  
  
  //512dword = 2048byte
  wire mainSRAM_cs = avm_m0_address[31:16] == 16'h0200;
  
  wire [31:0] mainSRAM_readdata;
  mainSRAM  mainSRAM_inst (
    .clock ( clk ),
    .data ( avm_m0_writedata ),
    .address ( mainSRAM_address ),
    .byteena ( avm_m0_byteenable ),
    .wren ( mainSRAM_write ),
    .q ( mainSRAM_readdata )
  );
  
reg mainSRAM_read_ack;
wire mainSRAM_waitrequest = ((mainSRAM_cs && avm_m0_read) && !mainSRAM_read_ack);
always@(posedge clk or negedge reset_n) begin
  if(!reset_n) begin
    mainSRAM_read_ack <= 0;
  end else begin
    if((mainSRAM_cs && avm_m0_read) && !mainSRAM_read_ack)begin
      mainSRAM_read_ack <= 1;
    end
    
    if(!(mainSRAM_cs && avm_m0_read) && mainSRAM_read_ack)begin
      mainSRAM_read_ack <= 0;
    end

  end
end

  wire mainSRAM_write = mainSRAM_cs ? avm_m0_write : 1'b0;
  
  //wire [12:0] mainSRAM_address = avm_m0_address[14:2];//~[12:0]
  wire [8:0] mainSRAM_address = avm_m0_address[10:2];//~[8:0]
  
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  assign debug8[0] = flush_cache;//mouse_send_req_buff;
  //assign debug8[1] = mbitpos[1];//mouse_send_ack;
  //assign debug8[2] = mbitpos[2];//mouse_read_req_buff;
  //assign debug8[3] = mbitpos[3];//mouse_read_ack;
  //assign debug8[4] = send_err;
  //assign debug8[5] = read_err;
  //assign debug8[7:6] = mouse_send_state;

  wire cacheCtrl_cs = avm_m0_address[31:16] == 16'h0201;
  reg              flush_cache;
  reg   [14:0]     flush_page;
  always@(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      flush_cache <= 0;
      flush_page <= 0;
    end else begin
      if(cacheCtrl_cs && avm_m0_write)begin
        if         (avm_m0_address[15:2]==0)begin
          flush_cache <= avm_m0_writedata[31];
          flush_page = avm_m0_writedata[14:0];
        end else if(avm_m0_address[15:2]==1)begin
        end
      end
    end
  end
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  wire irq_Ctrl_cs = avm_m0_address[31:16] == 16'h0202;

  wire irq_req = irq_num!=0;
  reg [31:0] irq_num;
  wire [31:0] irq_Ctrl_readdata = irq_num;

  wire mytimer_irq_req;
  reg  mytimer_irq_req_buff;
  reg  mytimer_irq_ack;
  wire myuart_irq_req;
  reg  myuart_irq_ack;
  reg  mykeyb_irq_req;
  reg  mykeyb_irq_req_buff;
  reg  mykeyb_irq_ack;

  always@(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      irq_num <= 0;
      myuart_irq_ack <= 0;
    end else begin
      if(irq_Ctrl_cs && avm_m0_write)begin
        if         (avm_m0_address[15:2]==0)begin
          irq_num <= irq_num & (~avm_m0_writedata);
        end else if(avm_m0_address[15:2]==1)begin
        end
      end
      mytimer_irq_req_buff = mytimer_irq_req;
      mykeyb_irq_req_buff = mykeyb_irq_req;
      
      if(mytimer_irq_req_buff && !mytimer_irq_ack)begin
        irq_num[0] <= 1;
        mytimer_irq_ack <= 1;
      end
      if(!mytimer_irq_req_buff && mytimer_irq_ack)begin
        mytimer_irq_ack <= 0;
      end

      if(myuart_irq_req && !myuart_irq_ack)begin
        irq_num[1] <= 1;
        myuart_irq_ack <= 1;
      end
      if(!myuart_irq_req && myuart_irq_ack)begin
        myuart_irq_ack <= 0;
      end
      
      if(mykeyb_irq_req_buff && !mykeyb_irq_ack)begin
        irq_num[2] <= 1;
        mykeyb_irq_ack <= 1;
      end
      if(!mykeyb_irq_req_buff && mykeyb_irq_ack)begin
        mykeyb_irq_ack <= 0;
      end

    end
  end
  
  
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  
  
  wire mytimer_cs = avm_m0_address[31:16] == 16'h0203;

  wire [31:0] mytimer_readdata;
  wire        mytimer_waitrequest;
  mytimer mytimer_inst (
    .clk                (clk),                       //       clock.clk
		.clk_50M            (clk_50M),
    .reset_n            (reset_n), //       reset.reset_n
    .avs_s0_address     (avm_m0_address ),
    .avs_s0_read        (mytimer_read ),
    .avs_s0_write       (mytimer_write ),
    .avs_s0_readdata    (mytimer_readdata ),
    .avs_s0_writedata   (avm_m0_writedata ),
    .avs_s0_waitrequest (mytimer_waitrequest ),
    .avs_s0_byteenable  (avm_m0_byteenable ),
    .irq_req            (mytimer_irq_req),       //  irq0.irq
    .irq_ack            (mytimer_irq_ack)
  );
  
  wire mytimer_read;
  assign mytimer_read = mytimer_cs ? avm_m0_read : 1'b0;
  wire mytimer_write;
  assign mytimer_write = mytimer_cs ? avm_m0_write : 1'b0;
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  
  wire myuart_cs = avm_m0_address[31:16] == 16'h0204;

  wire [31:0] myuart_readdata;
  wire        myuart_waitrequest;
  myuart myuart_inst (
    .clk                (clk),                       //       clock.clk
		.clk_50M            (clk_50M),
    .reset_n            (reset_n), //       reset.reset_n
    .avs_s0_address     (myuart_address ),
    .avs_s0_read        (myuart_read ),
    .avs_s0_write       (myuart_write ),
    .avs_s0_readdata    (myuart_readdata ),
    .avs_s0_writedata   (avm_m0_writedata ),
    .avs_s0_waitrequest (myuart_waitrequest ),
    .avs_s0_byteenable  (avm_m0_byteenable ),
    .irq_req            (myuart_irq_req),       //  irq0.irq
    .irq_ack            (myuart_irq_ack),

    
    .uart_rxd           (myuart_rxd),
    .uart_txd           (myuart_txd)
  );
  
  wire        myuart_address;
  assign myuart_address = avm_m0_address[2];//~[0]
  
  wire myuart_read;
  assign myuart_read = myuart_cs ? avm_m0_read : 1'b0;
  wire myuart_write;
  assign myuart_write = myuart_cs ? avm_m0_write : 1'b0;
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  
  
  wire softspi_cs = avm_m0_address[31:16] == 16'h0205;

  wire [31:0] softspi_readdata;
  wire        softspi_waitrequest;
  wire [7:0] spidebug8;
  softspi softspi_inst (
    .clk                (clk),                       //       clock.clk
		.clk_50M            (clk_50M),
    .reset_n            (reset_n), //       reset.reset_n
    .avs_s0_address     (softspi_address ),
    .avs_s0_read        (softspi_read ),
    .avs_s0_write       (softspi_write ),
    .avs_s0_readdata    (softspi_readdata ),
    .avs_s0_writedata   (avm_m0_writedata ),
    .avs_s0_waitrequest (softspi_waitrequest ),
    .avs_s0_byteenable  (avm_m0_byteenable ),
    
    .debug8 (spidebug8),
    
    .MISO           (softspi_MISO),
    .MOSI           (softspi_MOSI),
    .SCLK           (softspi_SCLK),
    .SS_n           (softspi_SS_n)

  );
  
  wire   [1:0] softspi_address;
  assign softspi_address = avm_m0_address[3:2];//~[0]
  
  wire softspi_read = softspi_cs ? avm_m0_read : 1'b0;
  wire softspi_write = softspi_cs ? avm_m0_write : 1'b0;

  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  wire vga_cs = avm_m0_address[31:16] == 16'h0206;
  reg [1:0] vga_mode;
  reg blockvga;
  //read_line_base_addr
  always@(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      vga_mode <= 3;
    end else begin
      if(vga_cs && avm_m0_write)begin
        if         (avm_m0_address[15:2]==0)begin
          vga_mode <= avm_m0_writedata[1:0];
        end else if(avm_m0_address[15:2]==1)begin
          read_line_base_addr = avm_m0_writedata[15:0];
        end else if(avm_m0_address[15:2]==2)begin
          blockvga = avm_m0_writedata[0];
        end
      end
    end
  end
  wire vga_waitrequest = 0;
  
  //128m byte range 64k line * 2048byte per line
  reg  [15:0] read_line_base_addr   ;//input [9:0] read_buff_addr,

  wire blanking;
  vga_driver8m u_vga_driver8m(
    .sys_rst_n      (reset_n),
		.vga_clk_25M(vga_clk_25M),
		.vga_clk_65M(vga_clk_65M),

    .blockvga(blockvga),
    .vga_mode(vga_mode),
    .blanking(blanking),
    .read_line_base_addr (read_line_base_addr ),
    
    .read_line_req       (read_line_req       ),
    .read_line_A_B       (read_line_A_B       ),
    .read_line_addr      (read_line_addr      ),
    
    .read_pixelA_data    (read_pixelA_data    ),
    .read_pixelB_data    (read_pixelB_data    ),
    .read_pixel_addr     (read_pixel_addr     ),
    .read_pixel_clk      (read_pixel_clk      ),
    
    .vga_hs         (vga_hs),
    .vga_vs         (vga_vs),
    .vga_rgb        (vga_rgb)
  );

  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  
    assign key_data   = 1'bz;
    assign key_clk    = 1'bz;

  wire mykeyb_cs = avm_m0_address[31:16] == 16'h0207;
  
  reg [15:0] keyboard_timer;
  reg [3:0] bitpos;
  reg key_clk_buff0;
  reg key_clk_buff1;
  reg [9:0] keyboard_data;
  reg       keyboard_data_valid;
  reg       keyboard_clr_req;
  reg       keyboard_clr_req_buff;
  reg       keyboard_clr_ack;
  wire [10:0] mykeyb_readdata = {keyboard_data_valid,keyboard_data};
  always@(posedge clk_50M or negedge reset_n) begin
    if(!reset_n) begin
      keyboard_timer <= 0;
      key_clk_buff0 <= 0;
      key_clk_buff1 <= 0;
      keyboard_data_valid <= 0;
    end else begin
      key_clk_buff0 <= key_clk;
      key_clk_buff1 <= key_clk_buff0;
      keyboard_clr_req_buff <= keyboard_clr_req;
      
      if(keyboard_timer!=16'hFFFF)begin
        keyboard_timer<=keyboard_timer+1'b1;
      end
      
        //pos edge
      if(!key_clk_buff1 && key_clk_buff0)begin//1 old 0 new
        keyboard_timer <= 0;
        if(keyboard_timer==16'hFFFF)begin
          bitpos<=1;
          keyboard_data[0]<=key_data;
        end else begin
          bitpos<=bitpos+1'b1;
          keyboard_data[bitpos]<=key_data;
          if(bitpos==10)begin
            bitpos <= 0;
            keyboard_data_valid <= 1;
            mykeyb_irq_req <= 1;
          end
        end
      end
      
      if(mykeyb_irq_ack)begin
        mykeyb_irq_req <= 0;
      end
      
      if(keyboard_clr_req_buff && !keyboard_clr_ack)begin
        keyboard_data_valid <= 0;
        keyboard_clr_ack <= 1;
      end
      if(!keyboard_clr_req_buff && keyboard_clr_ack)begin
        keyboard_clr_ack <= 0;
      end
      
    end
  end
  
  always@(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      keyboard_clr_req <= 0;
    end else begin

      if(mykeyb_cs && avm_m0_read)begin
        keyboard_clr_req <= 1;
      end
      
      if(keyboard_clr_ack)begin
        keyboard_clr_req <= 0;
      end
    end
  end

  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //assign debug8 = mouse_send_data;
  //assign debug8[0] = mbitpos[0];//mouse_send_req_buff;
  //assign debug8[1] = mbitpos[1];//mouse_send_ack;
  //assign debug8[2] = mbitpos[2];//mouse_read_req_buff;
  //assign debug8[3] = mbitpos[3];//mouse_read_ack;
  //assign debug8[4] = send_err;
  //assign debug8[5] = read_err;
  //assign debug8[7:6] = mouse_send_state;
  reg send_err;
  reg read_err;
  
  assign mouse_data = mouse_data_reg ? 1'bz : 1'b0;
  assign mouse_clk  = mouse_clk_reg  ? 1'bz : 1'b0;
  
  wire mymouse_cs = avm_m0_address[31:16] == 16'h0208;

  reg mouse_data_reg;
  reg mouse_clk_reg;

  reg [29:0]  timer_mouse;
  reg [10:0]  mymouse_data;
  reg  [2:0]  mouse_send_state;
  reg mouse_clk_buff0;
  reg mouse_clk_buff1;
  reg mouse_send_req_buff;
  reg mouse_read_req_buff;
  reg         mouse_send_ack;
  reg         mouse_read_ack;
  reg [3:0] mbitpos;
  reg parity;
  always@(posedge clk_50M or negedge reset_n) begin
    if(!reset_n) begin
      timer_mouse <= 0;
      mouse_clk_buff0 <= 0;
      mouse_clk_buff1 <= 0;
      mouse_send_state <= 0;
      mouse_clk_reg <= 1;
      mouse_data_reg <= 1;
      mouse_send_ack <= 0;
      mouse_read_ack <= 0;
      mouse_send_req_buff <= 0;
      mouse_read_req_buff <= 0;
      mbitpos <= 0;
      send_err <= 0;
      read_err <= 0;
    end else begin
      mouse_clk_buff0 <= mouse_clk;
      mouse_clk_buff1 <= mouse_clk_buff0;
      mouse_send_req_buff <= mouse_send_req;
      mouse_read_req_buff <= mouse_read_req;
      
      if(mouse_send_req_buff && !mouse_send_ack)begin
        if         (mouse_send_state==0)begin
          parity <= 1;
          timer_mouse <= 0;
          mouse_clk_reg <= 1;
          mouse_send_state <= 1;
          send_err <= 0;
        end else if(mouse_send_state==1)begin
          timer_mouse <= timer_mouse + 1'b1;
          if(timer_mouse == 200*50)begin// 100us
            mouse_clk_reg <= 0;
            mouse_data_reg <= 0;
            timer_mouse <= 0;
            mouse_send_state<=2;
          end
        end else if(mouse_send_state==2)begin
          timer_mouse <= timer_mouse + 1'b1;
          if(timer_mouse == 200*50)begin// 100us
            mouse_clk_reg <= 1;
            timer_mouse <= 0;
            mouse_send_state<=3;
          end
        end else if(mouse_send_state==3)begin
          timer_mouse <= timer_mouse + 1'b1;
          if(timer_mouse == 4*50)begin// 100us
            timer_mouse <= 0;
            mouse_send_state<=4;
          end
        end else if(mouse_send_state==4)begin
          if(timer_mouse!=50*10*1000)begin
            timer_mouse<=timer_mouse+1'b1;
          end else begin
            send_err <= 1;
            mouse_send_ack <= 1;
            mouse_data_reg <= 1;
            mouse_clk_reg <= 0;
            mouse_send_state <= 0;
          end

          //neg edge
          if(mouse_clk_buff1 && !mouse_clk_buff0)begin//1 old 0 new
            timer_mouse <= 0;
            mbitpos<=mbitpos+1'b1;
            mouse_data_reg <= mouse_send_data[mbitpos];
            if(mbitpos==9)begin
              mbitpos <= 0;
              mouse_data_reg <= 1;
              mouse_send_state <= 5;
            end
          end

        end else if(mouse_send_state==5)begin
          if(timer_mouse!=50*10*1000)begin
            timer_mouse<=timer_mouse+1'b1;
          end else begin
            send_err <= 1;
            mouse_send_ack <= 1;
            mouse_clk_reg <= 0;
            mouse_send_state <= 0;
          end
          if(mouse_data && mouse_clk_buff0)begin
            mouse_send_ack <= 1;
            mouse_clk_reg <= 0;
            mouse_send_state <= 0;
          end
        end
      end
      
      if(!mouse_send_req_buff && mouse_send_ack)begin
        mouse_send_ack <= 0;
      end
      
      if(mouse_read_req_buff && !mouse_read_ack)begin
        if         (mouse_send_state==0)begin
          read_err <= 0;
          timer_mouse <= 0;
          mouse_clk_reg <= 1;
          mouse_data_reg <= 1;
          mouse_send_state <= 1;
          mbitpos <= 0;
        end else if(mouse_send_state==1)begin
          timer_mouse <= timer_mouse + 1'b1;
          if(timer_mouse == 4*50)begin// 100us
            timer_mouse <= 0;
            mouse_send_state<=2;
          end
        end else if(mouse_send_state==2)begin
          if(timer_mouse!=50*10*1000)begin
            timer_mouse<=timer_mouse+1'b1;
          end else begin
            read_err <= 1;
            mouse_read_ack <= 1;
            mouse_clk_reg <= 0;
            mouse_send_state <= 0;
          end
          //pos edge
          if(!mouse_clk_buff1 && mouse_clk_buff0)begin//1 old 0 new
            timer_mouse <= 0;
            mbitpos<=mbitpos+1'b1;
            mymouse_data[mbitpos]<=mouse_data;
            if(mbitpos==9)begin
              mbitpos <= 0;
              mouse_send_state<=3;
            end
          end
        end else if(mouse_send_state==3)begin
          if(timer_mouse!=50*10*1000)begin
            timer_mouse<=timer_mouse+1'b1;
          end else begin
            read_err <= 1;
            mouse_read_ack <= 1;
            mouse_clk_reg <= 0;
            mouse_send_state <= 0;
          end
          //neg edge
          if(!mouse_clk_buff1 && mouse_clk_buff0)begin//1 old 0 new
            mouse_read_ack <= 1;
            mouse_clk_reg <= 0;
            mouse_send_state <= 0;
          end
        end
      end
      
      if(!mouse_read_req_buff && mouse_read_ack)begin
        mouse_read_ack <= 0;
      end
    end
  end

    
  wire [12:0] mymouse_readdata = avm_m0_address[15:2]==0?{mymouse_data_valid,(read_err?11'b0:mymouse_data)}:mouse_send_busy;
  wire        mouse_send_busy = mouse_send_req && !mouse_send_ack;
  reg         mouse_send_req;
  reg         mouse_read_req;
  reg         mymouse_data_valid;
  reg         mouse_read_ack_buff;
  reg  [9:0]  mouse_send_data;
  always@(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      mouse_send_req <= 0;
      mouse_read_req <= 0;
      mymouse_data_valid <= 0;
      mouse_read_ack_buff <= 0;
      mouse_send_data <= 0;
    end else begin
      mouse_read_ack_buff <= mouse_read_ack;
    
      if(mymouse_cs && avm_m0_write)begin
        if(avm_m0_address[15:2]==0)begin
          mymouse_data_valid <= 0;
          mouse_read_req <= 1;
        end
        if(avm_m0_address[15:2]==1)begin
          mouse_send_data <= avm_m0_writedata[9:0];
          mouse_send_req <= 1;
        end
      end
      
      if(mouse_read_ack_buff)begin
        mymouse_data_valid <= 1;
        mouse_read_req <= 0;
      end

      if(mouse_send_ack)begin
        mouse_send_req <= 0;
      end

    end
  end

  
  
 
  
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



  
endmodule
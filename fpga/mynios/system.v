module system (
    input  wire        clk,  //   clk50.clk
    input  wire        reset_n,  //   reset.reset_n

    output        mycpu_uart_txd, //   mycpu.uart_txd
    input         mycpu_uart_rxd, //        .uart_rxd
    output [7:0]  mycpu_debug8,   //        .debug8
    output [31:0] mycpu_debug32,   //        .debug32

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
    output  [7:0] sdrambus_debug8,
    
    input          softspi_MISO,   // softspi.MISO
    output         softspi_MOSI,   //        .MOSI
    output         softspi_SCLK,   //        .SCLK
    output  [2:0]  softspi_SS_n,   //        .SS_n
    
    input          myuart_rxd,     //  myuart.rxd
    output         myuart_txd,     //        .txd
    
    
    

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
    
     //SDRAM8m 芯片接口
    output        sdram8m_clk_out,            //SDRAM 芯片时钟
    output        sdram8m_cke,                //SDRAM 时钟有效
    output        sdram8m_cs_n,               //SDRAM 片选
    output        sdram8m_ras_n,              //SDRAM 行有效
    output        sdram8m_cas_n,              //SDRAM 列有效
    output        sdram8m_we_n,               //SDRAM 写有效
    output  [1:0] sdram8m_ba,                 //SDRAM Bank地址
    output [11:0] sdram8m_addr,               //SDRAM 行/列地址
    inout  [15:0] sdram8m_data,               //SDRAM 数据

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
  
  wire [31:0] avm_m0_address;
  wire        avm_m0_read;
  wire        avm_m0_write;
  wire [31:0] avm_m0_writedata;
  wire [ 3:0] avm_m0_byteenable;
  mycpu mycpu_0 (
    .clk                (clk),                       //       clock.clk
    .reset_n            (reset_n), //       reset.reset_n
    .avm_m0_address     (avm_m0_address),              //          m0.address
    .avm_m0_read        (avm_m0_read),                 //            .read
    .avm_m0_write       (avm_m0_write),                //            .write
    .avm_m0_readdata    (avm_m0_readdata),             //            .readdata
    .avm_m0_writedata   (avm_m0_writedata),            //            .writedata
    .avm_m0_byteenable  (avm_m0_byteenable),           //            .byteenable
    .avm_m0_waitrequest (avm_m0_waitrequest),          //            .waitrequest
    .inr_irq0_irq       (inr_irq0_irq),                //        irq0.irq

    .uart_txd           (mycpu_uart_txd),                  // conduit_end.export
    .uart_rxd           (mycpu_uart_rxd),                  //            .export
    .debug8             (mycpu_debug8),                    //            .export
    .debug32            (mycpu_debug32),                    //            .export
      
    .debugin8           (sdrambus_debug8)
      
  );
  reg [31:0] avm_m0_readdata;
  reg        avm_m0_waitrequest;
  reg [31:0] inr_irq0_irq;
  always @ (*) begin
    if         (sdrambus_cs )begin avm_m0_waitrequest <= sdrambus_waitrequest ; avm_m0_readdata <= sdrambus_readdata ;//TODO
    end else if(mainSRAM_cs )begin avm_m0_waitrequest <= mainSRAM_waitrequest ; avm_m0_readdata <= mainSRAM_readdata ;
    end else if(mytimer_cs  )begin avm_m0_waitrequest <= mytimer_waitrequest  ; avm_m0_readdata <= mytimer_readdata  ;
    end else if(myuart_cs   )begin avm_m0_waitrequest <= myuart_waitrequest   ; avm_m0_readdata <= myuart_readdata   ;
    end else if(softspi_cs  )begin avm_m0_waitrequest <= softspi_waitrequest  ; avm_m0_readdata <= softspi_readdata  ;
    end else if(vga_cs      )begin avm_m0_waitrequest <= vga_waitrequest      ; avm_m0_readdata <= 0                 ;
    end else if(vram_cs     )begin avm_m0_waitrequest <= vram_waitrequest     ; avm_m0_readdata <= 0                 ;
    end else if(dma_cs      )begin avm_m0_waitrequest <= dma_waitrequest      ; avm_m0_readdata <= 0                 ;
    end else                 begin avm_m0_waitrequest <= 0;                     avm_m0_readdata <= 0;
    end
  end
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  
  


  
reg         sdram_c_buffDMAread_req;  // input             
wire        sdram_c_buffDMAread_ack;  // output reg        
reg  [15:0] sdram_c_buffDMAread_addr;  // input      [15:0] 
reg         sdram_c_buffDMAread_A_B;  // input             

wire        sdram_c_buffDMAread_clk;  // output            
wire [15:0] sdram_c_buffDMAread_wrdata;  // output reg [15:0] 
wire  [7:0] sdram_c_buffDMAread_wraddress;  // output reg  [7:0] 
wire        sdram_c_buffDMAreadA_wren;  // output reg        
wire        sdram_c_buffDMAreadB_wren;  // output reg        

  
  wire sdrambus_cs = avm_m0_address[31:25] == 7'h0;// 0000:0000 ~ 01FF:FFFF 32M byte

  wire [31:0] sdrambus_readdata;
  wire sdrambus_waitrequest;
  sdrambus sdrambus_inst (
    .clk                (clk),                       //       clock.clk
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
      
    .buffDMAread_req        (sdram_c_buffDMAread_req        ),  // input             
    .buffDMAread_ack        (sdram_c_buffDMAread_ack        ),  // output reg        
    .buffDMAread_addr       (sdram_c_buffDMAread_addr       ),  // input      [15:0] 
    .buffDMAread_A_B        (sdram_c_buffDMAread_A_B        ),  // input             

    .buffDMAread_clk        (sdram_c_buffDMAread_clk        ),  // output            
    .buffDMAread_wrdata     (sdram_c_buffDMAread_wrdata     ),  // output reg [15:0] 
    .buffDMAread_wraddress  (sdram_c_buffDMAread_wraddress  ),  // output reg  [7:0] 
    .buffDMAreadA_wren      (sdram_c_buffDMAreadA_wren      ),  // output reg        
    .buffDMAreadB_wren      (sdram_c_buffDMAreadB_wren      ),  // output reg        
   
    .debug8 (sdrambus_debug8)

  );
  wire [22:0] sdrambus_address;
  assign      sdrambus_address = avm_m0_address[24:2];//~[0]
  
  wire   sdrambus_read  = sdrambus_cs ? avm_m0_read : 1'b0;
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

  
  
  wire mytimer_cs = avm_m0_address[31:16] == 16'h0201;

  wire [31:0] mytimer_readdata;
  wire        mytimer_waitrequest;
  mytimer mytimer_inst (
    .clk                (clk),                       //       clock.clk
    .reset_n            (reset_n), //       reset.reset_n
    .avs_s0_address     (mytimer_address ),
    .avs_s0_read        (mytimer_read ),
    .avs_s0_write       (mytimer_write ),
    .avs_s0_readdata    (mytimer_readdata ),
    .avs_s0_writedata   (avm_m0_writedata ),
    .avs_s0_waitrequest (mytimer_waitrequest ),
    .avs_s0_byteenable  (avm_m0_byteenable ),
    .dummy(dummy)
  );
  
  wire    [1:0]  mytimer_address;
  assign mytimer_address = avm_m0_address[3:2];//~[0]
  
  wire mytimer_read;
  assign mytimer_read = mytimer_cs ? avm_m0_read : 1'b0;
  wire mytimer_write;
  assign mytimer_write = mytimer_cs ? avm_m0_write : 1'b0;
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  
  wire myuart_cs = avm_m0_address[31:16] == 16'h0202;

  wire [31:0] myuart_readdata;
  wire        myuart_waitrequest;
  myuart myuart_inst (
    .clk                (clk),                       //       clock.clk
    .reset_n            (reset_n), //       reset.reset_n
    .avs_s0_address     (myuart_address ),
    .avs_s0_read        (myuart_read ),
    .avs_s0_write       (myuart_write ),
    .avs_s0_readdata    (myuart_readdata ),
    .avs_s0_writedata   (avm_m0_writedata ),
    .avs_s0_waitrequest (myuart_waitrequest ),
    .avs_s0_byteenable  (avm_m0_byteenable ),
    
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

  
  
  wire softspi_cs = avm_m0_address[31:16] == 16'h0203;

  wire [31:0] softspi_readdata;
  wire        softspi_waitrequest;
  softspi softspi_inst (
    .clk                (clk),                       //       clock.clk
    .reset_n            (reset_n), //       reset.reset_n
    .avs_s0_address     (softspi_address ),
    .avs_s0_read        (softspi_read ),
    .avs_s0_write       (softspi_write ),
    .avs_s0_readdata    (softspi_readdata ),
    .avs_s0_writedata   (avm_m0_writedata ),
    .avs_s0_waitrequest (softspi_waitrequest ),
    .avs_s0_byteenable  (avm_m0_byteenable ),
    
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


  wire vram_cs = avm_m0_address[31:24] == 8'h80;

  //直连
  assign sdram8m_buffDMAWrite_clk    = sdram_c_buffDMAread_clk;
  assign sdram8m_buffDMAWriteAB_data = sdram_c_buffDMAread_wrdata;
  assign sdram8m_buffDMAWriteAB_addr = sdram_c_buffDMAread_wraddress;
  assign sdram8m_buffDMAWriteA_en    = sdram_c_buffDMAreadA_wren;
  assign sdram8m_buffDMAWriteB_en    = sdram_c_buffDMAreadB_wren;

  wire        sdram8m_read_buff_req;
  wire        sdram8m_read_buff_A_B;
  reg         sdram8m_buffDMAwrite_req   ;             //input        
  reg  [13:0] sdram8m_buffDMAwrite_addr  ;            //input [13:0] 
  reg         sdram8m_buffDMAwrite_A_B   ;             //input        
  wire        sdram8m_buffDMAwrite_ack   ;             //output reg   

  wire        sdram8m_buffDMAWrite_clk   ;         //input        
  wire [15:0] sdram8m_buffDMAWriteAB_data;         //input [15:0] 
  wire  [7:0] sdram8m_buffDMAWriteAB_addr;         //input  [7:0] 
  wire        sdram8m_buffDMAWriteA_en   ;         //input        
  wire        sdram8m_buffDMAWriteB_en   ;         //input        


  wire        read_line_req         ;//input read_buffA_req,
  wire        read_line_A_B         ;//input read_buffB_req,
  wire [11:0] read_line_addr        ;//input [9:0] read_buff_addr,
  reg  [11:0] read_line_base_addr   ;//input [9:0] read_buff_addr,
  wire [15:0] read_pixelA_data      ;//output [15:0] buff_readA_data,
  wire [15:0] read_pixelB_data      ;//output [15:0] buff_readB_data,
  wire  [9:0] read_pixel_addr       ;//input [9:0]   buff_readB_addr,
  wire        read_pixel_clk        ;//input         buff_readB_clk,

  wire vram_waitrequest;

  wire sdram8m_read = vram_cs ? avm_m0_read : 1'b0;
  wire sdram8m_write = vram_cs ? avm_m0_write : 1'b0;
  wire [22:0] sdram8m_address = avm_m0_address[24:2];
  sdram8mvga ins_sdram8mvga(
    .sys_clk    (clk    ),       // 时钟信号
    .sys_rst_n  (reset_n),       // 复位信号

    //SDRAM 芯片接口
    .sdram_clk_out   (sdram8m_clk_out),
    .sdram_cke       (sdram8m_cke),    //SDRAM 时钟有效
    .sdram_cs_n      (sdram8m_cs_n),    //SDRAM 片选
    .sdram_ras_n     (sdram8m_ras_n),    //SDRAM 行有效  
    .sdram_cas_n     (sdram8m_cas_n),    //SDRAM 列有效
    .sdram_we_n      (sdram8m_we_n),    //SDRAM 写有效
    .sdram_ba        (sdram8m_ba),      //SDRAM Bank地址
    .sdram_addr      (sdram8m_addr),    //SDRAM 行/列地址
    .sdram_data      (sdram8m_data),    //SDRAM 数据  

    .avs_s0_address     (sdram8m_address ),
    .avs_s0_read        (sdram8m_read ),
    .avs_s0_write       (sdram8m_write ),
    .avs_s0_readdata    (sdram8m_readdata ),
    .avs_s0_writedata   (avm_m0_writedata ),
    .avs_s0_waitrequest (vram_waitrequest ),
    .avs_s0_byteenable  (avm_m0_byteenable ),

    
    .buffDMAwrite_req   (sdram8m_buffDMAwrite_req   ),             //input        
    .buffDMAwrite_addr  (sdram8m_buffDMAwrite_addr  ),            //input [11:0] 
    .buffDMAwrite_A_B   (sdram8m_buffDMAwrite_A_B   ),             //input        
    .buffDMAwrite_ack   (sdram8m_buffDMAwrite_ack   ),             //output reg   
    .buffDMAWrite_clk   (sdram8m_buffDMAWrite_clk   ),         //input        

    .buffDMAWriteAB_data(sdram8m_buffDMAWriteAB_data),         //input [15:0] 
    .buffDMAWriteAB_addr(sdram8m_buffDMAWriteAB_addr),         //input  [7:0] 
    .buffDMAWriteA_en   (sdram8m_buffDMAWriteA_en   ),         //input        
    .buffDMAWriteB_en   (sdram8m_buffDMAWriteB_en   ),         //input        
        
    .read_line_req   (read_line_req       ),//input read_buffA_req,
    .read_line_A_B   (read_line_A_B       ),//input read_buffB_req,
    .read_line_addr  (read_line_addr      ),//input [9:0] read_buff_addr,
    .read_pixelA_data(read_pixelA_data    ),//output [15:0] buff_readA_data,
    .read_pixelB_data(read_pixelB_data    ),//output [15:0] buff_readB_data,
    .read_pixel_addr (read_pixel_addr     ),//input [9:0]   buff_readB_addr,
    .read_pixel_clk  (read_pixel_clk      ),//input         buff_readB_clk,

    .isDMA (sdram8m_isDMA)
  );
  
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  wire vga_cs = avm_m0_address[31:16] == 16'h0204;
  reg [1:0] vga_mode;
  reg blockvga_from_system;
  //read_line_base_addr
  always@(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      vga_mode <= 3;
    end else begin
      if(vga_cs && avm_m0_write)begin
        if         (avm_m0_address[15:2]==0)begin
          vga_mode <= avm_m0_writedata[1:0];
        end else if(avm_m0_address[15:2]==1)begin
          read_line_base_addr = avm_m0_writedata[11:0];
        end else if(avm_m0_address[15:2]==2)begin
          blockvga_from_system = avm_m0_writedata[0];
        end
      end
    end
  end
  wire vga_waitrequest = 0;

  wire blanking;
  wire blockvga = blockvga_from_DMA | blockvga_from_system;
  vga_driver8m u_vga_driver8m(
    .sys_clk        (clk    ),
    .sys_rst_n      (reset_n),

    .blockvga(blockvga),
    .vga_mode(vga_mode),
    .blanking(blanking),
    
    .read_line_req       (read_line_req       ),
    .read_line_A_B       (read_line_A_B       ),
    .read_line_addr      (read_line_addr      ),
    .read_line_base_addr (read_line_base_addr ),
    
    .read_pixelA_data    (read_pixelA_data    ),
    .read_pixelB_data    (read_pixelB_data    ),
    .read_pixel_addr     (read_pixel_addr     ),
    .read_pixel_clk      (read_pixel_clk      ),
    
    .vga_hs         (vga_hs),
    .vga_vs         (vga_vs),
    .vga_rgb        (vga_rgb)
  );

  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //dma controler


  wire dma_cs = avm_m0_address[31:16] == 16'h0205;
  reg [15:0]  DMA_src_page;//4page per line  512byte per page //   int dma_src = trackBar1.Value * 4;//4page per line
  reg [15:0]  DMA_des_page;//4page per line  512byte per page //   int dma_src = trackBar1.Value * 4;//4page per line
  reg [15:0]  DMA_page_length;//int page_len = 256;   256page per time       total 12times
  reg         DMA_req;
  
  wire dma_waitrequest = DMA_req;

  always@(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      DMA_src_page <= 0;
      DMA_des_page <= 0;
      DMA_page_length <= 0;
      DMA_req <= 0;
    end else begin
      if(dma_cs && avm_m0_write)begin
        if         (avm_m0_address[15:2]==0)begin
          DMA_src_page <= avm_m0_writedata[15:0];
        end else if(avm_m0_address[15:2]==1)begin
          DMA_des_page <= avm_m0_writedata[15:0];
        end else if(avm_m0_address[15:2]==2)begin
          DMA_page_length <= avm_m0_writedata[15:0];
        end else if(avm_m0_address[15:2]==3)begin
          DMA_req <= 1;
        end
      end
      
      if(DMA_ack)begin
        DMA_req <= 0;
      end
    end
  end

  reg [20:0] timer12;//1Mpage * 512byte = 512Mbyte

  reg start;
  reg inited;
  reg blanking_buff;
  reg blanking_last;

  reg sdram8m_isDMA;
  reg DMA_ack;
  reg blockvga_from_DMA;
  always @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin

      sdram_c_buffDMAread_req <= 0;
      sdram8m_buffDMAwrite_req <= 0;

      blockvga_from_DMA<=0;
      start<=0;
      inited<=0;

      sdram8m_isDMA <= 0;
      
    end else begin
      blanking_buff <= blanking;
      blanking_last <= blanking_buff;

      if(DMA_req && !DMA_ack) begin//memcopy
        if(!start)begin
          if(blanking_buff && !blanking_last)begin
            blockvga_from_DMA<=1;
            start <= 1;
            sdram_c_buffDMAread_addr <= DMA_src_page;//65536page 1page=256word
            sdram_c_buffDMAread_A_B <= 0;
            sdram8m_buffDMAwrite_addr <= DMA_des_page + 16'hffff;//(page-1) 4096 page 1page=256word
            sdram8m_buffDMAwrite_A_B <= 1;
            sdram8m_isDMA <= 1;
            timer12 <= 0;
          end
        end
   
        if(start)begin
          if(!inited)begin
            inited <= 1;
            sdram_c_buffDMAread_req <= 1;
            if(timer12!=0)begin
              sdram8m_buffDMAwrite_req <= 1;
            end
          end else begin
            if(sdram_c_buffDMAread_req && sdram_c_buffDMAread_ack && (sdram8m_buffDMAwrite_req && sdram8m_buffDMAwrite_ack || timer12==0) )begin
              sdram_c_buffDMAread_req <= 0;
              sdram8m_buffDMAwrite_req <= 0;
              sdram_c_buffDMAread_addr <= sdram_c_buffDMAread_addr + 1'b1;
              sdram_c_buffDMAread_A_B <= !sdram_c_buffDMAread_A_B;
              sdram8m_buffDMAwrite_addr <= sdram8m_buffDMAwrite_addr + 1'b1;
              sdram8m_buffDMAwrite_A_B <= !sdram8m_buffDMAwrite_A_B;
              if(timer12==DMA_page_length)begin
                blockvga_from_DMA <= 0;
                inited <= 0;
                start <= 0;
                sdram8m_isDMA <= 0;
                DMA_ack <= 1;
              end
            end
            if(!sdram_c_buffDMAread_req && !sdram_c_buffDMAread_ack && (!sdram8m_buffDMAwrite_req && !sdram8m_buffDMAwrite_ack || timer12==0) )begin
              timer12<=timer12+1'b1;
              inited <= 0;
            end
          end
        end
      end
      
      if(!DMA_req && DMA_ack) begin
        DMA_ack <= 0;
      end
      
    end
  end






  
endmodule
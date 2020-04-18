module system (
    input  wire        clk,  //   clk50.clk
    input  wire        reset_n,  //   reset.reset_n

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
  
  
  
  assign debug8 = sdrambus_debug8;
  assign debug32 = mycpu_debug32;
  
  
  
  
  
  wire [7:0]  mycpu_debug8;
  wire [31:0]  mycpu_debug32;
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
      
    .debugin8           (sdrambus_debug8),
    
    .vga_blanking(blanking)
      
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

  
  wire sdrambus_cs = avm_m0_address[31:25] == 7'h0;// 0000:0000 ~ 01FF:FFFF 32M byte

  wire [31:0] sdrambus_readdata;
  wire sdrambus_waitrequest;
  sdrambusvga sdrambusvga_inst (
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
      
    .read_line_req   (read_line_req       ),//input read_buffA_req,
    .read_line_A_B   (read_line_A_B       ),//input read_buffB_req,
    .read_line_addr  (read_line_addr      ),//input [9:0] read_buff_addr,
    .read_pixelA_data(read_pixelA_data    ),//output [15:0] buff_readA_data,
    .read_pixelB_data(read_pixelB_data    ),//output [15:0] buff_readB_data,
    .read_pixel_addr (read_pixel_addr     ),//input [9:0]   buff_readB_addr,
    .read_pixel_clk  (read_pixel_clk      ),//input         buff_readB_clk,
   
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

  wire vga_cs = avm_m0_address[31:16] == 16'h0204;
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





  
endmodule
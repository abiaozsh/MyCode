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
    
    .debugin8           (sdrambus_debug8),
    
		.uart_txd           (mycpu_uart_txd),                  // conduit_end.export
		.uart_rxd           (mycpu_uart_rxd),                  //            .export
		.debug8             (mycpu_debug8),                    //            .export
		.debug32            (mycpu_debug32)                    //            .export
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
    end else                 begin avm_m0_waitrequest <= 0;                     avm_m0_readdata <= 0;
    end
  end
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  
  
  
  
  
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
    
    .buffDMAread_req      (buffDMAread_req      ),
    .buffDMAread_ack      (buffDMAread_ack      ),
    .buffDMAread_addr     (buffDMAread_addr     ),
    .buffDMAread_clk      (buffDMAread_clk      ),
    .buffDMAread_A_B      (buffDMAread_A_B      ),
    .buffDMAread_wrdata   (buffDMAread_wrdata   ),
    .buffDMAread_wraddress(buffDMAread_wraddress),
    .buffDMAreadA_wren    (buffDMAreadA_wren    ),
    .buffDMAreadB_wren    (buffDMAreadB_wren    ),
    
    .debug8 (sdrambus_debug8)

  );
  wire [22:0] sdrambus_address;
  assign      sdrambus_address = avm_m0_address[24:2];//~[0]
  
  wire   sdrambus_read  = sdrambus_cs ? avm_m0_read : 1'b0;
  wire   sdrambus_write = sdrambus_cs ? avm_m0_write : 1'b0;

  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  
  
  
  
  
  wire mainSRAM_cs = avm_m0_address[31:16] == 16'h0200;
  
  wire [31:0] mainSRAM_readdata;
  mainSRAM	mainSRAM_inst (
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

  
endmodule
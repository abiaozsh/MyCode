 
module flow_led(
  input sys_clk,
  input key1,
  input key2,
  output led,
 
	 //flash
	 //input  flash_data0,
	 //output flash_sdo,
	 //output flash_sce,
	 //output flash_dclk,
/*
set_location_assignment PIN_K1 -to flash_data0
set_location_assignment PIN_D1 -to flash_sdo
set_location_assignment PIN_E2 -to flash_sce
set_location_assignment PIN_K2 -to flash_dclk
set_global_assignment -name CYCLONEII_RESERVE_NCEO_AFTER_CONFIGURATION "USE AS REGULAR IO"
set_global_assignment -name RESERVE_DATA0_AFTER_CONFIGURATION "USE AS REGULAR IO"
set_global_assignment -name RESERVE_DATA1_AFTER_CONFIGURATION "USE AS REGULAR IO"
set_global_assignment -name RESERVE_FLASH_NCE_AFTER_CONFIGURATION "USE AS REGULAR IO"
set_global_assignment -name RESERVE_DCLK_AFTER_CONFIGURATION "USE AS REGULAR IO"
*/
   //uart接口
  input uart_rxd,
  output uart_txd,
 
  output segled_clk,
  output segled_dat, 
  output segled_str,

  output [7:0] debug,
 
	input dummy
);
wire sys_rst_n;
assign sys_rst_n = key1;

wire [7:0] seg_data0;
wire [7:0] seg_data1;
wire [7:0] seg_data2;
wire [7:0] seg_data3;
seg_led_hex595 ins_seg_led_hex595(
  .sys_clk(sys_clk), 
  .sys_rst_n(sys_rst_n),
 
  .clk(segled_clk),
  .dat(segled_dat),
  .str(segled_str),

  .data0(seg_data0),
  .data1(seg_data1),
  .data2(seg_data2),
  .data3(seg_data3)
);

	
assign seg_data3 = outpin32[31:24];
assign seg_data2 = outpin32[23:16];
assign seg_data1 = outpin32[15:8];
assign seg_data0 = outpin32[7:0];

wire [31:0] outpin32;
wire [7:0] inpin8;
assign inpin8[0] = key2;
wire [15:0] debug2;

sys u0 (
		.clk_clk        (sys_clk),        //     clk.clk
		.reset_reset_n  (sys_rst_n),  //   reset.reset_n
		.pio0out_export (outpin32), // pio0out.export
		.pio1in_export  (inpin8),  //  pio1in.export
		.mysdram_uart_rxd (uart_rxd), // uart_loader.uart_rxd
		.mysdram_uart_txd (uart_txd), //            .uart_txd
		.mysdram_debug    (debug),     //            .debug
		.mysdram_debug2   (debug2)
);

endmodule




module flow_led(
  input sys_clk,
  input key1,
  input key2,
  output reg led,

  output segled_clk,
  output segled_dat,
  output segled_str,
   
 output AA13,
 output AA14,
 output AA15,
 output AA16,
 output AA17,
 output AA18,
 output AA19,
 output AA20,
 
 
  //uart接口
  input uart_rx_from_pc,
  output uart_tx_to_pc
);
wire sys_rst_n;
assign sys_rst_n = key1;


wire uart_rec;
wire [7:0] uart_data_out_from_pc;
reg uart_send;
reg [7:0] uart_data_in_to_pc;
uart_hs (
  .sys_clk(sys_clk), 
  .sys_rst_n(sys_rst_n),

  .uart_txd(uart_tx_to_pc),
  .uart_rxd(uart_rx_from_pc),
  
  .uart_rec(uart_rec),
  .uart_data_out(uart_data_out_from_pc),

  .uart_send(uart_send),
  .uart_data_in(uart_data_in_to_pc)
);

assign AA13 = debug[0];
assign AA14 = debug[1];
assign AA15 = debug[2];
assign AA16 = debug[3];
assign AA17 = debug[4];
assign AA18 = debug[5];
assign AA19 = debug[6];
assign AA20 = debug[7];



wire [7:0] debug;
reg [7:0] seg_data0;
reg [7:0] seg_data1;
reg [7:0] seg_data2;
reg [7:0] seg_data3;
seg_led_hex595 (
  .sys_clk(sys_clk), 
  .sys_rst_n(sys_rst_n),

  .clk(segled_clk),
  .dat(segled_dat),
  .str(segled_str),

  .debug(debug),
  
  .data0(seg_data0),
  .data1(seg_data1),
  .data2(seg_data2),
  .data3(seg_data3)
);





reg key2_old;

always @(posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    uart_data_in_to_pc <= 48;//'0'
    uart_send <= 0;
    key2_old <= 0;
    led = 0;
    seg_data0<=0;
    seg_data1<=0;
    seg_data2<=0;
    seg_data3<=0;
  end else begin
    key2_old = key2;
     
    if(uart_rec)begin// && !uart_rec_last
      seg_data0<=uart_data_out_from_pc;
      seg_data1<=seg_data0;
      seg_data2<=seg_data1;
      seg_data3<=seg_data2;
      led <= !led;
    end
    uart_send <= 0;
    if(key2_old && !key2)begin
      uart_send <= 1;     
      uart_data_in_to_pc <= uart_data_in_to_pc + 1;
    end

  end
end




endmodule




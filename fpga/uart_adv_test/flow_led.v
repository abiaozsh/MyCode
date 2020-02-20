
module flow_led(
  input           sys_clk,          //外部50M时钟
  input key1,
  input key2,
  output led,
 
  output segled_clk,
  output segled_dat, 
  output segled_str,
 
  //uart接口
  input uart_rxd,
  output uart_txd
);
wire sys_rst_n;
assign sys_rst_n = key1;

reg [7:0] seg_data0;
reg [7:0] seg_data1;
reg [7:0] seg_data2;
reg [7:0] seg_data3;
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
 

wire uart_rec;
wire [7:0] uart_data_out;
reg uart_send;
reg [7:0] uart_data_in;
uart_hs (
  .sys_clk(sys_clk), 
  .sys_rst_n(sys_rst_n),

  .uart_txd(uart_txd),
  .uart_rxd(uart_rxd),
  
  .uart_rec(uart_rec),
  .uart_data_out(uart_data_out),

  .uart_send(uart_send),
  .uart_data_in(uart_data_in)
);


always @(posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    seg_data0 <= 0;
    seg_data1 <= 0;
    seg_data2 <= 0;
		seg_data3 <= 0;
    uart_send <= 0;
  end else begin  
    if(uart_rec)begin// && !uart_rec_last
      seg_data0<=uart_data_out;
      seg_data1<=seg_data0;
      seg_data2<=seg_data1;
      seg_data3<=seg_data2;
    end
    
    if         (!key2)begin
      uart_send <= 1;     
      uart_data_in <= 8'h11;
    end else begin
      uart_send<=0;
      if(uart_rec)begin// && !uart_rec_last
        uart_data_in <= uart_data_out;
        uart_send <= 1;
      end
    end

    
  end
end




endmodule





module flow_led(
  input sys_clk,
  input sys_rst_n,
  
  input [3:0] key,
  output reg [3:0] led,

  output [5:0] seg_sel,
  output [7:0] seg_led,

  //uart接口
  input uart_rxd,
  output uart_txd
);
 
wire  [3:0]  key_stable;
genvar i;
generate
  for(i=0; i<4; i=i+1) begin:BLOCK1
    key_debounce(
        .sys_clk        (sys_clk),
        .sys_rst_n      (sys_rst_n),
  
        .key            (key[i]),
        .key_value      (key_stable[i])
      );
  end
endgenerate

always @(posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    led<=0;
  end else begin
    led<=key_stable;
  end
end

  
reg [7:0] data0;
reg [7:0] data1;
reg [7:0] data2;
seg_led_hex(
  .sys_clk(sys_clk),
  .sys_rst_n(sys_rst_n),

  .seg_sel(seg_sel),
  .seg_led(seg_led),

  .data0(data0),
  .data1(data1),
  .data2(data2),
  .en(1)
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

//reg uart_rec_last;
//always @(posedge sys_clk or negedge sys_rst_n) begin
//  if (!sys_rst_n) begin
//    uart_rec_last<=0;
//  end else begin  
//      uart_rec_last<=uart_rec;
//  end
//end

always @(posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    data0 <= 0;
    data1 <= 0;
    data2 <= 0;
    uart_send <= 0;
  end else begin  
    if(uart_rec)begin// && !uart_rec_last
      data0<=uart_data_out;
      data1<=data0;
      data2<=data1;
    end
    
    if         (!key_stable[0])begin
      uart_send <= 1;     
      uart_data_in <= 8'h11;
    end else if(!key_stable[1])begin
      uart_send <= 1;     
      uart_data_in <= 8'h22;
    end else if(!key_stable[2])begin
      uart_send <= 1;     
      uart_data_in <= 8'h33;
    end else if(!key_stable[3])begin
      uart_send <= 1;     
      uart_data_in <= 8'h44;
    end else begin
      uart_send<=0;
      if(uart_rec)begin// && !uart_rec_last
        uart_data_in <= uart_data_out;
        uart_send <= 1;
      end
    end

    
  end
end









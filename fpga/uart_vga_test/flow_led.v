
module flow_led(
  input sys_clk,
  input key1,
  input key2,
  output reg led,

  output segled_clk,
  output segled_dat,
  output segled_str,
   
    //VGA接口                          
    output          vga_hs,         //行同步信号
    output          vga_vs,         //场同步信号
    output  [15:0]  vga_rgb,         //红绿蓝三原色输出 

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







//wire define
wire         vga_clk_w;             //PLL分频得到25Mhz时钟
wire         locked_w;              //PLL输出稳定信号
wire         rst_n_w;               //内部复位信号
wire [15:0]  pixel_data_w;          //像素点数据
wire [ 9:0]  pixel_xpos_w;          //像素点横坐标
wire [ 9:0]  pixel_ypos_w;          //像素点纵坐标    
    
//*****************************************************
//**                    main code
//***************************************************** 
//待PLL输出稳定之后，停止复位
assign rst_n_w = sys_rst_n && locked_w;
   
vga_pll	u_vga_pll(                  //时钟分频模块
	.inclk0         (sys_clk),    
	.areset         (~sys_rst_n),
    
	.c0             (vga_clk_w),    //VGA时钟 25M
	.locked         (locked_w)
	); 

vga_driver u_vga_driver(
    .vga_clk        (vga_clk_w),    
    .sys_rst_n      (rst_n_w),    

    .vga_hs         (vga_hs),       
    .vga_vs         (vga_vs),       
    .vga_rgb        (vga_rgb),      
    
    .pixel_data     (pixel_data_w), 
    .pixel_xpos     (pixel_xpos_w), 
    .pixel_ypos     (pixel_ypos_w)
    ); 
    
vga_display u_vga_display(
    .vga_clk        (vga_clk_w),
    .sys_rst_n      (rst_n_w),
    
    .pixel_xpos     (pixel_xpos_w),
    .pixel_ypos     (pixel_ypos_w),
    .pixel_data     (pixel_data_w)
    );   
    




endmodule




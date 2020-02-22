
module uart_hs(
    input sys_clk,
    input sys_rst_n,
    
    input  uart_rxd,                 //UART接收端口
    output uart_txd,                  //UART发送端口
    
    output            uart_rec,                //接收一帧数据完成标志信号 上升沿表示接收到
    output      [7:0] uart_data_out,                 //接收的数据
    input             uart_send,                  //发送使能信号
    input       [7:0] uart_data_in                 //待发送数据
);

  uart_send_hs ins_uart_send_hs(
    .sys_clk (sys_clk),
    .sys_rst_n (sys_rst_n),

    .uart_txd (uart_txd),
    
    .uart_send (uart_send),
    .uart_data_in (uart_data_in)
  );

uart_recv_hs ins_uart_recv_hs(
    .sys_clk (sys_clk),
    .sys_rst_n (sys_rst_n),

    .uart_rxd (uart_rxd),
    
    .uart_rec (uart_rec),
    .uart_data_out (uart_data_out)
  );
endmodule

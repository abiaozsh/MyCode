module uart_send_hs(
    input sys_clk,
    input sys_rst_n,
    
    output reg uart_txd,//UART发送端口

    input uart_send,//发送使能信号
    input [7:0] uart_data_in//待发送数据
  );

//parameter define
localparam BPS_CNT  = 25; //2000000 bps  50000000/2000000
localparam BPS_CNT_HALF  = 12;        //2000000 bps  50000000/2000000

reg uart_send_last1;
reg uart_send_last2;
reg tx_flag;//发送过程标志信号
reg [7:0] tx_data;//寄存发送数据
reg [7:0] clk_cnt;//系统时钟计数器

//根据发送数据计数器来给uart发送端口赋值
always @(posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    tx_flag <= 0;
    clk_cnt <= 0;
    uart_send_last1 <= 0;
    uart_send_last2 <= 0;
    uart_txd <= 1'b1;
    tx_data <= 0;
  end else begin
    uart_send_last1 <= uart_send;
    uart_send_last2 <= uart_send_last1;

    if(uart_send_last2 && !uart_send_last1) begin //检测到发送使能上升沿
      tx_flag <= 1; //进入发送过程，标志位tx_flag拉高
      tx_data <= uart_data_in; //寄存待发送的数据
    end else if ((clk_cnt == (9 * BPS_CNT + BPS_CNT_HALF))) begin//计数到停止位中间时，停止发送过程
      tx_flag <= 0; //发送过程结束，标志位tx_flag拉低
    end

    if (tx_flag) begin
      clk_cnt <= clk_cnt + 1'b1;
      case(clk_cnt)
        (0 * BPS_CNT): uart_txd <= 1'b0; //起始位 
        (1 * BPS_CNT): uart_txd <= tx_data[0]; //数据位最低位
        (2 * BPS_CNT): uart_txd <= tx_data[1];
        (3 * BPS_CNT): uart_txd <= tx_data[2];
        (4 * BPS_CNT): uart_txd <= tx_data[3];
        (5 * BPS_CNT): uart_txd <= tx_data[4];
        (6 * BPS_CNT): uart_txd <= tx_data[5];
        (7 * BPS_CNT): uart_txd <= tx_data[6];
        (8 * BPS_CNT): uart_txd <= tx_data[7]; //数据位最高位
        (9 * BPS_CNT): uart_txd <= 1'b1; //停止位
        default: ;
      endcase
    end else begin
      clk_cnt <= 0;//对系统时钟计数达一个波特率周期后清零
      uart_txd <= 1'b1; //空闲时发送端口为高电平
    end
  end
end

endmodule


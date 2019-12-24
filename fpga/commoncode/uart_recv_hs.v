module uart_recv_hs(
    input sys_clk,
    input sys_rst_n,

    input uart_rxd,//UART接收端口
    
    output reg uart_rec,//接收一帧数据完成标志信号 上升沿表示接收到
    output reg [7:0] uart_data_out//接收的数据
  );
    
  //parameter define
  localparam BPS_CNT  = 25;        //2000000 bps  50000000/2000000
  localparam BPS_CNT_HALF  = 12;        //2000000 bps  50000000/2000000

  //需要对系统时钟计数BPS_CNT次
  reg [ 4:0] clk_cnt;                             //系统时钟计数器
  reg [ 3:0] rx_cnt;                              //接收数据计数器
  reg [ 7:0] rxdata;                              //接收数据寄存器

  //wire define
  wire start_flag;

  //捕获接收端口下降沿(起始位)，得到一个时钟周期的脉冲信号
  assign start_flag = uart_rxd_last & (~uart_rxd);

  reg uart_rxd_last;
  //对UART接收端口的数据延迟两个时钟周期
  always @(posedge sys_clk or negedge sys_rst_n) begin 
    if (!sys_rst_n) begin 
      uart_rxd_last <= 1'b0;
    end else begin
      uart_rxd_last  <= uart_rxd;
    end
  end

  reg rx_flag; //接收过程标志信号
  //当脉冲信号start_flag到达时，进入接收过程
  always @(posedge sys_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
      rx_flag <= 1'b0;
    end else begin
      if(start_flag) begin//检测到起始位
        rx_flag <= 1'b1; //进入接收过程，标志位rx_flag拉高
      end else if((rx_cnt == 4'd9)&&(clk_cnt == BPS_CNT_HALF)) begin
        rx_flag <= 1'b0; //计数到停止位中间时，停止接收过程
      end else begin
        rx_flag <= rx_flag;
      end
    end
  end

  //进入接收过程后，启动系统时钟计数器与接收数据计数器
  always @(posedge sys_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
      clk_cnt <= 5'd0;
      rx_cnt  <= 4'd0;
    end else if ( rx_flag ) begin //处于接收过程
      if (clk_cnt < BPS_CNT) begin
        clk_cnt <= clk_cnt + 1'b1;
        rx_cnt  <= rx_cnt;
      end else begin
        clk_cnt <= 5'd0; //对系统时钟计数达一个波特率周期后清零
        rx_cnt  <= rx_cnt + 1'b1; //此时接收数据计数器加1
      end
    end else begin //接收过程结束，计数器清零
      clk_cnt <= 5'd0;
      rx_cnt  <= 4'd0;
    end
  end

  //根据接收数据计数器来寄存uart接收端口数据
  always @(posedge sys_clk or negedge sys_rst_n) begin 
    if ( !sys_rst_n) begin
      rxdata <= 8'd0;
    end else if(rx_flag) begin //系统处于接收过程
      if (clk_cnt == BPS_CNT_HALF) begin //判断系统时钟计数器计数到数据位中间
        case ( rx_cnt )
          4'd1 : rxdata[0] <= uart_rxd_last; //寄存数据位最低位
          4'd2 : rxdata[1] <= uart_rxd_last;
          4'd3 : rxdata[2] <= uart_rxd_last;
          4'd4 : rxdata[3] <= uart_rxd_last;
          4'd5 : rxdata[4] <= uart_rxd_last;
          4'd6 : rxdata[5] <= uart_rxd_last;
          4'd7 : rxdata[6] <= uart_rxd_last;
          4'd8 : rxdata[7] <= uart_rxd_last; //寄存数据位最高位
          default:;
        endcase
      end else begin
        rxdata <= rxdata;
      end
    end else begin
      rxdata <= 8'd0;
    end
  end

  //数据接收完毕后给出标志信号并寄存输出接收到的数据
  always @(posedge sys_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
      uart_data_out <= 8'd0;
      uart_rec <= 1'b0;
    end else if(rx_cnt == 4'd9) begin //接收数据计数器计数到停止位时
      uart_data_out <= rxdata; //寄存输出接收到的数据
      uart_rec <= 1'b1; //并将接收完成标志位拉高
    end else begin
      uart_data_out <= 8'd0;
      uart_rec <= 1'b0;
    end
  end

endmodule

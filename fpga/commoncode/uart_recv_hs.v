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
  reg [ 7:0] clk_cnt;                             //系统时钟计数器

  //wire define
  wire start_flag;

  //捕获接收端口下降沿(起始位)，得到一个时钟周期的脉冲信号
  assign start_flag = uart_rxd_last & (~uart_rxd);

  reg uart_rxd_last;
  //对UART接收端口的数据延迟两个时钟周期
  always @(posedge sys_clk or negedge sys_rst_n) begin 
    if (!sys_rst_n) begin 
      uart_rxd_last <= 1;
    end else begin
      uart_rxd_last  <= uart_rxd;
    end
  end

  reg rx_flag; //接收过程标志信号
  //当脉冲信号start_flag到达时，进入接收过程
  always @(posedge sys_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
      rx_flag <= 0;
    end else begin
      if(start_flag) begin//检测到起始位
        rx_flag <= 1; //进入接收过程，标志位rx_flag拉高
      end else if(clk_cnt == (BPS_CNT * 9 + BPS_CNT_HALF)) begin
        rx_flag <= 0; //计数到停止位中间时，停止接收过程
      end
    end
  end

  //进入接收过程后，启动系统时钟计数器与接收数据计数器
  always @(posedge sys_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
      clk_cnt <= 0;
    end else begin
      if ( rx_flag ) begin //处于接收过程
        clk_cnt <= clk_cnt + 1;
      end else begin //接收过程结束，计数器清零
        clk_cnt <= 0;
      end
    end
  end

  //根据接收数据计数器来寄存uart接收端口数据
  always @(posedge sys_clk or negedge sys_rst_n) begin 
    if ( !sys_rst_n) begin
      uart_data_out <= 8'd0;
    end else begin
      if(rx_flag) begin //系统处于接收过程
        case ( clk_cnt )
          (1*BPS_CNT+BPS_CNT_HALF) : uart_data_out[0] <= uart_rxd; //寄存数据位最低位
          (2*BPS_CNT+BPS_CNT_HALF) : uart_data_out[1] <= uart_rxd;
          (3*BPS_CNT+BPS_CNT_HALF) : uart_data_out[2] <= uart_rxd;
          (4*BPS_CNT+BPS_CNT_HALF) : uart_data_out[3] <= uart_rxd;
          (5*BPS_CNT+BPS_CNT_HALF) : uart_data_out[4] <= uart_rxd;
          (6*BPS_CNT+BPS_CNT_HALF) : uart_data_out[5] <= uart_rxd;
          (7*BPS_CNT+BPS_CNT_HALF) : uart_data_out[6] <= uart_rxd;
          (8*BPS_CNT+BPS_CNT_HALF) : begin uart_data_out[7] <= uart_rxd;uart_rec <= 1; end //寄存数据位最高位
          default:uart_rec <= 0;
        endcase
      end else begin
        uart_rec <= 0;
      end
    end
  end

endmodule

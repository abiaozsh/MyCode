module seg_led_hex595(
  input sys_clk,
  input sys_rst_n,

  output clk,
  output reg dat,
  output reg str,

  output [7:0] debug,
  
  input [7:0] data0,
  input [7:0] data1,
  input [7:0] data2,
  input [7:0] data3
);

//50000000 / 1024 / 8 / 16 / 8 = 47hz
//2^13
reg [9:0] timer1;
assign clk = timer1[9];
always @ (posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    timer1 <= 0;
  end else begin
    timer1 <= timer1+1;
  end
end

reg [3:0] timer595;//发送16位
always @ (posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    timer595 <= 0;
  end else begin
    if(timer1==10'b1111111111) begin
      timer595<=timer595+1;
    end
  end
end

reg [2:0] timer2;//0~7
always @ (posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    timer2 <= 0;
  end else begin
    if(timer1==10'b1111111111 && timer595==4'b1111) begin
      timer2<=timer2+1;
    end
  end
end

assign debug = timer2;

always @ (posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    dat <= 0;
    str <= 0;
  end else begin
  
    //led_table = LED_0F + LED[0];
    //i = *led_table;
    //LED_OUT(i);			先送数据   先送高位 后送低位
    //LED_OUT(0x01);		后送选字
    if(timer2==0) begin
      if         (timer595==0) begin
        dat<=dig_data[7];
      end else if(timer595==1) begin
        dat<=dig_data[6];
      end else if(timer595==2) begin
        dat<=dig_data[5];
      end else if(timer595==3) begin
        dat<=dig_data[4];
      end else if(timer595==4) begin
        dat<=dig_data[3];
      end else if(timer595==5) begin
        dat<=dig_data[2];
      end else if(timer595==6) begin
        dat<=dig_data[1];
      end else if(timer595==7) begin
        dat<=dig_data[0];
      end else if(timer595==8) begin
        dat<=dig_select[7];
      end else if(timer595==9) begin
        dat<=dig_select[6];
      end else if(timer595==10) begin
        dat<=dig_select[5];
      end else if(timer595==11) begin
        dat<=dig_select[4];
      end else if(timer595==12) begin
        dat<=dig_select[3];
      end else if(timer595==13) begin
        dat<=dig_select[2];
      end else if(timer595==14) begin
        dat<=dig_select[1];
      end else if(timer595==15) begin
        dat<=dig_select[0];
      end
    end else begin
      dat<=0;
    end
    
    if(timer595==15) begin
      str<=1;
    end else begin
      str<=0;
    end
  end
end

reg  [7:0] dig_select;
always @ (posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    dig_select <= 1;
  end else begin
    if(timer1==10'b1111111111 && timer595==4'b1111 && timer2==3'b111) begin
      dig_select<={dig_select[6:0],dig_select[7]};
    end
  end
end

reg [3:0] num_disp;
always @ (posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    num_disp <= 4'b0;
  end else begin
    case (dig_select)
      8'd00000001 :begin
          num_disp <= data0[3:0];
      end
      8'd00000010 :begin
          num_disp <= data0[7:4];
      end
      8'd00000100 :begin
          num_disp <= data1[3:0];
      end
      8'd00001000 :begin
          num_disp <= data1[7:4];
      end
      8'd00010000 :begin
          num_disp <= data2[3:0];
      end
      8'd00100000 :begin
          num_disp <= data2[7:4];
      end
      8'd01000000 :begin
          num_disp <= data3[3:0];
      end
      8'd10000000 :begin
          num_disp <= data3[7:4];
      end
      default :;
    endcase
  end
end

reg  [7:0] dig_data;
//控制数码管段选信号，显示字符
always @ (posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    dig_data <= 0;
  end else begin
    //   0    1    2    3    4    5    6    7    8    9    A    b    C    d    E    F    
    //0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
    case (num_disp)
      //  a0
      //f0  b0
      //  g1
      //e0  c0
      //  d0  h1
      //0               hgfedcba
      0 : dig_data <= 8'b11000000; //显示数字 0 C0
      //  a1
      //f1  b0
      //  g1
      //e1  c0
      //  d1  h1
      //1               hgfedcba
      1 : dig_data <= 8'b11111001; //显示数字 1 F9
      //  a0
      //f1  b0
      //  g0
      //e0  c1
      //  d0  h1
      //2               hgfedcba
      2 : dig_data <= 8'b10100100; //显示数字 2 A4
      //  a0
      //f1  b0
      //  g0
      //e1  c0
      //  d0  h1
      //3               hgfedcba
      3 : dig_data <= 8'b10110000; //显示数字 3 B0
      //  a1
      //f0  b0
      //  g0
      //e1  c0
      //  d1  h1
      //4               hgfedcba
      4 : dig_data <= 8'b10011001; //显示数字 4 99
      //  a0
      //f0  b1
      //  g0
      //e1  c0
      //  d0  h1
      //5               hgfedcba
      5 : dig_data <= 8'b10010010; //显示数字 5 92
      //  a0
      //f0  b1
      //  g0
      //e0  c0
      //  d0  h1
      //6               hgfedcba
      6 : dig_data <= 8'b10000010; //显示数字 6 82
      //  a0
      //f1  b0
      //  g1
      //e1  c0
      //  d1  h1
      //7               hgfedcba
      7 : dig_data <= 8'b11111000; //显示数字 7 F8
      //  a0
      //f0  b0
      //  g0
      //e0  c0
      //  d0  h1
      //8               hgfedcba
      8 : dig_data <= 8'b10000000; //显示数字 8 80
      //  a0
      //f0  b0
      //  g0
      //e1  c0
      //  d0  h1
      //9               hgfedcba
      9 : dig_data <= 8'b10010000; //显示数字 9 90
      //  a0
      //f0  b0
      //  g0
      //e0  c0
      //  d1  h1
      //A               hgfedcba
      10: dig_data <= 8'b10001000; //显示数字 A 88
      //  a1
      //f0  b1
      //  g0
      //e0  c0
      //  d0  h1
      //b               hgfedcba
      11: dig_data <= 8'b10000011; //显示数字 b 83
      //  a0
      //f0  b1
      //  g1
      //e0  c1
      //  d0  h1
      //C               hgfedcba
      12: dig_data <= 8'b11000110; //显示数字 C C6
      //  a1
      //f1  b0
      //  g0
      //e0  c0
      //  d0  h1
      //d               hgfedcba
      13: dig_data <= 8'b10100001; //显示数字 d A1
      //  a0
      //f0  b1
      //  g0
      //e0  c1
      //  d0  h1
      //E               hgfedcba
      14: dig_data <= 8'b10000110; //显示数字 E 86
      //  a0
      //f0  b1
      //  g0
      //e0  c1
      //  d1  h1
      //F               hgfedcba
      15: dig_data <= 8'b10001110; //显示数字 F 8E
    endcase
  end
end

endmodule 
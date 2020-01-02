
module flow_led(
  input               sys_clk  ,  //系统时钟
  input               sys_rst_n,  //系统复位，低电平有效

  input        [3:0]  key,
  output    [3:0]  led,         //4个LED灯
  
  //uart接口
  input uart_rxd,
  output uart_txd,

  output T4,
  output T5,
  output T6,
  output T7,
  output T8,
  output T9,
  output T10,
  output T11,
  output T12,
  output T13,
  output T14,
  output M8,
  output P8,
  output L9,
  output M9,
  output P9,
  
////////////
  input P3,
  input R3,
  input R4,
  input R5,
  
  input R6,
  input R7,
  input R8,
  output R9
/*
#define  CTL1P3  21
#define  CTL2R3  20//TODO
#define  CTL3R4  26
#define  CTL4R5  19
#define  CTL5R6  25
#define  CTL6R7  22
#define  CTL7R8  27
#define  CTL8R9  17
*/
    );
    
    //assign led[3] = T11;//bit0
    //assign led[3] = T7;// bit1
    //assign led[1] = L9;// bit2
    //assign led[2] = P8;// bit3
    //assign led[3] = M8;// bit4
    //assign led[2] = T10;//bit5
    //assign led[2] = T6;// bit6
    //assign led[0] = T8;// bit7
    //assign led[1] = T9;// bit8
    //assign led[2] = T13;//bit9
    //assign led[1] = T14;//bit10
    //assign led[3] = T12;//bit11
    //assign led[1] = T5;// bit12
    //assign led[0] = T4;// bit13
    //assign led[0] = P9;// bit14
    //assign led[0] = M9;// bit15
    reg [15:0]outdata;
    assign T11 = outdata[0];
    assign T7  = outdata[1];
    assign L9  = outdata[2];
    assign P8  = outdata[3];
    assign M8  = outdata[4];
    assign T10 = outdata[5];
    assign T6  = outdata[6];
    assign T8  = outdata[7];
    assign T9  = outdata[8];
    assign T13 = outdata[9];
    assign T14 = outdata[10];
    assign T12 = outdata[11];
    assign T5  = outdata[12];
    assign T4  = outdata[13];
    assign P9  = outdata[14];
    assign M9  = outdata[15];
    
    wire req;
    assign req = R8;//CTL7R8
    reg ack;
    assign R9 = ack;//CTL8R9
    wire rst;
    assign rst = R7;//CTL6R7
   
   assign led[0] = req;
   assign led[1] = ack;


    reg [15:0] count;
    reg flg;
always @(posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    ack <= 0;
    outdata <= 0;
    count <= 0;
    flg <= 0;
  end else begin
    if(rst)begin
      ack <= 0;
      outdata <= 0;
      count <= 0;
      flg <= 0;
    end else begin
      if(req && !ack)begin
        flg<=1;
        outdata <= count;
        if(flg)begin
          count <= count+1;
          ack <= 1;
        end
      end
      if(!req && ack)begin
        ack <= 0;
        flg <= 0;
      end
    end
  end
end

endmodule 





module flow_led(
    input               sys_clk  ,  //系统时钟
    input               sys_rst_n,  //系统复位，低电平有效
    input 		[3:0]			key,
    output  reg  [3:0]  led         //4个LED灯
    );

//reg define
reg [16:0] counter1;
reg [16:0] counter2;
reg trig1;
reg trig2;

wire             locked         ;  //locked信号拉高,锁相环开始稳定输出时钟

mypll myplla(
    .areset       (~sys_rst_n     ),  //锁相环高电平复位,所以复位信号取反
    .inclk0       (sys_clk        ),
    .c0           (clk0       ),
    .c1           (clk1  ),
    .locked       (locked         )
    ); 



always @(posedge clk0 or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        counter1 <= 16'd0;
        trig1 <= trig1;
		  end
    else if (counter1 < 16'd5000)
			begin
      counter1 <= counter1 + 1'b1;
		  trig1 <= trig1;
		  end
    else
	 begin
      counter1 <= 16'd0;
		  trig1 <= !trig1;
		  end
end
always @(posedge trig1 or negedge sys_rst_n) begin
	if (!sys_rst_n)begin
		led[0] <= 1'b0;
		led[1] <= 1'b0;
		end
	else
	begin
		led[0] <= key[0];
		led[1] <= key[1];
		end
end

always @(posedge clk1 or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        counter2 <= 16'd0;
		  trig2 <= trig2;
		  end
    else if (counter2 < 16'd5000)
			begin
        counter2 <= counter2 + 1'b1;
		  trig2 <= trig2;
		  end
    else
	 begin
        counter2 <= 16'd0;
		  trig2 <= !trig2;
		  end
end
always @(posedge trig2 or negedge sys_rst_n) begin
	if (!sys_rst_n)begin
		led[2] <= 1'b0;
		led[3] <= 1'b0;
		end
	else
	begin
		led[2] <= key[2];
		led[3] <= key[3];
		end
end

       /*                                                                                                                                                                                                               
//计数器对系统时钟计数，计时0.2秒
always @(posedge sys_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        counter <= 24'd0;
		  trig <= 1'b0;
		  end
    else if (counter < 24'd5000000)
			begin
        counter <= counter + 1'b1;
		  trig <= trig;
		  end
    else
	 begin
        counter <= 24'd0;
		  trig <= !trig;
		  end
end

//通过移位寄存器控制IO口的高低电平，从而改变LED的显示状态
always @(posedge trig or negedge sys_rst_n) begin
	if (!sys_rst_n)
		led <= 4'b0;
	else
		led <= led + 1'b1;
end
*/
endmodule 


 
module flow_led(
  input sys_clk,
  input key1,
  input key2,
  output led,
 
	 //flash
	 //input  flash_data0,
	 //output flash_sdo,
	 //output flash_sce,
	 //output flash_dclk,
/*
set_location_assignment PIN_K1 -to flash_data0
set_location_assignment PIN_D1 -to flash_sdo
set_location_assignment PIN_E2 -to flash_sce
set_location_assignment PIN_K2 -to flash_dclk
set_global_assignment -name CYCLONEII_RESERVE_NCEO_AFTER_CONFIGURATION "USE AS REGULAR IO"
set_global_assignment -name RESERVE_DATA0_AFTER_CONFIGURATION "USE AS REGULAR IO"
set_global_assignment -name RESERVE_DATA1_AFTER_CONFIGURATION "USE AS REGULAR IO"
set_global_assignment -name RESERVE_FLASH_NCE_AFTER_CONFIGURATION "USE AS REGULAR IO"
set_global_assignment -name RESERVE_DCLK_AFTER_CONFIGURATION "USE AS REGULAR IO"
*/
   //uart接口
  input  uart_rxd,
  output uart_txd,
 
  output segled_clk,
  output segled_dat, 
  output segled_str,

  output [7:0] debug,


  //SDRAM 芯片接口
  output        sdram_clk_out,                //SDRAM 芯片时钟
  output        sdram_cke,                //SDRAM 时钟有效
  output        sdram_cs_n,               //SDRAM 片选
  output        sdram_ras_n,              //SDRAM 行有效
  output        sdram_cas_n,              //SDRAM 列有效
  output        sdram_we_n,               //SDRAM 写有效
  output [ 1:0] sdram_ba,                 //SDRAM Bank地址
  output [12:0] sdram_addr,               //SDRAM 行/列地址
  inout  [15:0] sdram_data,               //SDRAM 数据
  output [ 1:0] sdram_dqm,                //SDRAM 数据掩码
	
	
	input dummy
);
wire sys_rst_n;
assign sys_rst_n = key1;

wire clk;
wire reset_n;
assign clk = sys_clk;
assign reset_n = sys_rst_n;


wire [7:0] seg_data0;
wire [7:0] seg_data1;
wire [7:0] seg_data2;
wire [7:0] seg_data3;
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

	
assign seg_data0 = debug0;
assign seg_data1 = debug1;
assign seg_data2 = debug2;
assign seg_data3 = debug3;
wire [7:0] debug0;
wire [7:0] debug1;
wire [7:0] debug2;
wire [7:0] debug3;



wire uart_rec;
wire [7:0] uart_data_out;
reg uart_send;
reg [7:0] uart_data_in;
uart_hs (
  .sys_clk(clk), 
  .sys_rst_n(reset_n),

  .uart_txd(uart_txd),
  .uart_rxd(uart_rxd),
  
  .uart_rec(uart_rec),
  .uart_data_out(uart_data_out),

  .uart_send(uart_send),
  .uart_data_in(uart_data_in)
);
  
reg [7:0] command;
reg [7:0] data;
reg [7:0] command_temp;
reg data_cmd;//1:data 0:cmd
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin
    command <= 0;
    data <= 0;
  end else begin
    if (uart_rec) begin 
      if(data_cmd==0)begin
        if(uart_data_out!=0)begin
          data_cmd<=1;
          command_temp<=uart_data_out;
        end
      end else begin
        data_cmd <= 0;
        command <= command_temp;
        data <= uart_data_out;
      end

    end else begin
      if(command_done)begin
        command <= 0;
      end
    end
  end
end


reg command_done;
reg [7:0] timer;
reg [7:0] step;
reg [7:0] timer_log;

always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin

    command_done <= 0;
    timer<=0;
		step<=0;
		timer_log<=0;
		//byteenable<=4'b1111;
		
  end else begin
    uart_send<=0;
    
    if(command_done)begin
      if          (command == 8'h00) begin 
        command_done<=0;
      end
    end else begin//command_done==0
      if          (command == 8'h00) begin

      end else if (command == 8'hE0) begin test_clk<=0; command_done<=1;
      end else if (command == 8'hE1) begin test_clk<=1; command_done<=1;
      end else if (command == 8'hE2) begin reg1<=data; command_done<=1;
      end else if (command == 8'hE3) begin reg2<=data; command_done<=1;
        
      end else begin
        command_done<=1;
      end
    end


  end
end

//alu test
  assign debug[3] = of;
  assign debug[2] = sf;
  assign debug[1] = cf;
  assign debug[0] = zf;
  assign debug0 = reg1;
  assign debug1 = reg2;
  assign debug2 = reg3;
  reg test_clk;
  reg [7:0] reg1;
  reg [7:0] reg2;
  reg [7:0] reg3;
  reg of;//"overflow,"
  reg sf;//"sign neg,"
  reg cf;//"carry," : 
  reg zf;//"zero," : "
  reg [8:0] tmp1;
  reg [8:0] tmp2;
  reg [8:0] tmp3;
  
always @(posedge test_clk or negedge reset_n) begin
  if (!reset_n) begin
    reg3 <= 0;
  end else begin
    tmp1 = {1'b0,reg1};
    tmp2 = {1'b0,reg2};
    tmp3 = tmp1-tmp2;
    reg3 <= tmp3[7:0];
    zf <= tmp3[7:0]==0;
    cf <= tmp3[8];
  end
end
//alu test


endmodule



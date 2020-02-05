module cy68013_mcu(
    input  sys_clk  ,
    input  sys_rst_n,
    
    inout [7:0] cy_D,
    inout [7:0] cy_B,
    input cy_SCL,
    input cy_SDA,
    input cy_IFCLK                     ,
    input cy_to_fpga_CTL0_FLAGA        ,
    input cy_to_fpga_CTL2_FLAGC        ,
    input cy_to_fpga_CTL1_FLAGB        ,
    input cy_to_fpga_A7_FLAGD          ,
    output reg cy_from_fpga_RDY1_SLWR       ,//output
    output reg cy_from_fpga_RDY0_SLRD       ,//output
    output reg cy_from_fpga_A2_SLOE         ,//output
    input cy_A0_INT0                   ,// in from pc
    output reg cy_A1_INT1                   ,// out to pc
    input cy_A3_WU2                    ,
    output reg cy_from_fpga_A4_FIFOADR0     ,//output
    output reg cy_from_fpga_A5_FIFOADR1     ,//output
    output reg cy_from_fpga_A6_PKTEND       ,//output

  output reg [7:0] cy_cmd,
  output reg [7:0] cy_dat,
  output reg [7:0] cy_snd_data0,
  output reg [7:0] cy_snd_data1,
  
  output debug0,
  output debug1,
  output debug2,
  output debug3,
  output debug4,
  output debug5,
  output debug6,
  output debug7,
    output [15:0] debug8,
    output [15:0] debug9,
    
    
    //SDRAM 芯片接口
    output        sdram_clk_out,            //SDRAM 芯片时钟
    output        sdram_cke,                //SDRAM 时钟有效
    output        sdram_cs_n,               //SDRAM 片选
    output        sdram_ras_n,              //SDRAM 行有效
    output        sdram_cas_n,              //SDRAM 列有效
    output        sdram_we_n,               //SDRAM 写有效
    output [ 1:0] sdram_ba,                 //SDRAM Bank地址
    output [12:0] sdram_addr,               //SDRAM 行/列地址
    inout  [15:0] sdram_data,               //SDRAM 数据
    output [ 1:0] sdram_dqm,                //SDRAM 数据掩码

    //SDRAM2m 芯片接口
    output        sdram2m_clk_out,            //SDRAM 芯片时钟
    output        sdram2m_cke,                //SDRAM 时钟有效
    output        sdram2m_cs_n,               //SDRAM 片选
    output        sdram2m_ras_n,              //SDRAM 行有效
    output        sdram2m_cas_n,              //SDRAM 列有效
    output        sdram2m_we_n,               //SDRAM 写有效
    output        sdram2m_ba,                 //SDRAM Bank地址
    output [10:0] sdram2m_addr,               //SDRAM 行/列地址
    inout  [15:0] sdram2m_data,               //SDRAM 数据

      //VGA接口                          
    output vga_hs,       //行同步信号
    output vga_vs,       //场同步信号
    output [15:0] vga_rgb,      //红绿蓝三原色输出

    output blanking,
    
    output busy

);

assign busy = command != 0 && command_done==0;
    
assign cy_D = cy_out_to_pc ? cy_D_out : 8'hzz;
assign cy_B = cy_out_to_pc ? cy_B_out : 8'hzz;
reg [7:0] cy_D_out;
reg [7:0] cy_B_out;

reg cy_out_to_pc;


reg cy_rec_req;
reg cy_snd_ack;
reg [4:0]cy_rec_cnt;
reg [4:0]cy_snd_cnt;
always @(posedge cy_A3_WU2 or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    cy_cmd<=0;
    cy_dat<=0;
    cy_rec_cnt<=0;
    cy_rec_req<=0;
    cy_snd_ack<=0;
    cy_A1_INT1<=0;
  end else begin
    if         (cy_rec_cnt==0)begin
      if(cy_A0_INT0 && !cy_rec_req)begin
        cy_rec_cnt<=1;
      end
    end else if(cy_rec_cnt==1 )begin cy_cmd[0] <= cy_A0_INT0;cy_rec_cnt<=2;
    end else if(cy_rec_cnt==2 )begin cy_cmd[1] <= cy_A0_INT0;cy_rec_cnt<=3;
    end else if(cy_rec_cnt==3 )begin cy_cmd[2] <= cy_A0_INT0;cy_rec_cnt<=4;
    end else if(cy_rec_cnt==4 )begin cy_cmd[3] <= cy_A0_INT0;cy_rec_cnt<=5;
    end else if(cy_rec_cnt==5 )begin cy_cmd[4] <= cy_A0_INT0;cy_rec_cnt<=6;
    end else if(cy_rec_cnt==6 )begin cy_cmd[5] <= cy_A0_INT0;cy_rec_cnt<=7;
    end else if(cy_rec_cnt==7 )begin cy_cmd[6] <= cy_A0_INT0;cy_rec_cnt<=8;
    end else if(cy_rec_cnt==8 )begin cy_cmd[7] <= cy_A0_INT0;cy_rec_cnt<=9;

    end else if(cy_rec_cnt==9 )begin cy_dat[0] <= cy_A0_INT0;cy_rec_cnt<=10;
    end else if(cy_rec_cnt==10)begin cy_dat[1] <= cy_A0_INT0;cy_rec_cnt<=11;
    end else if(cy_rec_cnt==11)begin cy_dat[2] <= cy_A0_INT0;cy_rec_cnt<=12;
    end else if(cy_rec_cnt==12)begin cy_dat[3] <= cy_A0_INT0;cy_rec_cnt<=13;
    end else if(cy_rec_cnt==13)begin cy_dat[4] <= cy_A0_INT0;cy_rec_cnt<=14;
    end else if(cy_rec_cnt==14)begin cy_dat[5] <= cy_A0_INT0;cy_rec_cnt<=15;
    end else if(cy_rec_cnt==15)begin cy_dat[6] <= cy_A0_INT0;cy_rec_cnt<=16;
    end else if(cy_rec_cnt==16)begin cy_dat[7] <= cy_A0_INT0;cy_rec_cnt<=0;cy_rec_req<=1;
    end

    if(cy_snd_req && !cy_snd_ack)begin
      if         (cy_snd_cnt==0 )begin cy_snd_cnt<=1 ;cy_A1_INT1 <= 1;
      
      end else if(cy_snd_cnt==1 )begin cy_snd_cnt<=2 ;cy_A1_INT1 <= cy_snd_data0[0];
      end else if(cy_snd_cnt==2 )begin cy_snd_cnt<=3 ;cy_A1_INT1 <= cy_snd_data0[1];
      end else if(cy_snd_cnt==3 )begin cy_snd_cnt<=4 ;cy_A1_INT1 <= cy_snd_data0[2];
      end else if(cy_snd_cnt==4 )begin cy_snd_cnt<=5 ;cy_A1_INT1 <= cy_snd_data0[3];
      end else if(cy_snd_cnt==5 )begin cy_snd_cnt<=6 ;cy_A1_INT1 <= cy_snd_data0[4];
      end else if(cy_snd_cnt==6 )begin cy_snd_cnt<=7 ;cy_A1_INT1 <= cy_snd_data0[5];
      end else if(cy_snd_cnt==7 )begin cy_snd_cnt<=8 ;cy_A1_INT1 <= cy_snd_data0[6];
      end else if(cy_snd_cnt==8 )begin cy_snd_cnt<=9 ;cy_A1_INT1 <= cy_snd_data0[7];

      end else if(cy_snd_cnt==9 )begin cy_snd_cnt<=10;cy_A1_INT1 <= cy_snd_data1[0];
      end else if(cy_snd_cnt==10)begin cy_snd_cnt<=11;cy_A1_INT1 <= cy_snd_data1[1];
      end else if(cy_snd_cnt==11)begin cy_snd_cnt<=12;cy_A1_INT1 <= cy_snd_data1[2];
      end else if(cy_snd_cnt==12)begin cy_snd_cnt<=13;cy_A1_INT1 <= cy_snd_data1[3];
      end else if(cy_snd_cnt==13)begin cy_snd_cnt<=14;cy_A1_INT1 <= cy_snd_data1[4];
      end else if(cy_snd_cnt==14)begin cy_snd_cnt<=15;cy_A1_INT1 <= cy_snd_data1[5];
      end else if(cy_snd_cnt==15)begin cy_snd_cnt<=16;cy_A1_INT1 <= cy_snd_data1[6];
      end else if(cy_snd_cnt==16)begin cy_snd_cnt<=17;cy_A1_INT1 <= cy_snd_data1[7];
      end else if(cy_snd_cnt==17)begin cy_snd_cnt<=18;cy_A1_INT1 <= 0;cy_snd_ack<=1;
      end
    end else begin
      cy_snd_cnt<=0;
      cy_snd_ack<=0;
    end
    
    if(cy_rec_req && cy_rec_ack)begin
      cy_rec_req<=0;
    end

  end
end


//cy_from_fpga_RDY0_SLRD
//cy_to_fpga_CTL0_FLAGA        ,
//cy_to_fpga_CTL2_FLAGC        ,
//cy_to_fpga_CTL1_FLAGB        ,
//cy_to_fpga_A7_FLAGD          ,
// FLAGA:8 1 0 0 0 EP2 EF
// FLAGB:C 1 1 0 0 EP2 FF
// FLAGC:A 1 0 1 0 EP6 EF
// FLAGD:E 1 1 1 0 EP6 FF
//assign buffok = cy_to_fpga_CTL0_FLAGA == 0 true &&
// cy_to_fpga_CTL1_FLAGB == 1 false &&
// cy_to_fpga_CTL2_FLAGC == 0 true &&
// cy_to_fpga_A7_FLAGD   == 1 false;

wire EP2EF;
wire EP2FF;
wire EP6EF;
wire EP6FF;
assign EP2EMPTY = !cy_to_fpga_CTL0_FLAGA;
assign EP2FULL  = !cy_to_fpga_CTL1_FLAGB;
assign EP6EMPTY = !cy_to_fpga_CTL2_FLAGC;
assign EP6FULL  = !cy_to_fpga_A7_FLAGD;

assign debug4 = EP2EMPTY;
assign debug5 = EP2FULL;
assign debug6 = EP6EMPTY;
assign debug7 = EP6FULL;

assign debug0 = transfer_req_buff;
assign debug1 = transfer_ack;
assign debug2 = transfer_req;
assign debug3 = cy_IFCLK;

reg transfer_req_buff;
reg transfer_ack;
reg [24:0] transfer_timer;
reg [15:0] sdram_c_data_in_trans;

reg [15:0] data_accu ;
reg [15:0] data_count;
assign debug8 = data_accu;
assign debug9 = data_count;
reg init;

always @(negedge cy_IFCLK or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    transfer_req_buff <= 0;
    sdram_c_write_en <= 0;//传输专用
    sdram_c_data_in_trans <= 0;//传输专用

    cy_from_fpga_RDY0_SLRD<=1;
    cy_from_fpga_RDY1_SLWR<=1;
    cy_from_fpga_A4_FIFOADR0<=0;
    cy_from_fpga_A5_FIFOADR1<=0;
    cy_from_fpga_A6_PKTEND<=1;

    data_accu <= 0;
    data_count <= 0;
		init <= 0;
		sdram_c_write_en <= 0;
  end else begin
    transfer_req_buff <= transfer_req;
    
    if(transfer_req_buff && !transfer_ack)begin
			init <= 1;
			if(!init)begin
				data_count<=0;
				data_accu<=0;
				transfer_timer <= 0;
			end else begin
				if(transfer_type==1)begin
				
					if(transfer_timer == 768*2*512)begin
						cy_from_fpga_RDY0_SLRD <= 1;//high deActive
						transfer_timer <= 0;
						init <= 0;
						sdram_c_write_en <= 0;
						transfer_ack <= 1;
					end else begin
						if(!EP2EMPTY)begin
							transfer_timer <= transfer_timer + 1'b1;
							data_count <= data_count + 1'b1;
							data_accu <= data_accu + {cy_D,cy_B};
							sdram_c_data_in_trans <= {cy_D,cy_B};
							sdram_c_write_en <= 1;
							cy_from_fpga_RDY0_SLRD <= 0;//low active
						end else begin
							sdram_c_write_en <= 0;
							cy_from_fpga_RDY0_SLRD <= 1;//high deActive
						end
					
					end
				end else begin
				
					transfer_ack <= 1;
				end
			end
    end
    
    
    if(!transfer_req_buff && transfer_ack)begin
      transfer_ack <= 0;
    end

  end


end



reg [7:0] cy_address0;
reg [7:0] cy_address1;
reg [7:0] cy_address2;
reg [7:0] cy_data0;
reg [7:0] cy_data1;

reg cy_rec_ack;
reg [7:0] command;
reg [7:0] data;
reg cy_rec_req_buff;

always @(posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    command <= 0;
    data <= 0;
    cy_rec_ack<=0;
  end else begin
    cy_rec_req_buff<=cy_rec_req;

    if(!cy_rec_req_buff && cy_rec_ack)begin
      cy_rec_ack <= 0;
    end
    if(cy_cmd==8'h62)begin
		end
		
    if (cy_rec_req_buff && !cy_rec_ack) begin //cy数据到达
      command <= cy_cmd;
      cy_rec_ack <= 1;
    end
    if(command_done)begin
      command <= 0;
    end
  end
end

reg vga_mode;

reg command_done;
reg [10:0] timer;
reg [7:0] reg_temp;
reg [9:0] timer2;
reg [23:0] read_address;
reg hibit;

reg [15:0] timer3;

reg [7:0] sum;
reg [15:0] sum16;

reg cy_snd_req;

reg [16:0] temp_val;

reg [10:0] timer11;
reg [20:0] timer12;

reg start;
reg inited;
reg blanking_buff;
reg blanking_last;

reg [31:0] counttotal;

reg transfer_req;
reg transfer_type;
reg control_by_transfer;

reg [15:0] sdram_c_data_in_norm;

always @(posedge sys_clk or negedge sys_rst_n) begin
  if (!sys_rst_n) begin
    cy_address0 <= 0;
    cy_address1 <= 0;
    cy_address2 <= 0;
    cy_data0 <= 0;
    cy_data1 <= 0;

    command_done <= 0;
    timer<=0;
    timer2<=0;
    timer3<=0;
    sdram_c_data_in_norm <= 0;
    sdram_c_read_req <= 0;
    sdram_c_write_req <= 0;
    sum <=0;
    sum16 <=0;
    sdram_c_buffDMAread_req <= 0;
    sdram2m_buffDMAwrite_req <= 0;
    
    cy_out_to_pc<=0;
  
    cy_snd_req<=0;
    cy_snd_data0 <= 0;
    cy_snd_data1 <= 0;

    blockvga<=1;
    vga_mode<=1;
    start<=0;
    inited<=0;
    counttotal<=0;
		
		transfer_req<= 0;
		
  end else begin
    blanking_buff <= blanking;
    blanking_last <= blanking_buff;
    
    if(cy_snd_ack)begin
      cy_snd_req <= 0;
    end
    
    if(command_done)begin
      if          (command == 8'h00) begin 
        command_done<=0;
      end
    end else begin//command_done==0
      if          (command == 8'h00) begin

      end else if (command == 8'h10) begin cy_data0<=cy_dat; command_done<=1;
      end else if (command == 8'h11) begin cy_data1<=cy_dat; command_done<=1;
      end else if (command == 8'h12) begin cy_address0<=cy_dat; command_done<=1;
      end else if (command == 8'h13) begin cy_address1<=cy_dat; command_done<=1;
      end else if (command == 8'h14) begin cy_address2<=cy_dat; command_done<=1;

      end else if (command == 8'h21) begin//set mode 640*480
        vga_mode<=0;
        command_done<=1;
          
      end else if (command == 8'h22) begin//set mode 1024*768
        vga_mode<=1;
        command_done<=1;
        
      end else if (command == 8'h23) begin//vga off
        blockvga<=1;
        command_done<=1;
          
      end else if (command == 8'h24) begin//vga on
        blockvga<=0;
        command_done<=1;


      end else if (command == 8'h60) begin//sdram read
				control_by_transfer <= 1;
				command_done <= 1;
      end else if (command == 8'h61) begin//sdram read
				control_by_transfer <= 0;
				command_done <= 1;
      end else if (command == 8'h62) begin//sdram read
				inited <= 1;
				if(!inited)begin
					transfer_req <= 1;
					transfer_type <= 1;
					cy_from_fpga_A2_SLOE<=0;
					sdram2m_c_address <= {cy_address2,cy_address1,cy_address0};
					sdram_c_write_latch_address <= 1;
				end else begin
					//内存地址初始化
					sdram_c_write_latch_address <= 0;
					if(transfer_ack)begin
						cy_from_fpga_A2_SLOE<=1;
						transfer_req <= 0;
						cy_snd_req <= 1;
						cy_snd_data0 <= 8'h12;
						cy_snd_data1 <= 8'h34;
						command_done <= 1;
					end
				end


				
      end else if (command == 8'hA0) begin//sdram write
        timer<=timer+1'b1;
        if(timer==0)begin
          sdram_c_address <= {cy_address2,cy_address1,cy_address0};
          sdram_c_data_in_norm <= {cy_data1,cy_data0};
          sdram_c_write_req<=1;
        end else begin
          if(sdram_c_write_ack)begin
            timer<=0;
            sdram_c_write_req<=0;
            command_done<=1;
          end
        end

      end else if (command == 8'hA1) begin//sdram read
        timer<=timer+1'b1;
        if(timer==0)begin
          sdram_c_address <= {cy_address2,cy_address1,cy_address0};
          sdram_c_read_req<=1;
        end else begin
          if(sdram_c_read_ack)begin
            cy_snd_req <= 1;
            cy_snd_data0 <= sdram_c_data_out[7:0];
            cy_snd_data1 <= sdram_c_data_out[15:8];
            timer <= 0;
            sdram_c_read_req<=0;
            command_done <= 1;
          end
        end

//      end else if (command == 8'hA2) begin//sdram long write ok
//        timer3 <= timer3 + 1'b1;
//        sdram_c_write_latch_address<=0;
//
//        if         (timer3==0)begin                                  //step0
//          cy_from_fpga_A2_SLOE<=0;//on
//          cy_from_fpga_RDY0_SLRD<=0;//on
//          sdram_c_address <= {cy_address2,cy_address1,cy_address0};
//        end else if(timer3==(512 * 4 + 2))begin                                  //step5  n字*4+2
//          cy_from_fpga_A2_SLOE<=1;//off
//          cy_from_fpga_RDY0_SLRD<=1;//off
//          timer3<=0;
//          command_done<=1;
//          cy_snd_req <= 1;
//          cy_snd_data0 <= 8'h12;
//          cy_snd_data1 <= 8'h34;
//        end else begin
//          if         (timer3[1:0]==1)begin                                  //step1
//            cy_from_fpga_RDY0_SLRD<=0;//on
//          end else if(timer3[1:0]==2)begin                                  //step2
//            cy_from_fpga_RDY0_SLRD<=1;//off
//            //读取 并写入sdram
//            sdram_c_write_en<=1;
//            if(timer3==2)begin sdram_c_write_latch_address<=1; end
//            sdram_c_data_in<={cy_D,cy_B};
//          end else if(timer3[1:0]==3)begin                                  //step3
//            cy_from_fpga_RDY0_SLRD<=1;//off
//            sdram_c_write_en<=0;
//          end else begin                                      //step4
//            cy_from_fpga_RDY0_SLRD<=0;//on
//            sdram_c_write_en<=0;
//          end
//        end
        

//      end else if (command == 8'hA3) begin//sdram long read
//        timer2<=timer2+1'b1;
//        
//        if(timer2==0)begin
//          if(timer==0)begin//锁存地址
//            sdram_c_address <= {cy_address2,cy_address1,cy_address0};
//            sum16 <= 0;
//            cy_from_fpga_A4_FIFOADR0<=0;//set channel
//            cy_from_fpga_A5_FIFOADR1<=1;
//            cy_out_to_pc<=1;//set out
//          end else begin
//            sdram_c_address <= sdram_c_address + 1'b1;
//            
//          end
//          sdram_c_read_req<=1;
//        end else if(timer2==30) begin//sdram 响应
//          if(!sdram_c_read_ack)begin
//            //err
//          end
//          timer<=timer+1'b1;
//          sdram_c_read_req<=0;
//          sum16 <= sum16 + sdram_c_data_out;
//          cy_D_out = sdram_c_data_out[15:8];
//          cy_B_out = sdram_c_data_out[7:0];
//          cy_from_fpga_RDY1_SLWR<=0;//set wr
// 
//        end else if(timer2==60)begin
//          cy_from_fpga_RDY1_SLWR<=1;//set wr
//          if(timer==513)begin
//            cy_from_fpga_A4_FIFOADR0<=0;//set channel
//            cy_from_fpga_A5_FIFOADR1<=0;
//            cy_out_to_pc<=0;//set out
//            timer<=0;
//            command_done<=1;
//            cy_snd_req <= 1;
//            cy_snd_data1 <= sum16[15:8];
//            cy_snd_data0 <= sum16[7:0];
//          end
//          timer2<=0;
//        end
        
      end else if (command == 8'hB0) begin//sdram2m write
        timer<=timer+1'b1;
        if(timer==0)begin
          sdram2m_c_address <= {cy_address2,cy_address1,cy_address0};
          sdram2m_c_data_in <= {cy_data1,cy_data0};
          sdram2m_c_write_req<=1;
        end else begin
          if(sdram2m_c_write_ack)begin
            timer<=0;
            sdram2m_c_write_req<=0;
            command_done<=1;
          end
        end

      end else if (command == 8'hB1) begin//sdram2m read
        timer<=timer+1'b1;
        if(timer==0)begin
          sdram2m_c_address <= {cy_address2,cy_address1,cy_address0};
          sdram2m_c_read_req<=1;
        end else begin
          if(sdram2m_c_read_ack)begin
            cy_snd_req <= 1;
            cy_snd_data0 <= sdram2m_c_data_out[7:0];
            cy_snd_data1 <= sdram2m_c_data_out[15:8];
            timer <= 0;
            sdram2m_c_read_req<=0;
            command_done <= 1;
          end
        end
      

//      end else if (command == 8'hB2) begin//sdram2m long write ok
//        timer3 <= timer3 + 1'b1;
//        sdram2m_c_write_latch_address<=0;
//        if         (timer3==0)begin                                  //step0
//          cy_from_fpga_A2_SLOE<=0;//on
//          cy_from_fpga_RDY0_SLRD<=0;//on
//          sdram2m_c_address <= {cy_address2,cy_address1,cy_address0};
//        end else if(timer3==(512 * 4 + 2))begin                                  //step5  n字*4+2
//          cy_from_fpga_A2_SLOE<=1;//off
//          cy_from_fpga_RDY0_SLRD<=1;//off
//          timer3<=0;
//          command_done<=1;
//          cy_snd_req <= 1;
//          cy_snd_data0 <= 8'h12;
//          cy_snd_data1 <= 8'h34;
//        end else begin
//          if         (timer3[1:0]==1)begin                                  //step1
//            cy_from_fpga_RDY0_SLRD<=0;//on
//          end else if(timer3[1:0]==2)begin                                  //step2
//            cy_from_fpga_RDY0_SLRD<=1;//off
//            //读取 并写入sdram2m
//            sdram2m_c_write_en<=1;
//            if(timer3==2)begin sdram2m_c_write_latch_address<=1; end
//            sdram2m_c_data_in<={cy_D,cy_B};
//          end else if(timer3[1:0]==3)begin                                  //step3
//            cy_from_fpga_RDY0_SLRD<=1;//off
//            sdram2m_c_write_en<=0;
//          end else begin                                      //step4
//            cy_from_fpga_RDY0_SLRD<=0;//on
//            sdram2m_c_write_en<=0;
//          end
//        end

//      end else if (command == 8'hB3) begin//sdram2m long read
//        timer2<=timer2+1'b1;
//        if(timer2==0)begin
//          if(timer==0)begin//锁存地址
//            sdram2m_c_address <= {cy_address2,cy_address1,cy_address0};
//            cy_from_fpga_A4_FIFOADR0<=0;//set channel
//            cy_from_fpga_A5_FIFOADR1<=1;
//            cy_out_to_pc<=1;//set out
//          end else begin
//            sdram2m_c_address <= sdram2m_c_address + 1'b1;
//            
//          end
//          sdram2m_c_read_req<=1;
//        end else if(timer2==30) begin//sdram2m 响应
//          if(!sdram2m_c_read_ack)begin
//            //err
//          end
//          timer<=timer+1'b1;
//          sdram2m_c_read_req<=0;
//          cy_D_out = sdram2m_c_data_out[15:8];
//          cy_B_out = sdram2m_c_data_out[7:0];
//          cy_from_fpga_RDY1_SLWR<=0;//set wr
//        end else if(timer2==60)begin
//          cy_from_fpga_RDY1_SLWR<=1;//set wr
//          if(timer==513)begin
//            cy_from_fpga_A4_FIFOADR0<=0;//set channel
//            cy_from_fpga_A5_FIFOADR1<=0;
//            cy_out_to_pc<=0;//set out
//            timer<=0;
//            cy_snd_req <= 1;
//            cy_snd_data1 <= 8'h12;
//            cy_snd_data0 <= 8'h34;
//            command_done<=1;
//          end
//          timer2<=0;
//        end

//      end else if (command == 8'hF1) begin//srambuff long read
//        sdram2m_read_buff_addr <= {cy_address2,cy_address1,cy_address0};
//        sdram2m_read_buffA_req <= 1;
//        command_done<=1;
//      end else if (command == 8'hF2) begin//srambuff long read
//        sdram2m_read_buffA_req <= 0;
//        command_done<=1;
//      end else if (command == 8'hF3) begin//srambuff long read
//        sdram2m_buff_buff_readA_addr <= {cy_address2,cy_address1,cy_address0};
//        command_done<=1;
//
//      end else if (command == 8'hF4) begin//srambuff long read
//        cy_snd_req <= 1;
//        cy_snd_data1 <= sdram2m_buff_buff_readA_data[15:8];
//        cy_snd_data0 <= sdram2m_buff_buff_readA_data[7:0];
//        command_done<=1;

//      end else if (command == 8'hE1) begin//sram buff address write 1word
//        timer<=timer+1'b1;
//        if(timer==0)begin
//          sdram2m_buffDMAWriteA_addr <= cy_dat;
//          sdram2m_buffDMAWriteA_data <= {cy_data1,cy_data0};
//          sdram2m_buffDMAWriteA_en<=1;
//        end else begin
//          timer<=0;
//          sdram2m_buffDMAWriteA_en<=0;
//          command_done<=1;
//        end

//      end else if (command == 8'hE2) begin//sram buff buff write
//        timer<=timer+1'b1;
//        if(timer==0)begin
//          sdram2m_buffDMAwrite_addr <= {cy_address2,cy_address1,cy_address0};
//          sdram2m_buffDMAwrite_req<=1;
//          sdram2m_buffDMAwrite_A_B<=1;
//        end else begin
//          if(sdram2m_buffDMAwrite_ack)begin
//            cy_snd_req <= 1;
//            cy_snd_data0 <= sdram2m_c_data_out[7:0];
//            cy_snd_data1 <= sdram2m_c_data_out[15:8];
//            timer <= 0;
//            sdram2m_buffDMAwrite_req<=0;
//            command_done <= 1;
//          end
//        end


//      end else if (command == 8'hBE) begin//B2  sdram2m long write ok
//        if(!blanking_last && blanking_buff)begin
//          start <= 1;
//        end
//        if(start)begin
//          timer3 <= timer3 + 1'b1;
//          sdram2m_c_write_latch_address<=0;
//          if         (timer3==0)begin                                  //step0
//            cy_from_fpga_A2_SLOE<=0;//on
//            cy_from_fpga_RDY0_SLRD<=0;//on
//            sdram2m_c_address <= {cy_address2,cy_address1,cy_address0};
//          end else if(timer3==(512 * 4 + 2))begin                                  //step5  n字*4+2
//            cy_from_fpga_A2_SLOE<=1;//off
//            cy_from_fpga_RDY0_SLRD<=1;//off
//            timer3<=0;
//            start<=0;
//            command_done<=1;
//            cy_snd_req <= 1;
//            cy_snd_data0 <= 8'h12;
//            cy_snd_data1 <= 8'h34;
//          end else begin
//            if         (timer3[1:0]==1)begin                                  //step1
//              cy_from_fpga_RDY0_SLRD<=0;//on
//            end else if(timer3[1:0]==2)begin                                  //step2
//              cy_from_fpga_RDY0_SLRD<=1;//off
//              //读取 并写入sdram2m
//              sdram2m_c_write_en<=1;
//              if(timer3==2)begin sdram2m_c_write_latch_address<=1; end
//              sdram2m_c_data_in<={cy_D,cy_B};
//            end else if(timer3[1:0]==3)begin                                  //step3
//              cy_from_fpga_RDY0_SLRD<=1;//off
//              sdram2m_c_write_en<=0;
//            end else begin                                      //step4
//              cy_from_fpga_RDY0_SLRD<=0;//on
//              sdram2m_c_write_en<=0;
//            end
//          end
//        end

      end else if (command == 8'hBF) begin//memcopy
        counttotal<=counttotal+1'b1;
        //先A2指令写入32M
        //先关vga, 等7个周期  等sdram2m空闲后,开始copy ,打开vga  参考hE2
        blockvga<=1;
        if(!start)begin
          if(timer11==300)begin
            if(!sdram2m_busy)begin
              start <= 1;
              sdram_c_buffDMAread_addr <= 0;
              sdram_c_buffDMAread_A_B <= 0;
              sdram2m_buffDMAwrite_addr <= 12'b1111_1111_1111;
              sdram2m_buffDMAwrite_A_B <= 1;
              timer12 <= 0;
            end
          end else begin
            timer11<=timer11+1'b1;
          end
        end
          
        if(start)begin
          inited <= 1;
          if(!inited)begin
            sdram_c_buffDMAread_req <= 1;
            sdram2m_buffDMAwrite_req <= 1;
          end else begin
            if(sdram_c_buffDMAread_req && sdram2m_buffDMAwrite_req && sdram_c_buffDMAread_ack && sdram2m_buffDMAwrite_ack)begin
              sdram_c_buffDMAread_req <= 0;
              sdram2m_buffDMAwrite_req <= 0;
              sdram_c_buffDMAread_addr <= sdram_c_buffDMAread_addr + 1'b1;
              sdram_c_buffDMAread_A_B <= !sdram_c_buffDMAread_A_B;
              sdram2m_buffDMAwrite_addr <= sdram2m_buffDMAwrite_addr + 1'b1;
              sdram2m_buffDMAwrite_A_B <= !sdram2m_buffDMAwrite_A_B;
              timer12<=timer12+1;
              if(timer12==12'b1111_1111_1111)begin
                blockvga <= 0;
                inited <= 0;
                start <= 0;
                counttotal<=0;
                timer11<=0;
                command_done <= 1;
                cy_snd_req <= 1;
                cy_snd_data0 <= 8'h12;
                cy_snd_data1 <= 8'h34;
              end
            end
            if(!sdram_c_buffDMAread_req && !sdram2m_buffDMAwrite_req && !sdram_c_buffDMAread_ack && !sdram2m_buffDMAwrite_ack)begin
              inited <= 0;
            end
          end
        end
     


		 
        
      end
    end
  end
end

//assign debug2 = sdram2m_busy;
//assign debug3 = start;
//assign debug4 = sdram_c_buffDMAread_req ;
//assign debug5 = sdram2m_buffDMAwrite_req;
//assign debug6 = sdram_c_buffDMAread_ack;
//assign debug7 = sdram2m_buffDMAwrite_ack;

wire        sdram_clk;
reg  [23:0] sdram_c_address;
wire [15:0] sdram_c_data_in;
wire [15:0] sdram_c_data_out;
reg         sdram_c_read_req;
wire        sdram_c_read_ack;
reg         sdram_c_write_req;
wire        sdram_c_write_ack;
reg         sdram_c_write_en;
reg         sdram_c_write_latch_address;


reg         sdram_c_buffDMAread_req;  // input             
wire        sdram_c_buffDMAread_ack;  // output reg        
reg  [15:0] sdram_c_buffDMAread_addr;  // input      [15:0] 
reg         sdram_c_buffDMAread_A_B;  // input             

wire        sdram_c_buffDMAread_clk;  // output            
wire [15:0] sdram_c_buffDMAread_wrdata;  // output reg [15:0] 
wire  [7:0] sdram_c_buffDMAread_wraddress;  // output reg  [7:0] 
wire        sdram_c_buffDMAreadA_wren;  // output reg        
wire        sdram_c_buffDMAreadB_wren;  // output reg        

//直连
assign sdram2m_buffDMAWrite_clk = sdram_c_buffDMAread_clk;
assign sdram2m_buffDMAWriteA_data = sdram_c_buffDMAread_wrdata;
assign sdram2m_buffDMAWriteA_addr = sdram_c_buffDMAread_wraddress;
assign sdram2m_buffDMAWriteB_data = sdram_c_buffDMAread_wrdata;
assign sdram2m_buffDMAWriteB_addr = sdram_c_buffDMAread_wraddress;
assign sdram2m_buffDMAWriteA_en = sdram_c_buffDMAreadA_wren;
assign sdram2m_buffDMAWriteB_en = sdram_c_buffDMAreadB_wren;

assign sdram_clk = control_by_transfer ? cy_IFCLK : sys_clk;
assign sdram_c_data_in = control_by_transfer ? sdram_c_data_in_trans : sdram_c_data_in_norm;

sdram ins_sdram(
  .sys_clk    (sys_clk  ),       // 时钟信号
  .sys_rst_n  (sys_rst_n),       // 复位信号

  //SDRAM 芯片接口
  .sdram_clk_out     (sdram_clk_out),
  .sdram_cke      (sdram_cke),    //SDRAM 时钟有效
  .sdram_cs_n      (sdram_cs_n),    //SDRAM 片选
  .sdram_ras_n    (sdram_ras_n),    //SDRAM 行有效  
  .sdram_cas_n    (sdram_cas_n),    //SDRAM 列有效
  .sdram_we_n      (sdram_we_n),    //SDRAM 写有效
  .sdram_ba        (sdram_ba),      //SDRAM Bank地址
  .sdram_addr      (sdram_addr),    //SDRAM 行/列地址
  .sdram_data      (sdram_data),    //SDRAM 数据  
  .sdram_dqm    (sdram_dqm),

  .clk        (sdram_clk),//in
  .address    (sdram_c_address),//in
  .data_in    (sdram_c_data_in),//in
  .data_out   (sdram_c_data_out),//out
  .read_req   (sdram_c_read_req),//in
  .read_ack   (sdram_c_read_ack),//out
  .write_req  (sdram_c_write_req),//in
  .write_ack  (sdram_c_write_ack),//out
  .write_en   (sdram_c_write_en),//in
  .write_latch_address(sdram_c_write_latch_address),//in
  
  .buffDMAread_req        (sdram_c_buffDMAread_req        ),  // input             
  .buffDMAread_ack        (sdram_c_buffDMAread_ack        ),  // output reg        
  .buffDMAread_addr       (sdram_c_buffDMAread_addr       ),  // input      [15:0] 
  .buffDMAread_A_B        (sdram_c_buffDMAread_A_B        ),  // input             

  .buffDMAread_clk        (sdram_c_buffDMAread_clk        ),  // output            
  .buffDMAread_wrdata     (sdram_c_buffDMAread_wrdata     ),  // output reg [15:0] 
  .buffDMAread_wraddress  (sdram_c_buffDMAread_wraddress  ),  // output reg  [7:0] 
  .buffDMAreadA_wren      (sdram_c_buffDMAreadA_wren      ),  // output reg        
  .buffDMAreadB_wren      (sdram_c_buffDMAreadB_wren      )  // output reg        
  
  
);


reg  [19:0] sdram2m_c_address;
reg  [15:0] sdram2m_c_data_in;
wire [15:0] sdram2m_c_data_out;
reg  sdram2m_c_read_req;
wire  sdram2m_c_read_ack;
reg  sdram2m_c_write_req;
wire  sdram2m_c_write_ack;
reg sdram2m_c_write_en;
reg sdram2m_c_write_latch_address;


reg         sdram2m_buffDMAwrite_req  ;             //input        
reg [11:0]  sdram2m_buffDMAwrite_addr ;            //input [11:0] 
reg         sdram2m_buffDMAwrite_A_B  ;             //input        
wire        sdram2m_buffDMAwrite_ack  ;             //output reg   

wire        sdram2m_buffDMAWrite_clk  ;         //input        
wire [15:0] sdram2m_buffDMAWriteA_data;         //input [15:0] 
wire  [9:0] sdram2m_buffDMAWriteA_addr;         //input  [9:0] 
wire        sdram2m_buffDMAWriteA_en  ;         //input        
wire [15:0] sdram2m_buffDMAWriteB_data;         //input [15:0] 
wire  [9:0] sdram2m_buffDMAWriteB_addr;         //input  [9:0] 
wire        sdram2m_buffDMAWriteB_en  ;         //input        


wire         sdram2m_read_buffA_req;//input read_buffA_req,
wire         sdram2m_read_buffB_req;//input read_buffB_req,
wire   [9:0] sdram2m_read_buff_addr;//input [9:0] read_buff_addr,
wire  [15:0] sdram2m_buff_buff_readA_data;//output [15:0] buff_readA_data,
wire   [9:0] sdram2m_buff_buff_readA_addr;//input [9:0]   buff_readA_addr,
wire         sdram2m_buff_buff_readA_clk;//input         buff_readA_clk,

wire [15:0]  sdram2m_buff_buff_readB_data;//output [15:0] buff_readB_data,
wire   [9:0] sdram2m_buff_buff_readB_addr;//input [9:0]   buff_readB_addr,
wire         sdram2m_buff_buff_readB_clk ;//input         buff_readB_clk,

wire sdram2m_busy;

sdram2m(
  .sys_clk    (sys_clk  ),       // 时钟信号
  .sys_rst_n  (sys_rst_n),       // 复位信号

  //SDRAM 芯片接口
  .sdram_clk_out     (sdram2m_clk_out),
  .sdram_cke      (sdram2m_cke),    //SDRAM 时钟有效
  .sdram_cs_n      (sdram2m_cs_n),    //SDRAM 片选
  .sdram_ras_n    (sdram2m_ras_n),    //SDRAM 行有效  
  .sdram_cas_n    (sdram2m_cas_n),    //SDRAM 列有效
  .sdram_we_n      (sdram2m_we_n),    //SDRAM 写有效
  .sdram_ba        (sdram2m_ba),      //SDRAM Bank地址
  .sdram_addr      (sdram2m_addr),    //SDRAM 行/列地址
  .sdram_data      (sdram2m_data),    //SDRAM 数据  

  .clk        (sys_clk),//in
  .address    (sdram2m_c_address),//in
  .data_in    (sdram2m_c_data_in),//in
  .data_out   (sdram2m_c_data_out),//out
  .read_req   (sdram2m_c_read_req),//in
  .read_ack   (sdram2m_c_read_ack),//out
  .write_req  (sdram2m_c_write_req),//in
  .write_ack  (sdram2m_c_write_ack),//out
  .write_en   (sdram2m_c_write_en),//in
  .write_latch_address(sdram2m_c_write_latch_address),//in


.buffDMAwrite_req  (sdram2m_buffDMAwrite_req  ),             //input        
.buffDMAwrite_addr (sdram2m_buffDMAwrite_addr ),            //input [11:0] 
.buffDMAwrite_A_B  (sdram2m_buffDMAwrite_A_B  ),             //input        
.buffDMAwrite_ack  (sdram2m_buffDMAwrite_ack  ),             //output reg   
.buffDMAWrite_clk  (sdram2m_buffDMAWrite_clk  ),         //input        

.buffDMAWriteA_data(sdram2m_buffDMAWriteA_data),         //input [15:0] 
.buffDMAWriteA_addr(sdram2m_buffDMAWriteA_addr),         //input  [9:0] 
.buffDMAWriteA_en  (sdram2m_buffDMAWriteA_en  ),         //input        
.buffDMAWriteB_data(sdram2m_buffDMAWriteB_data),         //input [15:0] 
.buffDMAWriteB_addr(sdram2m_buffDMAWriteB_addr),         //input  [9:0] 
.buffDMAWriteB_en  (sdram2m_buffDMAWriteB_en  ),         //input        
    

    
.read_buff_req(sdram2m_read_buff_req),//input read_buffA_req,
.read_buff_A_B(sdram2m_read_buff_A_B),//input read_buffB_req,
.read_buff_addr(sdram2m_read_buff_addr),//input [9:0] read_buff_addr,

.buff_readA_data(sdram2m_buff_buff_readA_data),//output [15:0] buff_readA_data,
.buff_readA_addr(sdram2m_buff_buff_readA_addr),//input [9:0]   buff_readA_addr,
.buff_readA_clk (sdram2m_buff_buff_readA_clk ),//input         buff_readA_clk,

.buff_readB_data(sdram2m_buff_buff_readB_data),//output [15:0] buff_readB_data,
.buff_readB_addr(sdram2m_buff_buff_readB_addr),//input [9:0]   buff_readB_addr,
.buff_readB_clk (sdram2m_buff_buff_readB_clk ),//input         buff_readB_clk,

.busy(sdram2m_busy)
);




reg blockvga;
vga_driver u_vga_driver(
    .sys_clk        (sys_clk),    
    .sys_rst_n      (sys_rst_n),    

    .blockvga(blockvga),
    .vga_mode (vga_mode),
    .blanking(blanking),
    
    .read_buff_req  (sdram2m_read_buff_req       ),
    .read_buff_A_B  (sdram2m_read_buff_A_B       ),
    .read_buff_addr (sdram2m_read_buff_addr      ),
    .buff_readA_data(sdram2m_buff_buff_readA_data),
    .buff_readA_addr(sdram2m_buff_buff_readA_addr),
    .buff_readA_clk (sdram2m_buff_buff_readA_clk ),
    .buff_readB_data(sdram2m_buff_buff_readB_data),
    .buff_readB_addr(sdram2m_buff_buff_readB_addr),
    .buff_readB_clk (sdram2m_buff_buff_readB_clk ),
    
    .vga_hs         (vga_hs),       
    .vga_vs         (vga_vs),       
    .vga_rgb        (vga_rgb)
    ); 
    

endmodule

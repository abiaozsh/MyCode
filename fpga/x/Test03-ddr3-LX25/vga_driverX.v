
module vga_driverX(
    input           sys_clk,
    input           sys_rst_n,
input vga_clk_25M,
input vga_clk_65M,
    //00 640*480 txt
    //01 640*480 img
    //1x 1024*768 img
    input [1:0]  vga_mode,
    output reg   blanking,
    input        blockvga,
    input [15:0] read_line_base_addr,//start line

    output [7:0] debug8,
    output [31:0] debug32,
    
    output reg        c3_p1_cmd_en,
    input             c3_p1_cmd_full,
    output reg        c3_p1_cmd_rw,
    output reg [5:0]  c3_p1_cmd_bl,
    output reg [29:0] c3_p1_cmd_byte_addr,

    output reg        c3_p1_rd_en,
    input wire [31:0] c3_p1_rd_data,
    input wire        c3_p1_rd_empty,

    output     reg     vga_hs,
    output     reg     vga_vs,
    output  [15:0]  vga_rgb
    
  );

`include "config.v"




 reg       read_line_req ;
 reg       read_line_A_B ;
wire    [15:0] read_line_addr;//4kline total
//
wire [15:0] read_pixelA_data;
wire [15:0] read_pixelB_data;
wire [9:0] read_pixel_addr;
wire       read_pixel_clk;


//    input         read_line_req  ,
//    input         read_line_A_B  ,
//    input  [15:0] read_line_addr ,//line address -> 1024 * 1024 word * 4 frame = 4Mword  **
//
//    output [15:0] read_pixelA_data,
//    output [15:0] read_pixelB_data,
//    input   [9:0] read_pixel_addr ,//10bit 1024word per line
//    input         read_pixel_clk  ,

reg [15:0] buffAB_wrdata;
reg [9:0]  buffAB_wraddress;
reg        buffA_wren;
reg        buffB_wren;

wire        wrclock    = sys_clk        ;
wire        rdclock    = read_pixel_clk   ;
wire [15:0] data       = buffAB_wrdata    ;
wire  [9:0] wraddress  = buffAB_wraddress ;
wire  [9:0] rdaddress  = read_pixel_addr  ;
wire        wrenA      = buffA_wren       ;
wire        wrenB      = buffB_wren       ;

assign      read_pixelA_data = qA;
wire [15:0] qA;
buff1024x16  buffReadA (
  .clka  (wrclock),   //input clka;
  .wea   (wrenA),     //input [0 : 0] wea;
  .addra (wraddress), //input [9 : 0] addra;
  .dina  (data),      //input [15 : 0] dina;
  .clkb  (rdclock),   //input clkb;
  .addrb (rdaddress), //input [9 : 0] addrb;
  .doutb (qA)         //output [15 : 0] doutb;
);



assign      read_pixelB_data = qB;
wire [15:0] qB;
buff1024x16  buffReadB (
  .clka  (wrclock),   //input clka;
  .wea   (wrenB),     //input [0 : 0] wea;
  .addra (wraddress), //input [9 : 0] addra;
  .dina  (data),      //input [15 : 0] dina;
  .clkb  (rdclock),   //input clkb;
  .addrb (rdaddress), //input [9 : 0] addrb;
  .doutb (qB)         //output [15 : 0] doutb;
);



//    c3_p1_cmd_en,
//    c3_p1_cmd_full,
//    c3_p1_cmd_rw,
//    c3_p1_cmd_bl,
//    c3_p1_cmd_byte_addr,
//    
//    c3_p1_rd_en,
//    c3_p1_rd_data,
//    c3_p1_rd_empty,

reg [3:0] curr_read_count;
reg read_line_req_buff;
reg read_line_ack;
reg  [10:0] cmd_count;
reg  [11:0] read_count;
reg [15:0] readBufferHigh;


assign debug8[0] = read_line_req_buff;
assign debug8[1] = read_line_ack;
assign debug8[2] = c3_p1_rd_empty;
assign debug8[7:4] = curr_read_count;

assign debug32[15:0] = cmd_count;
assign debug32[31:16] = read_count;

reg read_phase;//0: read/write low, 1: write high
wire ok_to_send_cmd = cmd_count!=512 && !c3_p1_cmd_full && curr_read_count!=7;//cmd_count++
wire ok_to_read_data = !c3_p1_rd_empty && read_phase==0;//read_count++
always@(posedge sys_clk or negedge sys_rst_n) begin // sdram 主控
  if(!sys_rst_n) begin
   
    read_line_req_buff <= 0;
    
    read_line_ack <= 0;
    cmd_count <= 0;
    read_count <= 0;
    
    curr_read_count <= 0;
    c3_p1_rd_en <= 0;
    read_phase <= 0;
  end else begin
    read_line_req_buff <= read_line_req;
        
    buffA_wren <= 0;
    buffB_wren <= 0;

    //vga line read
    if(read_line_req_buff && !read_line_ack)begin
      if(ok_to_send_cmd)begin
        cmd_count<=cmd_count+1'b1;
        c3_p1_cmd_byte_addr <= {read_line_addr,cmd_count[8:0],2'b0};
        c3_p1_cmd_bl <= 0;
        c3_p1_cmd_en <= 1;
        c3_p1_cmd_rw <= 1;
      end else begin
        c3_p1_cmd_en <= 0;
      end
      
      if(ok_to_read_data)begin
        read_phase <= 1;
        c3_p1_rd_en <= 1;
        read_count <= read_count+1'b1;
        buffAB_wrdata <= c3_p1_rd_data[15:0];
        readBufferHigh <= c3_p1_rd_data[31:16];
        buffAB_wraddress <= read_count;
        if(read_line_A_B)begin
          buffA_wren <= 1;
        end else begin
          buffB_wren <= 1;
        end
      end
      if(read_phase)begin
        read_phase <= 0;
        c3_p1_rd_en <= 0;
        read_count <= read_count+1'b1;
        buffAB_wrdata <= readBufferHigh;
        buffAB_wraddress <= read_count;
        if(read_line_A_B)begin
          buffA_wren <= 1;
        end else begin
          buffB_wren <= 1;
        end
      end
      
      //无法读取，写入cmd
      if(ok_to_send_cmd && !ok_to_read_data)begin
        curr_read_count <= curr_read_count+1'b1;
      end
      //读取，没有写入cmd
      if(!ok_to_send_cmd && ok_to_read_data)begin
        curr_read_count <= curr_read_count-1'b1;
      end
      
      if(cmd_count==512&& read_count==1024)begin
        cmd_count<=0;
        read_count<=0;
        curr_read_count<=0;
        read_line_ack <= 1;
      end
    end
    
    if(!read_line_req_buff && read_line_ack)begin
      read_line_ack <= 0;
    end

  end
end



//parameter define  
parameter  H25_SYNC   =  11'd96;    
parameter  H25_BACK   =  11'd48;    
parameter  H25_DISP   =  11'd640;   
parameter  H25_TOTAL  =  11'd800;   

parameter  V25_SYNC   =  11'd2;     
parameter  V25_BACK   =  11'd33;    
parameter  V25_DISP   =  11'd480;   
parameter  V25_TOTAL  =  11'd525;   


//1024*768 60FPS_65MHz
parameter  H65_SYNC   =  11'd136;   
parameter  H65_BACK   =  11'd160;   
parameter  H65_DISP   =  11'd1024;  
parameter  H65_TOTAL  =  11'd1344;  

parameter  V65_SYNC   =  11'd6;     
parameter  V65_BACK   =  11'd29;    
parameter  V65_DISP   =  11'd768;   
parameter  V65_TOTAL  =  11'd806;   


//wire define

wire vga_en  = h_active && v_active;
                 
assign vga_rgb = vga_en ?  pixel_data: 16'd0;//16'hffff  pixel_data

wire vga_clk;
assign vga_clk = vga_mode[1] ? vga_clk_65M : vga_clk_25M;

assign read_pixel_clk = vga_clk;

reg h_active;
reg v_active;
reg v_active_ram;

//reg define
reg  [10:0] cnt_h;
reg  [10:0] cnt_v;

reg [10:0]h_total;
reg [10:0]v_total;
reg [10:0]h_sync;
reg [10:0]v_sync;
reg [10:0]h_start;
reg [10:0]v_start;
reg [10:0]h_end;
reg [10:0]v_end;

reg [15:0] curr_read_line_base_addr;
assign read_line_addr = curr_read_line_base_addr + cnt_v - v_start + 1'b1;
wire [15:0]  pixel_data = blockvga ? 16'b0 : (read_line_addr[0]?read_pixelB_data:read_pixelA_data);

wire [10:0]temp_read_pixel_addr = (cnt_h-h_start);
assign read_pixel_addr = temp_read_pixel_addr[9:0];

//80*30
//128*32:4096byte //BUFFA BUFFB
//8*16 char table 
//16byte per char
//128char = 2048byte
always @(posedge vga_clk or negedge sys_rst_n) begin
    if (!sys_rst_n)begin
      cnt_h <= 0;
      cnt_v <= 0;
      vga_hs <= 0;
      vga_vs <= 0;
      h_active <= 0;
      v_active <= 0;
      h_total <= H25_TOTAL;
      v_total <= V25_TOTAL;
      h_sync <= H25_SYNC;
      v_sync <= V25_SYNC;
      h_start <= H25_SYNC + H25_BACK;
      v_start <= V25_SYNC + V25_BACK;
      h_end <= H25_SYNC + H25_BACK + H25_DISP;
      v_end <= V25_SYNC + V25_BACK + V25_DISP;
      blanking <= 0;
      curr_read_line_base_addr <= 0;
    end else begin
      if(vga_mode[1])begin
        h_total <= H65_TOTAL;
        v_total <= V65_TOTAL;
        h_sync <= H65_SYNC;
        v_sync <= V65_SYNC;
        h_start <= H65_SYNC + H65_BACK;
        v_start <= V65_SYNC + V65_BACK;
        h_end <= H65_SYNC + H65_BACK + H65_DISP;
        v_end <= V65_SYNC + V65_BACK + V65_DISP;
      end else begin
        h_total <= H25_TOTAL;
        v_total <= V25_TOTAL;
        h_sync <= H25_SYNC;
        v_sync <= V25_SYNC;
        h_start <= H25_SYNC + H25_BACK;
        v_start <= V25_SYNC + V25_BACK;
        h_end <= H25_SYNC + H25_BACK + H25_DISP;
        v_end <= V25_SYNC + V25_BACK + V25_DISP;
      end
    
      cnt_h <= cnt_h + 1'b1;
      if(cnt_h == h_total)begin
        cnt_h <= 10'd0;
        vga_hs <= 1;
        cnt_v <= cnt_v + 1'b1;
        if(cnt_v == v_total)begin
          cnt_v <= 10'd0;
          vga_vs <= 1;
        end
      end
    
      if(cnt_h == h_sync)begin
        vga_hs <= 0;
      end
      if(cnt_v == v_sync)begin
        vga_vs <= 0;
      end
      
        
      if(cnt_h == h_start)begin
        h_active <= 1;
        if(v_active_ram)begin
          read_line_A_B<=read_line_addr[0];
        end
        if(!blockvga)begin
          read_line_req<=1;
        end

      end
		
      if(read_line_ack)begin
        read_line_req <= 0;
      end
		
      if(cnt_h == h_end)begin
        h_active <= 0;
      end
      
      if(cnt_v == v_start)begin
        v_active <= 1;
      end

      if(cnt_v == v_end)begin
        v_active <= 0;
      end
      
      if(cnt_v == (v_start-1))begin
        v_active_ram <= 1;
      end

      if(cnt_v == (v_end-1))begin
        v_active_ram <= 0;
        curr_read_line_base_addr <= read_line_base_addr;
      end
      
      if(cnt_v == (v_start-11'b11))begin
        blanking <= 0;
      end
      
      if(cnt_v == (v_end+1'b1))begin
        blanking <= 1;
      end

    end
end

endmodule 
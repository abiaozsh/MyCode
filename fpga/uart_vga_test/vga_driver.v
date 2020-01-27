
module vga_driver(
    input           sys_rst_n,    //复位信号
	
    input           vga_clk_25M,      //VGA驱动时钟
    input           vga_clk_65M,      //VGA驱动时钟

	input  vga_mode,
	
    //VGA接口                          
    output     reg     vga_hs,       //行同步信号
    output     reg     vga_vs,       //场同步信号
    output  [15:0]  vga_rgb      //红绿蓝三原色输出
    
    );                             
                                                        
//parameter define  
parameter  H25_SYNC   =  11'd96;    //行同步
parameter  H25_BACK   =  11'd48;    //行显示后沿
parameter  H25_DISP   =  11'd640;   //行有效数据
parameter  H25_TOTAL  =  11'd800;   //行扫描周期

parameter  V25_SYNC   =  11'd2;     //场同步
parameter  V25_BACK   =  11'd33;    //场显示后沿
parameter  V25_DISP   =  11'd480;   //场有效数据
parameter  V25_TOTAL  =  11'd525;   //场扫描周期


//1024*768 60FPS_65MHz
parameter  H65_SYNC   =  11'd136;   //行同步     
parameter  H65_BACK   =  11'd160;   //行显示后沿
parameter  H65_DISP   =  11'd1024;  //行有效数据
parameter  H65_TOTAL  =  11'd1344;  //行扫描周期  注意位宽长度,需要11位的位宽

parameter  V65_SYNC   =  11'd6;     //场同步
parameter  V65_BACK   =  11'd29;    //场显示后沿
parameter  V65_DISP   =  11'd768;   //场有效数据
parameter  V65_TOTAL  =  11'd806;   //场扫描周期


//wire define
wire       vga_en;


//使能RGB565数据输出
assign vga_en  = h_active && v_active;
                 
//RGB565数据输出                 
assign vga_rgb = vga_en ?  pixel_data: 16'd0;//16'hffff  pixel_data

//像素点坐标                
wire [10:0] pixel_xpos;
wire [10:0] pixel_ypos;
assign pixel_xpos = cnt_h - h_start;
assign pixel_ypos = cnt_v - v_start;

wire vga_clk;
assign vga_clk = vga_mode ? vga_clk_65M : vga_clk_25M;

reg h_active;
reg v_active;

reg start_load;

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

//行计数器对像素时钟计数
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
    end else begin
		if(vga_mode)begin
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
		end
		if(cnt_v == v_start)begin
			v_active <= 1;
		end
		start_load <= 0;
		if(cnt_h == h_end)begin
			h_active <= 0;
			start_load <= 1;
		end
		if(cnt_v == v_end)begin
			v_active <= 0;
		end

    end
end

reg [7:0] R;
reg [7:0] G;
reg [7:0] B;
wire [15:0]  pixel_data;
assign pixel_data = {R[7:3],G[7:2],B[7:3]};
//*****************************************************
//**                    main code
//*****************************************************
//根据当前像素点坐标指定当前像素点颜色数据，在屏幕上显示彩条
always @(posedge vga_clk or negedge sys_rst_n) begin         
    if (!sys_rst_n) begin
        R<=0;
        G<=0;
        B<=0;
    end else begin
            R <= pixel_xpos;
            G <= pixel_ypos;
            B <= 8'HFF;

    end
end


endmodule 
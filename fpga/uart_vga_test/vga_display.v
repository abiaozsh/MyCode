//****************************************Copyright (c)***********************************//
//技术支持：www.openedv.com
//淘宝店铺：http://openedv.taobao.com 
//关注微信公众平台微信号："正点原子"，免费获取FPGA & STM32资料。
//版权所有，盗版必究。
//Copyright(C) 正点原子 2018-2028
//All rights reserved	                               
//----------------------------------------------------------------------------------------
// File name:           vga_display
// Last modified Date:  2018/1/30 11:12:36
// Last Version:        V1.1
// Descriptions:        vga彩条显示模块
//----------------------------------------------------------------------------------------
// Created by:          正点原子
// Created date:        2018/1/29 10:55:56
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
// Modified by:		    正点原子
// Modified date:	    2018/1/30 11:12:36
// Version:			    V1.1
// Descriptions:	    根据当前像素点坐标指定当前像素点颜色，在屏幕上显示彩条
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

module vga_display(
    input             vga_clk,                  //VGA驱动时钟
    input             sys_rst_n,                //复位信号
    
    input      [ 9:0] pixel_xpos,               //像素点横坐标
    input      [ 9:0] pixel_ypos,               //像素点纵坐标    
    output     [15:0] pixel_data                //像素点数据
    );    
    
parameter  H_DISP = 10'd640;                    //分辨率——行
parameter  V_DISP = 10'd480;                    //分辨率——列

reg [7:0] R;
reg [7:0] G;
reg [7:0] B;
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
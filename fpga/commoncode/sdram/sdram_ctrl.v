module sdram_ctrl (
    input            clk,			    //系统时钟
    input            rst_n,			    //复位信号，低电平有效
    
    input            sdram_wr_req,	    //写SDRAM请求信号
    input            sdram_rd_req,	    //读SDRAM请求信号
    output           sdram_wr_ack,	    //写SDRAM响应信号
    output           sdram_rd_ack,	    //读SDRAM响应信号
    input      [9:0] sdram_wr_burst,	//突发写SDRAM字节数（1-512个）
    input      [9:0] sdram_rd_burst,	//突发读SDRAM字节数（1-256个）	
    output           sdram_init_done,   //SDRAM系统初始化完毕信号

    output reg [4:0] init_state,	    //SDRAM初始化状态
    output reg [3:0] work_state,	    //SDRAM工作状态
    output reg [9:0] cnt_clk,	        //时钟计数器
    output reg       sdram_rd_wr 		//SDRAM读/写控制信号，低电平为写，高电平为读
  );

endmodule 
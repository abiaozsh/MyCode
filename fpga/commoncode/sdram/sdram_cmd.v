module sdram_cmd(
    input             clk,              //系统时钟
    input             rst_n,            //低电平复位信号

    input      [23:0] sys_wraddr,       //写SDRAM时地址
    input      [23:0] sys_rdaddr,       //读SDRAM时地址
    input      [ 9:0] sdram_wr_burst,   //突发写SDRAM字节数
    input      [ 9:0] sdram_rd_burst,   //突发读SDRAM字节数
    
    input      [ 4:0] init_state,       //SDRAM初始化状态
    input      [ 3:0] work_state,       //SDRAM工作状态
    input      [ 9:0] cnt_clk,          //延时计数器 
    input             sdram_rd_wr,      //SDRAM读/写控制信号，低电平为写
    
    output            sdram_cke,        //SDRAM时钟有效信号
    output            sdram_cs_n,       //SDRAM片选信号
    output            sdram_ras_n,      //SDRAM行地址选通脉冲
    output            sdram_cas_n,      //SDRAM列地址选通脉冲
    output            sdram_we_n,       //SDRAM写允许位
    output reg [ 1:0] sdram_ba,         //SDRAM的L-Bank地址线
    output reg [12:0] sdram_addr        //SDRAM地址总线
    );

endmodule 
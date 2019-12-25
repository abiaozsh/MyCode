
module sdram_data(
    input             clk,              //系统时钟
    input             rst_n,            //低电平复位信号

    input   [15:0]    sdram_data_in,    //写入SDRAM中的数据
    output  [15:0]    sdram_data_out,   //从SDRAM中读取的数据
    input   [ 3:0]    work_state,       //SDRAM工作状态寄存器
    input   [ 9:0]    cnt_clk,          //时钟计数
    
    inout   [15:0]    sdram_data        //SDRAM数据总线
    );

endmodule 
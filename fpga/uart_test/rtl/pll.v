

module pll(
    input               sys_clk        ,  //系统时钟
    input               sys_rst_n      ,  //系统复位，低电平有效
    //输出时钟
    output              clk0       ,  //100Mhz时钟频率
    output              clk1  //100Mhz时钟频率,相位偏移180度
    );

//wire define
wire             rst_n          ;  //复位信号
wire             locked         ;  //locked信号拉高,锁相环开始稳定输出时钟

//*****************************************************
//**                    main code
//*****************************************************

//系统复位与锁相环locked相与,作为其它模块的复位信号
assign  rst_n = sys_rst_n & locked;

//锁相环
pll_clk ipcore(
    .areset       (~sys_rst_n     ),  //锁相环高电平复位,所以复位信号取反
    .inclk0       (sys_clk        ),
    .c0           (clk0       ),
    .c1           (clk1  ),
    .locked       (locked         )
    ); 

endmodule
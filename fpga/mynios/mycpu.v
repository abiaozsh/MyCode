// mycpu.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module mycpu (
    input  wire        clk,
    input  wire        reset_n,
    input              clk_50M,
    //bus
    output wire [31:0] avm_m0_address       ,     //    m0.address
    output wire        avm_m0_read          ,        //      .read
    output wire        avm_m0_write         ,       //      .write
    input  wire [31:0] avm_m0_readdata      ,    //      .readdata
    output wire [31:0] avm_m0_writedata     ,   //      .writedata
    output wire [3:0]  avm_m0_byteenable    ,    //      .readdata
    input  wire        avm_m0_waitrequest   , //      .waitrequest
    input  wire        irq_req,
    
    input wire          uart_rxd,
    output wire         uart_txd,
    output [7:0]    debug8,
    output [31:0]   debug32,
    
    
    input  [7:0] debugin8,
    input [15:0] cache_life0   ,
    input [15:0] cache_life1   ,
    input [15:0] cache_life2   ,
    input [15:0] cache_life3   ,
    input [15:0] cacheAddrHigh0,
    input [15:0] cacheAddrHigh1,
    input [15:0] cacheAddrHigh2,
    input [15:0] cacheAddrHigh3,
    input [31:0] debugin32
  );

	
wire uart_rec_req;
reg  uart_rec_ack;
wire [7:0] uart_data_out;
wire uart_send_req = uart_send;
reg [7:0] uart_data_in;
wire uart_send_ack;
uart_hs uart_hs_inst (
	.sys_clk(clk_50M),
	.sys_rst_n(reset_n),

	.uart_txd(uart_txd),
	.uart_rxd(uart_rxd),
	
	.uart_rec_req(uart_rec_req),
	.uart_rec_ack(uart_rec_ack),
	.uart_data_out(uart_data_out),

	.uart_send_req(uart_send_req),
	.uart_send_ack(uart_send_ack),
	.uart_data_in(uart_data_in)
);

reg [7:0] command;
reg [7:0] data;
reg [7:0] command_temp;
reg data_cmd;//1:data 0:cmd
reg uart_rec_req_buff;
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin
    command <= 0;
    data <= 0;
		uart_rec_req_buff<=0;
		uart_rec_ack<=0;
  end else begin
		uart_rec_req_buff <= uart_rec_req;
	
    if (uart_rec_req_buff && !uart_rec_ack) begin 
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
			uart_rec_ack <= 1;
    end
		
    if (!uart_rec_req_buff && uart_rec_ack) begin 
			uart_rec_ack <= 0;
    end
		
		if(command_done)begin
			command <= 0;
		end
  end
end

//https://www.intel.cn/content/www/cn/zh/programmable/products/processors/support.html

reg command_done;

reg [31:0] debug_address;
reg        debug_read;
reg        debug_write;
reg [31:0] debug_data;
reg [31:0] debug_writedata;
reg  [2:0] debug_readmem_step;
reg        debug_step;
reg  [4:0] debug_reg;
reg        debug_reset_n;
reg  [3:0] debug_byteenable;
reg halt_uart;

reg [15:0] accessTime;

reg uart_send;
reg uart_send_ack_buff;
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin

    command_done <= 0;
    debug_step<=0;
    debug_readmem_step<=0;

    halt_uart<=1;
    debug_read<=0;
    debug_write<=0;
    debug_reset_n<=1;
    debug_byteenable <= 4'b1111;
    accessTime <= 0;
    uart_send_ack_buff<=0;
  end else begin
    uart_send_ack_buff <= uart_send_ack;

    if(uart_send_ack_buff)begin
      uart_send<=0;
    end

    if(command_done)begin
      if          (command == 8'h00) begin 
        command_done<=0;
      end
    end else begin//command_done==0
      if          (command == 8'h00) begin

      end else if (command == 8'h01) begin halt_uart<=data[0]; command_done<=1;

      end else if (command == 8'h02) begin debug_reset_n<=data[0]; command_done<=1;

      end else if (command == 8'h03) begin debug_step<=~debug_step; command_done<=1;

      end else if (command == 8'h04) begin uart_send<=1; uart_data_in<=accessTime[ 7: 0]; command_done<=1;
      end else if (command == 8'h05) begin uart_send<=1; uart_data_in<=accessTime[15: 8]; command_done<=1;

      end else if (command == 8'h06) begin uart_send<=1; uart_data_in<=data; command_done<=1;

      end else if (command == 8'h10) begin uart_send<=1; uart_data_in<=debug_data[ 7: 0]; command_done<=1;
      end else if (command == 8'h11) begin uart_send<=1; uart_data_in<=debug_data[15: 8]; command_done<=1;
      end else if (command == 8'h12) begin uart_send<=1; uart_data_in<=debug_data[23:16]; command_done<=1;
      end else if (command == 8'h13) begin uart_send<=1; uart_data_in<=debug_data[31:24]; command_done<=1;
      
      end else if (command == 8'h14) begin uart_send<=1; uart_data_in<=halt_cpu; command_done<=1;
      end else if (command == 8'h15) begin uart_send<=1; uart_data_in<=halt_uart; command_done<=1;
      end else if (command == 8'h16) begin uart_send<=1; uart_data_in<=avm_m0_waitrequest; command_done<=1;
      end else if (command == 8'h17) begin uart_send<=1; uart_data_in<={3'b0,irq_enable,3'b0,irq_req}; command_done<=1;
      end else if (command == 8'h18) begin uart_send<=1; uart_data_in<=RtypeCmd; command_done<=1;
      
      end else if (command == 8'h20) begin debug_address[ 7: 0] <= data; command_done<=1;
      end else if (command == 8'h21) begin debug_address[15: 8] <= data; command_done<=1;
      end else if (command == 8'h22) begin debug_address[23:16] <= data; command_done<=1;
      end else if (command == 8'h23) begin debug_address[31:24] <= data; command_done<=1;

      end else if (command == 8'h24) begin debug_writedata[ 7: 0] <= data; command_done<=1;
      end else if (command == 8'h25) begin debug_writedata[15: 8] <= data; command_done<=1;
      end else if (command == 8'h26) begin debug_writedata[23:16] <= data; command_done<=1;
      end else if (command == 8'h27) begin debug_writedata[31:24] <= data; command_done<=1;

      end else if (command == 8'h28) begin debug_byteenable <= data[3:0]; command_done<=1;
      
      end else if (command == 8'h50) begin uart_send<=1; uart_data_in<=cache_life0[ 7: 0]; command_done<=1;
      end else if (command == 8'h51) begin uart_send<=1; uart_data_in<=cache_life0[15: 8]; command_done<=1;
      end else if (command == 8'h52) begin uart_send<=1; uart_data_in<=cache_life1[ 7: 0]; command_done<=1;
      end else if (command == 8'h53) begin uart_send<=1; uart_data_in<=cache_life1[15: 8]; command_done<=1;
      end else if (command == 8'h54) begin uart_send<=1; uart_data_in<=cache_life2[ 7: 0]; command_done<=1;
      end else if (command == 8'h55) begin uart_send<=1; uart_data_in<=cache_life2[15: 8]; command_done<=1;
      end else if (command == 8'h56) begin uart_send<=1; uart_data_in<=cache_life3[ 7: 0]; command_done<=1;
      end else if (command == 8'h57) begin uart_send<=1; uart_data_in<=cache_life3[15: 8]; command_done<=1;
      
      end else if (command == 8'h58) begin uart_send<=1; uart_data_in<=cacheAddrHigh0[ 7: 0]; command_done<=1;
      end else if (command == 8'h59) begin uart_send<=1; uart_data_in<=cacheAddrHigh0[15: 8]; command_done<=1;
      end else if (command == 8'h5A) begin uart_send<=1; uart_data_in<=cacheAddrHigh1[ 7: 0]; command_done<=1;
      end else if (command == 8'h5B) begin uart_send<=1; uart_data_in<=cacheAddrHigh1[15: 8]; command_done<=1;
      end else if (command == 8'h5C) begin uart_send<=1; uart_data_in<=cacheAddrHigh2[ 7: 0]; command_done<=1;
      end else if (command == 8'h5D) begin uart_send<=1; uart_data_in<=cacheAddrHigh2[15: 8]; command_done<=1;
      end else if (command == 8'h5E) begin uart_send<=1; uart_data_in<=cacheAddrHigh3[ 7: 0]; command_done<=1;
      end else if (command == 8'h5F) begin uart_send<=1; uart_data_in<=cacheAddrHigh3[15: 8]; command_done<=1;
      
      end else if (command == 8'h60) begin uart_send<=1; uart_data_in<=debugin32[ 7: 0]; command_done<=1;
      end else if (command == 8'h61) begin uart_send<=1; uart_data_in<=debugin32[15: 8]; command_done<=1;
      end else if (command == 8'h62) begin uart_send<=1; uart_data_in<=debugin32[23:16]; command_done<=1;
      end else if (command == 8'h63) begin uart_send<=1; uart_data_in<=debugin32[31:24]; command_done<=1;
      end else if (command == 8'h64) begin uart_send<=1; uart_data_in<=debugin8; command_done<=1;



      end else if (command == 8'h30) begin
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          debug_read <= 1;
          accessTime <= 0;
        end else if(debug_readmem_step==1)begin
          accessTime <= accessTime + 1'b1;
          if(!avm_m0_waitrequest)begin
            debug_data <= avm_m0_readdata;
            debug_read <= 0;
            debug_readmem_step <= 0;
            uart_send<=1; uart_data_in<=123;
            command_done = 1;
          end
        end
        
      end else if (command == 8'h31) begin
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          debug_write <= 1;
          accessTime <= 0;
        end else if(debug_readmem_step==1)begin
          accessTime <= accessTime + 1'b1;
          if(!avm_m0_waitrequest)begin
            debug_write <= 0;
            debug_readmem_step <= 0;
            uart_send<=1; uart_data_in<=123;
            command_done <= 1;
          end
        end
        
      end else if (command == 8'h40) begin debug_data<=irq_addr; command_done<=1;
      end else if (command == 8'h43) begin debug_data<=pc; command_done<=1;
      end else if (command == 8'h44) begin debug_data<=private_offset; command_done<=1;

      end else if (command == 8'h47) begin
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          debug_reg <= data[4:0];
        end else if(debug_readmem_step==1)begin
          debug_data <= regDataOut;
          debug_readmem_step <= 0;
          command_done <= 1;
        end
      end else begin
        command_done<=1;
      end
    end


  end
end
  //大家好，分享一个diy cpu的视频
  //这次是整体工作的review视频，代码细节会在后面的视频里解释
  //所有涉及到的源码，已经放到github上了
  //cpu是32位处理器，指令集参考了NIOSII，总线参考了qsys总线，都有所修改
  //编译器是使用NIOSII的编译器，
  //这次的demo：从rom中的bios启动，加载位于sd卡上的操作系统
  //从操作系统再启动3个应用程序：图片查看程序，画图板，俄罗斯方块
  //开发板：勤谋的Altera EP4CE15 
  //外设：vga口，sd卡，键盘，鼠标，串口

  //示波器前期准备
  
  //sd卡座拆分
  
  
  
  

  //流水线难点：
  //指令地址相关性
  //寄存器相关性
  

  //问题：总线速度低
  
  //存储仲裁器，取指令模块，执行模块
  
  
  //指令 cache： 环状可回退fifo

  //起始指针，结束指针，当前指针
  
  
  //32:ins
  //n:预测字 0：无预测，PC+4 否则-n
  
  //流水线每一级的信号：停止信号，气泡（not valid）
  
  //1，取指令+解码         解码将指令字转成各个alu的EA???
  //hlt状态下，存储器等待，时产生气泡
  //hlt指令：置hlt状态1，并flush流水线
  //参考PC 和 预测字段，获取指令
  
  //以下每级流水线有PCchange标志，和寄存器change标志，取指令级发现PC冲突和寄存器冲突，则产生气泡
  //{pc冲突时，停止取指令} 流水线里有PC指令
  //{总线冲突时，停止取指令}（mem 指令）
  //{寄存器冲突时，发送气泡}
  //{寄存器端口冲突（存取地址）}
  //2，寄存器A
  //3，寄存器B
  //4，执行
  //5，写回+PC更新（冲刷）+hlt
  
  //全局信号： Valid PCChange MEMAccess [4:0]regChange
  





  wire cpu_reset_n = reset_n && debug_reset_n;

  wire [31:0] affect_address    = halt_accept == 1   ? debug_address                                : (cycle == 0 ? fetch_address : {exec_address[31:2],2'b0});
  assign avm_m0_address         = affect_address[31] ? {1'b0,affect_address[30:0]} + private_offset : affect_address;
  assign avm_m0_writedata       = halt_accept == 1   ? debug_writedata                              : exec_writedata;
  assign avm_m0_byteenable      = halt_accept == 1   ? debug_byteenable                             : byteenable;
  assign avm_m0_write           = halt_accept == 1   ? debug_write                                  : exec_write;
  assign avm_m0_read            = halt_accept == 1   ? debug_read                                   : (cycle == 0 ? fetch_read : exec_read);

  
  
  reg [31:0] latch_readdata;
  reg        Rtype;
  reg [6:0]  RtypeCmd;
  reg [4:0]  regB;
  reg [4:0]  regC;
  reg [4:0]  IMM5;
  reg [15:0] IMM16;
  reg [31:0] IMM16zx;
  reg [31:0] IMM16sx;
  reg [25:0] IMM26;

  reg cycle;//0:fetchCode 1:execCode
  wire [31:0] fetch_address;
  assign fetch_address = pc;
  
  
  reg [31:0] regfile[32];
  
  reg regWrite;
  reg [4:0] regAddr;
  reg [4:0] regWriteAddr;
  wire [4:0] reg_addr;
  assign reg_addr = (halt_accept == 1) ? debug_reg : regAddr;
  reg  [31:0] regDataIn;
  wire [31:0] regDataOut = regfile[reg_addr];
  always @(posedge clk or negedge cpu_reset_n) begin
    if (!cpu_reset_n) begin
    end else begin
      if(regWrite)begin
        regfile[regWriteAddr] <= regDataIn;
      end
    end
  end

  
  reg [31:0] regfileA;
  reg [31:0] regfileB;
  reg        PCChange;
  reg        MEMAccess;
  
  reg        fetch_read;
  reg [ 2:0] fetch_step;
  reg        debug_step_buff;
  reg        halt_accept;
  reg        irq_req_buff;
  reg        irq_req_buff2;
  always @(posedge clk or negedge cpu_reset_n) begin
    if (!cpu_reset_n) begin
      cycle <= 0;
      fetch_read <= 0;
      fetch_step <= 0;
      halt_accept <= 0;
      debug_step_buff <= 0;
      regWrite <= 0;
      regAddr <= 0;
      regWriteAddr <= 0;
      PCChange <= 0;
      MEMAccess <= 0;
    end else begin
      regWrite <= 0;
      
      irq_req_buff = irq_req;

      if(cycle==0)begin
        if         (fetch_step==0)begin
          if(halt)begin
            halt_accept <= 1;
          end else begin
            halt_accept <= 0;
            fetch_step <= 1;

            if(irq_req_buff && irq_enable)begin
              irq_req_buff2 <= 1;
            end else begin
              irq_req_buff2 <= 0;
              fetch_read <= 1;
            end

          end
        end else if(fetch_step==1)begin
          if(irq_req_buff2)begin
            //#irqcall                     @         xxx @                     xx @  37 @          100101      0x3A,0x25*
            //7'b1100101 : begin//ok
            RtypeCmd <= 7'b1100101;
            PCChange <= 1;
            MEMAccess <= 0;
            fetch_step <= 0;
            cycle <= 1;
          end else begin
            if(!avm_m0_waitrequest)begin
              latch_readdata <= avm_m0_readdata;
              Rtype <= avm_m0_readdata[5:0]==6'h3A;
              regAddr <= avm_m0_readdata[31:27];//regA
              
              PCChange <= 0;
              MEMAccess <= 0;
              if(avm_m0_readdata[5:0]==6'h3A)begin
                if(latch_readdata[13:11]==3'b101)begin//latch_readdata[16:11] ???101
                  PCChange <= 1;
                end
              end else begin
                if(latch_readdata[1:0]==2'b10)begin//latch_readdata[5:0]  ????10
                  PCChange <= 1;
                end
                if(latch_readdata[2:0]==3'b111)begin
                  MEMAccess <= 1;
                end
              end
              
              
              fetch_read <= 0;
              fetch_step <= 2;
            end
          end
        end else if(fetch_step==2)begin
          RtypeCmd <= {Rtype, (Rtype ? latch_readdata[16:11] : latch_readdata[5:0])};
          regB <= latch_readdata[26:22];
          regAddr <= latch_readdata[26:22];//regB;
          regC <= latch_readdata[21:17];
          IMM5 <= latch_readdata[10:6];
          IMM26 <= latch_readdata[31:6];
          IMM16 <= latch_readdata[21:6];
          regfileA <= regDataOut;
          fetch_step <= 3;
        end else if(fetch_step==3)begin
          regfileB <= regDataOut;
          IMM16sx <= {{16{IMM16[15]}},IMM16};
          IMM16zx <= {16'b0,IMM16};
          fetch_step <= 0;
          cycle<=1;
        end
      end else begin
        if(cmd_ack)begin
          debug_step_buff <= debug_step;
          if(regResultB && regB!=0)begin
            regWriteAddr <= regB;
            regDataIn <= regResult;
            regWrite <= 1;
          end
          if(regResultC && regC!=0)begin
            regWriteAddr <= regC;
            regDataIn <= regResult;
            regWrite <= 1;
          end
          if(regResultRA)begin
            regWriteAddr <= 31;
            regDataIn <= regResult;
            regWrite <= 1;
          end
          cycle <= 0;
        end
      end

    end
  end
  

  reg [3:0] byteenable;

  reg [31:0] regResult;
  reg        regResultB;
  reg        regResultC;
  reg        regResultRA;

  reg [31:0] pc;

  reg [31:0] private_offset;
  
  wire [31:0] nextpc;
  assign nextpc = pc + 4;
  
  wire halt;
  assign halt = (halt_cpu || halt_uart) && (debug_step == debug_step_buff);
  
  reg halt_cpu;

  reg cmd_ack;
  
  wire [31:0] exec_address;
  reg        exec_read;
  reg        exec_write;
  reg [31:0] exec_writedata;
  
  reg [ 1:0] exec_step;
  

  wire comp_eq = regfileA==regfileB;
  wire comp_ge = $signed(regfileA)>=$signed(regfileB);
  wire comp_lt = $signed(regfileA)<$signed(regfileB);
  wire comp_ltu = regfileA < regfileB;
  
  //wire [31:0] dsAddr = ds + regfileA + IMM16sx;
  assign exec_address = regfileA + IMM16sx;

  wire [31:0] shiftDataIn;
  assign shiftDataIn = regfileA;
  reg        shiftDirection;
  reg  [4:0]  shiftDistance;
  wire  [31:0]  shiftResultLogical;
  wire  [31:0]  shiftResultArithmetic;
  wire  [31:0]  shiftResultRotate;

  shiftLogical shiftLogical_inst (
    .data ( shiftDataIn ),
    .direction ( shiftDirection ),
    .distance ( shiftDistance ),
    .result ( shiftResultLogical )
  );
  shiftArithmetic shiftArithmetic_inst (
    .data ( shiftDataIn ),
    .direction ( shiftDirection ),
    .distance ( shiftDistance ),
    .result ( shiftResultArithmetic )
  );
  shiftRotate shiftRotate_inst (
    .data ( shiftDataIn ),
    .direction ( shiftDirection ),
    .distance ( shiftDistance ),
    .result ( shiftResultRotate )
    );

  wire [31:0] mulDataA;
  assign mulDataA = regfileA;
  reg [31:0] mulDataB;
  wire [63:0] mulResultSigned;
  mulSigned  mulSigned_inst (
    .dataa  ( mulDataA ),
    .datab  ( mulDataB ),
    .result ( mulResultSigned )
  );
  
parameter MUL_DIV_DELAY = 5;
  
  reg irq_enable;
  reg [31:0] irq_addr;
  reg [7:0] exec_cnt;
  //assign debug32 = dsAddr;
  always @(posedge clk or negedge cpu_reset_n) begin
    if (!cpu_reset_n) begin
      cmd_ack<=0;
      //exec_address<=0;
      exec_read<=0;
      exec_write<=0;
      exec_writedata<=0;
       
      private_offset<=32'h0000_0000;
      pc<=32'h0200_0000;
      byteenable <= 4'b1111;
      exec_step<=0;
       
      halt_cpu<=0;
      
      irq_enable<=0;
      irq_addr<=0;
    end else begin
      if(cycle==1 && cmd_ack==0)begin
        regResultB <= 0;
        regResultC <= 0;
        regResultRA <= 0;
        
        case(RtypeCmd)
          //call sym                     @          22 @                      2 @   6 @          000110      0x06*
          7'b0000110 : begin//ok
            pc <= {pc[31:28],IMM26,2'b00};
            //regfile[31] <= nextpc; code is 31
            regResult <= nextpc;
            regResultRA <= 1;
            cmd_ack <= 1;
          end
          //jmpi sym                     @          21 @                      2 @  62 @          111110      0x3e*
          7'b0111110 : begin//ok
            pc <= {pc[31:28],IMM26,2'b00};
            cmd_ack <= 1;
          end
          //hlt ins                      @           5 @                      0 @   0 @          000000      0x00*
          7'b0000000 : begin//ok
            halt_cpu <= IMM16[0];
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //br sym                       @          20 @                      0 @   6 @          000110      0x06
          7'b0000110 : begin//ok
            pc <= nextpc + IMM16sx;
            cmd_ack <= 1;
          end
          //bne reg, reg, sym            @          15 @                      0 @  30 @          011110      0x1e bne rA, rB, label
          7'b0011110 : begin//ok
            if(comp_eq) begin
              pc <= nextpc;
            end else begin
              pc <= nextpc + IMM16sx;
            end
            cmd_ack <= 1;
          end
          //beq reg, reg, sym            @          15 @                      0 @  38 @          100110      0x26
          7'b0100110 : begin//ok
            if(comp_eq) begin
              pc <= nextpc + IMM16sx;
            end else begin
              pc <= nextpc;
            end
            cmd_ack <= 1;
          end
          //bge reg, reg, sym            @          15 @                      0 @  14 @          001110      0x0e
          7'b0001110 : begin//ok
            if(comp_ge) begin//if ((signed) rA >= (signed) rB)
              pc <= nextpc + IMM16sx;//then PC ← PC + 4 + σ(IMM16)
            end else begin
              pc <= nextpc;//else PC ← PC + 4
            end
            cmd_ack <= 1;
          end
          //blt reg, reg, sym            @          15 @                      0 @  22 @          010110      0x16
          7'b0010110 : begin//ok
            if(comp_lt) begin//if ((signed) rA < (signed) rB)
              pc <= nextpc + IMM16sx;//then PC ← PC + 4 + σ(IMM16)
            end else begin
              pc <= nextpc;//else PC ← PC + 4
            end
            cmd_ack <= 1;
          end
          //bltu reg, reg, sym           @          15 @                      0 @  54 @          110110      0x36
          7'b0110110 : begin//ok
            if(comp_ltu) begin//if ((unsigned) rA < (unsigned) rB) 
              pc <= nextpc + IMM16sx;//then PC ← PC + 4 + σ(IMM16)
            end else begin
              pc <= nextpc;//else PC ← PC + 4
            end
            cmd_ack <= 1;
          end
          //bgeu reg, reg, sym           @          15 @                      0 @  46 @          101110      0x2e
          7'b0101110 : begin//ok
            if(!comp_ltu) begin//if ((unsigned) rA >= (unsigned) rB) 
              pc <= nextpc + IMM16sx;//then PC ← PC + 4 + σ(IMM16)
            end else begin
              pc <= nextpc;//else PC ← PC + 4
            end
            cmd_ack <= 1;
          end
          //andi reg, reg, ins           @          10 @                      0 @  12 @          001100      0x0c andhi rB, rA, IMM16
          7'b0001100 : begin//ok
            regResult <= {16'b0, (regfileA[15:0] & IMM16)};//rB ← rA & (0x0000 : IMM16)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //andhi reg, reg, ins          @          10 @                      0 @  44 @          101100      0x2c
          7'b0101100 : begin//ok
            regResult <= {(regfileA[31:16] & IMM16), 16'b0};//rB ← rA & (IMM16 : 0x0000)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //addi reg, reg, ins           @          10 @                      0 @   4 @          000100      0x04
          7'b0000100 : begin//ok
            regResult <= regfileA + IMM16sx;//rB ← rA + σ(IMM16)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //orhi reg, reg, ins           @          10 @                      0 @  52 @          110100      0x34
          7'b0110100 : begin//ok
            regResult <= {(regfileA[31:16] | IMM16),regfileA[15:0]};//rB ← rA | (IMM16 : 0x0000)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //ori  reg, reg, ins           @          10 @                      0 @  20 @          010100      0x14
          7'b0010100 : begin//ok
            regResult <= {regfileA[31:16],(regfileA[15:0] | IMM16)};//rB ← rA | (0x0000 : IMM16)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //xori reg, reg, ins           @          10 @                      0 @  28 @          011100      0x1c
          7'b0011100 : begin//ok
            regResult <= {regfileA[31:16], (regfileA[15:0] ^ IMM16)};//rB ← rA ^ (0x0000 : IMM16)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //xorhi reg, reg, ins          @          10 @                      0 @  60 @          111100      0x3c
          7'b0111100 : begin//ok
            regResult <= {(regfileA[31:16] ^ IMM16), regfileA[15:0]};//rB ← rA ^ (IMM16 : 0x0000)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //muli reg, reg, ins           @          10 @                      0 @  36 @          100100      0x24
          7'b0100100 : begin//ok
            if         (exec_step==0)begin
              exec_cnt <= 0;
              mulDataB <= IMM16sx;
              exec_step <= 1;
            end else if(exec_step==1)begin
              exec_cnt <= exec_cnt + 1'b1;
              if(exec_cnt == MUL_DIV_DELAY)begin//50Mhz
                // shift left logical immediate
                regResult <= mulResultSigned[31:0];
                regResultB <= 1;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          end
          //cmpeqi reg, reg, ins         @          10 @                      0 @  32 @          100000      0x20
          7'b0100000 : begin//ok
            regResult <= {31'b0,(regfileA == IMM16sx)};//if (rA == σ(IMM16)) then rB ← 1 else rB ← 0
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //cmplti reg, reg, ins         @          10 @                      0 @  16 @          010000      0x10
          7'b0010000 : begin//ok
            regResult <= {31'b0,($signed(regfileA) < $signed(IMM16sx))};//if ((signed) rA < (signed) σ(IMM16))
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //cmpltui reg, reg, ins        @          10 @                      0 @  48 @          110000      0x30
          7'b0110000 : begin//ok
            regResult <= {31'b0,(regfileA < {16'b0,IMM16})};//if ((unsigned) rA < (unsigned) (0x0000 : IMM16)) then rB ← 1 else rB ← 0
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //cmpgei reg, reg, ins         @          10 @                      0 @   8 @          001000      0x08
          7'b0001000 : begin//ok
            regResult <= {31'b0,($signed(regfileA) >= $signed(IMM16sx))};//if ((signed) rA >= (signed) σ(IMM16)) then rB ← 1 else rB ← 0
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //cmpnei reg, reg, ins         @          10 @                      0 @  24 @          011000      0x18
          7'b0011000 : begin//ok
            regResult <= {31'b0,(regfileA != IMM16sx)};//if (rA != σ(IMM16)) then rB ← 1 else rB ← 0
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //cmpgeui reg, reg, ins        @          10 @                      0 @  40 @          101000      0x28
          7'b0101000 : begin//ok
            regResult <= {31'b0,(regfileA >= IMM16zx)};//if ((unsigned) rA >= (unsigned) (0x0000 : IMM16)) then rB ← 1 else rB ← 0
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //ldw reg, regins              @          11 @                      0 @   7 @          000111      0x07 ldw rB, byte_offset(rA)
          7'b0000111 : begin//ok
            if         (exec_step==0)begin
              exec_step <= 1;
              //exec_address <= {dsAddr[31:2],2'b0};//rB ← Mem32[rA + σ(IMM16)]
              exec_read <= 1;
            end else if(exec_step==1)begin
              if(!avm_m0_waitrequest)begin
                regResult <= avm_m0_readdata;
                regResultB <= 1;
                exec_read <= 0;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          end
          //stw reg, regins              @          11 @                      0 @  15 @          001111      0x0f stw rB, byte_offset(rA)
          7'b0001111 : begin//ok
            if         (exec_step==0)begin
              exec_step <= 1;
              //exec_address <= {dsAddr[31:2],2'b0};//Mem32[rA + σ(IMM16)] ← rB
              exec_writedata <= regfileB;
              exec_write <= 1;
            end else if(exec_step==1)begin
              if(!avm_m0_waitrequest)begin
                exec_write <= 0;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          end
          //ldbu reg, regins             @          11 @                      0 @  23 @          010111      0x17
          7'b0010111 : begin//ok
            if         (exec_step==0)begin
              exec_step <= 1;
              //exec_address <= {dsAddr[31:2],2'b0};//rB ← 0x000000 : Mem8[rA + σ(IMM16)]
              exec_read <= 1;
            end else if(exec_step==1)begin
              if(!avm_m0_waitrequest)begin
                case(exec_address[1:0])
                  0:begin regResult <= {24'b0,avm_m0_readdata[ 7: 0]};end
                  1:begin regResult <= {24'b0,avm_m0_readdata[15: 8]};end
                  2:begin regResult <= {24'b0,avm_m0_readdata[23:16]};end
                  3:begin regResult <= {24'b0,avm_m0_readdata[31:24]};end
                endcase
                regResultB <= 1;
                exec_read <= 0;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          end
          //ldb reg, regins              @          11 @                      0 @  31 @          011111      0x1f*
          7'b0011111 : begin//ok
            if         (exec_step==0)begin
              exec_step <= 1;
              //exec_address <= {dsAddr[31:2],2'b0};//rB ← 0x000000 : Mem8[rA + σ(IMM16)]
              exec_read <= 1;
            end else if(exec_step==1)begin
              if(!avm_m0_waitrequest)begin
                case(exec_address[1:0])
                  0:begin regResult <= {{24{avm_m0_readdata[ 7]}},avm_m0_readdata[ 7: 0]};end
                  1:begin regResult <= {{24{avm_m0_readdata[15]}},avm_m0_readdata[15: 8]};end
                  2:begin regResult <= {{24{avm_m0_readdata[23]}},avm_m0_readdata[23:16]};end
                  3:begin regResult <= {{24{avm_m0_readdata[31]}},avm_m0_readdata[31:24]};end
                endcase
                regResultB <= 1;
                exec_read <= 0;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          end
          //ldhu reg, regins             @          11 @                      0 @  39 @          100111      0x27
          7'b0100111 : begin//ok
            if         (exec_step==0)begin
              exec_step <= 1;
              //exec_address <= {dsAddr[31:2],2'b0};//rB ← 0x0000 : Mem16[rA + σ(IMM16)]
              exec_read <= 1;
            end else if(exec_step==1)begin
              if(!avm_m0_waitrequest)begin
                case(exec_address[1])
                  0:begin regResult <= {16'b0,avm_m0_readdata[15: 0]};end
                  1:begin regResult <= {16'b0,avm_m0_readdata[31:16]};end
                endcase
                regResultB <= 1;
                exec_read <= 0;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          end
          //ldh reg, regins              @          11 @                      0 @  47 @          101111      0x2f
          7'b0101111 : begin//ok
            if         (exec_step==0)begin
              exec_step <= 1;
              //exec_address <= {dsAddr[31:2],2'b0};//rB ← σ(Mem16[rA + σ(IMM16)])
              exec_read <= 1;
            end else if(exec_step==1)begin
              if(!avm_m0_waitrequest)begin
                case(exec_address[1])
                  0:begin regResult <= {{16{avm_m0_readdata[15]}},avm_m0_readdata[15: 0]};end
                  1:begin regResult <= {{16{avm_m0_readdata[31]}},avm_m0_readdata[31:16]};end
                endcase
                regResultB <= 1;
                exec_read <= 0;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          end
          //stb reg, regins              @          11 @                      0 @  55 @          110111      0x37*
          7'b0110111 : begin//ok
            if         (exec_step==0)begin
              exec_step <= 1;
              //exec_address <= {dsAddr[31:2],2'b0};//Mem8[rA + σ(IMM16)] ← rB7..0
              case(exec_address[1:0])
                0:begin byteenable <= 4'b0001; exec_writedata <= {24'b0,regfileB[7:0]};end
                1:begin byteenable <= 4'b0010; exec_writedata <= {16'b0,regfileB[7:0],8'b0};end
                2:begin byteenable <= 4'b0100; exec_writedata <= {8'b0,regfileB[7:0],16'b0};end
                3:begin byteenable <= 4'b1000; exec_writedata <= {regfileB[7:0],24'b0};end
              endcase
              exec_write <= 1;
            end else if(exec_step==1)begin
              if(!avm_m0_waitrequest)begin
                byteenable <= 4'b1111;
                exec_write <= 0;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          end
          //sth reg, regins              @          11 @                      0 @  63 @          111111      0x3f*
          7'b0111111 : begin//ok
            if         (exec_step==0)begin
              exec_step <= 1;
              //exec_address <= {dsAddr[31:2],2'b0};//Mem8[rA + σ(IMM16)] ← rB7..0
              case(exec_address[1])
                0:begin byteenable <= 4'b0011; exec_writedata <= {16'b0,regfileB[15:0]};end
                1:begin byteenable <= 4'b1100; exec_writedata <= {regfileB[15:0],16'b0};end
              endcase
              exec_write <= 1;
            end else if(exec_step==1)begin
              if(!avm_m0_waitrequest)begin
                byteenable <= 4'b1111;
                exec_write <= 0;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          end
          //ret                          @           1 @                      1 @   5 @          000101      0x3A,0x05
          7'b1000101 : begin
            pc <= regfileA;//regA == 31
            cmd_ack <= 1;
          end
          //reti                         @           1 @                      1 @  21 @          010101      0x3A,0x15*
          7'b1010101 : begin//ok
            pc <= regfileA;
            irq_enable <= 1;
            cmd_ack <= 1;
          end
          //#irqcall                     @         xxx @                     xx @  37 @          100101      0x3A,0x25*
          7'b1100101 : begin//ok
            pc <= irq_addr;
            //regfile[31] <= nextpc; code is 31
            regResult <= pc;
            irq_enable <= 0;
            regResultRA <= 1;
            cmd_ack <= 1;
          end
          //stoff reg                    @          50 @                      1 @  10 @          001010      0x3A,0x0A*
          7'b1001010 : begin//ok
            private_offset <= regfileA;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //callr reg                    @          50 @                      1 @  29 @          011101      0x3A,0x1d
          7'b1011101 : begin//ok
            pc <= regfileA;
            regResult <= pc + 4;//rC ← rA + rB
            regResultC <= 1;// c is ra
            cmd_ack <= 1;
          end
          //jmp reg                      @          50 @                      1 @  13 @          001101      0x3A,0x0d
          7'b1001101 : begin//ok
            pc <= regfileA;
            cmd_ack <= 1;
          end
          //add reg, reg, reg            @          30 @                      1 @  49 @          110001      0x3A,0x31 add rC, rA, rB
          7'b1110001 : begin
            regResult <= regfileA + regfileB;//rC ← rA + rB
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //sub reg, reg, reg            @          30 @                      1 @  57 @          111001      0x3A,0x39
          7'b1111001 : begin
            regResult <= regfileA - regfileB;// rC ← rA – rB
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //and reg, reg, reg            @          30 @                      1 @  14 @          001110      0x3A,0x0e
          7'b1001110 : begin
            regResult <= regfileA & regfileB;// rC ← rA | rB
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //or reg, reg, reg             @          30 @                      1 @  22 @          010110      0x3A,0x16
          7'b1010110 : begin
            regResult <= regfileA | regfileB;// rC ← rA | rB
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //nor reg, reg, reg            @          30 @                      1 @   6 @          000110      0x3A,0x06
          7'b1000110 : begin
            regResult <= ~(regfileA | regfileB);// rC ← ~(rA | rB)
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //xor reg, reg, reg            @          30 @                      1 @  30 @          011110      0x3A,0x1e
          7'b1011110 : begin
            regResult <= regfileA ^ regfileB;// rC ← rA ^ rB
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //sll reg, reg, reg            @          30 @                      1 @  19 @          010011      0x3A,0x13
          7'b1010011 : begin
            if         (exec_step==0)begin
              shiftDirection <= 0;//0:left 1:right
              shiftDistance <= regfileB[4:0];//rC ← rA << (rB4..0)
              exec_step <= 1;
            end else if(exec_step==1)begin
              // shift left logical
              regResult <= shiftResultLogical;
              regResultC <= 1;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end
          //sra reg, reg, reg            @          30 @                      1 @  59 @          111011      0x3A,0x3b
          7'b1111011 : begin
            if         (exec_step==0)begin
              shiftDirection <= 1;//0:left 1:right
              shiftDistance <= regfileB[4:0];//rC ← (signed) rA >> ((unsigned) rB4..0)
              exec_step <= 1;
            end else if(exec_step==1)begin
              // shift right arithmetic
              regResult <= shiftResultArithmetic;
              regResultC <= 1;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end
          //rol reg, reg, reg            @          30 @                      1 @   3 @          000011      0x3A,0x03
          7'b1000011 : begin
            if         (exec_step==0)begin
              shiftDirection <= 0;//0:left 1:right
              shiftDistance <= regfileB[4:0];//rC ← rA rotated left rB4..0 bit positions
              exec_step <= 1;
            end else if(exec_step==1)begin
              // rotate left
              regResult <= shiftResultRotate;
              regResultC <= 1;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end
          //srl reg, reg, reg            @          30 @                      1 @  27 @          011011      0x3A,0x1b
          7'b1011011 : begin
            if         (exec_step==0)begin
              shiftDirection <= 1;//0:left 1:right
              shiftDistance <= regfileB[4:0];//rC ← (unsigned) rA >> ((unsigned) rB4..0)
              exec_step <= 1;
            end else if(exec_step==1)begin
              // shift right logical
              regResult <= shiftResultLogical;
              regResultC <= 1;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end
          //mul reg, reg, reg            @          30 @                      1 @  39 @          100111      0x3A,0x27
          7'b1100111 : begin//ok
            if         (exec_step==0)begin
              exec_cnt <= 0;
              mulDataB <= regfileB;
              exec_step <= 1;
            end else if(exec_step==1)begin//50Mhz
              exec_cnt <= exec_cnt + 1'b1;
              if(exec_cnt == MUL_DIV_DELAY)begin//50Mhz
                regResult <= mulResultSigned[31:0];
                regResultC <= 1;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          end

          //setirq reg, reg, ins         @          40 @                      1 @  34 @          100010      0x3A,0x22* ins=0,1
          7'b1100010 : begin//ok
            irq_enable <= IMM5[0];//rB ← rA & (0x0000 : IMM16)
            irq_addr <= regfileA;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //cmpeq reg, reg, reg          @          30 @                      1 @  32 @          100000      0x3A,0x20
          7'b1100000 : begin
            regResult <= {31'b0,comp_eq};//if (rA == rB) then rC ← 1 else rC ← 0
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //cmpne reg, reg, reg          @          30 @                      1 @  24 @          011000      0x3A,0x18
          7'b1011000 : begin
            regResult <= {31'b0,(~comp_eq)};//if (rA != rB) then rC ← 1 else rC ← 0
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //cmpge reg, reg, reg          @          30 @                      1 @   8 @          001000      0x3A,0x08
          7'b1001000 : begin
            regResult <= {31'b0,(comp_ge)};// if ((signed) rA >= (signed) rB) then rC ← 1  else rC ← 0
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //cmplt reg, reg, reg          @          30 @                      1 @  16 @          010000      0x3A,0x10
          7'b1010000 : begin
            regResult <= {31'b0,(~comp_ge)};//if ((signed) rA < (signed) rB) then rC ← 1 else rC ← 0
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //cmpltu reg, reg, reg         @          30 @                      1 @  48 @          110000      0x3A,0x30
          7'b1110000 : begin
            regResult <= {31'b0,(comp_ltu)};//if ((unsigned) rA < (unsigned) rB) then rC ← 1 else rC ← 0
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
          //slli reg, reg, ins           @          40 @                      1 @  18 @          010010      0x3A,0x12
          7'b1010010 : begin
            // rC ← rA << IMM5
            if         (exec_step==0)begin
              shiftDirection <= 0;//0:left 1:right
              shiftDistance <= IMM5;
              exec_step <= 1;
            end else if(exec_step==1)begin
              // shift left logical immediate
              regResult <= shiftResultLogical;
              regResultC <= 1;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end
          //srai reg, reg, ins           @          40 @                      1 @  58 @          111010      0x3A,0x3a
          7'b1111010 : begin
            // rC ← (signed) rA >> ((unsigned) IMM5)
            if         (exec_step==0)begin
              shiftDirection <= 1;//0:left 1:right
              shiftDistance <= IMM5;
              exec_step <= 1;
            end else if(exec_step==1)begin
              // shift left logical immediate
              regResult <= shiftResultArithmetic;
              regResultC <= 1;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end
          //srli reg, reg, ins           @          40 @                      1 @  26 @          011010      0x3A,0x1a
          7'b1011010 : begin
            // rC ← (unsigned) rA >> ((unsigned) IMM5)
            if         (exec_step==0)begin
              shiftDirection <= 1;//0:left 1:right
              shiftDistance <= IMM5;
              exec_step <= 1;
            end else if(exec_step==1)begin
              // shift left logical immediate
              regResult <= shiftResultLogical;
              regResultC <= 1;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end
          default : begin
            halt_cpu <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end

        endcase
      end
      
      if(cycle==0 && cmd_ack==1)begin
        cmd_ack<=0;
      end
    end
  end


endmodule

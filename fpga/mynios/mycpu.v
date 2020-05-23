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
      end else if (command == 8'h18) begin uart_send<=1; uart_data_in<=Cmd; command_done<=1;
      end else if (command == 8'h19) begin uart_send<=1; uart_data_in<=Cmd3; command_done<=1;
      
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
      end else if (command == 8'h64) begin uart_send<=1; uart_data_in<=debug_flgw; command_done<=1;//debugin8



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
      end else if (command == 8'h45) begin debug_data<=pcResult; command_done<=1;
      
      end else if (command == 8'h47) begin
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          debug_reg <= data[4:0];
        end else if(debug_readmem_step==1)begin
          debug_data <= regDataOutA;
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
  //{pc冲突时，停止取指令} 条件：流水线里有PC指令
  //{总线冲突时，停止取指令}（mem 指令）
  //{寄存器冲突时，发送气泡} （变更的寄存器）
  //2，取指令-
  
  wire [7:0] debug_flgw = {stage1_Valid,stage2_Valid,stage3_Valid,exec_write,halt_accept,controlByExec};
  reg [7:0] debug_flg;
  
  //全局信号： Valid PCChange MEMAccess CMP CMDALU1 CMDALU2 [4:0]regChange
  reg [5:0]  Cmd;
  reg [2:0]  Cmd3;
  
  wire cpu_reset_n = reset_n && debug_reset_n;
  wire halt = (halt_cpu || halt_uart) && (debug_step == debug_step_buff);

  wire controlByExec = stage2_Valid && stage2_MEMAccess;

  wire [31:0] fetch_address = pc;
  
  wire [31:0] affect_address    = halt_accept == 1   ? debug_address                                : ((!controlByExec) ? fetch_address : {exec_address[31:2],2'b0});
  assign avm_m0_address         = affect_address[31] ? {1'b0,affect_address[30:0]} + private_offset : affect_address;
  assign avm_m0_writedata       = halt_accept == 1   ? debug_writedata                              : exec_writedata;
  assign avm_m0_byteenable      = halt_accept == 1   ? debug_byteenable                             : byteenable;
  assign avm_m0_write           = halt_accept == 1   ? debug_write                                  : exec_write;
  assign avm_m0_read            = halt_accept == 1   ? debug_read                                   : ((!controlByExec) ? fetch_read : exec_read);

  reg [31:0] pc;
  wire [31:0] nextpc = pc + 4;
  reg [31:0] latch_readdata;
  reg        irq_req_buff;
  reg        fetch_read;
  reg  [1:0] fetch_status;
  reg        debug_step_buff;
  reg        halt_accept;
  always @(posedge clk or negedge cpu_reset_n) begin
    if (!cpu_reset_n) begin
      //stage1寄存器初始化
      stage1_Valid <= 0;
      stage1_PCChange <= 0;
      stage1_MEMAccess <= 0;
      debug_flg <= 0;
      fetch_status <= 0;
      pc<=32'h0200_0000;
      irq_req_buff <= 0;
      debug_step_buff <= 0;
    end else begin
      irq_req_buff = irq_req;
    
      //执行单元mem权限高于fetch
      //流水线里有pc，mem指令时，fetch关闭
      stage1_Valid <= 0;
      if(
        (stage1_Valid && (stage1_PCChange || stage1_MEMAccess)) ||
        (stage2_Valid && (stage2_PCChange || stage2_MEMAccess)) ||
        (stage3_Valid && (stage3_PCChange))//stage3_MEMAccess 这个可能可以去掉
      )begin
      end else begin
        if         (fetch_status==0)begin
          if(halt)begin
            halt_accept <= 1;
          end else begin
            debug_step_buff <= debug_step;
            halt_accept <= 0;
            if(irq_req_buff && irq_enable)begin
              stage1_Valid <= 1;
              stage1_PCChange <= 1;
              stage1_PCChangeCmd <= 3'b111;
              stage1_PCChangeCmdExt <= 3'b111;
              stage1_MEMAccess <= 0;
              stage1_CMP <= 0;
              stage1_ALU1 <= 0;
              stage1_ALU2 <= 0;
              fetch_status <= 3;
            end else begin
              debug_flg[0] <= 1;
              fetch_read <= 1;
              fetch_status <= 1;
            end
          end
        end else if(fetch_status==1)begin//wait fetch
          if(!avm_m0_waitrequest)begin
            fetch_read <= 0;
            debug_flg[1] <= 1;
            //发送指令前判断寄存器冲突
            if(
                (stage1_Valid && stage1_regChange != 5'b0 && (stage1_regChange == avm_m0_readdata[31:27] || stage1_regChange == avm_m0_readdata[26:22])) || //regA regB
                (stage2_Valid && stage2_regChange != 5'b0 && (stage2_regChange == avm_m0_readdata[31:27] || stage2_regChange == avm_m0_readdata[26:22])) || //regA regB
                (stage3_Valid && stage3_regChange != 5'b0 && (stage3_regChange == avm_m0_readdata[31:27] || stage3_regChange == avm_m0_readdata[26:22]))    //regA regB
            )begin
              latch_readdata <= avm_m0_readdata;
              fetch_status <= 2;
              debug_flg[2] <= 1;
            end else begin

              stage1_Valid <= 1;
              
              stage1_CMP <= 0;
              stage1_ALU1 <= 0;
              stage1_ALU2 <= 0;
              stage1_MEMAccess <= 0;
              stage1_PCChange <= 0;
              //AAAAABBBBBiimm1616iimm161600000
              //regA regB imm16             cmd
              //AAAAABBBBBCCCCC1111111111100000
              //          regC ext
              //imm26
              stage1_regA <= avm_m0_readdata[31:27];
              stage1_regB <= avm_m0_readdata[26:22];
              stage1_regC <= avm_m0_readdata[21:17];
              stage1_IMM8 <= avm_m0_readdata[13:6];
              stage1_IMM16 = avm_m0_readdata[21:6];
              stage1_IMM26 <= avm_m0_readdata[31:6];
              stage1_IMM16sx <= {{16{stage1_IMM16[15]}},stage1_IMM16};
              stage1_IMM16zx <= {16'b0,stage1_IMM16};

              Cmd <= avm_m0_readdata[5:0];
              Cmd3 <= avm_m0_readdata[16:14];
              
              case(avm_m0_readdata[2:0])
              3'b000 : begin
                stage1_CMP <= 1;
                stage1_CMPCmd <= avm_m0_readdata[5:2];
                stage1_regChange <= avm_m0_readdata[26:22];//regB
                fetch_status <= 0;
                pc <= nextpc;
              end
              3'b100 : begin
                stage1_CMP <= 1;
                stage1_CMPCmd <= avm_m0_readdata[5:2];
                stage1_regChange <= avm_m0_readdata[21:17];//regC
                fetch_status <= 0;
                pc <= nextpc;
              end
              
              3'b001 : begin
                stage1_ALU1 <= 1;
                stage1_ALU1Cmd <= avm_m0_readdata[5:2];
                stage1_regChange <= avm_m0_readdata[26:22];//regB
                fetch_status <= 0;
                pc <= nextpc;
              end
              3'b101 : begin
                stage1_ALU1 <= 1;
                stage1_ALU1Cmd <= avm_m0_readdata[5:2];
                stage1_regChange <= avm_m0_readdata[21:17];//regC
                fetch_status <= 0;
                pc <= nextpc;
              end

              3'b010 : begin
                stage1_ALU2 <= 1;
                stage1_ALU2Cmd <= avm_m0_readdata[5:2];
                stage1_regChange <= avm_m0_readdata[26:22];//regB
                fetch_status <= 0;
                pc <= nextpc;
              end
              3'b110 : begin
                stage1_ALU2 <= 1;
                stage1_ALU2Cmd <= avm_m0_readdata[5:2];
                stage1_regChange <= avm_m0_readdata[21:17];//regC
                fetch_status <= 0;
                pc <= nextpc;
              end

              3'b011 : begin
                stage1_MEMAccess <= 1;
                stage1_MEMAccessCmd <= avm_m0_readdata[5:3];
                stage1_regChange <= 0;//MEM的情况下，后续执行逻辑会再变更这个字段
                fetch_status <= 0;
                pc <= nextpc;
              end
              
              3'b111 : begin
                stage1_PCChange <= 1;
                stage1_PCChangeCmd <= avm_m0_readdata[5:3];
                stage1_PCChangeCmdExt <= avm_m0_readdata[16:14];
                stage1_regChange <= 0;//PC的情况下，后续执行逻辑会再变更这个字段
                fetch_status <= 3;//update pc
                debug_flg[3] <= 1;
              end
              endcase
            end


          end

        end else if(fetch_status==2)begin//
          //同1
          if(
              (stage1_Valid && stage1_regChange != 5'b0 && (stage1_regChange == latch_readdata[31:27] || stage1_regChange == latch_readdata[26:22])) || //regA regB
              (stage2_Valid && stage2_regChange != 5'b0 && (stage2_regChange == latch_readdata[31:27] || stage2_regChange == latch_readdata[26:22])) || //regA regB
              (stage3_Valid && stage3_regChange != 5'b0 && (stage3_regChange == latch_readdata[31:27] || stage3_regChange == latch_readdata[26:22]))    //regA regB
          )begin
            fetch_status <= 2;
          end else begin

            stage1_Valid <= 1;
            
            stage1_CMP <= 0;
            stage1_ALU1 <= 0;
            stage1_ALU2 <= 0;
            stage1_MEMAccess <= 0;
            stage1_PCChange <= 0;
            //AAAAABBBBBiimm1616iimm161600000
            //regA regB imm16             cmd
            //AAAAABBBBBCCCCC1111111111100000
            //          regC ext
            //imm26
            stage1_regA <= latch_readdata[31:27];
            stage1_regB <= latch_readdata[26:22];
            stage1_regC <= latch_readdata[21:17];
            stage1_IMM8 <= latch_readdata[13:6];
            stage1_IMM16 = latch_readdata[21:6];
            stage1_IMM26 <= latch_readdata[31:6];
            stage1_IMM16sx <= {{16{stage1_IMM16[15]}},stage1_IMM16};
            stage1_IMM16zx <= {16'b0,stage1_IMM16};
            
            Cmd <= latch_readdata[5:0];
            Cmd3 <= latch_readdata[16:14];
            
            case(latch_readdata[2:0])
            3'b000 : begin
              stage1_CMP <= 1;
              stage1_CMPCmd <= latch_readdata[5:2];
              stage1_regChange <= latch_readdata[26:22];//regB
              fetch_status <= 0;
              pc <= nextpc;
            end
            3'b100 : begin
              stage1_CMP <= 1;
              stage1_CMPCmd <= latch_readdata[5:2];
              stage1_regChange <= latch_readdata[21:17];//regC
              fetch_status <= 0;
              pc <= nextpc;
            end
            
            3'b001 : begin
              stage1_ALU1 <= 1;
              stage1_ALU1Cmd <= latch_readdata[5:2];
              stage1_regChange <= latch_readdata[26:22];//regB
              fetch_status <= 0;
              pc <= nextpc;
            end
            3'b101 : begin
              stage1_ALU1 <= 1;
              stage1_ALU1Cmd <= latch_readdata[5:2];
              stage1_regChange <= latch_readdata[21:17];//regC
              fetch_status <= 0;
              pc <= nextpc;
            end

            3'b010 : begin
              stage1_ALU2 <= 1;
              stage1_ALU2Cmd <= latch_readdata[5:2];
              stage1_regChange <= latch_readdata[26:22];//regB
              fetch_status <= 0;
              pc <= nextpc;
            end
            3'b110 : begin
              stage1_ALU2 <= 1;
              stage1_ALU2Cmd <= latch_readdata[5:2];
              stage1_regChange <= latch_readdata[21:17];//regC
              fetch_status <= 0;
              pc <= nextpc;
            end

            3'b011 : begin
              stage1_MEMAccess <= 1;
              stage1_MEMAccessCmd <= latch_readdata[5:3];
              stage1_regChange <= 0;//MEM的情况下，后续执行逻辑会再变更这个字段
              fetch_status <= 0;
              pc <= nextpc;
            end
            
            3'b111 : begin
              stage1_PCChange <= 1;
              stage1_PCChangeCmd <= latch_readdata[5:3];
              stage1_PCChangeCmdExt <= latch_readdata[16:14];
              stage1_regChange <= 0;//PC的情况下，后续执行逻辑会再变更这个字段
              fetch_status <= 3;//update pc
            end
            endcase
          end

        end else if(fetch_status==3)begin//wait rw
          debug_flg[4] <= 1;
          pc <= pcResult;
          fetch_status <= 0;
        end
      end
    end
  end
  reg        stage1_Valid;
  reg        stage1_PCChange;
  reg [2:0]  stage1_PCChangeCmd;
  reg [2:0]  stage1_PCChangeCmdExt;//cmd3
  reg        stage1_MEMAccess;
  reg [2:0]  stage1_MEMAccessCmd;
  reg        stage1_CMP;
  reg [3:0]  stage1_CMPCmd;
  reg        stage1_ALU1;
  reg [3:0]  stage1_ALU1Cmd;
  reg        stage1_ALU2;
  reg [3:0]  stage1_ALU2Cmd;
  reg [4:0]  stage1_regChange;
  reg [4:0]  stage1_regA;
  reg [4:0]  stage1_regB;
  reg [4:0]  stage1_regC;
  reg [7:0]  stage1_IMM8;
  reg [15:0] stage1_IMM16;
  reg [31:0] stage1_IMM16zx;
  reg [31:0] stage1_IMM16sx;
  reg [25:0] stage1_IMM26;
  //////////////////////////////////////////////////////////
  reg [31:0] regfile[32];
  wire [4:0] reg_addr = (halt_accept == 1) ? debug_reg : stage1_regA;
  reg  [31:0] regDataIn;
  wire [31:0] regDataOutA = regfile[reg_addr];
  wire [31:0] regDataOutB = regfile[stage1_regB];

  //////////////////////////////////////////////////////////
  
  
  //3，stage1 取寄存器/乘法移位预处理
  always @(posedge clk or negedge cpu_reset_n) begin
    if (!cpu_reset_n) begin
      //stage2寄存器初始化
    end else begin
      if(!holdStage)begin
        stage2_Valid <= 0;
        if(stage1_Valid)begin
          stage2_Valid <= 1;
          stage2_PCChange       <= stage1_PCChange;
          stage2_PCChangeCmd    <= stage1_PCChangeCmd;
          stage2_PCChangeCmdExt <= stage1_PCChangeCmdExt;
          stage2_MEMAccess      <= stage1_MEMAccess;
          stage2_MEMAccessCmd   <= stage1_MEMAccessCmd;
          stage2_CMP            <= stage1_CMP;
          stage2_CMPCmd         <= stage1_CMPCmd;
          stage2_ALU1           <= stage1_ALU1;
          stage2_ALU1Cmd        <= stage1_ALU1Cmd;
          stage2_ALU2           <= stage1_ALU2;
          stage2_ALU2Cmd        <= stage1_ALU2Cmd;
          stage2_regChange      <= stage1_regChange;
          //stage2_regA           <= stage1_regA;
          stage2_regB           <= stage1_regB;
          stage2_regC           <= stage1_regC;
          stage2_IMM8           <= stage1_IMM8;
          stage2_IMM16          <= stage1_IMM16;
          stage2_IMM16zx        <= stage1_IMM16zx;
          stage2_IMM16sx        <= stage1_IMM16sx;
          stage2_IMM26          <= stage1_IMM26;

          stage2_regfileA       <= regDataOutA;
          stage2_regfileB       <= regDataOutB;
          
            //muli reg, reg, ins           @          10 @                      0 @   2 @          000001      0x02*
            //6'b000001 : begin//ok
            //mul reg, reg, reg            @          30 @                      3 @  000101
            //6'b000101 : begin//ok
          if(stage1_ALU1)begin
            if(stage1_ALU1Cmd[0]==0)begin
              mulDataB <= stage1_IMM16sx;
            end else begin
              mulDataB <= regDataOutB;
            end
          end

          case(stage1_ALU1Cmd)
            //slli reg, reg, ins           @          10 @                      0 @   9 @          0010 01      0x09
            4'b0010 : begin
              shiftDirection <= 0;//0:left 1:right
              shiftDistance <= stage1_IMM16[4:0];
            end
            //srai reg, reg, ins           @          10 @                      0 @  17 @          0100 01      0x11
            4'b0100 : begin
              shiftDirection <= 1;//0:left 1:right
              shiftDistance <= stage1_IMM16[4:0];
            end
            //srli reg, reg, ins           @          10 @                      0 @  25 @          011001      0x19
            4'b0110 : begin
              shiftDirection <= 1;//0:left 1:right
              shiftDistance <= stage1_IMM16[4:0];
            end
            //roli reg, reg, ins           @          10 @                      0 @  33 @          100001      0x21
            4'b1000 : begin
              shiftDirection <= 0;//0:left 1:right
              shiftDistance <= stage1_IMM16[4:0];//rC ← rA rotated left rB4..0 bit positions
            end
            //sll reg, reg, reg            @          30 @                      3 @  001101
            4'b0011 : begin
              shiftDirection <= 0;//0:left 1:right
              shiftDistance <= regDataOutB[4:0];//rC ← rA << (rB4..0)
            end
            //sra reg, reg, reg            @          30 @                      1 @  010101
            4'b0101 : begin
              shiftDirection <= 1;//0:left 1:right
              shiftDistance <= regDataOutB[4:0];//rC ← (signed) rA >> ((unsigned) rB4..0)
            end
            //srl reg, reg, reg            @          30 @                      3 @  011101
            4'b0111 : begin
              shiftDirection <= 1;//0:left 1:right
              shiftDistance <= regDataOutB[4:0];//rC ← (unsigned) rA >> ((unsigned) rB4..0)
            end
            //rol reg, reg, reg            @          30 @                      3 @  100101
            4'b1001 : begin
              shiftDirection <= 0;//0:left 1:right
              shiftDistance <= regDataOutB[4:0];//rC ← rA rotated left rB4..0 bit positions
            end
          endcase
          
        end
      end
    end
  end
  reg        stage2_Valid;
  reg        stage2_PCChange;
  reg [2:0]  stage2_PCChangeCmd;
  reg [2:0]  stage2_PCChangeCmdExt;//cmd3
  reg        stage2_MEMAccess;
  reg [2:0]  stage2_MEMAccessCmd;
  reg        stage2_CMP;
  reg [3:0]  stage2_CMPCmd;
  reg        stage2_ALU1;
  reg [3:0]  stage2_ALU1Cmd;
  reg        stage2_ALU2;
  reg [3:0]  stage2_ALU2Cmd;
  reg [4:0]  stage2_regChange;
  //reg [4:0]  stage2_regA;
  reg [4:0]  stage2_regB;
  reg [4:0]  stage2_regC;
  reg [7:0]  stage2_IMM8;
  reg [15:0] stage2_IMM16;
  reg [31:0] stage2_IMM16zx;
  reg [31:0] stage2_IMM16sx;
  reg [25:0] stage2_IMM26;
  reg [31:0] stage2_regfileA;
  reg [31:0] stage2_regfileB;
  reg        shiftDirection;
  reg  [4:0] shiftDistance;
  reg [31:0] mulDataB;
  //其他计算好的前级
  
  //////////////////////////////////////////////////////////
  wire [31:0] shiftDataIn;
  assign shiftDataIn = stage2_regfileA;
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

  wire [31:0] mulDataA = stage2_regfileA;
  wire [63:0] mulResultSigned;
  mulSigned  mulSigned_inst (
    .dataa  ( mulDataA ),
    .datab  ( mulDataB ),
    .result ( mulResultSigned )
  );
  //////////////////////////////////////////////////////////
  
  //4，stage2 执行
  wire       holdStage = stage2_Valid && stage2_MEMAccess && !(exec_step==1 && !avm_m0_waitrequest);

  reg        exec_step;
  //wire [31:0] dsAddr = ds + stage2_regfileA + IMM16sx;
  wire [31:0] exec_address = stage2_regfileA + stage2_IMM16sx;
  reg        exec_read;
  reg        exec_write;
  reg [31:0] exec_writedata;
  reg  [3:0] byteenable;

  reg        irq_enable;
  reg [31:0] irq_addr;
  reg [31:0] private_offset;
  reg        halt_cpu;

  wire comp_eq = stage2_regfileA==stage2_regfileB;
  wire comp_ge = $signed(stage2_regfileA)>=$signed(stage2_regfileB);
  wire comp_lt = $signed(stage2_regfileA)<$signed(stage2_regfileB);
  wire comp_ltu = stage2_regfileA < stage2_regfileB;

  always @(posedge clk or negedge cpu_reset_n) begin
    if (!cpu_reset_n) begin
      //stage3寄存器初始化
      stage3_Valid <= 0;
      stage3_regChange <= 0;
      stage3_regResult <= 0;
      pcResult <= 0;
      exec_step <= 0;
      byteenable <= 4'b1111;
      exec_read <= 0;
      exec_write <= 0;
      irq_enable <= 0;
      halt_cpu <= 0;
    end else begin
      stage3_Valid <= 0;
      if(stage2_Valid)begin
        stage3_Valid <= 1;
        stage3_PCChange <= stage2_PCChange;
        stage3_regChange <= stage2_regChange;

        if(stage2_PCChange)begin
          case(stage2_PCChangeCmd)
            //call sym                     @          22 @                      2 @  000111
            3'b000 : begin//ok
              pcResult <= {pc[31:28],stage2_IMM26,2'b00};
              //regfile[31] <= nextpc; code is 31
              stage3_regResult <= nextpc;
              //regResultRA <= 1;
              stage3_regChange <= 31;
            end
            //bge reg, reg, sym            @          15 @                      0 @  001111
            3'b001 : begin//ok
              if(comp_ge) begin//if ((signed) rA >= (signed) rB)
                pcResult <= nextpc + stage2_IMM16sx;//then PC ← PC + 4 + σ(IMM16)
              end else begin
                pcResult <= nextpc;//else PC ← PC + 4
              end
            end
            //blt reg, reg, sym            @          15 @                      0 @  010111
            3'b010 : begin//ok
              if(comp_lt) begin//if ((signed) rA < (signed) rB)
                pcResult <= nextpc + stage2_IMM16sx;//then PC ← PC + 4 + σ(IMM16)
              end else begin
                pcResult <= nextpc;//else PC ← PC + 4
              end
            end
            //bne reg, reg, sym            @          15 @                      0 @  011111
            3'b011 : begin//ok
              if(!comp_eq) begin
                pcResult <= nextpc + stage2_IMM16sx;
              end else begin
                pcResult <= nextpc;
              end
            end
            //beq reg, reg, sym            @          15 @                      0 @  100111
            3'b100 : begin//ok
              if(comp_eq) begin
                pcResult <= nextpc + stage2_IMM16sx;
              end else begin
                pcResult <= nextpc;
              end
            end
            //bgeu reg, reg, sym           @          15 @                      0 @  101111
            3'b101 : begin//ok
              if(!comp_ltu) begin//if ((unsigned) rA >= (unsigned) rB) 
                pcResult <= nextpc + stage2_IMM16sx;//then PC ← PC + 4 + σ(IMM16)
              end else begin
                pcResult <= nextpc;//else PC ← PC + 4
              end
            end
            //bltu reg, reg, sym           @          15 @                      0 @  110111
            3'b110 : begin//ok
              if(comp_ltu) begin//if ((unsigned) rA < (unsigned) rB) 
                pcResult <= nextpc + stage2_IMM16sx;//then PC ← PC + 4 + σ(IMM16)
              end else begin
                pcResult <= nextpc;//else PC ← PC + 4
              end
            end
            // 111111 
            3'b111 : begin//ok
              case(stage2_PCChangeCmdExt)
                //callr reg                    @          50 @                      1 @  111111
                3'b000 : begin//ok
                  pcResult <= stage2_regfileA;
                  stage3_regResult <= nextpc;//rC ← rA + rB
                  stage3_regChange <= stage2_regC;//regResultC <= 1;// c is ra
                end
                //jmp reg                      @          51 @                      1 @  111111
                3'b001 : begin//ok
                  pcResult <= stage2_regfileA;
                end
                //setirq reg, reg, ins         @          52 @                      1 @  111111
                3'b010 : begin//ok
                  irq_enable <= stage2_IMM8[0];//rB ← rA & (0x0000 : IMM16)
                  irq_addr <= stage2_regfileA;
                  pcResult <= nextpc;
                end
                //stoff reg                    @          53 @                      1 @  111111
                3'b011 : begin//ok
                  private_offset <= stage2_regfileA;
                  pcResult <= nextpc;
                end
                //hlt ins                      @          54 @                      1 @  111111
                3'b100 : begin//ok
                  halt_cpu <= stage2_IMM8[0];
                  pcResult <= nextpc;
                end
                //ret                          @          55 @                      1 @  111111
                3'b101 : begin
                  pcResult <= stage2_regfileA;//regA == 31
                end
                //reti                         @          56 @                      1 @  111111
                3'b110 : begin//ok
                  pcResult <= stage2_regfileA;
                  irq_enable <= 1;
                end
                //#irqcall                     @          57 @                      1 @  111111
                3'b111 : begin//ok
                  pcResult <= irq_addr;
                  irq_enable <= 0;
                  stage3_regResult <= pc;
                  stage3_regChange <= 31;// regResultRA <= 1;
                end
              endcase
            end
          endcase
        end
        
        if(stage2_MEMAccess)begin
          case(stage2_MEMAccessCmd)
            //ldw reg, regins              @          11 @                      0 @   7 @          000111      0x07 ldw rB, byte_offset(rA)
            3'b000 : begin//ok
              stage3_regChange <= stage2_regB;//regB
              if         (exec_step==0)begin
                exec_step <= 1;
                //exec_address <= {dsAddr[31:2],2'b0};//rB ← Mem32[rA + σ(IMM16)]
                exec_read <= 1;
              end else if(exec_step==1)begin
                if(!avm_m0_waitrequest)begin
                  stage3_regResult <= avm_m0_readdata;
                  exec_read <= 0;
                  exec_step <= 0;
                end
              end
            end
            //stw reg, regins              @          11 @                      0 @  15 @          001111      0x0f stw rB, byte_offset(rA)
            3'b001 : begin//ok
              if         (exec_step==0)begin
                exec_step <= 1;
                //exec_address <= {dsAddr[31:2],2'b0};//Mem32[rA + σ(IMM16)] ← rB
                exec_writedata <= stage2_regfileB;
                exec_write <= 1;
              end else if(exec_step==1)begin
                if(!avm_m0_waitrequest)begin
                  exec_write <= 0;
                  exec_step <= 0;
                end
              end
            end
            //ldbu reg, regins             @          11 @                      0 @  23 @          010111      0x17
            3'b010 : begin//ok
              stage3_regChange <= stage2_regB;//regB
              if         (exec_step==0)begin
                exec_step <= 1;
                //exec_address <= {dsAddr[31:2],2'b0};//rB ← 0x000000 : Mem8[rA + σ(IMM16)]
                exec_read <= 1;
              end else if(exec_step==1)begin
                if(!avm_m0_waitrequest)begin
                  case(exec_address[1:0])
                    0:begin stage3_regResult <= {24'b0,avm_m0_readdata[ 7: 0]};end
                    1:begin stage3_regResult <= {24'b0,avm_m0_readdata[15: 8]};end
                    2:begin stage3_regResult <= {24'b0,avm_m0_readdata[23:16]};end
                    3:begin stage3_regResult <= {24'b0,avm_m0_readdata[31:24]};end
                  endcase
                  exec_read <= 0;
                  exec_step <= 0;
                end
              end
            end
            //ldb reg, regins              @          11 @                      0 @  31 @          011111      0x1f*
            3'b011 : begin//ok
              stage3_regChange <= stage2_regB;//regB
              if         (exec_step==0)begin
                exec_step <= 1;
                //exec_address <= {dsAddr[31:2],2'b0};//rB ← 0x000000 : Mem8[rA + σ(IMM16)]
                exec_read <= 1;
              end else if(exec_step==1)begin
                if(!avm_m0_waitrequest)begin
                  case(exec_address[1:0])
                    0:begin stage3_regResult <= {{24{avm_m0_readdata[ 7]}},avm_m0_readdata[ 7: 0]};end
                    1:begin stage3_regResult <= {{24{avm_m0_readdata[15]}},avm_m0_readdata[15: 8]};end
                    2:begin stage3_regResult <= {{24{avm_m0_readdata[23]}},avm_m0_readdata[23:16]};end
                    3:begin stage3_regResult <= {{24{avm_m0_readdata[31]}},avm_m0_readdata[31:24]};end
                  endcase
                  exec_read <= 0;
                  exec_step <= 0;
                end
              end
            end
            //ldhu reg, regins             @          11 @                      0 @  39 @          100111      0x27
            3'b100 : begin//ok
              stage3_regChange <= stage2_regB;//regB
              if         (exec_step==0)begin
                exec_step <= 1;
                //exec_address <= {dsAddr[31:2],2'b0};//rB ← 0x0000 : Mem16[rA + σ(IMM16)]
                exec_read <= 1;
              end else if(exec_step==1)begin
                if(!avm_m0_waitrequest)begin
                  case(exec_address[1])
                    0:begin stage3_regResult <= {16'b0,avm_m0_readdata[15: 0]};end
                    1:begin stage3_regResult <= {16'b0,avm_m0_readdata[31:16]};end
                  endcase
                  exec_read <= 0;
                  exec_step <= 0;
                end
              end
            end
            //ldh reg, regins              @          11 @                      0 @  47 @          101111      0x2f
            3'b101 : begin//ok
              stage3_regChange <= stage2_regB;//regB
              if         (exec_step==0)begin
                exec_step <= 1;
                //exec_address <= {dsAddr[31:2],2'b0};//rB ← σ(Mem16[rA + σ(IMM16)])
                exec_read <= 1;
              end else if(exec_step==1)begin
                if(!avm_m0_waitrequest)begin
                  case(exec_address[1])
                    0:begin stage3_regResult <= {{16{avm_m0_readdata[15]}},avm_m0_readdata[15: 0]};end
                    1:begin stage3_regResult <= {{16{avm_m0_readdata[31]}},avm_m0_readdata[31:16]};end
                  endcase
                  exec_read <= 0;
                  exec_step <= 0;
                end
              end
            end
            //stb reg, regins              @          11 @                      0 @  55 @          110111      0x37*
            3'b110 : begin//ok
              if         (exec_step==0)begin
                exec_step <= 1;
                //exec_address <= {dsAddr[31:2],2'b0};//Mem8[rA + σ(IMM16)] ← rB7..0
                case(exec_address[1:0])
                  0:begin byteenable <= 4'b0001; exec_writedata <= {24'b0,stage2_regfileB[7:0]};end
                  1:begin byteenable <= 4'b0010; exec_writedata <= {16'b0,stage2_regfileB[7:0],8'b0};end
                  2:begin byteenable <= 4'b0100; exec_writedata <= {8'b0,stage2_regfileB[7:0],16'b0};end
                  3:begin byteenable <= 4'b1000; exec_writedata <= {stage2_regfileB[7:0],24'b0};end
                endcase
                exec_write <= 1;
              end else if(exec_step==1)begin
                if(!avm_m0_waitrequest)begin
                  byteenable <= 4'b1111;
                  exec_write <= 0;
                  exec_step <= 0;
                end
              end
            end
            //sth reg, regins              @          11 @                      0 @  63 @          111111      0x3f*
            3'b111 : begin//ok
              if         (exec_step==0)begin
                exec_step <= 1;
                //exec_address <= {dsAddr[31:2],2'b0};//Mem8[rA + σ(IMM16)] ← rB7..0
                case(exec_address[1])
                  0:begin byteenable <= 4'b0011; exec_writedata <= {16'b0,stage2_regfileB[15:0]};end
                  1:begin byteenable <= 4'b1100; exec_writedata <= {stage2_regfileB[15:0],16'b0};end
                endcase
                exec_write <= 1;
              end else if(exec_step==1)begin
                if(!avm_m0_waitrequest)begin
                  byteenable <= 4'b1111;
                  exec_write <= 0;
                  exec_step <= 0;
                end
              end
            end
          endcase
        end

        if(stage2_CMP)begin
          case(stage2_CMPCmd)
            //cmpgei reg, reg, ins         @          10 @                      0 @  001000
            4'b0010 : begin stage3_regResult <= {31'b0,($signed(stage2_regfileA) >= $signed(stage2_IMM16sx))}; end//if ((signed) rA >= (signed) σ(IMM16)) then rB ← 1 else rB ← 0
            //cmplti reg, reg, ins         @          10 @                      0 @  010000
            4'b0100 : begin stage3_regResult <= {31'b0,($signed(stage2_regfileA) < $signed(stage2_IMM16sx))}; end//if ((signed) rA < (signed) σ(IMM16))
            //cmpnei reg, reg, ins         @          10 @                      0 @  011000
            4'b0110 : begin stage3_regResult <= {31'b0,(stage2_regfileA != stage2_IMM16sx)}; end//if (rA != σ(IMM16)) then rB ← 1 else rB ← 0
            //cmpeqi reg, reg, ins         @          10 @                      0 @  100000
            4'b1000 : begin stage3_regResult <= {31'b0,(stage2_regfileA == stage2_IMM16sx)}; end//if (rA == σ(IMM16)) then rB ← 1 else rB ← 0
            //cmpgeui reg, reg, ins        @          10 @                      0 @  101000
            4'b1010 : begin stage3_regResult <= {31'b0,(stage2_regfileA >= stage2_IMM16zx)}; end//if ((unsigned) rA >= (unsigned) (0x0000 : IMM16)) then rB ← 1 else rB ← 0
            //cmpltui reg, reg, ins        @          10 @                      0 @  110000
            4'b1100 : begin stage3_regResult <= {31'b0,(stage2_regfileA < stage2_IMM16zx)}; end//if ((unsigned) rA < (unsigned) (0x0000 : IMM16)) then rB ← 1 else rB ← 0

            //cmpge reg, reg, reg          @          30 @                      3 @  001100
            4'b0011 : begin stage3_regResult <= {31'b0,(comp_ge)}; end// if ((signed) rA >= (signed) rB) then rC ← 1  else rC ← 0
            //cmplt reg, reg, reg          @          30 @                      3 @  010100
            4'b0101 : begin stage3_regResult <= {31'b0,(~comp_ge)}; end//if ((signed) rA < (signed) rB) then rC ← 1 else rC ← 0
            //cmpne reg, reg, reg          @          30 @                      3 @  011100
            4'b0111 : begin stage3_regResult <= {31'b0,(~comp_eq)}; end//if (rA != rB) then rC ← 1 else rC ← 0
            //cmpeq reg, reg, reg          @          30 @                      3 @  100100
            4'b1001 : begin stage3_regResult <= {31'b0,comp_eq}; end//if (rA == rB) then rC ← 1 else rC ← 0
            //#cmpgeu reg, reg, reg         @          30 @                      3 @  101100
            //cmpltu reg, reg, reg         @          30 @                      3 @  110100
            4'b1101 : begin stage3_regResult <= {31'b0,(comp_ltu)}; end//if ((unsigned) rA < (unsigned) rB) then rC ← 1 else rC ← 0
          endcase
        end

        if(stage2_ALU1)begin
          case(stage2_ALU1Cmd)
            //muli reg, reg, ins           @          10 @                      0 @   2 @          000001      0x02*
            4'b0000 : begin stage3_regResult <= mulResultSigned[31:0]; end
            //slli reg, reg, ins           @          10 @                      0 @   9 @          001001      0x09
            4'b0010 : begin stage3_regResult <= shiftResultLogical; end
            //srai reg, reg, ins           @          10 @                      0 @  17 @          010001      0x11
            4'b0100 : begin stage3_regResult <= shiftResultArithmetic; end
            //srli reg, reg, ins           @          10 @                      0 @  25 @          011001      0x19
            4'b0110 : begin stage3_regResult <= shiftResultLogical; end
            //roli reg, reg, ins           @          10 @                      0 @  33 @          100001      0x21
            4'b1000 : begin stage3_regResult <= shiftResultRotate; end
            
            //mul reg, reg, reg            @          30 @                      3 @  000101
            4'b0001 : begin stage3_regResult <= mulResultSigned[31:0]; end
            //sll reg, reg, reg            @          30 @                      3 @  001101
            4'b0011 : begin stage3_regResult <= shiftResultLogical; end
            //sra reg, reg, reg            @          30 @                      1 @  010101
            4'b0101 : begin stage3_regResult <= shiftResultArithmetic; end
            //srl reg, reg, reg            @          30 @                      3 @  011101
            4'b0111 : begin stage3_regResult <= shiftResultLogical; end
            //rol reg, reg, reg            @          30 @                      3 @  100101
            4'b1001 : begin stage3_regResult <= shiftResultRotate; end

          endcase
        end

        if(stage2_ALU2)begin
          case(stage2_ALU2Cmd)
            //addi reg, reg, ins           @          10 @                      0 @  000010
            4'b0000 : begin stage3_regResult <= stage2_regfileA + stage2_IMM16sx; end//rB ← rA + σ(IMM16)
            //andi reg, reg, ins           @          10 @                      0 @  001010
            4'b0010 : begin stage3_regResult <= {16'b0, (stage2_regfileA[15:0] & stage2_IMM16)}; end//rB ← rA & (0x0000 : IMM16)
            //ori  reg, reg, ins           @          10 @                      0 @  010010
            4'b0100 : begin stage3_regResult <= {stage2_regfileA[31:16],(stage2_regfileA[15:0] | stage2_IMM16)}; end//rB ← rA | (0x0000 : IMM16)
            //xori reg, reg, ins           @          10 @                      0 @  011010
            4'b0110 : begin stage3_regResult <= {stage2_regfileA[31:16], (stage2_regfileA[15:0] ^ stage2_IMM16)}; end//rB ← rA ^ (0x0000 : IMM16)
            //#                                                                      100010
            //andhi reg, reg, ins          @          10 @                      0 @  101010
            4'b1010 : begin stage3_regResult <= {(stage2_regfileA[31:16] & stage2_IMM16), 16'b0}; end//rB ← rA & (IMM16 : 0x0000)
            //orhi reg, reg, ins           @          10 @                      0 @  110010
            4'b1100 : begin stage3_regResult <= {(stage2_regfileA[31:16] | stage2_IMM16),stage2_regfileA[15:0]}; end//rB ← rA | (IMM16 : 0x0000)
            //xorhi reg, reg, ins          @          10 @                      0 @  111010
            4'b1110 : begin stage3_regResult <= {(stage2_regfileA[31:16] ^ stage2_IMM16), stage2_regfileA[15:0]}; end//rB ← rA ^ (IMM16 : 0x0000)

            //add reg, reg, reg            @          30 @                      1 @  000110
            4'b0001 : begin stage3_regResult <= stage2_regfileA + stage2_regfileB; end//rC ← rA + rB
            //and reg, reg, reg            @          30 @                      1 @  001110
            4'b0011 : begin stage3_regResult <= stage2_regfileA & stage2_regfileB; end// rC ← rA | rB
            //or reg, reg, reg             @          30 @                      1 @  010110
            4'b0101 : begin stage3_regResult <= stage2_regfileA | stage2_regfileB; end// rC ← rA | rB
            //xor reg, reg, reg            @          30 @                      1 @  011110
            4'b0111 : begin stage3_regResult <= stage2_regfileA ^ stage2_regfileB; end// rC ← rA ^ rB
            //sub reg, reg, reg            @          30 @                      1 @  100110
            4'b1001 : begin stage3_regResult <= stage2_regfileA - stage2_regfileB; end// rC ← rA – rB
            //nor reg, reg, reg            @          30 @                      1 @  101110
            4'b1011 : begin stage3_regResult <= ~(stage2_regfileA | stage2_regfileB); end// rC ← ~(rA | rB)
          endcase
        end


      end
    end
  end
  reg        stage3_Valid;
  reg        stage3_PCChange;
  reg  [4:0] stage3_regChange;
  reg [31:0] stage3_regResult;
  reg [31:0] pcResult;
  //////////////////////////////////////////////////////////

  
  //5，stage3 寄存器/PC 更新
  always @(posedge clk or negedge cpu_reset_n) begin
    if (!cpu_reset_n) begin
      //stage2寄存器初始化
    end else begin
      if(stage3_Valid)begin
        if(stage3_regChange != 0)begin
          regfile[stage3_regChange] <= stage3_regResult;
        end
      end
    end
  end

endmodule

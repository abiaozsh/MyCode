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
    input  wire        clk,                // clock.clk
    input  wire        reset_n,              // reset.reset

    //bus
    output wire [31:0] avm_m0_address       ,     //    m0.address
    output wire        avm_m0_read          ,        //      .read
    output wire        avm_m0_write         ,       //      .write
    input  wire [31:0] avm_m0_readdata      ,    //      .readdata
    output wire [31:0] avm_m0_writedata     ,   //      .writedata
    output wire [3:0]  avm_m0_byteenable    ,    //      .readdata
    input  wire        avm_m0_waitrequest   , //      .waitrequest
    input  wire [31:0] inr_irq0_irq,        //  irq0.irq
    
    input wire          uart_rxd,
    output wire         uart_txd,
    output [7:0]    debug8,
    output [31:0]   debug32,
    
    
    input  [7:0]    debugin8,
    input [15:0] cache_life0   ,
    input [15:0] cache_life1   ,
    input [15:0] cache_life2   ,
    input [15:0] cache_life3   ,
    input [15:0] cacheAddrHigh0,
    input [15:0] cacheAddrHigh1,
    input [15:0] cacheAddrHigh2,
    input [15:0] cacheAddrHigh3,
    input [31:0] debugin32,
    
    
    input vga_blanking
  );

  wire uart_rec;
  wire [7:0] uart_data_out;
  reg uart_send;
  reg [7:0] uart_data_in;
  uart_hs uart_hs_inst (
    .sys_clk(clk), 
    .sys_rst_n(reset_n),

    .uart_txd(uart_txd),
    .uart_rxd(uart_rxd),
    
    .uart_rec(uart_rec),
    .uart_data_out(uart_data_out),

    .uart_send(uart_send),
    .uart_data_in(uart_data_in)
  );
  
reg [7:0] command;
reg [7:0] data;
reg [7:0] command_temp;
reg data_cmd;//1:data 0:cmd
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin
    command <= 0;
    data <= 0;
  end else begin
    if (uart_rec) begin 
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

    end else begin
      if(command_done)begin
        command <= 0;
      end
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

`define QUICKREG true

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
  end else begin
    uart_send<=0;
    
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

      end else if (command == 8'h10) begin uart_send<=1; uart_data_in<=debug_data[ 7: 0]; command_done<=1;
      end else if (command == 8'h11) begin uart_send<=1; uart_data_in<=debug_data[15: 8]; command_done<=1;
      end else if (command == 8'h12) begin uart_send<=1; uart_data_in<=debug_data[23:16]; command_done<=1;
      end else if (command == 8'h13) begin uart_send<=1; uart_data_in<=debug_data[31:24]; command_done<=1;
      
      end else if (command == 8'h14) begin uart_send<=1; uart_data_in<=halt_cpu; command_done<=1;
      end else if (command == 8'h15) begin uart_send<=1; uart_data_in<=halt_uart; command_done<=1;
      end else if (command == 8'h16) begin uart_send<=1; uart_data_in<=avm_m0_waitrequest; command_done<=1;
      end else if (command == 8'h18) begin uart_send<=1; uart_data_in<=cmd; command_done<=1;
      end else if (command == 8'h19) begin uart_send<=1; uart_data_in<=Rtype; command_done<=1;
      
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
        

  ;

      end else if (command == 8'h40) begin debug_data<=numer; command_done<=1;
      end else if (command == 8'h41) begin debug_data<=denom; command_done<=1;
      end else if (command == 8'h42) begin debug_data<=quotientUnsigned; command_done<=1;
      end else if (command == 8'h43) begin debug_data<=pc; command_done<=1;
      end else if (command == 8'h44) begin debug_data<=private_offset; command_done<=1;
      end else if (command == 8'h45) begin debug_data<=quotientSigned; command_done<=1;

      end else if (command == 8'h47) begin
        `ifdef QUICKREG
        debug_data <= regfile[data[4:0]];
        `else
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          debug_reg <= data[4:0];
        end else if(debug_readmem_step==1)begin
          debug_readmem_step <= 2;
        end else if(debug_readmem_step==2)begin
          debug_data <= regDataOut;
          debug_readmem_step <= 0;
          command_done <= 1;
        end
        `endif
      end else begin
        command_done<=1;
      end
    end


  end
end

assign debug8[0] = avm_m0_waitrequest;
assign debug8[1] = avm_m0_read;
assign debug8[2] = avm_m0_write;

assign debug8[4] = halt;
assign debug8[5] = cycle;
assign debug8[6] = debug_read;
assign debug8[7] = debug_write;

wire cpu_reset_n = reset_n && debug_reset_n;

  assign avm_m0_byteenable = halt_accept == 1 ? debug_byteenable : byteenable;
  
  wire [31:0] affect_address    = halt_accept == 1   ? debug_address                                : (cycle == 0 ? fetch_address : {exec_address[31:2],2'b0});
  assign avm_m0_address         = affect_address[31] ? {1'b0,affect_address[30:0]} + private_offset : affect_address;
  assign avm_m0_writedata       = halt_accept == 1   ? debug_writedata                              : exec_writedata;
  assign avm_m0_write           = halt_accept == 1   ? debug_write                                  : exec_write;
  assign avm_m0_read            = halt_accept == 1   ? debug_read                                   : (cycle == 0 ? fetch_read : exec_read);

  reg [31:0] latch_readdata;
  wire Rtype;
  wire [5:0]  cmd;
  wire [4:0]  regB;
  wire [4:0]  regC;
  wire [4:0]  IMM5;
  wire [15:0] IMM16;
  wire [31:0] IMM16zx;
  wire [31:0] IMM16sx;
  wire [25:0] IMM26;
  assign regB = latch_readdata[26:22];
  assign regC = latch_readdata[21:17];
  assign IMM5 = latch_readdata[10:6];
  assign IMM26 = latch_readdata[31:6];
  assign IMM16 = latch_readdata[21:6];
  assign IMM16sx = {{16{IMM16[15]}},IMM16};
  assign IMM16zx = {16'b0,IMM16};
  assign Rtype = latch_readdata[5:0]==6'h3A;
  assign cmd = Rtype ? latch_readdata[16:11] : latch_readdata[5:0];

  reg cycle;//0:fetchCode 1:execCode
  wire [31:0] fetch_address;
  assign fetch_address = pc;
  
`ifdef QUICKREG
  reg [31:0] regfile[32];
`else
  reg regWrite;
  reg [4:0] regAddr;
  wire [4:0] reg_addr;
  assign reg_addr = (halt_accept == 1) ? debug_reg : regAddr;
  reg  [31:0] regDataIn;
  wire [31:0] regDataOut;
  //用m9k实现可以省1K逻辑单元
  regfile  regfile_inst (
    .address ( reg_addr ),
    .clock ( clk ),
    .data ( regDataIn ),
    .wren ( regWrite ),
    .q ( regDataOut )
  );
`endif
  
  reg [31:0] regfileA;
  reg [31:0] regfileB;

  reg        fetch_read;
  reg [ 2:0] fetch_step;
  reg        debug_step_buff;
  reg        halt_accept;
  always @(posedge clk or negedge cpu_reset_n) begin
    if (!cpu_reset_n) begin
      cycle<=0;
      fetch_read<=0;
      fetch_step<=0;
      halt_accept <= 0;
      debug_step_buff <= 0;
    end else begin
      `ifdef QUICKREG
      `else
      regWrite <= 0;
      `endif
      
      if(cycle==0)begin
        if         (fetch_step==0)begin
          if(halt)begin
            halt_accept <= 1;
          end else begin
            halt_accept <= 0;
            fetch_step <= 1;
            fetch_read <= 1;
          end
        `ifdef QUICKREG
        end else if(fetch_step==1)begin
          if(!avm_m0_waitrequest)begin
            latch_readdata <= avm_m0_readdata;
            regfileA <= regfile[avm_m0_readdata[31:27]];//regA
            regfileB <= regfile[avm_m0_readdata[26:22]];
            fetch_read <= 0;
            fetch_step <= 0;
            cycle<=1;
          end
        `else
        end else if(fetch_step==1)begin
          if(!avm_m0_waitrequest)begin
            latch_readdata <= avm_m0_readdata;
            regAddr <= avm_m0_readdata[31:27];//regA
            fetch_read <= 0;
            fetch_step <= 2;
          end
        end else if(fetch_step==2)begin
          regAddr <= regB;
          fetch_step <= 3;
        end else if(fetch_step==3)begin
          regfileA <= regDataOut;
          fetch_step <= 4;
        end else if(fetch_step==4)begin
          regfileB <= regDataOut;
          fetch_step <= 0;
          cycle<=1;
        `endif
        end
      end else begin
        if(cmd_ack)begin
          debug_step_buff <= debug_step;
          `ifdef QUICKREG
          if(regResultB && regB!=0)begin
            regfile[regB] <= regResult;
          end
          if(regResultC && regC!=0)begin
            regfile[regC] <= regResult;
          end
          if(regResultRA)begin
            regfile[31] <= regResult;
          end
          `else
          if(regResultB && regB!=0)begin
            regAddr <= regB;
            regDataIn <= regResult;
            regWrite <= 1;
          end
          if(regResultC && regC!=0)begin
            regAddr <= regC;
            regDataIn <= regResult;
            regWrite <= 1;
          end
          if(regResultRA)begin
            regAddr <= 31;
            regDataIn <= regResult;
            regWrite <= 1;
          end
          `endif
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
  shiftRotate	shiftRotate_inst (
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
  
  wire [31:0] numer;
  assign numer = regfileA;
  wire [31:0] denom;
  assign denom = regfileB;
  wire [31:0] quotientSigned;
  wire [31:0] remainSigned;
  wire [31:0] quotientUnsigned;
  wire [31:0] remainUnsigned;

  divSigned  divSigned_inst (
    //.clock    (clk),
    .denom    (denom),
    .numer    (numer),
    .quotient (quotientSigned),
    .remain   (remainSigned)
  );
  divUnsigned  divUnsigned_inst (
    //.clock    (clk),
    .denom    (denom),
    .numer    (numer),
    .quotient (quotientUnsigned),
    .remain   (remainUnsigned)
  );



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
    end else begin
      if(cycle==1 && cmd_ack==0)begin
        regResultB <= 0;
        regResultC <= 0;
        regResultRA <= 0;
        if(Rtype==0)begin
          //longcall
          //longjmp
          //unknow cmd
          
          //call sym                     @          22 @                      2 @  26 @          011010      0x1A
          if         (cmd==6'h1A)begin//ok
            pc <= {IMM26,2'b00};
            //regfile[31] <= nextpc; code is 31
            regResult <= nextpc;
            regResultRA <= 1;
            cmd_ack <= 1;
          //jmpi sym                     @          21 @                      2 @  10 @          001010      0x0A
          end else if(cmd==6'h0A)begin//ok
            pc <= {IMM26,2'b00};
            cmd_ack <= 1;
          //hlt                          @           0 @                      1 @   2 @ extends
          end else if(cmd==6'd2)begin//ok
            halt_cpu <= IMM16[0];
            pc <= nextpc;
            cmd_ack <= 1;
            
            
          //br sym                       @          20 @                      0 @   6 @ 0x06
          end else if(cmd==6'd6)begin//ok
            pc <= nextpc + IMM16sx;
            cmd_ack <= 1;
          //bne reg, reg, sym            @          15 @                      0 @  30 @ 0x1e
          end else if(cmd==6'd30)begin//ok
            if(comp_eq) begin
              pc <= nextpc;
            end else begin
              pc <= nextpc + IMM16sx;
            end
            cmd_ack <= 1;
          // beq reg, reg, sym            @          15 @                      0 @  38 @ 0x26
          end else if(cmd==6'd38)begin//ok
            if(comp_eq) begin
              pc <= nextpc + IMM16sx;
            end else begin
              pc <= nextpc;
            end
            cmd_ack <= 1;
          
          
          //bge reg, reg, sym            @          15 @                      0 @  14 @ 0x0e
          end else if(cmd==6'd14)begin//ok
            if(comp_ge) begin//if ((signed) rA >= (signed) rB)
              pc <= nextpc + IMM16sx;//then PC ← PC + 4 + σ(IMM16)
            end else begin
              pc <= nextpc;//else PC ← PC + 4
            end
            cmd_ack <= 1;

          //blt reg, reg, sym            @          15 @                      0 @  22 @ 0x16
          end else if(cmd==6'd22)begin//ok
            if(comp_lt) begin//if ((signed) rA < (signed) rB)
              pc <= nextpc + IMM16sx;//then PC ← PC + 4 + σ(IMM16)
            end else begin
              pc <= nextpc;//else PC ← PC + 4
            end
            cmd_ack <= 1;
          //bltu reg, reg, sym           @          15 @                      0 @  54 @          110110      0x36
          end else if(cmd==6'h36)begin//ok
            if(comp_ltu) begin//if ((unsigned) rA < (unsigned) rB) 
              pc <= nextpc + IMM16sx;//then PC ← PC + 4 + σ(IMM16)
            end else begin
              pc <= nextpc;//else PC ← PC + 4
            end
            cmd_ack <= 1;
          //bgeu reg, reg, sym           @          15 @                      0 @  46 @          101110      0x2e
          end else if(cmd==6'h2e)begin//ok
            if(!comp_ltu) begin//if ((unsigned) rA >= (unsigned) rB) 
              pc <= nextpc + IMM16sx;//then PC ← PC + 4 + σ(IMM16)
            end else begin
              pc <= nextpc;//else PC ← PC + 4
            end
            cmd_ack <= 1;

          
          

          //andi reg, reg, ins           @          10 @                      0 @  12 @ 0x0c
          end else if(cmd==6'd12)begin//ok
            regResult <= {16'b0, (regfileA[15:0] & IMM16)};//rB ← rA & (0x0000 : IMM16)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          //andhi reg, reg, ins          @          10 @                      0 @  44 @ 0x2c
          end else if(cmd==6'd44)begin//ok
            regResult <= {(regfileA[31:16] & IMM16), 16'b0};//rB ← rA & (IMM16 : 0x0000)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;

          //addi reg, reg, ins           @          10 @                      0 @   4 @ 0x04
          end else if(cmd==6'd4)begin//ok
            regResult <= regfileA + IMM16sx;//rB ← rA + σ(IMM16)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          //orhi reg, reg, ins           @          10 @                      0 @  52 @ 0x34
          end else if(cmd==6'd52)begin//ok
            regResult <= {(regfileA[31:16] | IMM16),regfileA[15:0]};//rB ← rA | (IMM16 : 0x0000)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          //ori  reg, reg, ins           @          10 @                      0 @  20 @ 0x14
          end else if(cmd==6'd20)begin//ok
            regResult <= {regfileA[31:16],(regfileA[15:0] | IMM16)};//rB ← rA | (0x0000 : IMM16)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
            
          //muli reg, reg, ins           @          10 @                      0 @  36 @ 0x24
          end else if(cmd==6'd36)begin//ok
            if         (exec_step==0)begin
              mulDataB <= IMM16sx;
              exec_step <= 1;
            end else if(exec_step==1)begin
              // shift left logical immediate
              regResult <= mulResultSigned[31:0];
              regResultB <= 1;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end

          //xori reg, reg, ins           @          10 @                      0 @  28 @ 0x1c
          end else if(cmd==6'd28)begin//ok
            regResult <= {regfileA[31:16], (regfileA[15:0] ^ IMM16)};//rB ← rA ^ (0x0000 : IMM16)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          //xorhi reg, reg, ins          @          10 @                      0 @  60 @          111100      0x3c
          end else if(cmd==6'h3c)begin//ok
            regResult <= {(regfileA[31:16] ^ IMM16), regfileA[15:0]};//rB ← rA ^ (IMM16 : 0x0000)
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;

            
            
          //cmpeqi reg, reg, ins         @          10 @                      0 @  32 @ 0x20
          end else if(cmd==6'd32)begin//ok
            regResult <= {31'b0,(regfileA == IMM16sx)};//if (rA == σ(IMM16)) then rB ← 1 else rB ← 0
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;

          //cmplti reg, reg, ins         @          10 @                      0 @  16 @ 0x10
          end else if(cmd==6'd16)begin//ok
            regResult <= {31'b0,($signed(regfileA) < $signed(IMM16sx))};//if ((signed) rA < (signed) σ(IMM16))
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          //cmpltui reg, reg, ins        @          10 @                      0 @  48 @          110000      0x30
          end else if(cmd==6'h30)begin//ok
            regResult <= {31'b0,(regfileA < {16'b0,IMM16})};//if ((unsigned) rA < (unsigned) (0x0000 : IMM16)) then rB ← 1 else rB ← 0
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;

            
          //cmpgei reg, reg, ins         @          10 @                      0 @   8 @ 0x08
          end else if(cmd==6'h08)begin//ok
            regResult <= {31'b0,($signed(regfileA) >= $signed(IMM16sx))};//if ((signed) rA >= (signed) σ(IMM16)) then rB ← 1 else rB ← 0
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          //cmpnei reg, reg, ins         @          10 @                      0 @  24 @ 0x18
          end else if(cmd==6'd24)begin//ok
            regResult <= {31'b0,(regfileA != IMM16sx)};//if (rA != σ(IMM16)) then rB ← 1 else rB ← 0
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          //cmpgeui reg, reg, ins        @          10 @                      0 @  40 @          101000      0x28
          end else if(cmd==6'h28)begin//ok
            regResult <= {31'b0,(regfileA >= IMM16zx)};//if ((unsigned) rA >= (unsigned) (0x0000 : IMM16)) then rB ← 1 else rB ← 0
            regResultB <= 1;
            pc <= nextpc;
            cmd_ack <= 1;

            

          //ldw,ldwio reg, regins              @          11 @                      0 @  23 @ 0x17
          end else if(cmd==6'd23 || cmd==6'd55)begin//ok
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
          //stw,stwio reg, regins              @          11 @                      0 @  21 @ 0x15
          end else if(cmd==6'd21 || cmd==6'd53)begin//ok
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


          
          
          //ldbu reg, regins             @          11 @                      0 @   3 @ 0x03
          end else if(cmd==6'd3)begin//ok
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
          //ldb reg, regins              @          11 @                      0 @   7 @ 0x07            
          end else if(cmd==6'd7)begin//ok
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
          //ldhu reg, regins             @          11 @                      0 @  11 @ 0x0b
          end else if(cmd==6'd11)begin//ok
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

          //stb reg, regins              @          11 @                      0 @   5 @ 0x05
          end else if(cmd==6'd5)begin//ok
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
          //sth reg, regins              @          11 @                      0 @  13 @ 0x0d
          end else if(cmd==6'd13)begin//ok
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
          //ldh reg, regins              @          11 @                      0 @  15 @          001111      0x0f
          end else if(cmd==6'h0f)begin//ok
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

          
          
          end else begin
            halt_cpu <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
        end else begin
          
          //?
          
          //ret                          @           0 @                      1 @   5 @ 0x3A,0x05
          if         (cmd==6'd5)begin
            pc <= regfileA;//regA == 31
            cmd_ack <= 1;
            
            
          //stoff reg                    @          50 @                      1 @  10 @          001010      0x3A,0x0A*
          end else if(cmd==6'h0A)begin//ok
            private_offset <= regfileA;
            pc <= nextpc;
            cmd_ack <= 1;

          //callr reg                    @          50 @                      1 @  29 @          011101      0x3A,0x1d
          end else if(cmd==6'h1D)begin//ok
            pc <= regfileA;
            regResult <= pc + 4;//rC ← rA + rB
            regResultC <= 1;// c is ra
            cmd_ack <= 1;
          //jmp reg                      @          50 @                      1 @  13 @          001101      0x3A,0x0d
          end else if(cmd==6'h0D)begin//ok
            pc <= regfileA;
            cmd_ack <= 1;

          //add reg, reg, reg            @          30 @                      1 @  49 @ 0x3A,0x31
          end else if(cmd==6'd49)begin
            regResult <= regfileA + regfileB;//rC ← rA + rB
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          //sub reg, reg, reg            @          30 @                      1 @  57 @ 0x3A,0x39
          end else if(cmd==6'd57)begin
            regResult <= regfileA - regfileB;// rC ← rA – rB
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          //mul reg, reg, reg            @          30 @                      1 @  39 @ 0x3A,0x27
          end else if(cmd==6'd39)begin//ok
            if         (exec_step==0)begin
              exec_cnt <= 0;
              mulDataB <= regfileB;
              exec_step <= 1;
            end else if(exec_step==1)begin//50Mhz
              exec_cnt <= exec_cnt + 1'b1;
              if(exec_cnt == 5)begin//50Mhz
                regResult <= mulResultSigned[31:0];
                regResultC <= 1;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          //divu reg, reg, reg           @          30 @                      1 @  37 @          100101      0x3A,0x24
          end else if(cmd==6'h24)begin//ok
            if         (exec_step==0)begin
              exec_cnt <= 0;
              exec_step <= 1;
            end else if(exec_step==1)begin
              exec_cnt <= exec_cnt + 1'b1;
              if(exec_cnt == 5)begin//50Mhz
                regResult <= quotientUnsigned;
                regResultC <= 1;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          //div reg, reg, reg            @          30 @                      1 @  37 @          100101      0x3A,0x25
          end else if(cmd==6'h25)begin//ok
            if         (exec_step==0)begin
              exec_cnt <= 0;
              exec_step <= 1;
            end else if(exec_step==1)begin
              exec_cnt <= exec_cnt + 1'b1;
              if(exec_cnt == 5)begin//50Mhz
                regResult <= quotientSigned;
                regResultC <= 1;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end

          //and reg, reg, reg            @          30 @                      1 @  14 @ 0x3A,0x0e
          end else if(cmd==6'd14)begin
            regResult <= regfileA & regfileB;// rC ← rA | rB
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          
          //or reg, reg, reg             @          30 @                      1 @  22 @ 0x3A,0x16sssssssssss
          end else if(cmd==6'd22)begin
            regResult <= regfileA | regfileB;// rC ← rA | rB
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;

          //nor reg, reg, reg            @          30 @                      1 @   6 @ 0x3A,0x06
          end else if(cmd==6'd6)begin
            regResult <= ~(regfileA | regfileB);// rC ← ~(rA | rB)
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          //xor reg, reg, reg            @          30 @                      1 @  30 @          011110      0x3A,0x1e
          end else if(cmd==6'h1e)begin
            regResult <= regfileA ^ regfileB;// rC ← rA ^ rB
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;

            
          //sll reg, reg, reg            @          30 @                      1 @  19 @ 0x3A,0x13
          end else if(cmd==6'd19)begin
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
          //sra reg, reg, reg            @          30 @                      1 @  59 @ 0x3A,0x3b
          end else if(cmd==6'd59)begin
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
          //rol reg, reg, reg            @          30 @                      1 @   3 @          000011      0x3A,0x03
          end else if(cmd==6'h03)begin
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
          //srl reg, reg, reg            @          30 @                      1 @  27 @          011011      0x3A,0x1b
          end else if(cmd==6'h1b)begin
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



          
          //cmpeq reg, reg, reg          @          30 @                      1 @  32 @ 0x3A,0x20
          end else if(cmd==6'd32)begin
            regResult <= {31'b0,comp_eq};//if (rA == rB) then rC ← 1 else rC ← 0
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          //cmpne reg, reg, reg          @          30 @                      1 @  24 @ 0x3A,0x18
          end else if(cmd==6'd24)begin
            regResult <= {31'b0,(~comp_eq)};//if (rA != rB) then rC ← 1 else rC ← 0
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;

          //cmpge reg, reg, reg          @          30 @                      1 @   8 @ 0x3A,0x08
          end else if(cmd==6'd8)begin
            regResult <= {31'b0,(comp_ge)};// if ((signed) rA >= (signed) rB) then rC ← 1  else rC ← 0
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;

          //cmplt reg, reg, reg          @          30 @                      1 @  16 @ 0x3A,0x10
          end else if(cmd==6'd16)begin
            regResult <= {31'b0,(~comp_ge)};//if ((signed) rA < (signed) rB) then rC ← 1 else rC ← 0
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;

          //cmpltu reg, reg, reg         @          30 @                      1 @  48 @          110000      0x3A,0x30
          end else if(cmd==6'h30)begin
            regResult <= {31'b0,(comp_ltu)};//if ((unsigned) rA < (unsigned) rB) then rC ← 1 else rC ← 0
            regResultC <= 1;
            pc <= nextpc;
            cmd_ack <= 1;

            
          //slli reg, reg, ins           @          40 @                      1 @  18 @ 0x3A,0x12
          end else if(cmd==6'd18)begin
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
          //srai reg, reg, ins           @          40 @                      1 @  58 @ 0x3A,0x3a
          end else if(cmd==6'd58)begin
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
          //srli reg, reg, ins           @          40 @                      1 @  26 @          011010      0x3A,0x1a
          end else if(cmd==6'h1A)begin
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

          end else begin
            halt_cpu <= 1;
            pc <= nextpc;
            cmd_ack <= 1;
          end
        
        
        end
        
      end
      
      if(cycle==0 && cmd_ack==1)begin
        cmd_ack<=0;
      end
    end
  end


endmodule

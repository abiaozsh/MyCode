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

		output wire [31:0] avm_m0_address       ,     //    m0.address
		output wire        avm_m0_read          ,        //      .read
		input  wire        avm_m0_waitrequest   , //      .waitrequest
		input  wire [31:0] avm_m0_readdata      ,    //      .readdata
		output wire [3:0]  avm_m0_byteenable    ,    //      .readdata
		output wire        avm_m0_write         ,       //      .write
		output wire [31:0] avm_m0_writedata     ,   //      .writedata
		input  wire [31:0] inr_irq0_irq,        //  irq0.irq
    
		input wire 			   uart_rxd,
		output wire			   uart_txd,
    output [7:0]   debug,
    output [7:0]   debug0,
    output [7:0]   debug1,
    output [7:0]   debug2,
    output [7:0]   debug3

	);

	wire uart_rec;
	wire [7:0] uart_data_out;
	reg uart_send;
	reg [7:0] uart_data_in;
	uart_hs (
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

reg command_done;
reg [7:0] timer;
reg [7:0] timer_log;

reg [31:0] debug_address;
reg        debug_read;
reg        debug_write;
reg [31:0] debug_data;
reg [31:0] debug_writedata;
reg  [3:0] debug_readmem_step;
reg        debug_step;
reg halt_uart;
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin

    command_done <= 0;
    timer<=0;
    debug_step<=0;
    debug_readmem_step<=0;
    timer_log<=0;
    halt_uart<=1;
    debug_read<=0;
    debug_write<=0;
    
  end else begin
    uart_send<=0;
    
    if(command_done)begin
      if          (command == 8'h00) begin 
        command_done<=0;
      end
    end else begin//command_done==0
      if          (command == 8'h00) begin

      end else if (command == 8'h01) begin halt_uart<=1; command_done<=1;
      end else if (command == 8'h02) begin halt_uart<=0; command_done<=1;
      
      end else if (command == 8'h03) begin debug_step<=~debug_step; command_done<=1;
      
      end else if (command == 8'h10) begin uart_send<=1; uart_data_in<=debug_data[ 7: 0]; command_done<=1;
      end else if (command == 8'h11) begin uart_send<=1; uart_data_in<=debug_data[15: 8]; command_done<=1;
      end else if (command == 8'h12) begin uart_send<=1; uart_data_in<=debug_data[23:16]; command_done<=1;
      end else if (command == 8'h13) begin uart_send<=1; uart_data_in<=debug_data[31:24]; command_done<=1;
			
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

      end else if (command == 8'h30) begin
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          debug_read <= 1;
        end else if(debug_readmem_step==1)begin
          if(!avm_m0_waitrequest)begin
            debug_data <= avm_m0_readdata;
            debug_read <= 0;
            debug_readmem_step <= 0;
            command_done = 1;
          end
        end
        
      end else if (command == 8'h31) begin
        if         (debug_readmem_step==0)begin
          debug_readmem_step <= 1;
          debug_write <= 1;
        end else if(debug_readmem_step==1)begin
          if(!avm_m0_waitrequest)begin
            debug_write <= 0;
            debug_readmem_step <= 0;
            command_done <= 1;
          end
        end
        

      end else if (command == 8'h43) begin debug_data<=pc; command_done<=1;
      end else if (command == 8'h47) begin debug_data<=regfile[data[4:0]]; command_done<=1;

      end else begin
        command_done<=1;
      end
    end


  end
end

assign debug[0] = avm_m0_waitrequest;
assign debug[1] = avm_m0_read;
assign debug[2] = avm_m0_write;

assign debug[4] = halt;
assign debug[5] = cycle;
assign debug[6] = cmd_ack;


	assign avm_m0_byteenable = byteenable;

	assign avm_m0_address = halt == 1 ? debug_address : (cycle == 0 ? (fetch_address) : (exec_address));

	assign avm_m0_writedata = halt == 1 ? debug_writedata : exec_writedata;
	assign avm_m0_write = halt == 1 ? debug_write : exec_write;

	assign avm_m0_read = halt == 1 ? debug_read : (cycle == 0 ? fetch_read : (exec_read));

  reg [31:0] latch_readdata;
  wire Rtype;
  wire [5:0]  cmd;
  wire [4:0]  regA;
  wire [4:0]  regB;
  wire [4:0]  regC;
  wire [15:0] IMM16;
  wire [31:0] IMM16sx;
  wire [25:0] IMM26;
  assign regA = latch_readdata[31:27];
  assign regB = latch_readdata[26:22];
  assign regC = latch_readdata[21:17];
  assign IMM26 = latch_readdata[31:6];
  assign IMM16 = latch_readdata[21:6];
  assign IMM16sx = {{16{IMM16[15]}},IMM16};
  assign Rtype = latch_readdata[5:0]==6'h3A;
  assign cmd = Rtype ? latch_readdata[16:11] : latch_readdata[5:0];

  reg cycle;//0:fetchCode 1:execCode
  wire [31:0] fetch_address;
  assign fetch_address = cs + pc;
  
  reg        fetch_read;
  reg [ 1:0] fetch_step;
  reg        debug_step_buff;
  always @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      cycle<=0;
      //fetch_address<=0;
      fetch_read<=0;
      fetch_step<=0;

      debug_step_buff <= 0;
    end else begin
      if(cycle==0)begin
        if(!halt)begin
          if         (fetch_step==0)begin
            fetch_step <= 1;
            fetch_read <= 1;
            //fetch_address <= cs + pc;
          end else if(fetch_step==1)begin
            if(!avm_m0_waitrequest)begin
              latch_readdata <= avm_m0_readdata;
              fetch_read <= 0;
              fetch_step <= 0;
              cycle<=1;
            end

          end
        end
      end else begin
        if(cmd_ack)begin
          debug_step_buff <= debug_step;
          cycle <= 0;
        end
      end

    end
  end
  

	reg [3:0] byteenable;
  reg [31:0] regfile [32];
  

  wire [31:0] tempa;
  assign tempa     = regfile[24];//r24 et Exception temporary
  wire [31:0] tempb;
  assign tempb     = regfile[25];//r25 bt Break temporary
  //r1 at
  wire [31:0] tempc;
  assign tempc     = regfile[1];//r1 at temporary

  wire [31:0] cs;
  assign cs     = regfile[29];//r29 ea Exception return address
  wire [31:0] ds;
  assign ds     = regfile[30];//r30 ba • Normal register set: Break return address • Shadow register sets: SSTATUS register

  reg [31:0] pc;

  wire [31:0] nextpc;
  assign nextpc = pc + 4;
  
  wire halt;
  assign halt = (halt_cpu || halt_uart) && (debug_step == debug_step_buff);
  
  reg halt_cpu;

  reg cmd_ack;
  
  reg [31:0] exec_address;
  reg        exec_read;
  reg        exec_write;
  reg [31:0] exec_writedata;
  
  reg [ 1:0] exec_step;
  

  wire comp_eq;
  assign comp_eq  = regfile[regA]==regfile[regB];
  
  always @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      cmd_ack<=0;
      exec_address<=0;
      exec_read<=0;
      exec_write<=0;
      exec_writedata<=0;
      
      regfile[0]<=0;//r0
      regfile[27]<=32'h0200_1000;//sp :sram  // total 0x1000 byte code:0x0000~0x07FF   data:0x0800 ~0x1000
      regfile[29]<=32'h0000_0000;//cs
      regfile[30]<=32'h0000_0000;//ds
       
      pc<=32'h0200_0000;
      byteenable <= 4'b1111;
      exec_step<=0;
       
      halt_cpu<=0;
    end else begin
      if(cycle==1 && cmd_ack==0)begin
        if(Rtype==0)begin
          //longcall
          //longjmp
          //unknow cmd

          //call sym                     @          20 @                      2 @   0 @ 0x00
          if         (cmd==6'd0)begin//ok
            pc <= {IMM26,2'b00};
            regfile[31] <= nextpc;
            cmd_ack <= 1;
          //jmpi sym                     @          20 @                      2 @   1 @ 0x01
          end else if(cmd==6'd1)begin//ok
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

          //andi reg, reg, ins           @          10 @                      0 @  12 @ 0x0c
          end else if(cmd==6'd12)begin//ok
            regfile[regB] <= {16'b0,(regfile[regA][15:0] & IMM16)};//rB ← rA & (0x0000 : IMM16)
            pc <= nextpc;
            cmd_ack <= 1;
          //addi reg, reg, ins           @          10 @                      0 @   4 @ 0x04
          end else if(cmd==6'd4)begin//ok
            regfile[regB] <= regfile[regA] + IMM16sx;//rB ← rA + σ(IMM16)
            pc <= nextpc;
            cmd_ack <= 1;
          //orhi reg, reg, ins           @          10 @                      0 @  52 @ 0x34
          end else if(cmd==6'd52)begin//ok
            regfile[regB] <= {(regfile[regA][31:16] | IMM16),regfile[regA][15:0]};//rB ← rA | (IMM16 : 0x0000)
            pc <= nextpc;
            cmd_ack <= 1;

          //ldw reg, regins              @          11 @                      0 @  23 @ 0x17
          end else if(cmd==6'd23)begin//ok
            if         (exec_step==0)begin
              exec_step <= 1;
              exec_address <= ds + regfile[regA] + IMM16sx;//rB ← Mem32[rA + σ(IMM16)]
              exec_read <= 1;
            end else if(exec_step==1)begin
              if(!avm_m0_waitrequest)begin
                regfile[regB] <= avm_m0_readdata;
                exec_read <= 0;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          //stw reg, regins              @          11 @                      0 @  21 @ 0x15
          end else if(cmd==6'd21)begin//ok
            if         (exec_step==0)begin
              exec_step <= 1;
              exec_address <= ds + regfile[regA] + IMM16sx;//Mem32[rA + σ(IMM16)] ← rB
              exec_writedata <= regfile[regB];
              exec_write <= 1;
            end else if(exec_step==1)begin
              if(!avm_m0_waitrequest)begin
                exec_write <= 0;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          //ldwio reg, regins            @          11 @                      0 @  55 @ 0x37
          end else if(cmd==6'd55)begin//ok
            if         (exec_step==0)begin
              exec_step <= 1;
              exec_address <= regfile[regA] + IMM16sx;//rB ← Mem32[rA + σ(IMM16)]
              exec_read <= 1;
            end else if(exec_step==1)begin
              if(!avm_m0_waitrequest)begin
                regfile[regB] <= avm_m0_readdata;
                exec_read <= 0;
                exec_step <= 0;
                pc <= nextpc;
                cmd_ack <= 1;
              end
            end
          //stwio reg, regins            @          11 @                      0 @  53 @ 0x35
          end else if(cmd==6'd53)begin//ok
            if         (exec_step==0)begin
              exec_step <= 1;
              exec_address <= regfile[regA] + IMM16sx;//Mem32[rA + σ(IMM16)] ← rB
              exec_writedata <= regfile[regB];
              exec_write <= 1;
            end else if(exec_step==1)begin
              if(!avm_m0_waitrequest)begin
                exec_write <= 0;
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
            pc <= regfile[31];
            cmd_ack <= 1;

          //add reg, reg, reg            @          30 @                      1 @  49 @ 0x3A,0x31
          end else if(cmd==6'd49)begin
            regfile[regC] <= regfile[regA] + regfile[regB];//rC ← rA + rB
            pc <= nextpc;
            cmd_ack <= 1;
          //cmpeq reg, reg, reg          @          30 @                      1 @  32 @ 0x3A,0x20
          end else if(cmd==6'd32)begin
            //if (rA == rB) then rC ← 1 else rC ← 0
            regfile[regC] <= {31'b0,comp_eq};
            pc <= nextpc;
            cmd_ack <= 1;
          //cmpne reg, reg, reg          @          30 @                      1 @  24 @ 0x3A,0x18
          end else if(cmd==6'd24)begin
            //if (rA != rB) then rC ← 1 else rC ← 0
            regfile[regC] <= {31'b0,(~comp_eq)};
            pc <= nextpc;
            cmd_ack <= 1;

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

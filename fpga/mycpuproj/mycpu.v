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
    halt_uart<=0;
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
        

      end else if (command == 8'h42) begin debug_data<=status; command_done<=1;
      end else if (command == 8'h43) begin debug_data<=pc; command_done<=1;
      end else if (command == 8'h47) begin debug_data<=regfile[data[3:0]]; command_done<=1;

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

//assign debug0 = cmd;
//assign debug1 = reg1;
//assign debug2 = reg2;
//assign debug3 = ins1;

	assign avm_m0_byteenable = byteenable;

	assign avm_m0_address = halt == 1 ? debug_address : (cycle == 0 ? (fetch_address) : (exec_address));

	assign avm_m0_writedata = halt == 1 ? debug_writedata : exec_writedata;
	assign avm_m0_write = halt == 1 ? debug_write : exec_write;

	assign avm_m0_read = halt == 1 ? debug_read : (cycle == 0 ? fetch_read : (exec_read));

  reg [7:0]  cmd;
  reg [3:0]  reg1;//4
  reg [3:0]  reg2;//4
  reg [31:0] ins10;
  reg [31:0] ins11;
  reg [31:0] ins2;

  reg cycle;//0:fetchCode 1:execCode
  reg [31:0] fetch_address;
  reg        fetch_read;
  reg [ 2:0] fetch_step;
  reg        debug_step_buff;
  always @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      cycle<=0;
      fetch_address<=0;
      fetch_read<=0;
      fetch_step<=0;

      cmd<=0;
      ins2<=0;
      debug_step_buff <= 0;
    end else begin
      if(cycle==0)begin
        if(!halt)begin
          if         (fetch_step==0)begin
            fetch_step <= 1;
            fetch_read <= 1;
            fetch_address <= cs + pc;
          end else if(fetch_step==1)begin
            if(!avm_m0_waitrequest)begin
              cmd <= avm_m0_readdata[31:24];
              reg1 <= avm_m0_readdata[23:20];
              reg2 <= avm_m0_readdata[19:16];
              ins10 <= {{16{avm_m0_readdata[15]}},avm_m0_readdata[15:0]};
              ins11 <= {{8{avm_m0_readdata[23]}},avm_m0_readdata[23:0]};
              fetch_read <= 0;
              if(avm_m0_readdata[31]==1)begin
                fetch_step<=2;
              end else begin
                ins2<=0;
                fetch_step<=0;
                cycle<=1;
              end
            end
          end else if(fetch_step==2)begin
            fetch_step <= 3;
            fetch_read <= 1;
            fetch_address <= cs + nextpc;
          end else if(fetch_step==3)begin
            if(!avm_m0_waitrequest)begin
              ins2 <= avm_m0_readdata;
              fetch_read <= 0;
              fetch_step<=0;
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
  reg [31:0] regfile [16];
  wire [31:0] eax;//0
  wire [31:0] ebx;//1
  wire [31:0] ecx;//2
  wire [31:0] edx;//3
  wire [31:0] ebp;//4
  wire [31:0] esp;//5
  wire [31:0] esi;//6
  wire [31:0] edi;//7
  wire [31:0] ra;//8
  wire [31:0] rb;//9
  wire [31:0] cs;
  wire [31:0] ds;

  assign eax    = regfile[0];
  assign ebx    = regfile[1];
  assign ecx    = regfile[2];
  assign edx    = regfile[3];
  assign ebp    = regfile[4];
  assign esp    = regfile[5];
  assign esi    = regfile[6];
  assign edi    = regfile[7];
  assign ra     = regfile[8];
  assign rb     = regfile[9];
  assign cs     = regfile[14];
  assign ds     = regfile[15];
  
  reg [31:0] status;//          overflow,sign neg,carry,zero
  wire zf;
  wire cf;
  wire sf;
  wire of;
  assign zf = status[0];
  assign cf = status[1];
  assign sf = status[2];
  assign of = status[3];
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
  
  reg [ 2:0] exec_step;
  
  reg [32:0] temp1;
  reg [32:0] temp2;
  reg [32:0] temp3;
  reg [8:0] temp81;
  reg [8:0] temp82;
  reg [8:0] temp83;
  
  wire [31:0] dsregfilereg2ins10;
  assign dsregfilereg2ins10 = ds + regfile[reg2] + ins10;
  
  wire [31:0] dssp;
  assign dssp = ds + regfile[5];
  wire [31:0] dssp_4;
  assign dssp_4 = dssp - 4;
  wire [31:0] sp_4;
  assign sp_4 = regfile[5] - 4;
  wire [31:0] sp4;
  assign sp4 = regfile[5] + 4;

  always @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      cmd_ack<=0;
      exec_address<=0;
      exec_read<=0;
      exec_write<=0;
      exec_writedata<=0;
      
      regfile[0]<=0;//eax
      regfile[1]<=0;//ebx
      regfile[2]<=0;//ecx
      regfile[3]<=0;//edx
      regfile[4]<=0;//ebp
      regfile[5]<=32'h0000_1000;//esp :sram  // total 0x1000 byte code:0x0000~0x07FF   data:0x0800 ~0x1000
      regfile[6]<=0;//esi
      regfile[7]<=0;//edi
      regfile[8]<=0;//ra
      regfile[9]<=0;//rb

      regfile[12]<=0;
      regfile[13]<=0;
      regfile[14]<=32'h0200_0000;//cs
      regfile[15]<=32'h0200_0000;//ds
       
      status<=0;
      pc<=32'h0000_0000;
      byteenable <= 4'b1111;
      exec_step<=0;
      
      halt_cpu<=0;
    end else begin
      if(cycle==1 && cmd_ack==0)begin
        //hlt                                       @     0 @   0 @ hlt
        if         (cmd==8'd00)begin//ok
          halt_cpu <= ins10[0];
          pc <= nextpc;
          cmd_ack <= 1;
        //mov !reg, DWORD PTR \[!reg!ins\]          @     2 @   2 @ mov eax, DWORD PTR [esp+20]
        end else if(cmd==8'd02)begin//ok
          if         (exec_step==0)begin
            exec_step <= 1;
            exec_address <= dsregfilereg2ins10;
            exec_read <= 1;
          end else if(exec_step==1)begin
            if(!avm_m0_waitrequest)begin
              regfile[reg1] <= avm_m0_readdata;
              exec_read <= 0;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end
        //mov DWORD PTR \[!reg!ins\], !reg          @     2 @   3 @ mov DWORD PTR [esp+28], eax
        end else if(cmd==8'd03)begin//ok
          if         (exec_step==0)begin
            exec_step <= 1;
            exec_address <= dsregfilereg2ins10;
            exec_writedata <= regfile[reg1];
            exec_write <= 1;
          end else if(exec_step==1)begin
            if(!avm_m0_waitrequest)begin
              exec_write <= 0;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end
          
        //in32 reg, reg                @           5 @          0 @  28 @ in32 ra,rb
        end else if(cmd==8'd28)begin//ok
          if         (exec_step==0)begin
            exec_step <= 1;
            exec_address <= regfile[reg2];
            exec_read <= 1;
          end else if(exec_step==1)begin
            if(!avm_m0_waitrequest)begin
              regfile[reg1] <= avm_m0_readdata;
              exec_read <= 0;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end
          
        //out32 reg, reg               @           5 @          0 @  29 @ out32 ra,rb
        end else if(cmd==8'd29)begin//ok
          if         (exec_step==0)begin
            exec_step <= 1;
            exec_address <= regfile[reg2];
            exec_writedata <= regfile[reg1];
            exec_write <= 1;
          end else if(exec_step==1)begin
            if(!avm_m0_waitrequest)begin
              exec_write <= 0;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end

          
        //mov BYTE PTR [eax], dl
        end else if(cmd==8'd04)begin//ok
          if         (exec_step==0)begin
            exec_step <= 1;
            exec_address <= {dsregfilereg2ins10[31:2],2'b0};
            case(dsregfilereg2ins10[1:0])
              0:begin byteenable <= 4'b0001; exec_writedata <= {24'b0,regfile[reg1][7:0]};end
              1:begin byteenable <= 4'b0010; exec_writedata <= {16'b0,regfile[reg1][7:0],8'b0};end
              2:begin byteenable <= 4'b0100; exec_writedata <= {8'b0,regfile[reg1][7:0],16'b0};end
              3:begin byteenable <= 4'b1000; exec_writedata <= {regfile[reg1][7:0],24'b0};end
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

        //movzx eax, BYTE PTR [esp+28]
        end else if(cmd==8'd05)begin//ok
          if         (exec_step==0)begin
            exec_step <= 1;
            exec_address <= {dsregfilereg2ins10[31:2],2'b0};
            exec_read <= 1;
          end else if(exec_step==1)begin
            if(!avm_m0_waitrequest)begin
              case(dsregfilereg2ins10[1:0])
                0:begin regfile[reg1] <= {24'b0,avm_m0_readdata[ 7: 0]};end
                1:begin regfile[reg1] <= {24'b0,avm_m0_readdata[15: 8]};end
                2:begin regfile[reg1] <= {24'b0,avm_m0_readdata[23:16]};end
                3:begin regfile[reg1] <= {24'b0,avm_m0_readdata[31:24]};end
              endcase
              exec_read <= 0;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end

        
        //mov !reg, !reg                            @     5 @  10 @ mov ebp, esp
        end else if(cmd==8'd10)begin//ok
          regfile[reg1] <= regfile[reg2];
          pc <= nextpc;
          cmd_ack <= 1;

        //movzx reg, reg8
        end else if(cmd==8'd11)begin//ok
          regfile[reg1] <= {24'b0,regfile[reg2][7:0]};
          pc <= nextpc;
          cmd_ack <= 1;
          
        //movsx reg, reg8
        end else if(cmd==8'd12)begin//ok
          regfile[reg1] <= {{24{regfile[reg2][8]}},regfile[reg2][7:0]};
          pc <= nextpc;
          cmd_ack <= 1;

        //13 @ lea eax, [ebp+12]
        end else if(cmd==8'd13)begin//ok
          regfile[reg1] <= regfile[reg2] + ins10;
          pc <= nextpc;
          cmd_ack <= 1;
          
              
              
        //push !reg                                 @     4 @  32 @ push	ebp
        end else if(cmd==8'd32)begin//ok
          //ESP <= ESP - 4;
          //(SS:ESP) <= (SOURCE); (* dword assignment *)          
          if         (exec_step==0)begin
            exec_step <= 1;
            exec_address <= dssp_4;//esp
            regfile[5] <= sp_4;//esp
            exec_writedata <= regfile[reg1];
            exec_write <= 1;
          end else if(exec_step==1)begin
            if(!avm_m0_waitrequest)begin
              exec_write <= 0;
              exec_step <= 0;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end

                
            
        //jmp !sym                                  @     7 @  33 @ jmp L3
        end else if(cmd==8'd33)begin//ok
          pc <= pc + ins11;
          cmd_ack <= 1;
                
        //je !sym                                   @     7 @  34 @ je L3
        end else if(cmd==8'd34)begin//ok
          if(zf)begin
            pc <= pc + ins11;
          end else begin
            pc <= nextpc;
          end
          cmd_ack <= 1;
        //jne !sym                                  @     7 @  34 @ jne L3
        end else if(cmd==8'd38)begin//ok
          if(!zf)begin
            pc <= pc + ins11;
          end else begin
            pc <= nextpc;
          end
          cmd_ack <= 1;
          
        //JL/JNGE (SF xor OF) = 1 less/not greater nor equal          
        end else if(cmd==8'd39)begin//ok
          if(sf ^ of)begin
            pc <= pc + ins11;
          end else begin
            pc <= nextpc;
          end
          cmd_ack <= 1;

        //call !sym                                 @     7 @  35 @ call ___main
        end else if(cmd==8'd35)begin//ok
          //ESP <= ESP - 4;
          //(SS:ESP) <= (SOURCE); (* dword assignment *)          
          if         (exec_step==0)begin
            exec_step <= 1;
            exec_address <= dssp_4;//esp
            regfile[5] <= sp_4;//esp
            exec_writedata <= nextpc;
            exec_write <= 1;
          end else if(exec_step==1)begin
            if(!avm_m0_waitrequest)begin
              exec_write <= 0;
              exec_step <= 0;
              pc <= pc + ins11;//pc+=4
              cmd_ack <= 1;
            end
          end
          
        //ret                                       @     0 @  36 @ ret
        end else if(cmd==8'd36)begin//ok
          //DEST ← (SS:SP); (* copy a dword *)
          //SP ← SP + 4;            
          if         (exec_step==0)begin
            exec_step <= 1;
            exec_address <= dssp;//esp
            regfile[5] <= sp4;//esp
            exec_read <= 1;
          end else if(exec_step==1)begin
            if(!avm_m0_waitrequest)begin
              exec_read <= 0;
              exec_step <= 0;
              pc = avm_m0_readdata;
              cmd_ack <= 1;
            end
          end
          
        //pop !reg                                  @          4 @          0 @  37 @ pop ebp
        end else if(cmd==8'd37)begin//ok
          //DEST ← (SS:SP); (* copy a dword *)
          //SP ← SP + 4;            
          if         (exec_step==0)begin
            exec_step <= 1;
            exec_address <= dssp;//esp
            regfile[5] <= sp4;//esp
            exec_read <= 1;
          end else if(exec_step==1)begin
            if(!avm_m0_waitrequest)begin
              exec_read <= 0;
              exec_step <= 0;
              regfile[reg1] = avm_m0_readdata;
              pc <= nextpc;
              cmd_ack <= 1;
            end
          end
          


        //add !reg, !reg                            @     5 @  17 @ add esp, ra
        end else if(cmd==8'd17)begin//ok
          temp1 = {1'b0,regfile[reg1]};
          temp2 = {1'b0,regfile[reg2]};
          temp3 = temp1 + temp2;
          regfile[reg1] <= temp3;
          pc <= nextpc;
          cmd_ack <= 1;
          
        //and !reg, !reg                            @     5 @  18 @ and esp, ra
        end else if(cmd==8'd18)begin//ok
          regfile[reg1] <= regfile[reg1] & regfile[reg2];
          pc <= nextpc;
          cmd_ack <= 1;
          
        //sub !reg, !reg                            @     5 @  19 @ sub esp, ra
        end else if(cmd==8'd19)begin//ok
          temp1 = {1'b0,regfile[reg1]};
          temp2 = {1'b0,regfile[reg2]};
          temp3 = temp1 - temp2;
          regfile[reg1] <= temp3;
          pc <= nextpc;
          cmd_ack <= 1;
          
        //cmp !reg, !reg                            @     5 @  16 @ cmp eax, ra
        end else if(cmd==8'd16)begin//ok
          temp1 = {1'b0,regfile[reg1]};
          temp2 = {1'b0,regfile[reg2]};
          temp3 = temp1 - temp2;
          status[0]<=temp3[31:0]==0;//zf
          status[1]<=temp3[32];//cf
          status[2]<=temp3[31];//sf
          status[3]<=temp3[32]^temp3[31]^regfile[reg1][31]^regfile[reg2][31];//of
          
          pc <= nextpc;
          cmd_ack <= 1;
        //cmp reg8, reg8            @           5 @          0 @  20 @ cmp eax, ra
        end else if(cmd==8'd21)begin//ok
          temp81 = {1'b0,regfile[reg1][7:0]};
          temp82 = {1'b0,regfile[reg2][7:0]};
          temp83 = temp81 - temp82;
          status[0]<=temp83[7:0]==0;//zf
          status[1]<=temp83[8];//cf
          status[2]<=temp83[7];//sf
          status[3]<=temp83[8]^temp83[7]^regfile[reg1][7]^regfile[reg2][7];//of
          
          pc <= nextpc;
          cmd_ack <= 1;
          
        //test reg, reg            @           5 @          0 @  20 @ test eax, ra
        end else if(cmd==8'd20)begin//ok
          temp1 = {1'b0,regfile[reg1]};
          temp2 = {1'b0,regfile[reg2]};
          temp3 = temp1 & temp2;
          status[0]<=temp3[31:0]==0;//zf
          pc <= nextpc;
          cmd_ack <= 1;

        //test reg8, reg8              @          10 @          0 @  22 @ test al, bl
        end else if(cmd==8'd22)begin//ok
          temp81 = {1'b0,regfile[reg1][7:0]};
          temp82 = {1'b0,regfile[reg2][7:0]};
          temp83 = temp81 & temp82;
          status[0]<=temp83[7:0]==0;//zf
          pc <= nextpc;
          cmd_ack <= 1;

        //mov !reg, !ins                            @     1 @ 128 @
        end else if(cmd==8'd128)begin//ok
          regfile[reg1] <= ins2;
          pc <= pc + 8;
          cmd_ack <= 1;
        
        //unknow cmd
        end else begin
          halt_cpu <= 1;
          pc <= nextpc;
          cmd_ack <= 1;

        end
      end
      
      if(cycle==0 && cmd_ack==1)begin
        cmd_ack<=0;
      end
    end
  end



//MOVS/MOVSB/MOVSW/MOVSD    Move Data from String to String

//The terms "above" and "below" refer to the relation between two
//unsigned values (neither SF nor OF is tested). The terms "greater" and
//"less" refer to the relation between two signed values (SF and OF are
//tested).
//
//Bit Name Function
//0 CF Carry Flag    Set on high-order bit carry or borrow; cleared
//otherwise.
//2 PF Parity Flag    Set if low-order eight bits of result contain
//an even number of 1 bits; cleared otherwise.
//4 AF Adjust flag    Set on carry from or borrow to the low order
//four bits of AL; cleared otherwise. Used for decimal
//arithmetic.
//6 ZF Zero Flag    Set if result is zero; cleared otherwise.
//7 SF Sign Flag    Set equal to high-order bit of result (0 is
//positive, 1 if negative).
//11 OF Overflow Flag    Set if result is too large a positive number
//or too small a negative number (excluding sign-bit) to fit in
//destination operand; cleared otherwise.



//Instruction           |OF|SF|ZF|AF|PF|CF
//AAA                   |  |  |  |TM|  | M
//AAS                   |  |  |  |TM|  | M
//AAD                   |  | M| M|  | M|  
//AAM                   |  | M| M|  | M|  
//DAA                   |  | M| M|TM| M|TM
//DAS                   |  | M| M|TM| M|TM
//ADC                   | M| M| M| M| M|TM
//ADD                   | M| M| M| M| M| M
//SBB                   | M| M| M| M| M|TM
//SUB                   | M| M| M| M| M| M
//CMP                   | M| M| M| M| M| M
//CMPS                  | M| M| M| M| M| M
//SCAS                  | M| M| M| M| M| M
//NEG                   | M| M| M| M| M| M
//DEC                   | M| M| M| M| M|
//INC                   | M| M| M| M| M|
//IMUL                  | M|  |  |  |  | M
//MUL                   | M|  |  |  |  | M
//RCL/RCR 1             | M|  |  |  |  |TM
//RCL/RCR count         |  |  |  |  |  |TM
//ROL/ROR 1             | M|  |  |  |  | M
//ROL/ROR count         |  |  |  |  |  | M
//SAL/SAR/SHL/SHR 1     | M| M| M|  | M| M
//SAL/SAR/SHL/SHR count |  | M| M|  | M| M
//SHLD/SHRD             |  | M| M|  | M| M
//BSF/BSR               |  |  | M|  |  |  
//BT/BTS/BTR/BTC        |  |  |  |  |  | M
//AND                   | 0| M| M|  | M| 0
//OR                    | 0| M| M|  | M| 0
//TEST                  | 0| M| M|  | M| 0
//XOR                   | 0| M| M|  | M| 0

endmodule

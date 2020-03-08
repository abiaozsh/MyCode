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

assign debug[0] = halt;

reg command_done;
reg [7:0] timer;
reg [7:0] step;
reg [7:0] timer_log;

always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin

    command_done <= 0;
    timer<=0;
		step<=0;
		timer_log<=0;
		//byteenable<=4'b1111;
		
  end else begin
    uart_send<=0;
    
    if(command_done)begin
      if          (command == 8'h00) begin 
        command_done<=0;
      end
    end else begin//command_done==0
      if          (command == 8'h00) begin

      end else if (command == 8'hA0) begin uart_send<=1; uart_data_in<=regfile[ 0][ 7: 0]; command_done<=1;
      end else if (command == 8'hA1) begin uart_send<=1; uart_data_in<=regfile[ 0][15: 8]; command_done<=1;
      end else if (command == 8'hA2) begin uart_send<=1; uart_data_in<=regfile[ 0][23:16]; command_done<=1;
      end else if (command == 8'hA3) begin uart_send<=1; uart_data_in<=regfile[ 0][31:24]; command_done<=1;
      end else if (command == 8'hA4) begin uart_send<=1; uart_data_in<=regfile[ 1][ 7: 0]; command_done<=1;
      end else if (command == 8'hA5) begin uart_send<=1; uart_data_in<=regfile[ 1][15: 8]; command_done<=1;
      end else if (command == 8'hA6) begin uart_send<=1; uart_data_in<=regfile[ 1][23:16]; command_done<=1;
      end else if (command == 8'hA7) begin uart_send<=1; uart_data_in<=regfile[ 1][31:24]; command_done<=1;
      end else if (command == 8'hA8) begin uart_send<=1; uart_data_in<=regfile[ 2][ 7: 0]; command_done<=1;
      end else if (command == 8'hA9) begin uart_send<=1; uart_data_in<=regfile[ 2][15: 8]; command_done<=1;
      end else if (command == 8'hAA) begin uart_send<=1; uart_data_in<=regfile[ 2][23:16]; command_done<=1;
      end else if (command == 8'hAB) begin uart_send<=1; uart_data_in<=regfile[ 2][31:24]; command_done<=1;
      end else if (command == 8'hAC) begin uart_send<=1; uart_data_in<=regfile[ 3][ 7: 0]; command_done<=1;
      end else if (command == 8'hAD) begin uart_send<=1; uart_data_in<=regfile[ 3][15: 8]; command_done<=1;
      end else if (command == 8'hAE) begin uart_send<=1; uart_data_in<=regfile[ 3][23:16]; command_done<=1;
      end else if (command == 8'hAF) begin uart_send<=1; uart_data_in<=regfile[ 3][31:24]; command_done<=1;
      end else if (command == 8'hB0) begin uart_send<=1; uart_data_in<=regfile[ 4][ 7: 0]; command_done<=1;
      end else if (command == 8'hB1) begin uart_send<=1; uart_data_in<=regfile[ 4][15: 8]; command_done<=1;
      end else if (command == 8'hB2) begin uart_send<=1; uart_data_in<=regfile[ 4][23:16]; command_done<=1;
      end else if (command == 8'hB3) begin uart_send<=1; uart_data_in<=regfile[ 4][31:24]; command_done<=1;
      end else if (command == 8'hB4) begin uart_send<=1; uart_data_in<=regfile[ 5][ 7: 0]; command_done<=1;
      end else if (command == 8'hB5) begin uart_send<=1; uart_data_in<=regfile[ 5][15: 8]; command_done<=1;
      end else if (command == 8'hB6) begin uart_send<=1; uart_data_in<=regfile[ 5][23:16]; command_done<=1;
      end else if (command == 8'hB7) begin uart_send<=1; uart_data_in<=regfile[ 5][31:24]; command_done<=1;
      end else if (command == 8'hB8) begin uart_send<=1; uart_data_in<=regfile[ 6][ 7: 0]; command_done<=1;
      end else if (command == 8'hB9) begin uart_send<=1; uart_data_in<=regfile[ 6][15: 8]; command_done<=1;
      end else if (command == 8'hBA) begin uart_send<=1; uart_data_in<=regfile[ 6][23:16]; command_done<=1;
      end else if (command == 8'hBB) begin uart_send<=1; uart_data_in<=regfile[ 6][31:24]; command_done<=1;
      end else if (command == 8'hBC) begin uart_send<=1; uart_data_in<=regfile[ 7][ 7: 0]; command_done<=1;
      end else if (command == 8'hBD) begin uart_send<=1; uart_data_in<=regfile[ 7][15: 8]; command_done<=1;
      end else if (command == 8'hBE) begin uart_send<=1; uart_data_in<=regfile[ 7][23:16]; command_done<=1;
      end else if (command == 8'hBF) begin uart_send<=1; uart_data_in<=regfile[ 7][31:24]; command_done<=1;
      end else if (command == 8'hC0) begin uart_send<=1; uart_data_in<=regfile[ 8][ 7: 0]; command_done<=1;
      end else if (command == 8'hC1) begin uart_send<=1; uart_data_in<=regfile[ 8][15: 8]; command_done<=1;
      end else if (command == 8'hC2) begin uart_send<=1; uart_data_in<=regfile[ 8][23:16]; command_done<=1;
      end else if (command == 8'hC3) begin uart_send<=1; uart_data_in<=regfile[ 8][31:24]; command_done<=1;
      end else if (command == 8'hC4) begin uart_send<=1; uart_data_in<=regfile[ 9][ 7: 0]; command_done<=1;
      end else if (command == 8'hC5) begin uart_send<=1; uart_data_in<=regfile[ 9][15: 8]; command_done<=1;
      end else if (command == 8'hC6) begin uart_send<=1; uart_data_in<=regfile[ 9][23:16]; command_done<=1;
      end else if (command == 8'hC7) begin uart_send<=1; uart_data_in<=regfile[ 9][31:24]; command_done<=1;
      end else if (command == 8'hC8) begin uart_send<=1; uart_data_in<=regfile[10][ 7: 0]; command_done<=1;
      end else if (command == 8'hC9) begin uart_send<=1; uart_data_in<=regfile[10][15: 8]; command_done<=1;
      end else if (command == 8'hCA) begin uart_send<=1; uart_data_in<=regfile[10][23:16]; command_done<=1;
      end else if (command == 8'hCB) begin uart_send<=1; uart_data_in<=regfile[10][31:24]; command_done<=1;
      end else if (command == 8'hCC) begin uart_send<=1; uart_data_in<=regfile[11][ 7: 0]; command_done<=1;
      end else if (command == 8'hCD) begin uart_send<=1; uart_data_in<=regfile[11][15: 8]; command_done<=1;
      end else if (command == 8'hCE) begin uart_send<=1; uart_data_in<=regfile[11][23:16]; command_done<=1;
      end else if (command == 8'hCF) begin uart_send<=1; uart_data_in<=regfile[11][31:24]; command_done<=1;
      end else if (command == 8'hD0) begin uart_send<=1; uart_data_in<=regfile[12][ 7: 0]; command_done<=1;
      end else if (command == 8'hD1) begin uart_send<=1; uart_data_in<=regfile[12][15: 8]; command_done<=1;
      end else if (command == 8'hD2) begin uart_send<=1; uart_data_in<=regfile[12][23:16]; command_done<=1;
      end else if (command == 8'hD3) begin uart_send<=1; uart_data_in<=regfile[12][31:24]; command_done<=1;
      end else if (command == 8'hD4) begin uart_send<=1; uart_data_in<=regfile[13][ 7: 0]; command_done<=1;
      end else if (command == 8'hD5) begin uart_send<=1; uart_data_in<=regfile[13][15: 8]; command_done<=1;
      end else if (command == 8'hD6) begin uart_send<=1; uart_data_in<=regfile[13][23:16]; command_done<=1;
      end else if (command == 8'hD7) begin uart_send<=1; uart_data_in<=regfile[13][31:24]; command_done<=1;
      end else if (command == 8'hD8) begin uart_send<=1; uart_data_in<=regfile[14][ 7: 0]; command_done<=1;
      end else if (command == 8'hD9) begin uart_send<=1; uart_data_in<=regfile[14][15: 8]; command_done<=1;
      end else if (command == 8'hDA) begin uart_send<=1; uart_data_in<=regfile[14][23:16]; command_done<=1;
      end else if (command == 8'hDB) begin uart_send<=1; uart_data_in<=regfile[14][31:24]; command_done<=1;
      end else if (command == 8'hDC) begin uart_send<=1; uart_data_in<=regfile[15][ 7: 0]; command_done<=1;
      end else if (command == 8'hDD) begin uart_send<=1; uart_data_in<=regfile[15][15: 8]; command_done<=1;
      end else if (command == 8'hDE) begin uart_send<=1; uart_data_in<=regfile[15][23:16]; command_done<=1;
      end else if (command == 8'hDF) begin uart_send<=1; uart_data_in<=regfile[15][31:24]; command_done<=1;
				
      end else begin
        command_done<=1;
      end
    end


  end
end




	assign avm_m0_byteenable = byteenable;

	assign avm_m0_address = cycle == 0 ? (fetch_address + cs) : (exec_address + ds);

	assign avm_m0_writedata = exec_writedata;
	assign avm_m0_write = exec_write;

	assign avm_m0_read = cycle == 0 ? fetch_read : (exec_read);

  assign eax    = regfile[0];
  assign ebx    = regfile[1];
  assign ecx    = regfile[2];
  assign edx    = regfile[3];
  assign ebp    = regfile[4];
  assign esp    = regfile[5];
  
  assign cs     = regfile[12];
  assign ds     = regfile[13];
  assign status = regfile[14];
  assign pc     = regfile[15];

  reg [31:0] cmd;
  reg [31:0] ins2;

  reg cycle;//0:fetchCode 1:execCode
  reg [31:0] fetch_address;
  reg        fetch_read;
  reg [ 2:0] fetch_step;
  always @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      cycle<=0;
      fetch_address<=0;
      fetch_read<=0;
      fetch_step<=0;

      cmd<=0;
      ins2<=0;

    end else begin
			if(cycle==0)begin
				if(!halt)begin
          if         (fetch_step==0)begin
            fetch_step <= 1;
            fetch_read <= 1;
            fetch_address <= regfile[15];//pc
          end else if(fetch_step==1)begin
            if(!avm_m0_waitrequest)begin
              cmd <= avm_m0_readdata;
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
            fetch_address <= regfile[15] + 8;//pc
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
  wire [31:0] cs;
  wire [31:0] ds;
  wire [31:0] status;
  wire [31:0] pc;

  reg halt;
  wire        prefix;//1
  wire [6:0]  ins;//7
  wire [3:0]  reg1;//4
  wire [3:0]  reg2;//4
  wire [15:0] ins1;//16
  assign prefix = cmd[31];
  assign ins = cmd[30:24];
  assign reg1 = cmd[23:20];
  assign reg2 = cmd[19:16];
  assign ins1 = cmd[15:0];

  reg cmd_ack;
  
  reg [31:0] exec_address;
  reg        exec_read;
  reg        exec_write;
  reg [31:0] exec_writedata;
  
  reg [ 2:0] exec_step;
  always @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      cmd_ack=0;
      exec_address<=0;
      exec_read<=0;
      exec_write<=0;
      exec_writedata<=0;
      
      regfile[0]<=0;//eax
      regfile[1]<=0;//ebx
      regfile[2]<=0;//ecx
      regfile[3]<=0;//edx
      regfile[4]<=0;//ebp
      regfile[5]<=0;//esp
      
      regfile[12]<=0;//cs
      regfile[13]<=0;//ds
      regfile[14]<=0;//status
      regfile[15]<=32'h02000000;//pc
      
      byteenable <= 4'b1111;
      exec_step<=0;
      
      halt<=0;
    end else begin
      if(cycle==1 && cmd_ack==0)begin
        if(prefix==0)begin

          //case 0: sIns = "nop"; break;
          if         (ins==7'h00)begin//halt                                       @ 4 @       0 @  0 @ halt
            halt <= 1;
            cmd_ack = 1;

          //case 1: sIns = "mov DWORD PTR [" + sreg1 + " " + sins1 + "], " + sreg2 + ""; break;
          end else if(ins==7'h01)begin
            if         (exec_step==0)begin
              exec_step <= 1;
              exec_address <= regfile[reg1] + ins1;
              exec_writedata <= ins2;
            end else if(exec_step==1)begin
							if(!avm_m0_waitrequest)begin
								cmd_ack = 1;
              end
            end
          //case 2: sIns = "mov " + sreg1 + ", DWORD PTR [" + sreg2 + " " + sins1 + "]"; break;
          //case 3: sIns = "cmp " + sreg1 + ", DWORD PTR [" + sreg2 + " " + sins1 + "]"; break;
          //case 4: sIns = "push " + sreg1 + ""; break;
          //case 5: sIns = "mov " + sreg1 + ", " + sreg2 + ""; break;
          //case 6: sIns = "call " + relPos; break;
          //case 7: sIns = "jmp " + relPos; break;
          //case 8: sIns = "je " + relPos; break;
          //case 9: sIns = "ret"; break;
          
          end
          
					if(cmd_ack==1)begin
					 regfile[15]<=regfile[15]+4;//pc+=4
					end
          
        end else begin
          //case 0: sIns = "mov DWORD PTR [" + sreg1 + " " + sins1 + "], " + sins2; break;
          if         (ins==7'h00)begin
            if         (exec_step==0)begin
              exec_step <= 1;
              exec_address <= regfile[reg1] + ins1;
              exec_writedata <= ins2;
            end else if(exec_step==1)begin
              
							cmd_ack = 1;
              
            end
          //case 1: sIns = "add DWORD PTR [" + sreg1 + " " + sins1 + "], " + sins2; break;
          end else if(ins==7'h01)begin//

          //case 2: sIns = "and " + sreg1 + ", " + sins2; break;
          //case 3: sIns = "sub " + sreg1 + ", " + sins2; break;
          
          end
					
					if(cmd_ack==1)begin
					  regfile[15]<=regfile[15]+8;//pc+=8
					end
          
        end
      end
      
      if(cycle==0 && cmd_ack==1)begin
        cmd_ack=0;
      end
    end
  end







endmodule

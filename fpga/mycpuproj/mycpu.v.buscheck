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

reg [31:0] addr_buff;
reg [31:0] data_topc0;
reg [31:0] data_frompc0;
reg [31:0] data_topc1;
reg [31:0] data_frompc1;
reg [31:0] data_topc2;
reg [31:0] data_frompc2;
reg [31:0] data_topc3;
reg [31:0] data_frompc3;

reg [31:0] address      ;
reg        read         ;
reg [3:0]  byteenable   ;
reg        write        ;
reg [31:0] writedata    ;

assign avm_m0_address       = address      ;
assign avm_m0_read          = read         ;
assign avm_m0_byteenable    = byteenable   ;
assign avm_m0_write         = write        ;
assign avm_m0_writedata     = writedata    ;

assign debug1 = command;
assign debug2 = timer;
assign debug3 = timer_log;

assign debug[0] = avm_m0_waitrequest;
assign debug[1] = 0;
assign debug[2] = avm_m0_read;
assign debug[3] = avm_m0_write;
assign debug[4] = command_done;
assign debug[5] = command!=0;
assign debug[6] = 0;
assign debug[7] = 0;

reg command_done;
reg [7:0] timer;
reg [7:0] step;
reg [7:0] timer_log;
//avm_m0_address = 32'b00000000000000000000000000000000;
//avm_m0_writedata = 32'b00000000000000000000000000000000;
//avm_m0_write = 1'b0;
//avm_m0_read = 1'b0;
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin
    address <= 0;
    writedata <= 0;
    write <= 0;
    read <= 0;
    
    addr_buff<=0;
    data_topc0<=0;
		data_frompc0<=0;
    data_topc1<=0;
		data_frompc1<=0;
    data_topc2<=0;
		data_frompc2<=0;
    data_topc3<=0;
		data_frompc3<=0;

    command_done <= 0;
    timer<=0;
		step<=0;
		timer_log<=0;
		byteenable<=4'b1111;
		
  end else begin
    uart_send<=0;
    
    if(command_done)begin
      if          (command == 8'h00) begin 
        command_done<=0;
      end
    end else begin//command_done==0
      if          (command == 8'h00) begin

      end else if (command == 8'h10) begin addr_buff[ 7: 0]<=data; command_done<=1;
      end else if (command == 8'h11) begin addr_buff[15: 8]<=data; command_done<=1;
      end else if (command == 8'h12) begin addr_buff[23:16]<=data; command_done<=1;
      end else if (command == 8'h13) begin addr_buff[31:24]<=data; command_done<=1;
			
      end else if (command == 8'h60) begin data_frompc0[ 7: 0]<=data; command_done<=1;
      end else if (command == 8'h61) begin data_frompc0[15: 8]<=data; command_done<=1;
      end else if (command == 8'h62) begin data_frompc0[23:16]<=data; command_done<=1;
      end else if (command == 8'h63) begin data_frompc0[31:24]<=data; command_done<=1;
      end else if (command == 8'h64) begin data_frompc1[ 7: 0]<=data; command_done<=1;
      end else if (command == 8'h65) begin data_frompc1[15: 8]<=data; command_done<=1;
      end else if (command == 8'h66) begin data_frompc1[23:16]<=data; command_done<=1;
      end else if (command == 8'h67) begin data_frompc1[31:24]<=data; command_done<=1;
      end else if (command == 8'h68) begin data_frompc2[ 7: 0]<=data; command_done<=1;
      end else if (command == 8'h69) begin data_frompc2[15: 8]<=data; command_done<=1;
      end else if (command == 8'h6A) begin data_frompc2[23:16]<=data; command_done<=1;
      end else if (command == 8'h6B) begin data_frompc2[31:24]<=data; command_done<=1;
      end else if (command == 8'h6C) begin data_frompc3[ 7: 0]<=data; command_done<=1;
      end else if (command == 8'h6D) begin data_frompc3[15: 8]<=data; command_done<=1;
      end else if (command == 8'h6E) begin data_frompc3[23:16]<=data; command_done<=1;
      end else if (command == 8'h6F) begin data_frompc3[31:24]<=data; command_done<=1;

      end else if (command == 8'h70) begin uart_send<=1; uart_data_in<=data_topc0[ 7: 0]; command_done<=1;
      end else if (command == 8'h71) begin uart_send<=1; uart_data_in<=data_topc0[15: 8]; command_done<=1;
      end else if (command == 8'h72) begin uart_send<=1; uart_data_in<=data_topc0[23:16]; command_done<=1;
      end else if (command == 8'h73) begin uart_send<=1; uart_data_in<=data_topc0[31:24]; command_done<=1;
      end else if (command == 8'h74) begin uart_send<=1; uart_data_in<=data_topc1[ 7: 0]; command_done<=1;
      end else if (command == 8'h75) begin uart_send<=1; uart_data_in<=data_topc1[15: 8]; command_done<=1;
      end else if (command == 8'h76) begin uart_send<=1; uart_data_in<=data_topc1[23:16]; command_done<=1;
      end else if (command == 8'h77) begin uart_send<=1; uart_data_in<=data_topc1[31:24]; command_done<=1;
      end else if (command == 8'h78) begin uart_send<=1; uart_data_in<=data_topc2[ 7: 0]; command_done<=1;
      end else if (command == 8'h79) begin uart_send<=1; uart_data_in<=data_topc2[15: 8]; command_done<=1;
      end else if (command == 8'h7A) begin uart_send<=1; uart_data_in<=data_topc2[23:16]; command_done<=1;
      end else if (command == 8'h7B) begin uart_send<=1; uart_data_in<=data_topc2[31:24]; command_done<=1;
      end else if (command == 8'h7C) begin uart_send<=1; uart_data_in<=data_topc3[ 7: 0]; command_done<=1;
      end else if (command == 8'h7D) begin uart_send<=1; uart_data_in<=data_topc3[15: 8]; command_done<=1;
      end else if (command == 8'h7E) begin uart_send<=1; uart_data_in<=data_topc3[23:16]; command_done<=1;
      end else if (command == 8'h7F) begin uart_send<=1; uart_data_in<=data_topc3[31:24]; command_done<=1;

      end else if (command == 8'h30) begin//byte read
        timer<=timer+1'b1;
        if(timer==0)begin
          read<=1;
          address <= {addr_buff[31:2],2'b00};
        end else begin
          if(!avm_m0_waitrequest)begin
            if         (addr_buff[1:0]==0)begin
              data_topc0<={24'b0,avm_m0_readdata[7:0]};
            end else if(addr_buff[1:0]==1)begin
              data_topc0<={24'b0,avm_m0_readdata[15:8]};
            end else if(addr_buff[1:0]==2)begin
              data_topc0<={24'b0,avm_m0_readdata[23:16]};
            end else if(addr_buff[1:0]==3)begin
              data_topc0<={24'b0,avm_m0_readdata[31:24]};
            end
            timer_log<=timer;
            timer <= 0;
            read <= 0;
            command_done<=1;
          end
        end
				
      end else if (command == 8'h31) begin//word read
        timer<=timer+1'b1;
        if(timer==0)begin
          read<=1;
          address <= {addr_buff[31:2],2'b00};
        end else begin
          if(!avm_m0_waitrequest)begin
            if         (addr_buff[1:0]==0)begin
              data_topc0<={16'b0,avm_m0_readdata[15:0]};
            end else if(addr_buff[1:0]==2)begin
              data_topc0<={16'b0,avm_m0_readdata[31:16]};
            end
            timer_log<=timer;
            timer <= 0;
            read <= 0;
            command_done<=1;
          end
        end
				
      end else if (command == 8'h32) begin//dword read
        timer<=timer+1'b1;
        if(timer==0)begin
          read<=1;
          address <= {addr_buff[31:2],2'b00};
        end else begin
          if(!avm_m0_waitrequest)begin
            data_topc0<=avm_m0_readdata;
            timer_log<=timer;
            timer <= 0;
            read <= 0;
            command_done<=1;
          end
        end
				
      end else if (command == 8'h33) begin//qword read
				timer<=timer+1'b1;
        if(step==0)begin
					step<=1;
          read<=1;
          address <= {addr_buff[31:2],2'b00};
        end else if(step==1)begin
          if(!avm_m0_waitrequest)begin
						address <= {(addr_buff[31:2]+1),2'b00};
            data_topc0<=avm_m0_readdata;
            step <= 2;
          end
        end else if(step==2)begin
          if(!avm_m0_waitrequest)begin
            data_topc1<=avm_m0_readdata;
            step <= 0;
						read <= 0;
						timer_log<=timer;
						timer <= 0;
						command_done<=1;
          end
        end
				
      end else if (command == 8'h34) begin//16byte read
				timer<=timer+1'b1;
        if(step==0)begin
					step<=1;
          read<=1;
          address <= {addr_buff[31:2],2'b00};
        end else if(step==1)begin
          if(!avm_m0_waitrequest)begin
						address <= {(addr_buff[31:2]+1),2'b00};
            data_topc0<=avm_m0_readdata;
            step <= 2;
          end
        end else if(step==2)begin
          if(!avm_m0_waitrequest)begin
						address <= {(addr_buff[31:2]+2),2'b00};
            data_topc1<=avm_m0_readdata;
            step <= 3;
          end
        end else if(step==3)begin
          if(!avm_m0_waitrequest)begin
						address <= {(addr_buff[31:2]+3),2'b00};
            data_topc2<=avm_m0_readdata;
            step <= 4;
          end
        end else if(step==4)begin
          if(!avm_m0_waitrequest)begin
            data_topc3<=avm_m0_readdata;
            step <= 0;
						read <= 0;
						timer_log<=timer;
						timer <= 0;
						command_done<=1;
          end
        end

				
				
      end else if (command == 8'h40) begin//byte write
				timer<=timer+1'b1;
        if(timer==0)begin
					timer<=1;
					address <= {addr_buff[31:2],2'b00};
          if         (addr_buff[1:0]==0)begin
            writedata <= {24'b0,data_frompc0[7:0]};
            byteenable <= 4'b0001;
          end else if(addr_buff[1:0]==1)begin
            writedata <= {16'b0,data_frompc0[7:0],8'b0};
            byteenable <= 4'b0010;
          end else if(addr_buff[1:0]==2)begin
            writedata <= {8'b0,data_frompc0[7:0],16'b0};
            byteenable <= 4'b0100;
          end else if(addr_buff[1:0]==3)begin
            writedata <= {data_frompc0[7:0],24'b0};
            byteenable <= 4'b1000;
          end
					write<=1;
        end else begin
          if(!avm_m0_waitrequest)begin
            write <= 0;
						byteenable <= 4'b1111;
            timer_log<=timer;
            timer<=0;
            command_done<=1;
          end
        end
        
      end else if (command == 8'h41) begin//word write
				timer<=timer+1'b1;
        if(timer==0)begin
					timer<=1;
					address <= {addr_buff[31:2],2'b00};
          if         (addr_buff[1:0]==0)begin
            writedata <= {16'b0,data_frompc0[15:0]};
            byteenable <= 4'b0011;
          end else if(addr_buff[1:0]==2)begin
            writedata <= {data_frompc0[15:0],16'b0};
            byteenable <= 4'b1100;
          end
					write<=1;
        end else begin
          if(!avm_m0_waitrequest)begin
            write <= 0;
						byteenable <= 4'b1111;
            timer_log<=timer;
            timer<=0;
            command_done<=1;
          end
        end
        
      end else if (command == 8'h42) begin//dword write
				timer<=timer+1'b1;
        if(timer==0)begin
					timer<=1;
					address <= {addr_buff[31:2],2'b00};
					writedata <= data_frompc0;
					write<=1;
        end else begin
          if(!avm_m0_waitrequest)begin
            write <= 0;
            timer_log<=timer;
            timer<=0;
            command_done<=1;
          end
        end
				
      end else if (command == 8'h43) begin//dword write
				timer<=timer+1'b1;
        if(step==0)begin
					step<=1;
					write<=1;
					address <= {addr_buff[31:2],2'b00};
					writedata <= data_frompc0;
        end else if(step==1)begin
          if(!avm_m0_waitrequest)begin
						step<=2;
						address <= {(addr_buff[31:2]+1),2'b00};
						writedata <= data_frompc1;
          end
        end else if(step==2)begin
          if(!avm_m0_waitrequest)begin
						step <= 0;
            write <= 0;
            timer_log<=timer;
            timer<=0;
            command_done<=1;
          end
        end
				
      end else if (command == 8'h44) begin//dword write
				timer<=timer+1'b1;
        if(step==0)begin
					step<=1;
					write<=1;
					address <= {addr_buff[31:2],2'b00};
					writedata <= data_frompc0;
        end else if(step==1)begin
          if(!avm_m0_waitrequest)begin
						step<=2;
						address <= {(addr_buff[31:2]+1),2'b00};
						writedata <= data_frompc1;
          end
        end else if(step==2)begin
          if(!avm_m0_waitrequest)begin
						step<=3;
						address <= {(addr_buff[31:2]+2),2'b00};
						writedata <= data_frompc2;
          end
        end else if(step==3)begin
          if(!avm_m0_waitrequest)begin
						step<=4;
						address <= {(addr_buff[31:2]+3),2'b00};
						writedata <= data_frompc3;
          end
        end else if(step==4)begin
          if(!avm_m0_waitrequest)begin
						step <= 0;
            write <= 0;
            timer_log<=timer;
            timer<=0;
            command_done<=1;
          end
        end

				
      end else begin
        command_done<=1;
      end
    end



  end
end


  
  
  
  
  
  
  
  /*
  
  
  
  
	// TODO: Auto-generated HDL template


	assign avm_m0_address = cycle == 0 ? (fetch_address + cs) : (exec_address + ds);

	assign avm_m0_writedata = exec_writedata;
	assign avm_m0_write = exec_write;

	assign avm_m0_read = cycle == 0 ? fetch_read : ();

	assign avm_m0_byteenable = 1'b0;

  reg [31:0] regfile [3:0];
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
      
      regfile[0]<=0;//eax
      regfile[1]<=0;//ebx
      regfile[2]<=0;//ecx
      regfile[3]<=0;//edx
      regfile[4]<=0;//ebp
      regfile[5]<=0;//esp
      
      regfile[12]<=0;//cs
      regfile[13]<=0;//ds
      regfile[14]<=0;//status
      regfile[15]<=0;//pc
      
      
      cmd<=0;
      ins2<=0;

    end else begin
      if(cycle==0)begin
        if         (fetch_step==0)begin
          fetch_step <= 1;
          fetch_read <= 1;
          fetch_address <= regfile[15];//pc
        end else if(fetch_step==1)begin
          if(!avm_m0_waitrequest && avm_m0_readdatavalid)begin
            cmd <= avm_m0_readdata;
            fetch_read <= 0;
            regfile[15] <= regfile[15] + 4;//pc
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
          fetch_address <= regfile[15];//pc
        end else if(fetch_step==3)begin
          if(!avm_m0_waitrequest && avm_m0_readdatavalid)begin
            ins2 <= avm_m0_readdata;
            fetch_read <= 0;
            regfile[15] <= regfile[15] + 4;//pc
            fetch_step<=0;
            cycle<=1;
          end

        end
      end else begin
        if(cmd_ack)begin
        end
      end
    
    end
  end

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
      cmd_ack<=0;
      exec_address<=0;
      exec_read<=0;
      exec_write<=0;
      exec_writedata<=0;
      
      exec_step<=0;
    end else begin
      if(cycle==1 && cmd_ack==0)begin
        if(prefix==0)begin

          //case 0: sIns = "nop"; break;
          if         (ins==7'h00)begin//nop                                       @ 4 @       0 @  0 @ nop
            cmd_ack <= 1;

          //case 1: sIns = "mov DWORD PTR [" + sreg1 + " " + sins1 + "], " + sreg2 + ""; break;
          end else if(ins==7'h01)begin
            if         (exec_step==0)begin
              exec_step <= 1;
              exec_address <= regfile[reg1] + ins1;
              exec_writedata <= ins2;
            end else if(exec_step==1)begin
              
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
          
          
        end else begin
          //case 0: sIns = "mov DWORD PTR [" + sreg1 + " " + sins1 + "], " + sins2; break;
          if         (ins==7'h00)begin
            if         (exec_step==0)begin
              exec_step <= 1;
              exec_address <= regfile[reg1] + ins1;
              exec_writedata <= ins2;
            end else if(exec_step==1)begin
              
            end
          //case 1: sIns = "add DWORD PTR [" + sreg1 + " " + sins1 + "], " + sins2; break;
          end else if(ins==7'h01)begin//

          //case 2: sIns = "and " + sreg1 + ", " + sins2; break;
          //case 3: sIns = "sub " + sreg1 + ", " + sins2; break;
          
          end

        end
      end
      
      if(cycle==0 && cmd_ack==1)begin
        cmd_ack<=0;
      end
    end
  end


*/




endmodule

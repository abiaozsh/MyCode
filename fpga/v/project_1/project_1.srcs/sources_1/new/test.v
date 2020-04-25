 
module test(
    input clk,
    output [3:0] led,
    input [3:0] keyin,
    output uart_txd,
    input uart_rxd,
    output debug,
    
output M00_AXI_awaddr,
output M00_AXI_awlen,
output M00_AXI_awsize,
output M00_AXI_awburst,
output M00_AXI_awvalid,
input  M00_AXI_awready,

output M00_AXI_araddr,
output M00_AXI_arlen,
output M00_AXI_arsize,
output M00_AXI_arburst,
output M00_AXI_arvalid,
input  M00_AXI_arready,

 
input  M00_AXI_rdata,
input  M00_AXI_rresp,
input  M00_AXI_rlast,
input  M00_AXI_rvalid,
output M00_AXI_rready,
    
output M00_AXI_wdata,
output M00_AXI_wstrb,
output M00_AXI_wlast,
output M00_AXI_wvalid,
input  M00_AXI_wready,
 
input  M00_AXI_bresp,
input  M00_AXI_bvalid,
output M00_AXI_bready

    );
    


(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *)
 wire [31 : 0] M00_AXI_araddr;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARLEN" *)
 wire [3 : 0] M00_AXI_arlen;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARSIZE" *)
 wire [2 : 0] M00_AXI_arsize;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARBURST" *)
 wire [1 : 0] M00_AXI_arburst;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID" *)
 reg M00_AXI_arvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARREADY" *)
 wire M00_AXI_arready;
 
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWADDR" *)
 wire [31 : 0] M00_AXI_awaddr;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWLEN" *)
 wire [3 : 0] M00_AXI_awlen;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWSIZE" *)
 wire [2 : 0] M00_AXI_awsize;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWBURST" *)
 wire [1 : 0] M00_AXI_awburst;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID" *)
 reg M00_AXI_awvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWREADY" *)
 wire M00_AXI_awready;
 
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RDATA" *)
 wire [31 : 0] M00_AXI_rdata;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RRESP" *)
 wire [1 : 0] M00_AXI_rresp;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RLAST" *)
 wire M00_AXI_rlast;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RVALID" *)
 wire M00_AXI_rvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RREADY" *)
 reg M00_AXI_rready;


(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *)
 reg [31 : 0] M00_AXI_wdata;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB" *)
 reg [3 : 0] M00_AXI_wstrb;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WLAST" *)
 reg M00_AXI_wlast;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WVALID" *)
 reg M00_AXI_wvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WREADY" *)
 wire M00_AXI_wready;
 
 
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BRESP" *)
 wire [1 : 0] M00_AXI_bresp;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BVALID" *)
 wire M00_AXI_bvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BREADY" *)
 reg M00_AXI_bready;
 
 
(* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1,\
 NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *)

 reg [31:0] addr;
  assign M00_AXI_araddr = addr;
  assign M00_AXI_awaddr = addr;
  assign M00_AXI_arlen = 0;
  assign M00_AXI_awlen = 0;
  assign M00_AXI_arsize = 2;
  assign M00_AXI_awsize = 2;
  assign M00_AXI_arburst = 0;
  assign M00_AXI_awburst = 0;
  
  
  assign reset_n = keyin[3];
  
  
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




    wire [7:0] debug;
    assign debug[0] = M00_AXI_arready;//1   L17靠近中间  靠近主板灯亮 1
    assign debug[1] = M00_AXI_rvalid;//0
    assign debug[2] = M00_AXI_awready;//1
    assign debug[3] = M00_AXI_wready;//1
    assign debug[4] = M00_AXI_bvalid;//0
    
    assign debug[5] = M00_AXI_arvalid;//0
    assign debug[6] = M00_AXI_rready;//wdata
    assign debug[7] = M00_AXI_awvalid;//wdata

    assign led[0] = M00_AXI_wvalid;//wresp
    assign led[1] = M00_AXI_bready;//wresp
    assign led[2] = flg;//wresp
    assign led[3] = command == 0;//cmd==0:dark   cmd==1:light

reg [7:0] uart_data;
reg command_done;

reg [1:0] debug_readmem_ar_status;
reg [1:0] debug_readmem_dr_status;
reg [1:0] debug_readmem_aw_status;
reg [1:0] debug_readmem_dw_status;
reg [1:0] debug_readmem_b_status ;
parameter STATUS_INIT = 0;
parameter STATUS_WAIT = 1;
parameter STATUS_FINISH = 2;

reg halt_uart;
// reg [31:0] addr;
// reg [31:0] data;
reg [31:0] data_buff;
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin

    command_done <= 0;
    debug_readmem_ar_status <= 0;
    debug_readmem_dr_status <= 0;  
    debug_readmem_aw_status <= 0;  
    debug_readmem_dw_status <= 0;  
    debug_readmem_b_status  <= 0;  

    M00_AXI_arvalid <= 0;
    M00_AXI_rready <= 0;
    
    M00_AXI_awvalid <= 0;
    M00_AXI_wvalid <= 0;
    M00_AXI_bready <= 0;
    
    M00_AXI_wstrb <= 4'b1111;
  end else begin
    uart_send<=0;
    
    if(command_done)begin
      if          (command == 8'h00) begin 
        command_done<=0;
      end
    end else begin//command_done==0
      if          (command == 8'h00) begin
      end else if (command == 8'h01) begin uart_send<=1; uart_data_in<=123; command_done<=1;

      end else if (command == 8'h10) begin uart_send<=1; uart_data_in<=data_buff[ 7: 0]; command_done<=1;
      end else if (command == 8'h11) begin uart_send<=1; uart_data_in<=data_buff[15: 8]; command_done<=1;
      end else if (command == 8'h12) begin uart_send<=1; uart_data_in<=data_buff[23:16]; command_done<=1;
      end else if (command == 8'h13) begin uart_send<=1; uart_data_in<=data_buff[31:24]; command_done<=1;
      
      end else if (command == 8'h20) begin addr[ 7: 0] <= data; command_done<=1;
      end else if (command == 8'h21) begin addr[15: 8] <= data; command_done<=1;
      end else if (command == 8'h22) begin addr[23:16] <= data; command_done<=1;
      end else if (command == 8'h23) begin addr[31:24] <= data; command_done<=1;
      
      end else if (command == 8'h24) begin M00_AXI_wdata[ 7: 0] <= data; command_done<=1;
      end else if (command == 8'h25) begin M00_AXI_wdata[15: 8] <= data; command_done<=1;
      end else if (command == 8'h26) begin M00_AXI_wdata[23:16] <= data; command_done<=1;
      end else if (command == 8'h27) begin M00_AXI_wdata[31:24] <= data; command_done<=1;
      
      end else if (command == 8'h30) begin M00_AXI_wstrb <= data[3:0]; command_done<=1;
      end else if (command == 8'h31) begin M00_AXI_wstrb <= data[3:0]; command_done<=1;
    
//

      //read
      //reg M00_AXI_arvalid;//raddr
      //wire M00_AXI_arready;//raddr
      //wire M00_AXI_rvalid;//rdata
      //reg M00_AXI_rready;//rdata
      //reg [1:0] debug_readmem_ar_status;
      //reg [1:0] debug_readmem_dr_status;
      end else if (command == 8'h50) begin
        if         (debug_readmem_ar_status==STATUS_INIT)begin
          M00_AXI_arvalid <= 1;
          if(M00_AXI_arready)begin
            debug_readmem_ar_status <= STATUS_FINISH;
          end else begin
            debug_readmem_ar_status <= STATUS_WAIT;
          end
        end else if(debug_readmem_ar_status==STATUS_WAIT)begin
          if(M00_AXI_arready)begin
            debug_readmem_ar_status <= STATUS_FINISH;
          end
        end else if(debug_readmem_ar_status==STATUS_FINISH)begin
          M00_AXI_arvalid <= 0;
        end
        
        if         (debug_readmem_dr_status==STATUS_INIT)begin
          M00_AXI_rready <= 1;
          if(M00_AXI_rvalid)begin
            data_buff <= M00_AXI_rdata;
            debug_readmem_dr_status <= STATUS_FINISH;
          end else begin
            debug_readmem_dr_status <= STATUS_WAIT;
          end
        end else if(debug_readmem_dr_status==STATUS_WAIT)begin
          if(M00_AXI_rvalid)begin
            data_buff <= M00_AXI_rdata;
            debug_readmem_dr_status <= STATUS_FINISH;
          end
        end else if(debug_readmem_dr_status==STATUS_FINISH)begin
          M00_AXI_rready <= 0;
        end
        
        if(debug_readmem_ar_status==STATUS_FINISH && debug_readmem_dr_status==STATUS_FINISH)begin
          command_done = 1;
          debug_readmem_ar_status <= STATUS_INIT;
          debug_readmem_dr_status <= STATUS_INIT;
        end

        
      //reg M00_AXI_awvalid;//1
      //wire M00_AXI_awready;//0
      //reg M00_AXI_wvalid;//wdata
      //wire M00_AXI_wready;//wdata
      //wire M00_AXI_bvalid;//wresp
      //reg M00_AXI_bready;//wresp
      //reg [1:0] debug_readmem_aw_status;
      //reg [1:0] debug_readmem_dw_status;
      //reg [1:0] debug_readmem_b_status;
      end else if (command == 8'h51) begin
        if         (debug_readmem_aw_status==STATUS_INIT)begin
          M00_AXI_awvalid <= 1;
          if(M00_AXI_awready)begin
            debug_readmem_aw_status <= STATUS_FINISH;
          end else begin
            debug_readmem_aw_status <= STATUS_WAIT;
          end
        end else if(debug_readmem_aw_status==STATUS_WAIT)begin
          if(M00_AXI_awready)begin
            debug_readmem_aw_status <= STATUS_FINISH;
          end
        end else if(debug_readmem_aw_status==STATUS_FINISH)begin
          M00_AXI_awvalid <= 0;
        end
        
        if         (debug_readmem_dw_status==STATUS_INIT)begin
          M00_AXI_wvalid <= 1;
          M00_AXI_wlast <= 1;
          if(M00_AXI_wready)begin
            debug_readmem_dw_status <= STATUS_FINISH;
          end else begin
            debug_readmem_dw_status <= STATUS_WAIT;
          end
        end else if(debug_readmem_dw_status==STATUS_WAIT)begin
          if(M00_AXI_wready)begin
            debug_readmem_dw_status <= STATUS_FINISH;
          end
        end else if(debug_readmem_dw_status==STATUS_FINISH)begin
          M00_AXI_wvalid <= 0;
          M00_AXI_wlast <= 0;
        end
 
        if         (debug_readmem_b_status==STATUS_INIT)begin
          M00_AXI_bready <= 1;
          if(M00_AXI_bvalid)begin
            debug_readmem_b_status <= STATUS_FINISH;
          end else begin
            debug_readmem_b_status <= STATUS_WAIT;
          end
        end else if(debug_readmem_b_status==STATUS_WAIT)begin
          if(M00_AXI_bvalid)begin
            debug_readmem_b_status <= STATUS_FINISH;
          end
        end else if(debug_readmem_b_status==STATUS_FINISH)begin
          M00_AXI_bready <= 0;
        end

        if(debug_readmem_aw_status == STATUS_FINISH && debug_readmem_dw_status==STATUS_FINISH && debug_readmem_b_status==STATUS_FINISH)begin
          command_done = 1;
          debug_readmem_aw_status <= STATUS_INIT;
          debug_readmem_dw_status <= STATUS_INIT;
          debug_readmem_b_status <= STATUS_INIT;
        end

      end
      
    end
    
    
  end
  
end



























reg flg;
reg [31:0] timer;
always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin

    timer <= 0;
    flg <= 0;
  end else begin
    timer<=timer+1'b1;
    if(timer==5000000)begin
        timer <= 0;
        flg <= ~flg;
    end
  end
end
    
endmodule

// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2018.1 (win64) Build 2188600 Wed Apr  4 18:40:38 MDT 2018
// Date        : Sun Apr 19 14:10:31 2020
// Host        : zsh-PC running 64-bit Service Pack 1  (build 7601)
// Command     : write_verilog -force -mode funcsim
//               e:/vivadoProj/vTestB/project_1/project_1.srcs/sources_1/bd/design_1/ip/design_1_test_0_0/design_1_test_0_0_sim_netlist.v
// Design      : design_1_test_0_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7z010clg400-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "design_1_test_0_0,test,{}" *) (* DowngradeIPIdentifiedWarnings = "yes" *) (* X_CORE_INFO = "test,Vivado 2018.1" *) 
(* NotValidForBitStream *)
module design_1_test_0_0
   (clk,
    clk33,
    led,
    keyin,
    uart_txd,
    uart_rxd,
    debug,
    M00_AXI_awaddr,
    M00_AXI_awlen,
    M00_AXI_awsize,
    M00_AXI_awburst,
    M00_AXI_awvalid,
    M00_AXI_awready,
    M00_AXI_araddr,
    M00_AXI_arlen,
    M00_AXI_arsize,
    M00_AXI_arburst,
    M00_AXI_arvalid,
    M00_AXI_arready,
    M00_AXI_rdata,
    M00_AXI_rresp,
    M00_AXI_rlast,
    M00_AXI_rvalid,
    M00_AXI_rready,
    M00_AXI_wdata,
    M00_AXI_wstrb,
    M00_AXI_wlast,
    M00_AXI_wvalid,
    M00_AXI_wready,
    M00_AXI_bresp,
    M00_AXI_bvalid,
    M00_AXI_bready);
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 clk CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME clk, ASSOCIATED_BUSIF M00_AXI, FREQ_HZ 50000000, PHASE 0.000, CLK_DOMAIN design_1_processing_system7_0_0_FCLK_CLK0" *) input clk;
  input clk33;
  output [3:0]led;
  input [3:0]keyin;
  output uart_txd;
  input uart_rxd;
  output [7:0]debug;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWADDR" *) output [31:0]M00_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWLEN" *) output [3:0]M00_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWSIZE" *) output [2:0]M00_AXI_awsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWBURST" *) output [1:0]M00_AXI_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID" *) output M00_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWREADY" *) input M00_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) output [31:0]M00_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARLEN" *) output [3:0]M00_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARSIZE" *) output [2:0]M00_AXI_arsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARBURST" *) output [1:0]M00_AXI_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID" *) output M00_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARREADY" *) input M00_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RDATA" *) input [31:0]M00_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RRESP" *) input [1:0]M00_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RLAST" *) input M00_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RVALID" *) input M00_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RREADY" *) output M00_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) output [31:0]M00_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB" *) output [3:0]M00_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WLAST" *) output M00_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WVALID" *) output M00_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WREADY" *) input M00_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BRESP" *) input [1:0]M00_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BVALID" *) input M00_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BREADY" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 1, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 16, PHASE 0.000, CLK_DOMAIN design_1_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0" *) output M00_AXI_bready;

  wire \<const0> ;
  wire \<const1> ;
  wire [31:0]M00_AXI_araddr;
  wire M00_AXI_arready;
  wire M00_AXI_awready;
  wire M00_AXI_bready;
  wire M00_AXI_bvalid;
  wire [31:0]M00_AXI_rdata;
  wire M00_AXI_rready;
  wire M00_AXI_rvalid;
  wire [31:0]M00_AXI_wdata;
  wire M00_AXI_wready;
  wire [3:0]M00_AXI_wstrb;
  wire clk;
  wire [6:0]\^debug ;
  wire [3:0]keyin;
  wire [2:2]\^led ;
  wire uart_rxd;
  wire uart_txd;

  assign M00_AXI_arburst[1] = \<const0> ;
  assign M00_AXI_arburst[0] = \<const0> ;
  assign M00_AXI_arlen[3] = \<const0> ;
  assign M00_AXI_arlen[2] = \<const0> ;
  assign M00_AXI_arlen[1] = \<const0> ;
  assign M00_AXI_arlen[0] = \<const0> ;
  assign M00_AXI_arsize[2] = \<const0> ;
  assign M00_AXI_arsize[1] = \<const1> ;
  assign M00_AXI_arsize[0] = \<const0> ;
  assign M00_AXI_arvalid = \^debug [0];
  assign M00_AXI_awaddr[31:0] = M00_AXI_araddr;
  assign M00_AXI_awburst[1] = \<const0> ;
  assign M00_AXI_awburst[0] = \<const0> ;
  assign M00_AXI_awlen[3] = \<const0> ;
  assign M00_AXI_awlen[2] = \<const0> ;
  assign M00_AXI_awlen[1] = \<const0> ;
  assign M00_AXI_awlen[0] = \<const0> ;
  assign M00_AXI_awsize[2] = \<const0> ;
  assign M00_AXI_awsize[1] = \<const1> ;
  assign M00_AXI_awsize[0] = \<const0> ;
  assign M00_AXI_awvalid = \^debug [4];
  assign M00_AXI_wvalid = \^debug [6];
  assign debug[7] = M00_AXI_wready;
  assign debug[6] = \^debug [6];
  assign debug[5] = M00_AXI_awready;
  assign debug[4] = \^debug [4];
  assign debug[3] = M00_AXI_rready;
  assign debug[2] = M00_AXI_rvalid;
  assign debug[1] = M00_AXI_arready;
  assign debug[0] = \^debug [0];
  assign led[3] = keyin[3];
  assign led[2] = \^led [2];
  assign led[1] = M00_AXI_bready;
  assign led[0] = M00_AXI_bvalid;
  GND GND
       (.G(\<const0> ));
  VCC VCC
       (.P(\<const1> ));
  design_1_test_0_0_test inst
       (.M00_AXI_araddr(M00_AXI_araddr),
        .M00_AXI_bvalid(M00_AXI_bvalid),
        .M00_AXI_rdata(M00_AXI_rdata),
        .M00_AXI_rready(M00_AXI_rready),
        .M00_AXI_wdata(M00_AXI_wdata),
        .M00_AXI_wstrb(M00_AXI_wstrb),
        .clk(clk),
        .debug({M00_AXI_wready,M00_AXI_awready,M00_AXI_rvalid,M00_AXI_arready}),
        .\debug[4] (\^debug [4]),
        .\debug[6] (\^debug [6]),
        .debug_0_sp_1(\^debug [0]),
        .keyin(keyin),
        .led({\^led ,M00_AXI_bready}),
        .uart_rxd(uart_rxd),
        .uart_txd(uart_txd));
endmodule

(* ORIG_REF_NAME = "test" *) 
module design_1_test_0_0_test
   (led,
    M00_AXI_araddr,
    M00_AXI_wdata,
    M00_AXI_wstrb,
    debug_0_sp_1,
    \debug[4] ,
    \debug[6] ,
    uart_txd,
    M00_AXI_rready,
    clk,
    M00_AXI_rdata,
    uart_rxd,
    debug,
    keyin,
    M00_AXI_bvalid);
  output [1:0]led;
  output [31:0]M00_AXI_araddr;
  output [31:0]M00_AXI_wdata;
  output [3:0]M00_AXI_wstrb;
  output debug_0_sp_1;
  output \debug[4] ;
  output \debug[6] ;
  output uart_txd;
  output M00_AXI_rready;
  input clk;
  input [31:0]M00_AXI_rdata;
  input uart_rxd;
  input [3:0]debug;
  input [3:0]keyin;
  input M00_AXI_bvalid;

  wire [31:0]M00_AXI_araddr;
  wire M00_AXI_arvalid_i_1_n_0;
  wire M00_AXI_awvalid_i_1_n_0;
  wire M00_AXI_bready_i_1_n_0;
  wire M00_AXI_bready_i_2_n_0;
  wire M00_AXI_bready_i_3_n_0;
  wire M00_AXI_bready_i_5_n_0;
  wire M00_AXI_bvalid;
  wire [31:0]M00_AXI_rdata;
  wire M00_AXI_rready;
  wire M00_AXI_rready_i_1_n_0;
  wire M00_AXI_rready_i_2_n_0;
  wire [31:0]M00_AXI_wdata;
  wire \M00_AXI_wdata[15]_i_1_n_0 ;
  wire \M00_AXI_wdata[15]_i_2_n_0 ;
  wire \M00_AXI_wdata[23]_i_1_n_0 ;
  wire \M00_AXI_wdata[23]_i_2_n_0 ;
  wire \M00_AXI_wdata[31]_i_1_n_0 ;
  wire \M00_AXI_wdata[31]_i_2_n_0 ;
  wire \M00_AXI_wdata[31]_i_3_n_0 ;
  wire \M00_AXI_wdata[31]_i_4_n_0 ;
  wire \M00_AXI_wdata[7]_i_1_n_0 ;
  wire \M00_AXI_wdata[7]_i_2_n_0 ;
  wire [3:0]M00_AXI_wstrb;
  wire \M00_AXI_wstrb[3]_i_1_n_0 ;
  wire \M00_AXI_wstrb[3]_i_2_n_0 ;
  wire M00_AXI_wvalid_i_1_n_0;
  wire \addr[15]_i_1_n_0 ;
  wire \addr[15]_i_2_n_0 ;
  wire \addr[23]_i_1_n_0 ;
  wire \addr[23]_i_2_n_0 ;
  wire \addr[23]_i_3_n_0 ;
  wire \addr[23]_i_4_n_0 ;
  wire \addr[31]_i_1_n_0 ;
  wire \addr[31]_i_2_n_0 ;
  wire \addr[31]_i_3_n_0 ;
  wire \addr[31]_i_4_n_0 ;
  wire \addr[31]_i_5_n_0 ;
  wire \addr[31]_i_6_n_0 ;
  wire \addr[7]_i_1_n_0 ;
  wire \addr[7]_i_2_n_0 ;
  wire clk;
  wire [7:0]command;
  wire command_done221_out;
  wire command_done2__1;
  wire command_done_i_1_n_0;
  wire command_done_i_2_n_0;
  wire command_done_i_3_n_0;
  wire command_done_i_4_n_0;
  wire command_done_i_5_n_0;
  wire command_done_reg_n_0;
  wire [7:0]command_temp;
  wire command_temp_1;
  wire data;
  wire [31:1]data0;
  wire data_buff;
  wire \data_buff_reg_n_0_[0] ;
  wire \data_buff_reg_n_0_[10] ;
  wire \data_buff_reg_n_0_[11] ;
  wire \data_buff_reg_n_0_[12] ;
  wire \data_buff_reg_n_0_[13] ;
  wire \data_buff_reg_n_0_[14] ;
  wire \data_buff_reg_n_0_[15] ;
  wire \data_buff_reg_n_0_[16] ;
  wire \data_buff_reg_n_0_[17] ;
  wire \data_buff_reg_n_0_[18] ;
  wire \data_buff_reg_n_0_[19] ;
  wire \data_buff_reg_n_0_[1] ;
  wire \data_buff_reg_n_0_[20] ;
  wire \data_buff_reg_n_0_[21] ;
  wire \data_buff_reg_n_0_[22] ;
  wire \data_buff_reg_n_0_[23] ;
  wire \data_buff_reg_n_0_[24] ;
  wire \data_buff_reg_n_0_[25] ;
  wire \data_buff_reg_n_0_[26] ;
  wire \data_buff_reg_n_0_[27] ;
  wire \data_buff_reg_n_0_[28] ;
  wire \data_buff_reg_n_0_[29] ;
  wire \data_buff_reg_n_0_[2] ;
  wire \data_buff_reg_n_0_[30] ;
  wire \data_buff_reg_n_0_[31] ;
  wire \data_buff_reg_n_0_[3] ;
  wire \data_buff_reg_n_0_[4] ;
  wire \data_buff_reg_n_0_[5] ;
  wire \data_buff_reg_n_0_[6] ;
  wire \data_buff_reg_n_0_[7] ;
  wire \data_buff_reg_n_0_[8] ;
  wire \data_buff_reg_n_0_[9] ;
  wire data_cmd_reg_n_0;
  wire \data_reg_n_0_[0] ;
  wire \data_reg_n_0_[1] ;
  wire \data_reg_n_0_[2] ;
  wire \data_reg_n_0_[3] ;
  wire \data_reg_n_0_[4] ;
  wire \data_reg_n_0_[5] ;
  wire \data_reg_n_0_[6] ;
  wire \data_reg_n_0_[7] ;
  wire [3:0]debug;
  wire \debug[4] ;
  wire \debug[6] ;
  wire debug_0_sn_1;
  wire \debug_readmem_step[0]_i_1_n_0 ;
  wire \debug_readmem_step[0]_i_2_n_0 ;
  wire \debug_readmem_step[0]_i_3_n_0 ;
  wire \debug_readmem_step[1]_i_1_n_0 ;
  wire \debug_readmem_step[1]_i_2_n_0 ;
  wire \debug_readmem_step[1]_i_3_n_0 ;
  wire \debug_readmem_step[1]_i_4_n_0 ;
  wire \debug_readmem_step[1]_i_5_n_0 ;
  wire \debug_readmem_step[1]_i_6_n_0 ;
  wire \debug_readmem_step[1]_i_7_n_0 ;
  wire \debug_readmem_step[1]_i_8_n_0 ;
  wire \debug_readmem_step_reg_n_0_[0] ;
  wire \debug_readmem_step_reg_n_0_[1] ;
  wire flg_i_10_n_0;
  wire flg_i_1_n_0;
  wire flg_i_3_n_0;
  wire flg_i_4_n_0;
  wire flg_i_5_n_0;
  wire flg_i_6_n_0;
  wire flg_i_7_n_0;
  wire flg_i_8_n_0;
  wire flg_i_9_n_0;
  wire [3:0]keyin;
  wire [1:0]led;
  wire [31:0]timer33;
  wire [31:0]timer33_2;
  wire \timer33_reg[12]_i_2_n_0 ;
  wire \timer33_reg[12]_i_2_n_1 ;
  wire \timer33_reg[12]_i_2_n_2 ;
  wire \timer33_reg[12]_i_2_n_3 ;
  wire \timer33_reg[16]_i_2_n_0 ;
  wire \timer33_reg[16]_i_2_n_1 ;
  wire \timer33_reg[16]_i_2_n_2 ;
  wire \timer33_reg[16]_i_2_n_3 ;
  wire \timer33_reg[20]_i_2_n_0 ;
  wire \timer33_reg[20]_i_2_n_1 ;
  wire \timer33_reg[20]_i_2_n_2 ;
  wire \timer33_reg[20]_i_2_n_3 ;
  wire \timer33_reg[24]_i_2_n_0 ;
  wire \timer33_reg[24]_i_2_n_1 ;
  wire \timer33_reg[24]_i_2_n_2 ;
  wire \timer33_reg[24]_i_2_n_3 ;
  wire \timer33_reg[28]_i_2_n_0 ;
  wire \timer33_reg[28]_i_2_n_1 ;
  wire \timer33_reg[28]_i_2_n_2 ;
  wire \timer33_reg[28]_i_2_n_3 ;
  wire \timer33_reg[31]_i_2_n_2 ;
  wire \timer33_reg[31]_i_2_n_3 ;
  wire \timer33_reg[4]_i_2_n_0 ;
  wire \timer33_reg[4]_i_2_n_1 ;
  wire \timer33_reg[4]_i_2_n_2 ;
  wire \timer33_reg[4]_i_2_n_3 ;
  wire \timer33_reg[8]_i_2_n_0 ;
  wire \timer33_reg[8]_i_2_n_1 ;
  wire \timer33_reg[8]_i_2_n_2 ;
  wire \timer33_reg[8]_i_2_n_3 ;
  wire [7:0]uart_data_in;
  wire \uart_data_in[0]_i_1_n_0 ;
  wire \uart_data_in[0]_i_2_n_0 ;
  wire \uart_data_in[1]_i_1_n_0 ;
  wire \uart_data_in[1]_i_2_n_0 ;
  wire \uart_data_in[2]_i_1_n_0 ;
  wire \uart_data_in[2]_i_2_n_0 ;
  wire \uart_data_in[3]_i_1_n_0 ;
  wire \uart_data_in[3]_i_2_n_0 ;
  wire \uart_data_in[4]_i_1_n_0 ;
  wire \uart_data_in[4]_i_2_n_0 ;
  wire \uart_data_in[5]_i_1_n_0 ;
  wire \uart_data_in[5]_i_2_n_0 ;
  wire \uart_data_in[6]_i_1_n_0 ;
  wire \uart_data_in[6]_i_2_n_0 ;
  wire \uart_data_in[7]_i_2_n_0 ;
  wire \uart_data_in[7]_i_3_n_0 ;
  wire \uart_data_in[7]_i_4_n_0 ;
  wire \uart_data_in[7]_i_5_n_0 ;
  wire \uart_data_in[7]_i_6_n_0 ;
  wire \uart_data_in[7]_i_7_n_0 ;
  wire uart_data_in_0;
  wire [7:0]uart_data_out;
  wire uart_hs_inst_n_0;
  wire uart_hs_inst_n_11;
  wire uart_hs_inst_n_13;
  wire uart_hs_inst_n_14;
  wire uart_hs_inst_n_15;
  wire uart_hs_inst_n_16;
  wire uart_hs_inst_n_17;
  wire uart_hs_inst_n_18;
  wire uart_hs_inst_n_19;
  wire uart_hs_inst_n_20;
  wire uart_hs_inst_n_21;
  wire uart_rxd;
  wire uart_send;
  wire uart_send_i_1_n_0;
  wire uart_txd;
  wire [3:2]\NLW_timer33_reg[31]_i_2_CO_UNCONNECTED ;
  wire [3:3]\NLW_timer33_reg[31]_i_2_O_UNCONNECTED ;

  assign debug_0_sp_1 = debug_0_sn_1;
  LUT6 #(
    .INIT(64'hF0F0F0F000F1F0F0)) 
    M00_AXI_arvalid_i_1
       (.I0(\debug_readmem_step_reg_n_0_[0] ),
        .I1(\debug_readmem_step_reg_n_0_[1] ),
        .I2(debug_0_sn_1),
        .I3(debug[0]),
        .I4(M00_AXI_bready_i_2_n_0),
        .I5(command[0]),
        .O(M00_AXI_arvalid_i_1_n_0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID" *) 
  FDCE M00_AXI_arvalid_reg
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(M00_AXI_arvalid_i_1_n_0),
        .Q(debug_0_sn_1));
  LUT6 #(
    .INIT(64'h2223AAAAAAAAAAAA)) 
    M00_AXI_awvalid_i_1
       (.I0(\debug[4] ),
        .I1(debug[2]),
        .I2(\debug_readmem_step_reg_n_0_[0] ),
        .I3(\debug_readmem_step_reg_n_0_[1] ),
        .I4(M00_AXI_bready_i_2_n_0),
        .I5(command[0]),
        .O(M00_AXI_awvalid_i_1_n_0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID" *) 
  FDCE M00_AXI_awvalid_reg
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(M00_AXI_awvalid_i_1_n_0),
        .Q(\debug[4] ));
  LUT5 #(
    .INIT(32'hF7F78000)) 
    M00_AXI_bready_i_1
       (.I0(command[0]),
        .I1(M00_AXI_bready_i_2_n_0),
        .I2(M00_AXI_bready_i_3_n_0),
        .I3(command_done2__1),
        .I4(led[0]),
        .O(M00_AXI_bready_i_1_n_0));
  LUT6 #(
    .INIT(64'h0000000000000400)) 
    M00_AXI_bready_i_2
       (.I0(command[3]),
        .I1(command[5]),
        .I2(M00_AXI_bready_i_5_n_0),
        .I3(command[4]),
        .I4(command_done_reg_n_0),
        .I5(\addr[31]_i_3_n_0 ),
        .O(M00_AXI_bready_i_2_n_0));
  (* SOFT_HLUTNM = "soft_lutpair24" *) 
  LUT5 #(
    .INIT(32'hFE0FFFFF)) 
    M00_AXI_bready_i_3
       (.I0(\debug[6] ),
        .I1(\debug[4] ),
        .I2(\debug_readmem_step_reg_n_0_[1] ),
        .I3(\debug_readmem_step_reg_n_0_[0] ),
        .I4(M00_AXI_bvalid),
        .O(M00_AXI_bready_i_3_n_0));
  (* SOFT_HLUTNM = "soft_lutpair24" *) 
  LUT4 #(
    .INIT(16'h0100)) 
    M00_AXI_bready_i_4
       (.I0(\debug[6] ),
        .I1(\debug[4] ),
        .I2(\debug_readmem_step_reg_n_0_[1] ),
        .I3(\debug_readmem_step_reg_n_0_[0] ),
        .O(command_done2__1));
  (* SOFT_HLUTNM = "soft_lutpair27" *) 
  LUT2 #(
    .INIT(4'hE)) 
    M00_AXI_bready_i_5
       (.I0(command[2]),
        .I1(command[1]),
        .O(M00_AXI_bready_i_5_n_0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BREADY" *) 
  FDCE M00_AXI_bready_reg
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(M00_AXI_bready_i_1_n_0),
        .Q(led[0]));
  LUT5 #(
    .INIT(32'hFBFB4000)) 
    M00_AXI_rready_i_1
       (.I0(command[0]),
        .I1(M00_AXI_bready_i_2_n_0),
        .I2(M00_AXI_rready_i_2_n_0),
        .I3(command_done221_out),
        .I4(M00_AXI_rready),
        .O(M00_AXI_rready_i_1_n_0));
  (* SOFT_HLUTNM = "soft_lutpair21" *) 
  LUT5 #(
    .INIT(32'h0ABBFFFF)) 
    M00_AXI_rready_i_2
       (.I0(\debug_readmem_step_reg_n_0_[0] ),
        .I1(\debug_readmem_step_reg_n_0_[1] ),
        .I2(debug_0_sn_1),
        .I3(debug[0]),
        .I4(debug[1]),
        .O(M00_AXI_rready_i_2_n_0));
  (* SOFT_HLUTNM = "soft_lutpair29" *) 
  LUT4 #(
    .INIT(16'hF100)) 
    M00_AXI_rready_i_3
       (.I0(\debug_readmem_step_reg_n_0_[0] ),
        .I1(\debug_readmem_step_reg_n_0_[1] ),
        .I2(debug_0_sn_1),
        .I3(debug[0]),
        .O(command_done221_out));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RREADY" *) 
  FDCE M00_AXI_rready_reg
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(M00_AXI_rready_i_1_n_0),
        .Q(M00_AXI_rready));
  LUT6 #(
    .INIT(64'hAAA8AAAAAAAAAAAA)) 
    \M00_AXI_wdata[15]_i_1 
       (.I0(\M00_AXI_wdata[15]_i_2_n_0 ),
        .I1(\addr[31]_i_3_n_0 ),
        .I2(command[4]),
        .I3(command[3]),
        .I4(command[5]),
        .I5(command[2]),
        .O(\M00_AXI_wdata[15]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000000000200000)) 
    \M00_AXI_wdata[15]_i_2 
       (.I0(command[2]),
        .I1(\addr[23]_i_3_n_0 ),
        .I2(command[0]),
        .I3(command[1]),
        .I4(\addr[23]_i_4_n_0 ),
        .I5(\M00_AXI_wdata[31]_i_4_n_0 ),
        .O(\M00_AXI_wdata[15]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hAAA8AAAAAAAAAAAA)) 
    \M00_AXI_wdata[23]_i_1 
       (.I0(\M00_AXI_wdata[23]_i_2_n_0 ),
        .I1(\addr[31]_i_3_n_0 ),
        .I2(command[4]),
        .I3(command[3]),
        .I4(command[5]),
        .I5(command[2]),
        .O(\M00_AXI_wdata[23]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0200000000000000)) 
    \M00_AXI_wdata[23]_i_2 
       (.I0(command[2]),
        .I1(\addr[23]_i_3_n_0 ),
        .I2(command[0]),
        .I3(\addr[23]_i_4_n_0 ),
        .I4(command[1]),
        .I5(\M00_AXI_wdata[31]_i_4_n_0 ),
        .O(\M00_AXI_wdata[23]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'h0200000000000000)) 
    \M00_AXI_wdata[31]_i_1 
       (.I0(\M00_AXI_wdata[31]_i_2_n_0 ),
        .I1(\addr[31]_i_3_n_0 ),
        .I2(\M00_AXI_wdata[31]_i_3_n_0 ),
        .I3(command[1]),
        .I4(command[0]),
        .I5(\M00_AXI_wdata[31]_i_4_n_0 ),
        .O(\M00_AXI_wdata[31]_i_1_n_0 ));
  LUT2 #(
    .INIT(4'h2)) 
    \M00_AXI_wdata[31]_i_2 
       (.I0(keyin[3]),
        .I1(command_done_reg_n_0),
        .O(\M00_AXI_wdata[31]_i_2_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair27" *) 
  LUT4 #(
    .INIT(16'hEFFF)) 
    \M00_AXI_wdata[31]_i_3 
       (.I0(command[4]),
        .I1(command[3]),
        .I2(command[5]),
        .I3(command[2]),
        .O(\M00_AXI_wdata[31]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'hFFEEFFEEFFDDDDDC)) 
    \M00_AXI_wdata[31]_i_4 
       (.I0(command[5]),
        .I1(command[3]),
        .I2(command[0]),
        .I3(command[2]),
        .I4(command[1]),
        .I5(command[4]),
        .O(\M00_AXI_wdata[31]_i_4_n_0 ));
  LUT6 #(
    .INIT(64'hAAA8AAAAAAAAAAAA)) 
    \M00_AXI_wdata[7]_i_1 
       (.I0(\M00_AXI_wdata[7]_i_2_n_0 ),
        .I1(\addr[31]_i_3_n_0 ),
        .I2(command[4]),
        .I3(command[3]),
        .I4(command[5]),
        .I5(command[2]),
        .O(\M00_AXI_wdata[7]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000000000020000)) 
    \M00_AXI_wdata[7]_i_2 
       (.I0(command[2]),
        .I1(\addr[23]_i_3_n_0 ),
        .I2(command[0]),
        .I3(command[1]),
        .I4(\addr[23]_i_4_n_0 ),
        .I5(\M00_AXI_wdata[31]_i_4_n_0 ),
        .O(\M00_AXI_wdata[7]_i_2_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[0] 
       (.C(clk),
        .CE(\M00_AXI_wdata[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[0] ),
        .Q(M00_AXI_wdata[0]),
        .R(\M00_AXI_wdata[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[10] 
       (.C(clk),
        .CE(\M00_AXI_wdata[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[2] ),
        .Q(M00_AXI_wdata[10]),
        .R(\M00_AXI_wdata[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[11] 
       (.C(clk),
        .CE(\M00_AXI_wdata[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[3] ),
        .Q(M00_AXI_wdata[11]),
        .R(\M00_AXI_wdata[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[12] 
       (.C(clk),
        .CE(\M00_AXI_wdata[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[4] ),
        .Q(M00_AXI_wdata[12]),
        .R(\M00_AXI_wdata[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[13] 
       (.C(clk),
        .CE(\M00_AXI_wdata[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[5] ),
        .Q(M00_AXI_wdata[13]),
        .R(\M00_AXI_wdata[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[14] 
       (.C(clk),
        .CE(\M00_AXI_wdata[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[6] ),
        .Q(M00_AXI_wdata[14]),
        .R(\M00_AXI_wdata[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[15] 
       (.C(clk),
        .CE(\M00_AXI_wdata[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[7] ),
        .Q(M00_AXI_wdata[15]),
        .R(\M00_AXI_wdata[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[16] 
       (.C(clk),
        .CE(\M00_AXI_wdata[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[0] ),
        .Q(M00_AXI_wdata[16]),
        .R(\M00_AXI_wdata[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[17] 
       (.C(clk),
        .CE(\M00_AXI_wdata[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[1] ),
        .Q(M00_AXI_wdata[17]),
        .R(\M00_AXI_wdata[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[18] 
       (.C(clk),
        .CE(\M00_AXI_wdata[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[2] ),
        .Q(M00_AXI_wdata[18]),
        .R(\M00_AXI_wdata[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[19] 
       (.C(clk),
        .CE(\M00_AXI_wdata[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[3] ),
        .Q(M00_AXI_wdata[19]),
        .R(\M00_AXI_wdata[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[1] 
       (.C(clk),
        .CE(\M00_AXI_wdata[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[1] ),
        .Q(M00_AXI_wdata[1]),
        .R(\M00_AXI_wdata[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[20] 
       (.C(clk),
        .CE(\M00_AXI_wdata[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[4] ),
        .Q(M00_AXI_wdata[20]),
        .R(\M00_AXI_wdata[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[21] 
       (.C(clk),
        .CE(\M00_AXI_wdata[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[5] ),
        .Q(M00_AXI_wdata[21]),
        .R(\M00_AXI_wdata[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[22] 
       (.C(clk),
        .CE(\M00_AXI_wdata[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[6] ),
        .Q(M00_AXI_wdata[22]),
        .R(\M00_AXI_wdata[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[23] 
       (.C(clk),
        .CE(\M00_AXI_wdata[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[7] ),
        .Q(M00_AXI_wdata[23]),
        .R(\M00_AXI_wdata[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[24] 
       (.C(clk),
        .CE(\M00_AXI_wdata[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[0] ),
        .Q(M00_AXI_wdata[24]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[25] 
       (.C(clk),
        .CE(\M00_AXI_wdata[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[1] ),
        .Q(M00_AXI_wdata[25]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[26] 
       (.C(clk),
        .CE(\M00_AXI_wdata[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[2] ),
        .Q(M00_AXI_wdata[26]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[27] 
       (.C(clk),
        .CE(\M00_AXI_wdata[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[3] ),
        .Q(M00_AXI_wdata[27]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[28] 
       (.C(clk),
        .CE(\M00_AXI_wdata[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[4] ),
        .Q(M00_AXI_wdata[28]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[29] 
       (.C(clk),
        .CE(\M00_AXI_wdata[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[5] ),
        .Q(M00_AXI_wdata[29]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[2] 
       (.C(clk),
        .CE(\M00_AXI_wdata[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[2] ),
        .Q(M00_AXI_wdata[2]),
        .R(\M00_AXI_wdata[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[30] 
       (.C(clk),
        .CE(\M00_AXI_wdata[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[6] ),
        .Q(M00_AXI_wdata[30]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[31] 
       (.C(clk),
        .CE(\M00_AXI_wdata[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[7] ),
        .Q(M00_AXI_wdata[31]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[3] 
       (.C(clk),
        .CE(\M00_AXI_wdata[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[3] ),
        .Q(M00_AXI_wdata[3]),
        .R(\M00_AXI_wdata[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[4] 
       (.C(clk),
        .CE(\M00_AXI_wdata[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[4] ),
        .Q(M00_AXI_wdata[4]),
        .R(\M00_AXI_wdata[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[5] 
       (.C(clk),
        .CE(\M00_AXI_wdata[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[5] ),
        .Q(M00_AXI_wdata[5]),
        .R(\M00_AXI_wdata[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[6] 
       (.C(clk),
        .CE(\M00_AXI_wdata[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[6] ),
        .Q(M00_AXI_wdata[6]),
        .R(\M00_AXI_wdata[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[7] 
       (.C(clk),
        .CE(\M00_AXI_wdata[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[7] ),
        .Q(M00_AXI_wdata[7]),
        .R(\M00_AXI_wdata[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[8] 
       (.C(clk),
        .CE(\M00_AXI_wdata[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[0] ),
        .Q(M00_AXI_wdata[8]),
        .R(\M00_AXI_wdata[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) 
  FDRE \M00_AXI_wdata_reg[9] 
       (.C(clk),
        .CE(\M00_AXI_wdata[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[1] ),
        .Q(M00_AXI_wdata[9]),
        .R(\M00_AXI_wdata[15]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000000000004000)) 
    \M00_AXI_wstrb[3]_i_1 
       (.I0(\M00_AXI_wstrb[3]_i_2_n_0 ),
        .I1(command[3]),
        .I2(command[5]),
        .I3(keyin[3]),
        .I4(\addr[31]_i_3_n_0 ),
        .I5(command_done_reg_n_0),
        .O(\M00_AXI_wstrb[3]_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair28" *) 
  LUT4 #(
    .INIT(16'hFFFE)) 
    \M00_AXI_wstrb[3]_i_2 
       (.I0(command[0]),
        .I1(command[2]),
        .I2(command[1]),
        .I3(command[4]),
        .O(\M00_AXI_wstrb[3]_i_2_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB" *) 
  FDRE \M00_AXI_wstrb_reg[0] 
       (.C(clk),
        .CE(\M00_AXI_wstrb[3]_i_1_n_0 ),
        .D(\data_reg_n_0_[0] ),
        .Q(M00_AXI_wstrb[0]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB" *) 
  FDRE \M00_AXI_wstrb_reg[1] 
       (.C(clk),
        .CE(\M00_AXI_wstrb[3]_i_1_n_0 ),
        .D(\data_reg_n_0_[1] ),
        .Q(M00_AXI_wstrb[1]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB" *) 
  FDRE \M00_AXI_wstrb_reg[2] 
       (.C(clk),
        .CE(\M00_AXI_wstrb[3]_i_1_n_0 ),
        .D(\data_reg_n_0_[2] ),
        .Q(M00_AXI_wstrb[2]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB" *) 
  FDRE \M00_AXI_wstrb_reg[3] 
       (.C(clk),
        .CE(\M00_AXI_wstrb[3]_i_1_n_0 ),
        .D(\data_reg_n_0_[3] ),
        .Q(M00_AXI_wstrb[3]),
        .R(1'b0));
  LUT6 #(
    .INIT(64'h2223AAAAAAAAAAAA)) 
    M00_AXI_wvalid_i_1
       (.I0(\debug[6] ),
        .I1(debug[3]),
        .I2(\debug_readmem_step_reg_n_0_[0] ),
        .I3(\debug_readmem_step_reg_n_0_[1] ),
        .I4(M00_AXI_bready_i_2_n_0),
        .I5(command[0]),
        .O(M00_AXI_wvalid_i_1_n_0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WVALID" *) 
  FDCE M00_AXI_wvalid_reg
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(M00_AXI_wvalid_i_1_n_0),
        .Q(\debug[6] ));
  LUT6 #(
    .INIT(64'hAAAAAAAAAAA8AAAA)) 
    \addr[15]_i_1 
       (.I0(\addr[15]_i_2_n_0 ),
        .I1(\addr[31]_i_3_n_0 ),
        .I2(command[4]),
        .I3(command[3]),
        .I4(command[5]),
        .I5(command[2]),
        .O(\addr[15]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000000400000000)) 
    \addr[15]_i_2 
       (.I0(\addr[31]_i_5_n_0 ),
        .I1(command[0]),
        .I2(\addr[23]_i_3_n_0 ),
        .I3(command[1]),
        .I4(command[2]),
        .I5(\addr[23]_i_4_n_0 ),
        .O(\addr[15]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hAAAAAAAAAAA8AAAA)) 
    \addr[23]_i_1 
       (.I0(\addr[23]_i_2_n_0 ),
        .I1(\addr[31]_i_3_n_0 ),
        .I2(command[4]),
        .I3(command[3]),
        .I4(command[5]),
        .I5(command[2]),
        .O(\addr[23]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000040000000000)) 
    \addr[23]_i_2 
       (.I0(command[0]),
        .I1(\addr[31]_i_5_n_0 ),
        .I2(\addr[23]_i_3_n_0 ),
        .I3(command[1]),
        .I4(command[2]),
        .I5(\addr[23]_i_4_n_0 ),
        .O(\addr[23]_i_2_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair20" *) 
  LUT3 #(
    .INIT(8'hFD)) 
    \addr[23]_i_3 
       (.I0(command[5]),
        .I1(command[3]),
        .I2(command[4]),
        .O(\addr[23]_i_3_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair30" *) 
  LUT4 #(
    .INIT(16'h0100)) 
    \addr[23]_i_4 
       (.I0(command_done_reg_n_0),
        .I1(command[7]),
        .I2(command[6]),
        .I3(keyin[3]),
        .O(\addr[23]_i_4_n_0 ));
  LUT6 #(
    .INIT(64'h000000008A888888)) 
    \addr[31]_i_1 
       (.I0(\addr[31]_i_2_n_0 ),
        .I1(\addr[31]_i_3_n_0 ),
        .I2(\addr[31]_i_4_n_0 ),
        .I3(\addr[31]_i_5_n_0 ),
        .I4(command[0]),
        .I5(\addr[31]_i_6_n_0 ),
        .O(\addr[31]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000100000000000)) 
    \addr[31]_i_2 
       (.I0(command[7]),
        .I1(command[6]),
        .I2(command[0]),
        .I3(command[1]),
        .I4(command_done_reg_n_0),
        .I5(keyin[3]),
        .O(\addr[31]_i_2_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair26" *) 
  LUT2 #(
    .INIT(4'hE)) 
    \addr[31]_i_3 
       (.I0(command[7]),
        .I1(command[6]),
        .O(\addr[31]_i_3_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair22" *) 
  LUT5 #(
    .INIT(32'h00000010)) 
    \addr[31]_i_4 
       (.I0(command[4]),
        .I1(command[3]),
        .I2(command[5]),
        .I3(command[1]),
        .I4(command[2]),
        .O(\addr[31]_i_4_n_0 ));
  LUT6 #(
    .INIT(64'hFFFEFFFEEEFFEEFE)) 
    \addr[31]_i_5 
       (.I0(command[2]),
        .I1(command[3]),
        .I2(command[1]),
        .I3(command[4]),
        .I4(command[0]),
        .I5(command[5]),
        .O(\addr[31]_i_5_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair25" *) 
  LUT4 #(
    .INIT(16'hFFEF)) 
    \addr[31]_i_6 
       (.I0(command[4]),
        .I1(command[3]),
        .I2(command[5]),
        .I3(command[2]),
        .O(\addr[31]_i_6_n_0 ));
  LUT6 #(
    .INIT(64'hAAAAAAAAAAA8AAAA)) 
    \addr[7]_i_1 
       (.I0(\addr[7]_i_2_n_0 ),
        .I1(\addr[31]_i_3_n_0 ),
        .I2(command[4]),
        .I3(command[3]),
        .I4(command[5]),
        .I5(command[2]),
        .O(\addr[7]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000000100000000)) 
    \addr[7]_i_2 
       (.I0(\addr[31]_i_5_n_0 ),
        .I1(command[0]),
        .I2(\addr[23]_i_3_n_0 ),
        .I3(command[1]),
        .I4(command[2]),
        .I5(\addr[23]_i_4_n_0 ),
        .O(\addr[7]_i_2_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[0] 
       (.C(clk),
        .CE(\addr[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[0] ),
        .Q(M00_AXI_araddr[0]),
        .R(\addr[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[10] 
       (.C(clk),
        .CE(\addr[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[2] ),
        .Q(M00_AXI_araddr[10]),
        .R(\addr[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[11] 
       (.C(clk),
        .CE(\addr[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[3] ),
        .Q(M00_AXI_araddr[11]),
        .R(\addr[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[12] 
       (.C(clk),
        .CE(\addr[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[4] ),
        .Q(M00_AXI_araddr[12]),
        .R(\addr[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[13] 
       (.C(clk),
        .CE(\addr[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[5] ),
        .Q(M00_AXI_araddr[13]),
        .R(\addr[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[14] 
       (.C(clk),
        .CE(\addr[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[6] ),
        .Q(M00_AXI_araddr[14]),
        .R(\addr[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[15] 
       (.C(clk),
        .CE(\addr[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[7] ),
        .Q(M00_AXI_araddr[15]),
        .R(\addr[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[16] 
       (.C(clk),
        .CE(\addr[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[0] ),
        .Q(M00_AXI_araddr[16]),
        .R(\addr[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[17] 
       (.C(clk),
        .CE(\addr[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[1] ),
        .Q(M00_AXI_araddr[17]),
        .R(\addr[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[18] 
       (.C(clk),
        .CE(\addr[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[2] ),
        .Q(M00_AXI_araddr[18]),
        .R(\addr[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[19] 
       (.C(clk),
        .CE(\addr[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[3] ),
        .Q(M00_AXI_araddr[19]),
        .R(\addr[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[1] 
       (.C(clk),
        .CE(\addr[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[1] ),
        .Q(M00_AXI_araddr[1]),
        .R(\addr[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[20] 
       (.C(clk),
        .CE(\addr[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[4] ),
        .Q(M00_AXI_araddr[20]),
        .R(\addr[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[21] 
       (.C(clk),
        .CE(\addr[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[5] ),
        .Q(M00_AXI_araddr[21]),
        .R(\addr[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[22] 
       (.C(clk),
        .CE(\addr[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[6] ),
        .Q(M00_AXI_araddr[22]),
        .R(\addr[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[23] 
       (.C(clk),
        .CE(\addr[23]_i_2_n_0 ),
        .D(\data_reg_n_0_[7] ),
        .Q(M00_AXI_araddr[23]),
        .R(\addr[23]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[24] 
       (.C(clk),
        .CE(\addr[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[0] ),
        .Q(M00_AXI_araddr[24]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[25] 
       (.C(clk),
        .CE(\addr[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[1] ),
        .Q(M00_AXI_araddr[25]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[26] 
       (.C(clk),
        .CE(\addr[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[2] ),
        .Q(M00_AXI_araddr[26]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[27] 
       (.C(clk),
        .CE(\addr[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[3] ),
        .Q(M00_AXI_araddr[27]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[28] 
       (.C(clk),
        .CE(\addr[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[4] ),
        .Q(M00_AXI_araddr[28]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[29] 
       (.C(clk),
        .CE(\addr[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[5] ),
        .Q(M00_AXI_araddr[29]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[2] 
       (.C(clk),
        .CE(\addr[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[2] ),
        .Q(M00_AXI_araddr[2]),
        .R(\addr[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[30] 
       (.C(clk),
        .CE(\addr[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[6] ),
        .Q(M00_AXI_araddr[30]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[31] 
       (.C(clk),
        .CE(\addr[31]_i_1_n_0 ),
        .D(\data_reg_n_0_[7] ),
        .Q(M00_AXI_araddr[31]),
        .R(1'b0));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[3] 
       (.C(clk),
        .CE(\addr[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[3] ),
        .Q(M00_AXI_araddr[3]),
        .R(\addr[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[4] 
       (.C(clk),
        .CE(\addr[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[4] ),
        .Q(M00_AXI_araddr[4]),
        .R(\addr[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[5] 
       (.C(clk),
        .CE(\addr[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[5] ),
        .Q(M00_AXI_araddr[5]),
        .R(\addr[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[6] 
       (.C(clk),
        .CE(\addr[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[6] ),
        .Q(M00_AXI_araddr[6]),
        .R(\addr[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[7] 
       (.C(clk),
        .CE(\addr[7]_i_2_n_0 ),
        .D(\data_reg_n_0_[7] ),
        .Q(M00_AXI_araddr[7]),
        .R(\addr[7]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[8] 
       (.C(clk),
        .CE(\addr[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[0] ),
        .Q(M00_AXI_araddr[8]),
        .R(\addr[15]_i_1_n_0 ));
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) 
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) 
  FDRE \addr_reg[9] 
       (.C(clk),
        .CE(\addr[15]_i_2_n_0 ),
        .D(\data_reg_n_0_[1] ),
        .Q(M00_AXI_araddr[9]),
        .R(\addr[15]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'hCCCCCCCC8080FF80)) 
    command_done_i_1
       (.I0(command_done_i_2_n_0),
        .I1(command_done_i_3_n_0),
        .I2(keyin[3]),
        .I3(command_done_i_4_n_0),
        .I4(command[7]),
        .I5(command_done_reg_n_0),
        .O(command_done_i_1_n_0));
  LUT6 #(
    .INIT(64'h0000001010100010)) 
    command_done_i_2
       (.I0(command[7]),
        .I1(command[6]),
        .I2(\debug_readmem_step[1]_i_7_n_0 ),
        .I3(M00_AXI_rready_i_2_n_0),
        .I4(command[0]),
        .I5(M00_AXI_bready_i_3_n_0),
        .O(command_done_i_2_n_0));
  LUT6 #(
    .INIT(64'hFFFFFFFFFFFFFFFD)) 
    command_done_i_3
       (.I0(command_done_reg_n_0),
        .I1(command[7]),
        .I2(command[6]),
        .I3(command[3]),
        .I4(command[5]),
        .I5(\M00_AXI_wstrb[3]_i_2_n_0 ),
        .O(command_done_i_3_n_0));
  LUT6 #(
    .INIT(64'h00000144111101CC)) 
    command_done_i_4
       (.I0(command[3]),
        .I1(command[5]),
        .I2(command[2]),
        .I3(command[4]),
        .I4(command[6]),
        .I5(command_done_i_5_n_0),
        .O(command_done_i_4_n_0));
  (* SOFT_HLUTNM = "soft_lutpair28" *) 
  LUT3 #(
    .INIT(8'hFE)) 
    command_done_i_5
       (.I0(command[1]),
        .I1(command[2]),
        .I2(command[0]),
        .O(command_done_i_5_n_0));
  FDCE command_done_reg
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(command_done_i_1_n_0),
        .Q(command_done_reg_n_0));
  FDCE \command_reg[0] 
       (.C(clk),
        .CE(uart_hs_inst_n_11),
        .CLR(uart_hs_inst_n_0),
        .D(uart_hs_inst_n_20),
        .Q(command[0]));
  FDCE \command_reg[1] 
       (.C(clk),
        .CE(uart_hs_inst_n_11),
        .CLR(uart_hs_inst_n_0),
        .D(uart_hs_inst_n_19),
        .Q(command[1]));
  FDCE \command_reg[2] 
       (.C(clk),
        .CE(uart_hs_inst_n_11),
        .CLR(uart_hs_inst_n_0),
        .D(uart_hs_inst_n_18),
        .Q(command[2]));
  FDCE \command_reg[3] 
       (.C(clk),
        .CE(uart_hs_inst_n_11),
        .CLR(uart_hs_inst_n_0),
        .D(uart_hs_inst_n_17),
        .Q(command[3]));
  FDCE \command_reg[4] 
       (.C(clk),
        .CE(uart_hs_inst_n_11),
        .CLR(uart_hs_inst_n_0),
        .D(uart_hs_inst_n_16),
        .Q(command[4]));
  FDCE \command_reg[5] 
       (.C(clk),
        .CE(uart_hs_inst_n_11),
        .CLR(uart_hs_inst_n_0),
        .D(uart_hs_inst_n_15),
        .Q(command[5]));
  FDCE \command_reg[6] 
       (.C(clk),
        .CE(uart_hs_inst_n_11),
        .CLR(uart_hs_inst_n_0),
        .D(uart_hs_inst_n_14),
        .Q(command[6]));
  FDCE \command_reg[7] 
       (.C(clk),
        .CE(uart_hs_inst_n_11),
        .CLR(uart_hs_inst_n_0),
        .D(uart_hs_inst_n_13),
        .Q(command[7]));
  FDRE \command_temp_reg[0] 
       (.C(clk),
        .CE(command_temp_1),
        .D(uart_data_out[0]),
        .Q(command_temp[0]),
        .R(1'b0));
  FDRE \command_temp_reg[1] 
       (.C(clk),
        .CE(command_temp_1),
        .D(uart_data_out[1]),
        .Q(command_temp[1]),
        .R(1'b0));
  FDRE \command_temp_reg[2] 
       (.C(clk),
        .CE(command_temp_1),
        .D(uart_data_out[2]),
        .Q(command_temp[2]),
        .R(1'b0));
  FDRE \command_temp_reg[3] 
       (.C(clk),
        .CE(command_temp_1),
        .D(uart_data_out[3]),
        .Q(command_temp[3]),
        .R(1'b0));
  FDRE \command_temp_reg[4] 
       (.C(clk),
        .CE(command_temp_1),
        .D(uart_data_out[4]),
        .Q(command_temp[4]),
        .R(1'b0));
  FDRE \command_temp_reg[5] 
       (.C(clk),
        .CE(command_temp_1),
        .D(uart_data_out[5]),
        .Q(command_temp[5]),
        .R(1'b0));
  FDRE \command_temp_reg[6] 
       (.C(clk),
        .CE(command_temp_1),
        .D(uart_data_out[6]),
        .Q(command_temp[6]),
        .R(1'b0));
  FDRE \command_temp_reg[7] 
       (.C(clk),
        .CE(command_temp_1),
        .D(uart_data_out[7]),
        .Q(command_temp[7]),
        .R(1'b0));
  LUT4 #(
    .INIT(16'h0400)) 
    \data_buff[31]_i_1 
       (.I0(M00_AXI_rready_i_2_n_0),
        .I1(keyin[3]),
        .I2(command[0]),
        .I3(M00_AXI_bready_i_2_n_0),
        .O(data_buff));
  FDRE \data_buff_reg[0] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[0]),
        .Q(\data_buff_reg_n_0_[0] ),
        .R(1'b0));
  FDRE \data_buff_reg[10] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[10]),
        .Q(\data_buff_reg_n_0_[10] ),
        .R(1'b0));
  FDRE \data_buff_reg[11] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[11]),
        .Q(\data_buff_reg_n_0_[11] ),
        .R(1'b0));
  FDRE \data_buff_reg[12] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[12]),
        .Q(\data_buff_reg_n_0_[12] ),
        .R(1'b0));
  FDRE \data_buff_reg[13] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[13]),
        .Q(\data_buff_reg_n_0_[13] ),
        .R(1'b0));
  FDRE \data_buff_reg[14] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[14]),
        .Q(\data_buff_reg_n_0_[14] ),
        .R(1'b0));
  FDRE \data_buff_reg[15] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[15]),
        .Q(\data_buff_reg_n_0_[15] ),
        .R(1'b0));
  FDRE \data_buff_reg[16] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[16]),
        .Q(\data_buff_reg_n_0_[16] ),
        .R(1'b0));
  FDRE \data_buff_reg[17] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[17]),
        .Q(\data_buff_reg_n_0_[17] ),
        .R(1'b0));
  FDRE \data_buff_reg[18] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[18]),
        .Q(\data_buff_reg_n_0_[18] ),
        .R(1'b0));
  FDRE \data_buff_reg[19] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[19]),
        .Q(\data_buff_reg_n_0_[19] ),
        .R(1'b0));
  FDRE \data_buff_reg[1] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[1]),
        .Q(\data_buff_reg_n_0_[1] ),
        .R(1'b0));
  FDRE \data_buff_reg[20] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[20]),
        .Q(\data_buff_reg_n_0_[20] ),
        .R(1'b0));
  FDRE \data_buff_reg[21] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[21]),
        .Q(\data_buff_reg_n_0_[21] ),
        .R(1'b0));
  FDRE \data_buff_reg[22] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[22]),
        .Q(\data_buff_reg_n_0_[22] ),
        .R(1'b0));
  FDRE \data_buff_reg[23] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[23]),
        .Q(\data_buff_reg_n_0_[23] ),
        .R(1'b0));
  FDRE \data_buff_reg[24] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[24]),
        .Q(\data_buff_reg_n_0_[24] ),
        .R(1'b0));
  FDRE \data_buff_reg[25] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[25]),
        .Q(\data_buff_reg_n_0_[25] ),
        .R(1'b0));
  FDRE \data_buff_reg[26] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[26]),
        .Q(\data_buff_reg_n_0_[26] ),
        .R(1'b0));
  FDRE \data_buff_reg[27] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[27]),
        .Q(\data_buff_reg_n_0_[27] ),
        .R(1'b0));
  FDRE \data_buff_reg[28] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[28]),
        .Q(\data_buff_reg_n_0_[28] ),
        .R(1'b0));
  FDRE \data_buff_reg[29] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[29]),
        .Q(\data_buff_reg_n_0_[29] ),
        .R(1'b0));
  FDRE \data_buff_reg[2] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[2]),
        .Q(\data_buff_reg_n_0_[2] ),
        .R(1'b0));
  FDRE \data_buff_reg[30] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[30]),
        .Q(\data_buff_reg_n_0_[30] ),
        .R(1'b0));
  FDRE \data_buff_reg[31] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[31]),
        .Q(\data_buff_reg_n_0_[31] ),
        .R(1'b0));
  FDRE \data_buff_reg[3] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[3]),
        .Q(\data_buff_reg_n_0_[3] ),
        .R(1'b0));
  FDRE \data_buff_reg[4] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[4]),
        .Q(\data_buff_reg_n_0_[4] ),
        .R(1'b0));
  FDRE \data_buff_reg[5] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[5]),
        .Q(\data_buff_reg_n_0_[5] ),
        .R(1'b0));
  FDRE \data_buff_reg[6] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[6]),
        .Q(\data_buff_reg_n_0_[6] ),
        .R(1'b0));
  FDRE \data_buff_reg[7] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[7]),
        .Q(\data_buff_reg_n_0_[7] ),
        .R(1'b0));
  FDRE \data_buff_reg[8] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[8]),
        .Q(\data_buff_reg_n_0_[8] ),
        .R(1'b0));
  FDRE \data_buff_reg[9] 
       (.C(clk),
        .CE(data_buff),
        .D(M00_AXI_rdata[9]),
        .Q(\data_buff_reg_n_0_[9] ),
        .R(1'b0));
  FDRE data_cmd_reg
       (.C(clk),
        .CE(1'b1),
        .D(uart_hs_inst_n_21),
        .Q(data_cmd_reg_n_0),
        .R(1'b0));
  FDCE \data_reg[0] 
       (.C(clk),
        .CE(data),
        .CLR(uart_hs_inst_n_0),
        .D(uart_data_out[0]),
        .Q(\data_reg_n_0_[0] ));
  FDCE \data_reg[1] 
       (.C(clk),
        .CE(data),
        .CLR(uart_hs_inst_n_0),
        .D(uart_data_out[1]),
        .Q(\data_reg_n_0_[1] ));
  FDCE \data_reg[2] 
       (.C(clk),
        .CE(data),
        .CLR(uart_hs_inst_n_0),
        .D(uart_data_out[2]),
        .Q(\data_reg_n_0_[2] ));
  FDCE \data_reg[3] 
       (.C(clk),
        .CE(data),
        .CLR(uart_hs_inst_n_0),
        .D(uart_data_out[3]),
        .Q(\data_reg_n_0_[3] ));
  FDCE \data_reg[4] 
       (.C(clk),
        .CE(data),
        .CLR(uart_hs_inst_n_0),
        .D(uart_data_out[4]),
        .Q(\data_reg_n_0_[4] ));
  FDCE \data_reg[5] 
       (.C(clk),
        .CE(data),
        .CLR(uart_hs_inst_n_0),
        .D(uart_data_out[5]),
        .Q(\data_reg_n_0_[5] ));
  FDCE \data_reg[6] 
       (.C(clk),
        .CE(data),
        .CLR(uart_hs_inst_n_0),
        .D(uart_data_out[6]),
        .Q(\data_reg_n_0_[6] ));
  FDCE \data_reg[7] 
       (.C(clk),
        .CE(data),
        .CLR(uart_hs_inst_n_0),
        .D(uart_data_out[7]),
        .Q(\data_reg_n_0_[7] ));
  LUT5 #(
    .INIT(32'h47FF4700)) 
    \debug_readmem_step[0]_i_1 
       (.I0(\debug_readmem_step[0]_i_2_n_0 ),
        .I1(\debug_readmem_step[1]_i_3_n_0 ),
        .I2(\debug_readmem_step[0]_i_3_n_0 ),
        .I3(\debug_readmem_step[1]_i_4_n_0 ),
        .I4(\debug_readmem_step_reg_n_0_[0] ),
        .O(\debug_readmem_step[0]_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair23" *) 
  LUT5 #(
    .INIT(32'h2020202C)) 
    \debug_readmem_step[0]_i_2 
       (.I0(M00_AXI_bvalid),
        .I1(\debug_readmem_step_reg_n_0_[0] ),
        .I2(\debug_readmem_step_reg_n_0_[1] ),
        .I3(\debug[4] ),
        .I4(\debug[6] ),
        .O(\debug_readmem_step[0]_i_2_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair21" *) 
  LUT5 #(
    .INIT(32'hC0C0EACC)) 
    \debug_readmem_step[0]_i_3 
       (.I0(debug[1]),
        .I1(debug[0]),
        .I2(debug_0_sn_1),
        .I3(\debug_readmem_step_reg_n_0_[1] ),
        .I4(\debug_readmem_step_reg_n_0_[0] ),
        .O(\debug_readmem_step[0]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'hB888FFFFB8880000)) 
    \debug_readmem_step[1]_i_1 
       (.I0(\debug_readmem_step[1]_i_2_n_0 ),
        .I1(\debug_readmem_step[1]_i_3_n_0 ),
        .I2(M00_AXI_rready_i_2_n_0),
        .I3(command_done221_out),
        .I4(\debug_readmem_step[1]_i_4_n_0 ),
        .I5(\debug_readmem_step_reg_n_0_[1] ),
        .O(\debug_readmem_step[1]_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair23" *) 
  LUT5 #(
    .INIT(32'h00000004)) 
    \debug_readmem_step[1]_i_2 
       (.I0(M00_AXI_bvalid),
        .I1(\debug_readmem_step_reg_n_0_[0] ),
        .I2(\debug_readmem_step_reg_n_0_[1] ),
        .I3(\debug[4] ),
        .I4(\debug[6] ),
        .O(\debug_readmem_step[1]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hFFFFFFFFFFEFFFFF)) 
    \debug_readmem_step[1]_i_3 
       (.I0(command[1]),
        .I1(command[2]),
        .I2(command[4]),
        .I3(command[0]),
        .I4(command[5]),
        .I5(\debug_readmem_step[1]_i_5_n_0 ),
        .O(\debug_readmem_step[1]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'h8880008088888888)) 
    \debug_readmem_step[1]_i_4 
       (.I0(\debug_readmem_step[1]_i_6_n_0 ),
        .I1(\debug_readmem_step[1]_i_7_n_0 ),
        .I2(\debug_readmem_step[0]_i_3_n_0 ),
        .I3(command[0]),
        .I4(\debug_readmem_step[0]_i_2_n_0 ),
        .I5(\debug_readmem_step[1]_i_8_n_0 ),
        .O(\debug_readmem_step[1]_i_4_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair20" *) 
  LUT5 #(
    .INIT(32'hFFEFEFEF)) 
    \debug_readmem_step[1]_i_5 
       (.I0(command[6]),
        .I1(command[7]),
        .I2(command[5]),
        .I3(command[3]),
        .I4(command[4]),
        .O(\debug_readmem_step[1]_i_5_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair30" *) 
  LUT3 #(
    .INIT(8'h01)) 
    \debug_readmem_step[1]_i_6 
       (.I0(command[6]),
        .I1(command[7]),
        .I2(command_done_reg_n_0),
        .O(\debug_readmem_step[1]_i_6_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair22" *) 
  LUT5 #(
    .INIT(32'h00000200)) 
    \debug_readmem_step[1]_i_7 
       (.I0(command[4]),
        .I1(command[2]),
        .I2(command[1]),
        .I3(command[5]),
        .I4(command[3]),
        .O(\debug_readmem_step[1]_i_7_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair29" *) 
  LUT2 #(
    .INIT(4'hE)) 
    \debug_readmem_step[1]_i_8 
       (.I0(\debug_readmem_step_reg_n_0_[0] ),
        .I1(\debug_readmem_step_reg_n_0_[1] ),
        .O(\debug_readmem_step[1]_i_8_n_0 ));
  FDCE \debug_readmem_step_reg[0] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(\debug_readmem_step[0]_i_1_n_0 ),
        .Q(\debug_readmem_step_reg_n_0_[0] ));
  FDCE \debug_readmem_step_reg[1] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(\debug_readmem_step[1]_i_1_n_0 ),
        .Q(\debug_readmem_step_reg_n_0_[1] ));
  LUT5 #(
    .INIT(32'hFFFE0001)) 
    flg_i_1
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(led[1]),
        .O(flg_i_1_n_0));
  LUT4 #(
    .INIT(16'hFFDF)) 
    flg_i_10
       (.I0(timer33[13]),
        .I1(timer33[12]),
        .I2(timer33[15]),
        .I3(timer33[14]),
        .O(flg_i_10_n_0));
  LUT5 #(
    .INIT(32'hFFFFFFF7)) 
    flg_i_3
       (.I0(timer33[18]),
        .I1(timer33[19]),
        .I2(timer33[16]),
        .I3(timer33[17]),
        .I4(flg_i_7_n_0),
        .O(flg_i_3_n_0));
  LUT5 #(
    .INIT(32'hFFFFFEFF)) 
    flg_i_4
       (.I0(timer33[26]),
        .I1(timer33[27]),
        .I2(timer33[25]),
        .I3(timer33[24]),
        .I4(flg_i_8_n_0),
        .O(flg_i_4_n_0));
  LUT5 #(
    .INIT(32'hFFFFFBFF)) 
    flg_i_5
       (.I0(timer33[3]),
        .I1(timer33[2]),
        .I2(timer33[1]),
        .I3(timer33[0]),
        .I4(flg_i_9_n_0),
        .O(flg_i_5_n_0));
  LUT5 #(
    .INIT(32'hFFFFFFFE)) 
    flg_i_6
       (.I0(timer33[10]),
        .I1(timer33[11]),
        .I2(timer33[8]),
        .I3(timer33[9]),
        .I4(flg_i_10_n_0),
        .O(flg_i_6_n_0));
  LUT4 #(
    .INIT(16'h7FFF)) 
    flg_i_7
       (.I0(timer33[21]),
        .I1(timer33[20]),
        .I2(timer33[23]),
        .I3(timer33[22]),
        .O(flg_i_7_n_0));
  LUT4 #(
    .INIT(16'hFFFE)) 
    flg_i_8
       (.I0(timer33[29]),
        .I1(timer33[28]),
        .I2(timer33[31]),
        .I3(timer33[30]),
        .O(flg_i_8_n_0));
  LUT4 #(
    .INIT(16'hFFDF)) 
    flg_i_9
       (.I0(timer33[4]),
        .I1(timer33[5]),
        .I2(timer33[6]),
        .I3(timer33[7]),
        .O(flg_i_9_n_0));
  FDCE flg_reg
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(flg_i_1_n_0),
        .Q(led[1]));
  LUT1 #(
    .INIT(2'h1)) 
    \timer33[0]_i_1 
       (.I0(timer33[0]),
        .O(timer33_2[0]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[10]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[10]),
        .O(timer33_2[10]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[11]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[11]),
        .O(timer33_2[11]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[12]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[12]),
        .O(timer33_2[12]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[13]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[13]),
        .O(timer33_2[13]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[14]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[14]),
        .O(timer33_2[14]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[15]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[15]),
        .O(timer33_2[15]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[16]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[16]),
        .O(timer33_2[16]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[17]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[17]),
        .O(timer33_2[17]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[18]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[18]),
        .O(timer33_2[18]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[19]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[19]),
        .O(timer33_2[19]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[1]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[1]),
        .O(timer33_2[1]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[20]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[20]),
        .O(timer33_2[20]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[21]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[21]),
        .O(timer33_2[21]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[22]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[22]),
        .O(timer33_2[22]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[23]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[23]),
        .O(timer33_2[23]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[24]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[24]),
        .O(timer33_2[24]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[25]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[25]),
        .O(timer33_2[25]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[26]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[26]),
        .O(timer33_2[26]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[27]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[27]),
        .O(timer33_2[27]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[28]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[28]),
        .O(timer33_2[28]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[29]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[29]),
        .O(timer33_2[29]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[2]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[2]),
        .O(timer33_2[2]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[30]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[30]),
        .O(timer33_2[30]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[31]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[31]),
        .O(timer33_2[31]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[3]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[3]),
        .O(timer33_2[3]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[4]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[4]),
        .O(timer33_2[4]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[5]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[5]),
        .O(timer33_2[5]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[6]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[6]),
        .O(timer33_2[6]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[7]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[7]),
        .O(timer33_2[7]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[8]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[8]),
        .O(timer33_2[8]));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \timer33[9]_i_1 
       (.I0(flg_i_3_n_0),
        .I1(flg_i_4_n_0),
        .I2(flg_i_5_n_0),
        .I3(flg_i_6_n_0),
        .I4(data0[9]),
        .O(timer33_2[9]));
  FDCE \timer33_reg[0] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[0]),
        .Q(timer33[0]));
  FDCE \timer33_reg[10] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[10]),
        .Q(timer33[10]));
  FDCE \timer33_reg[11] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[11]),
        .Q(timer33[11]));
  FDCE \timer33_reg[12] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[12]),
        .Q(timer33[12]));
  CARRY4 \timer33_reg[12]_i_2 
       (.CI(\timer33_reg[8]_i_2_n_0 ),
        .CO({\timer33_reg[12]_i_2_n_0 ,\timer33_reg[12]_i_2_n_1 ,\timer33_reg[12]_i_2_n_2 ,\timer33_reg[12]_i_2_n_3 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(data0[12:9]),
        .S(timer33[12:9]));
  FDCE \timer33_reg[13] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[13]),
        .Q(timer33[13]));
  FDCE \timer33_reg[14] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[14]),
        .Q(timer33[14]));
  FDCE \timer33_reg[15] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[15]),
        .Q(timer33[15]));
  FDCE \timer33_reg[16] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[16]),
        .Q(timer33[16]));
  CARRY4 \timer33_reg[16]_i_2 
       (.CI(\timer33_reg[12]_i_2_n_0 ),
        .CO({\timer33_reg[16]_i_2_n_0 ,\timer33_reg[16]_i_2_n_1 ,\timer33_reg[16]_i_2_n_2 ,\timer33_reg[16]_i_2_n_3 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(data0[16:13]),
        .S(timer33[16:13]));
  FDCE \timer33_reg[17] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[17]),
        .Q(timer33[17]));
  FDCE \timer33_reg[18] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[18]),
        .Q(timer33[18]));
  FDCE \timer33_reg[19] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[19]),
        .Q(timer33[19]));
  FDCE \timer33_reg[1] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[1]),
        .Q(timer33[1]));
  FDCE \timer33_reg[20] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[20]),
        .Q(timer33[20]));
  CARRY4 \timer33_reg[20]_i_2 
       (.CI(\timer33_reg[16]_i_2_n_0 ),
        .CO({\timer33_reg[20]_i_2_n_0 ,\timer33_reg[20]_i_2_n_1 ,\timer33_reg[20]_i_2_n_2 ,\timer33_reg[20]_i_2_n_3 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(data0[20:17]),
        .S(timer33[20:17]));
  FDCE \timer33_reg[21] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[21]),
        .Q(timer33[21]));
  FDCE \timer33_reg[22] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[22]),
        .Q(timer33[22]));
  FDCE \timer33_reg[23] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[23]),
        .Q(timer33[23]));
  FDCE \timer33_reg[24] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[24]),
        .Q(timer33[24]));
  CARRY4 \timer33_reg[24]_i_2 
       (.CI(\timer33_reg[20]_i_2_n_0 ),
        .CO({\timer33_reg[24]_i_2_n_0 ,\timer33_reg[24]_i_2_n_1 ,\timer33_reg[24]_i_2_n_2 ,\timer33_reg[24]_i_2_n_3 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(data0[24:21]),
        .S(timer33[24:21]));
  FDCE \timer33_reg[25] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[25]),
        .Q(timer33[25]));
  FDCE \timer33_reg[26] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[26]),
        .Q(timer33[26]));
  FDCE \timer33_reg[27] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[27]),
        .Q(timer33[27]));
  FDCE \timer33_reg[28] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[28]),
        .Q(timer33[28]));
  CARRY4 \timer33_reg[28]_i_2 
       (.CI(\timer33_reg[24]_i_2_n_0 ),
        .CO({\timer33_reg[28]_i_2_n_0 ,\timer33_reg[28]_i_2_n_1 ,\timer33_reg[28]_i_2_n_2 ,\timer33_reg[28]_i_2_n_3 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(data0[28:25]),
        .S(timer33[28:25]));
  FDCE \timer33_reg[29] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[29]),
        .Q(timer33[29]));
  FDCE \timer33_reg[2] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[2]),
        .Q(timer33[2]));
  FDCE \timer33_reg[30] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[30]),
        .Q(timer33[30]));
  FDCE \timer33_reg[31] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[31]),
        .Q(timer33[31]));
  CARRY4 \timer33_reg[31]_i_2 
       (.CI(\timer33_reg[28]_i_2_n_0 ),
        .CO({\NLW_timer33_reg[31]_i_2_CO_UNCONNECTED [3:2],\timer33_reg[31]_i_2_n_2 ,\timer33_reg[31]_i_2_n_3 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\NLW_timer33_reg[31]_i_2_O_UNCONNECTED [3],data0[31:29]}),
        .S({1'b0,timer33[31:29]}));
  FDCE \timer33_reg[3] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[3]),
        .Q(timer33[3]));
  FDCE \timer33_reg[4] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[4]),
        .Q(timer33[4]));
  CARRY4 \timer33_reg[4]_i_2 
       (.CI(1'b0),
        .CO({\timer33_reg[4]_i_2_n_0 ,\timer33_reg[4]_i_2_n_1 ,\timer33_reg[4]_i_2_n_2 ,\timer33_reg[4]_i_2_n_3 }),
        .CYINIT(timer33[0]),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(data0[4:1]),
        .S(timer33[4:1]));
  FDCE \timer33_reg[5] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[5]),
        .Q(timer33[5]));
  FDCE \timer33_reg[6] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[6]),
        .Q(timer33[6]));
  FDCE \timer33_reg[7] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[7]),
        .Q(timer33[7]));
  FDCE \timer33_reg[8] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[8]),
        .Q(timer33[8]));
  CARRY4 \timer33_reg[8]_i_2 
       (.CI(\timer33_reg[4]_i_2_n_0 ),
        .CO({\timer33_reg[8]_i_2_n_0 ,\timer33_reg[8]_i_2_n_1 ,\timer33_reg[8]_i_2_n_2 ,\timer33_reg[8]_i_2_n_3 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(data0[8:5]),
        .S(timer33[8:5]));
  FDCE \timer33_reg[9] 
       (.C(clk),
        .CE(1'b1),
        .CLR(uart_hs_inst_n_0),
        .D(timer33_2[9]),
        .Q(timer33[9]));
  LUT6 #(
    .INIT(64'hAEAEAEAAAAAEAAAA)) 
    \uart_data_in[0]_i_1 
       (.I0(\uart_data_in[0]_i_2_n_0 ),
        .I1(command[0]),
        .I2(\uart_data_in[7]_i_6_n_0 ),
        .I3(command[1]),
        .I4(\data_buff_reg_n_0_[8] ),
        .I5(\data_buff_reg_n_0_[24] ),
        .O(\uart_data_in[0]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'hFFFF541000005410)) 
    \uart_data_in[0]_i_2 
       (.I0(command[0]),
        .I1(command[1]),
        .I2(\data_buff_reg_n_0_[0] ),
        .I3(\data_buff_reg_n_0_[16] ),
        .I4(\uart_data_in[7]_i_6_n_0 ),
        .I5(keyin[0]),
        .O(\uart_data_in[0]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hAEAEAEAAAAAEAAAA)) 
    \uart_data_in[1]_i_1 
       (.I0(\uart_data_in[1]_i_2_n_0 ),
        .I1(command[0]),
        .I2(\uart_data_in[7]_i_6_n_0 ),
        .I3(command[1]),
        .I4(\data_buff_reg_n_0_[9] ),
        .I5(\data_buff_reg_n_0_[25] ),
        .O(\uart_data_in[1]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'hFFFF541000005410)) 
    \uart_data_in[1]_i_2 
       (.I0(command[0]),
        .I1(command[1]),
        .I2(\data_buff_reg_n_0_[1] ),
        .I3(\data_buff_reg_n_0_[17] ),
        .I4(\uart_data_in[7]_i_6_n_0 ),
        .I5(keyin[1]),
        .O(\uart_data_in[1]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hAEAEAEAAAAAEAAAA)) 
    \uart_data_in[2]_i_1 
       (.I0(\uart_data_in[2]_i_2_n_0 ),
        .I1(command[0]),
        .I2(\uart_data_in[7]_i_6_n_0 ),
        .I3(command[1]),
        .I4(\data_buff_reg_n_0_[10] ),
        .I5(\data_buff_reg_n_0_[26] ),
        .O(\uart_data_in[2]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'hFFFF541000005410)) 
    \uart_data_in[2]_i_2 
       (.I0(command[0]),
        .I1(command[1]),
        .I2(\data_buff_reg_n_0_[2] ),
        .I3(\data_buff_reg_n_0_[18] ),
        .I4(\uart_data_in[7]_i_6_n_0 ),
        .I5(keyin[2]),
        .O(\uart_data_in[2]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hAEAEAEAAAAAEAAAA)) 
    \uart_data_in[3]_i_1 
       (.I0(\uart_data_in[3]_i_2_n_0 ),
        .I1(command[0]),
        .I2(\uart_data_in[7]_i_6_n_0 ),
        .I3(command[1]),
        .I4(\data_buff_reg_n_0_[11] ),
        .I5(\data_buff_reg_n_0_[27] ),
        .O(\uart_data_in[3]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'hFFFF000054105410)) 
    \uart_data_in[3]_i_2 
       (.I0(command[0]),
        .I1(command[1]),
        .I2(\data_buff_reg_n_0_[3] ),
        .I3(\data_buff_reg_n_0_[19] ),
        .I4(keyin[3]),
        .I5(\uart_data_in[7]_i_6_n_0 ),
        .O(\uart_data_in[3]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hFFFFFFFF22200200)) 
    \uart_data_in[4]_i_1 
       (.I0(command[0]),
        .I1(\uart_data_in[7]_i_6_n_0 ),
        .I2(command[1]),
        .I3(\data_buff_reg_n_0_[12] ),
        .I4(\data_buff_reg_n_0_[28] ),
        .I5(\uart_data_in[4]_i_2_n_0 ),
        .O(\uart_data_in[4]_i_1_n_0 ));
  LUT5 #(
    .INIT(32'h000000AC)) 
    \uart_data_in[4]_i_2 
       (.I0(\data_buff_reg_n_0_[20] ),
        .I1(\data_buff_reg_n_0_[4] ),
        .I2(command[1]),
        .I3(\uart_data_in[7]_i_6_n_0 ),
        .I4(command[0]),
        .O(\uart_data_in[4]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hFFFFFFFF22200200)) 
    \uart_data_in[5]_i_1 
       (.I0(command[0]),
        .I1(\uart_data_in[7]_i_6_n_0 ),
        .I2(command[1]),
        .I3(\data_buff_reg_n_0_[13] ),
        .I4(\data_buff_reg_n_0_[29] ),
        .I5(\uart_data_in[5]_i_2_n_0 ),
        .O(\uart_data_in[5]_i_1_n_0 ));
  LUT5 #(
    .INIT(32'h000000AC)) 
    \uart_data_in[5]_i_2 
       (.I0(\data_buff_reg_n_0_[21] ),
        .I1(\data_buff_reg_n_0_[5] ),
        .I2(command[1]),
        .I3(\uart_data_in[7]_i_6_n_0 ),
        .I4(command[0]),
        .O(\uart_data_in[5]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hFFFFFFFF22200200)) 
    \uart_data_in[6]_i_1 
       (.I0(command[0]),
        .I1(\uart_data_in[7]_i_6_n_0 ),
        .I2(command[1]),
        .I3(\data_buff_reg_n_0_[14] ),
        .I4(\data_buff_reg_n_0_[30] ),
        .I5(\uart_data_in[6]_i_2_n_0 ),
        .O(\uart_data_in[6]_i_1_n_0 ));
  LUT5 #(
    .INIT(32'h000000AC)) 
    \uart_data_in[6]_i_2 
       (.I0(\data_buff_reg_n_0_[22] ),
        .I1(\data_buff_reg_n_0_[6] ),
        .I2(command[1]),
        .I3(\uart_data_in[7]_i_6_n_0 ),
        .I4(command[0]),
        .O(\uart_data_in[6]_i_2_n_0 ));
  LUT5 #(
    .INIT(32'h44404040)) 
    \uart_data_in[7]_i_1 
       (.I0(command_done_reg_n_0),
        .I1(keyin[3]),
        .I2(\uart_data_in[7]_i_3_n_0 ),
        .I3(\uart_data_in[7]_i_4_n_0 ),
        .I4(\uart_data_in[7]_i_5_n_0 ),
        .O(uart_data_in_0));
  LUT6 #(
    .INIT(64'hFFFFFFFF22200200)) 
    \uart_data_in[7]_i_2 
       (.I0(command[0]),
        .I1(\uart_data_in[7]_i_6_n_0 ),
        .I2(command[1]),
        .I3(\data_buff_reg_n_0_[15] ),
        .I4(\data_buff_reg_n_0_[31] ),
        .I5(\uart_data_in[7]_i_7_n_0 ),
        .O(\uart_data_in[7]_i_2_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair26" *) 
  LUT5 #(
    .INIT(32'h00010000)) 
    \uart_data_in[7]_i_3 
       (.I0(\M00_AXI_wstrb[3]_i_2_n_0 ),
        .I1(command[5]),
        .I2(command[3]),
        .I3(command[7]),
        .I4(command[6]),
        .O(\uart_data_in[7]_i_3_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair25" *) 
  LUT5 #(
    .INIT(32'h00000010)) 
    \uart_data_in[7]_i_4 
       (.I0(command[2]),
        .I1(\addr[31]_i_3_n_0 ),
        .I2(command[4]),
        .I3(command[3]),
        .I4(command[5]),
        .O(\uart_data_in[7]_i_4_n_0 ));
  LUT6 #(
    .INIT(64'hFFFFFFFFFFFFFFF4)) 
    \uart_data_in[7]_i_5 
       (.I0(command[5]),
        .I1(command[3]),
        .I2(command[4]),
        .I3(command[1]),
        .I4(command[2]),
        .I5(command[0]),
        .O(\uart_data_in[7]_i_5_n_0 ));
  LUT6 #(
    .INIT(64'hFFFFFFFFFFFFFEFF)) 
    \uart_data_in[7]_i_6 
       (.I0(command[3]),
        .I1(command[5]),
        .I2(command[2]),
        .I3(command[4]),
        .I4(command[6]),
        .I5(command[7]),
        .O(\uart_data_in[7]_i_6_n_0 ));
  LUT5 #(
    .INIT(32'h000000AC)) 
    \uart_data_in[7]_i_7 
       (.I0(\data_buff_reg_n_0_[23] ),
        .I1(\data_buff_reg_n_0_[7] ),
        .I2(command[1]),
        .I3(\uart_data_in[7]_i_6_n_0 ),
        .I4(command[0]),
        .O(\uart_data_in[7]_i_7_n_0 ));
  FDRE \uart_data_in_reg[0] 
       (.C(clk),
        .CE(uart_data_in_0),
        .D(\uart_data_in[0]_i_1_n_0 ),
        .Q(uart_data_in[0]),
        .R(1'b0));
  FDRE \uart_data_in_reg[1] 
       (.C(clk),
        .CE(uart_data_in_0),
        .D(\uart_data_in[1]_i_1_n_0 ),
        .Q(uart_data_in[1]),
        .R(1'b0));
  FDRE \uart_data_in_reg[2] 
       (.C(clk),
        .CE(uart_data_in_0),
        .D(\uart_data_in[2]_i_1_n_0 ),
        .Q(uart_data_in[2]),
        .R(1'b0));
  FDRE \uart_data_in_reg[3] 
       (.C(clk),
        .CE(uart_data_in_0),
        .D(\uart_data_in[3]_i_1_n_0 ),
        .Q(uart_data_in[3]),
        .R(1'b0));
  FDRE \uart_data_in_reg[4] 
       (.C(clk),
        .CE(uart_data_in_0),
        .D(\uart_data_in[4]_i_1_n_0 ),
        .Q(uart_data_in[4]),
        .R(1'b0));
  FDRE \uart_data_in_reg[5] 
       (.C(clk),
        .CE(uart_data_in_0),
        .D(\uart_data_in[5]_i_1_n_0 ),
        .Q(uart_data_in[5]),
        .R(1'b0));
  FDRE \uart_data_in_reg[6] 
       (.C(clk),
        .CE(uart_data_in_0),
        .D(\uart_data_in[6]_i_1_n_0 ),
        .Q(uart_data_in[6]),
        .R(1'b0));
  FDRE \uart_data_in_reg[7] 
       (.C(clk),
        .CE(uart_data_in_0),
        .D(\uart_data_in[7]_i_2_n_0 ),
        .Q(uart_data_in[7]),
        .R(1'b0));
  design_1_test_0_0_uart_hs uart_hs_inst
       (.D(uart_data_out),
        .E(command_temp_1),
        .Q(command_temp),
        .clk(clk),
        .\clk_cnt_reg[1] (uart_hs_inst_n_0),
        .command_done_reg(command_done_reg_n_0),
        .\command_reg[0] (uart_hs_inst_n_11),
        .\command_reg[7] ({uart_hs_inst_n_13,uart_hs_inst_n_14,uart_hs_inst_n_15,uart_hs_inst_n_16,uart_hs_inst_n_17,uart_hs_inst_n_18,uart_hs_inst_n_19,uart_hs_inst_n_20}),
        .data_cmd_reg(uart_hs_inst_n_21),
        .data_cmd_reg_0(data_cmd_reg_n_0),
        .\data_reg[0] (data),
        .keyin(keyin[3]),
        .\uart_data_in_reg[7] (uart_data_in),
        .uart_rxd(uart_rxd),
        .uart_send(uart_send),
        .uart_txd(uart_txd));
  LUT6 #(
    .INIT(64'h00EAFFFF00EA0000)) 
    uart_send_i_1
       (.I0(\uart_data_in[7]_i_3_n_0 ),
        .I1(\uart_data_in[7]_i_4_n_0 ),
        .I2(\uart_data_in[7]_i_5_n_0 ),
        .I3(command_done_reg_n_0),
        .I4(keyin[3]),
        .I5(uart_send),
        .O(uart_send_i_1_n_0));
  FDRE uart_send_reg
       (.C(clk),
        .CE(1'b1),
        .D(uart_send_i_1_n_0),
        .Q(uart_send),
        .R(1'b0));
endmodule

(* ORIG_REF_NAME = "uart_hs" *) 
module design_1_test_0_0_uart_hs
   (\clk_cnt_reg[1] ,
    uart_txd,
    E,
    D,
    \command_reg[0] ,
    \data_reg[0] ,
    \command_reg[7] ,
    data_cmd_reg,
    uart_send,
    clk,
    uart_rxd,
    keyin,
    data_cmd_reg_0,
    command_done_reg,
    Q,
    \uart_data_in_reg[7] );
  output \clk_cnt_reg[1] ;
  output uart_txd;
  output [0:0]E;
  output [7:0]D;
  output [0:0]\command_reg[0] ;
  output [0:0]\data_reg[0] ;
  output [7:0]\command_reg[7] ;
  output data_cmd_reg;
  input uart_send;
  input clk;
  input uart_rxd;
  input [0:0]keyin;
  input data_cmd_reg_0;
  input command_done_reg;
  input [7:0]Q;
  input [7:0]\uart_data_in_reg[7] ;

  wire [7:0]D;
  wire [0:0]E;
  wire [7:0]Q;
  wire clk;
  wire \clk_cnt_reg[1] ;
  wire command_done_reg;
  wire [0:0]\command_reg[0] ;
  wire [7:0]\command_reg[7] ;
  wire data_cmd_reg;
  wire data_cmd_reg_0;
  wire [0:0]\data_reg[0] ;
  wire [0:0]keyin;
  wire [7:0]\uart_data_in_reg[7] ;
  wire uart_rxd;
  wire uart_send;
  wire uart_txd;

  design_1_test_0_0_uart_recv_hs ins_uart_recv_hs
       (.D(D),
        .E(E),
        .Q(Q),
        .clk(clk),
        .command_done_reg(command_done_reg),
        .\command_reg[0] (\command_reg[0] ),
        .\command_reg[7] (\command_reg[7] ),
        .data_cmd_reg(data_cmd_reg),
        .data_cmd_reg_0(data_cmd_reg_0),
        .\data_reg[0] (\data_reg[0] ),
        .keyin(keyin),
        .\keyin[3] (\clk_cnt_reg[1] ),
        .uart_rxd(uart_rxd));
  design_1_test_0_0_uart_send_hs ins_uart_send_hs
       (.clk(clk),
        .\clk_cnt_reg[1]_0 (\clk_cnt_reg[1] ),
        .keyin(keyin),
        .\uart_data_in_reg[7] (\uart_data_in_reg[7] ),
        .uart_send(uart_send),
        .uart_txd(uart_txd));
endmodule

(* ORIG_REF_NAME = "uart_recv_hs" *) 
module design_1_test_0_0_uart_recv_hs
   (E,
    D,
    \command_reg[0] ,
    \data_reg[0] ,
    \command_reg[7] ,
    data_cmd_reg,
    uart_rxd,
    clk,
    \keyin[3] ,
    keyin,
    data_cmd_reg_0,
    command_done_reg,
    Q);
  output [0:0]E;
  output [7:0]D;
  output [0:0]\command_reg[0] ;
  output [0:0]\data_reg[0] ;
  output [7:0]\command_reg[7] ;
  output data_cmd_reg;
  input uart_rxd;
  input clk;
  input \keyin[3] ;
  input [0:0]keyin;
  input data_cmd_reg_0;
  input command_done_reg;
  input [7:0]Q;

  wire [7:0]D;
  wire [0:0]E;
  wire [7:0]Q;
  wire clk;
  wire \clk_cnt[7]_i_2__0_n_0 ;
  wire [7:0]clk_cnt_reg__0;
  wire command_done_reg;
  wire [0:0]\command_reg[0] ;
  wire [7:0]\command_reg[7] ;
  wire \command_temp[7]_i_3_n_0 ;
  wire data_cmd0__6;
  wire data_cmd_reg;
  wire data_cmd_reg_0;
  wire [0:0]\data_reg[0] ;
  wire [0:0]keyin;
  wire \keyin[3] ;
  wire [7:0]p_0_in__0;
  wire [0:0]p_7_in;
  wire rx_flag;
  wire rx_flag_i_1_n_0;
  wire rx_flag_i_2_n_0;
  wire rx_flag_i_3_n_0;
  wire rx_flag_i_4_n_0;
  wire \uart_data_out[0]_i_1_n_0 ;
  wire \uart_data_out[0]_i_2_n_0 ;
  wire \uart_data_out[1]_i_1_n_0 ;
  wire \uart_data_out[1]_i_2_n_0 ;
  wire \uart_data_out[2]_i_1_n_0 ;
  wire \uart_data_out[2]_i_2_n_0 ;
  wire \uart_data_out[2]_i_3_n_0 ;
  wire \uart_data_out[2]_i_4_n_0 ;
  wire \uart_data_out[3]_i_1_n_0 ;
  wire \uart_data_out[3]_i_2_n_0 ;
  wire \uart_data_out[4]_i_1_n_0 ;
  wire \uart_data_out[4]_i_2_n_0 ;
  wire \uart_data_out[5]_i_1_n_0 ;
  wire \uart_data_out[5]_i_2_n_0 ;
  wire \uart_data_out[6]_i_1_n_0 ;
  wire \uart_data_out[6]_i_2_n_0 ;
  wire \uart_data_out[7]_i_1_n_0 ;
  wire uart_rec;
  wire uart_rec0_out;
  wire uart_rec_i_1_n_0;
  wire uart_rec_i_2_n_0;
  wire uart_rec_i_3_n_0;
  wire uart_rec_i_4_n_0;
  wire uart_rec_i_5_n_0;
  wire uart_rec_i_6_n_0;
  wire uart_rxd;
  wire uart_rxd_last1;

  (* SOFT_HLUTNM = "soft_lutpair10" *) 
  LUT2 #(
    .INIT(4'h2)) 
    \clk_cnt[0]_i_1__0 
       (.I0(rx_flag),
        .I1(clk_cnt_reg__0[0]),
        .O(p_0_in__0[0]));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT3 #(
    .INIT(8'h48)) 
    \clk_cnt[1]_i_1__0 
       (.I0(clk_cnt_reg__0[0]),
        .I1(rx_flag),
        .I2(clk_cnt_reg__0[1]),
        .O(p_0_in__0[1]));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT4 #(
    .INIT(16'h7080)) 
    \clk_cnt[2]_i_1__0 
       (.I0(clk_cnt_reg__0[1]),
        .I1(clk_cnt_reg__0[0]),
        .I2(rx_flag),
        .I3(clk_cnt_reg__0[2]),
        .O(p_0_in__0[2]));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h78F00000)) 
    \clk_cnt[3]_i_1 
       (.I0(clk_cnt_reg__0[1]),
        .I1(clk_cnt_reg__0[0]),
        .I2(clk_cnt_reg__0[3]),
        .I3(clk_cnt_reg__0[2]),
        .I4(rx_flag),
        .O(p_0_in__0[3]));
  LUT6 #(
    .INIT(64'h7FFF000080000000)) 
    \clk_cnt[4]_i_1__0 
       (.I0(clk_cnt_reg__0[3]),
        .I1(clk_cnt_reg__0[2]),
        .I2(clk_cnt_reg__0[1]),
        .I3(clk_cnt_reg__0[0]),
        .I4(rx_flag),
        .I5(clk_cnt_reg__0[4]),
        .O(p_0_in__0[4]));
  LUT6 #(
    .INIT(64'hF708FF0000000000)) 
    \clk_cnt[5]_i_1 
       (.I0(clk_cnt_reg__0[3]),
        .I1(clk_cnt_reg__0[2]),
        .I2(\uart_data_out[2]_i_2_n_0 ),
        .I3(clk_cnt_reg__0[5]),
        .I4(clk_cnt_reg__0[4]),
        .I5(rx_flag),
        .O(p_0_in__0[5]));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT5 #(
    .INIT(32'h7F008000)) 
    \clk_cnt[6]_i_1 
       (.I0(clk_cnt_reg__0[4]),
        .I1(clk_cnt_reg__0[5]),
        .I2(\clk_cnt[7]_i_2__0_n_0 ),
        .I3(rx_flag),
        .I4(clk_cnt_reg__0[6]),
        .O(p_0_in__0[6]));
  LUT6 #(
    .INIT(64'h7F800000FF000000)) 
    \clk_cnt[7]_i_1 
       (.I0(\clk_cnt[7]_i_2__0_n_0 ),
        .I1(clk_cnt_reg__0[5]),
        .I2(clk_cnt_reg__0[4]),
        .I3(clk_cnt_reg__0[7]),
        .I4(rx_flag),
        .I5(clk_cnt_reg__0[6]),
        .O(p_0_in__0[7]));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT4 #(
    .INIT(16'h8000)) 
    \clk_cnt[7]_i_2__0 
       (.I0(clk_cnt_reg__0[3]),
        .I1(clk_cnt_reg__0[2]),
        .I2(clk_cnt_reg__0[1]),
        .I3(clk_cnt_reg__0[0]),
        .O(\clk_cnt[7]_i_2__0_n_0 ));
  FDCE \clk_cnt_reg[0] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(p_0_in__0[0]),
        .Q(clk_cnt_reg__0[0]));
  FDCE \clk_cnt_reg[1] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(p_0_in__0[1]),
        .Q(clk_cnt_reg__0[1]));
  FDCE \clk_cnt_reg[2] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(p_0_in__0[2]),
        .Q(clk_cnt_reg__0[2]));
  FDCE \clk_cnt_reg[3] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(p_0_in__0[3]),
        .Q(clk_cnt_reg__0[3]));
  FDCE \clk_cnt_reg[4] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(p_0_in__0[4]),
        .Q(clk_cnt_reg__0[4]));
  FDCE \clk_cnt_reg[5] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(p_0_in__0[5]),
        .Q(clk_cnt_reg__0[5]));
  FDCE \clk_cnt_reg[6] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(p_0_in__0[6]),
        .Q(clk_cnt_reg__0[6]));
  FDCE \clk_cnt_reg[7] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(p_0_in__0[7]),
        .Q(clk_cnt_reg__0[7]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \command[0]_i_1 
       (.I0(uart_rec),
        .I1(Q[0]),
        .O(\command_reg[7] [0]));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \command[1]_i_1 
       (.I0(uart_rec),
        .I1(Q[1]),
        .O(\command_reg[7] [1]));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \command[2]_i_1 
       (.I0(uart_rec),
        .I1(Q[2]),
        .O(\command_reg[7] [2]));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \command[3]_i_1 
       (.I0(uart_rec),
        .I1(Q[3]),
        .O(\command_reg[7] [3]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \command[4]_i_1 
       (.I0(uart_rec),
        .I1(Q[4]),
        .O(\command_reg[7] [4]));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \command[5]_i_1 
       (.I0(uart_rec),
        .I1(Q[5]),
        .O(\command_reg[7] [5]));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \command[6]_i_1 
       (.I0(uart_rec),
        .I1(Q[6]),
        .O(\command_reg[7] [6]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \command[7]_i_1 
       (.I0(data_cmd_reg_0),
        .I1(uart_rec),
        .I2(command_done_reg),
        .O(\command_reg[0] ));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \command[7]_i_2 
       (.I0(uart_rec),
        .I1(Q[7]),
        .O(\command_reg[7] [7]));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT4 #(
    .INIT(16'h0800)) 
    \command_temp[7]_i_1 
       (.I0(uart_rec),
        .I1(keyin),
        .I2(data_cmd_reg_0),
        .I3(data_cmd0__6),
        .O(E));
  LUT5 #(
    .INIT(32'hFFFFFFFE)) 
    \command_temp[7]_i_2 
       (.I0(D[7]),
        .I1(D[6]),
        .I2(D[4]),
        .I3(D[5]),
        .I4(\command_temp[7]_i_3_n_0 ),
        .O(data_cmd0__6));
  LUT4 #(
    .INIT(16'hFFFE)) 
    \command_temp[7]_i_3 
       (.I0(D[1]),
        .I1(D[0]),
        .I2(D[3]),
        .I3(D[2]),
        .O(\command_temp[7]_i_3_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \data[7]_i_1 
       (.I0(uart_rec),
        .I1(data_cmd_reg_0),
        .O(\data_reg[0] ));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT4 #(
    .INIT(16'h7870)) 
    data_cmd_i_1
       (.I0(keyin),
        .I1(uart_rec),
        .I2(data_cmd_reg_0),
        .I3(data_cmd0__6),
        .O(data_cmd_reg));
  LUT6 #(
    .INIT(64'hF7FFF7F700FF0000)) 
    rx_flag_i_1
       (.I0(rx_flag_i_2_n_0),
        .I1(rx_flag_i_3_n_0),
        .I2(rx_flag_i_4_n_0),
        .I3(uart_rxd_last1),
        .I4(p_7_in),
        .I5(rx_flag),
        .O(rx_flag_i_1_n_0));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT4 #(
    .INIT(16'h0800)) 
    rx_flag_i_2
       (.I0(clk_cnt_reg__0[3]),
        .I1(clk_cnt_reg__0[2]),
        .I2(clk_cnt_reg__0[4]),
        .I3(clk_cnt_reg__0[6]),
        .O(rx_flag_i_2_n_0));
  (* SOFT_HLUTNM = "soft_lutpair11" *) 
  LUT2 #(
    .INIT(4'h2)) 
    rx_flag_i_3
       (.I0(clk_cnt_reg__0[7]),
        .I1(clk_cnt_reg__0[1]),
        .O(rx_flag_i_3_n_0));
  (* SOFT_HLUTNM = "soft_lutpair10" *) 
  LUT2 #(
    .INIT(4'h7)) 
    rx_flag_i_4
       (.I0(clk_cnt_reg__0[0]),
        .I1(clk_cnt_reg__0[5]),
        .O(rx_flag_i_4_n_0));
  FDCE rx_flag_reg
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(rx_flag_i_1_n_0),
        .Q(rx_flag));
  LUT6 #(
    .INIT(64'hFFFFFFFB00000008)) 
    \uart_data_out[0]_i_1 
       (.I0(p_7_in),
        .I1(\uart_data_out[0]_i_2_n_0 ),
        .I2(clk_cnt_reg__0[1]),
        .I3(clk_cnt_reg__0[4]),
        .I4(clk_cnt_reg__0[7]),
        .I5(D[0]),
        .O(\uart_data_out[0]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000008000000000)) 
    \uart_data_out[0]_i_2 
       (.I0(clk_cnt_reg__0[0]),
        .I1(clk_cnt_reg__0[5]),
        .I2(clk_cnt_reg__0[2]),
        .I3(clk_cnt_reg__0[3]),
        .I4(clk_cnt_reg__0[6]),
        .I5(rx_flag),
        .O(\uart_data_out[0]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hFBFFFFFF08000000)) 
    \uart_data_out[1]_i_1 
       (.I0(p_7_in),
        .I1(\uart_data_out[1]_i_2_n_0 ),
        .I2(clk_cnt_reg__0[0]),
        .I3(clk_cnt_reg__0[5]),
        .I4(clk_cnt_reg__0[4]),
        .I5(D[1]),
        .O(\uart_data_out[1]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000200000000000)) 
    \uart_data_out[1]_i_2 
       (.I0(clk_cnt_reg__0[3]),
        .I1(clk_cnt_reg__0[7]),
        .I2(clk_cnt_reg__0[1]),
        .I3(clk_cnt_reg__0[2]),
        .I4(clk_cnt_reg__0[6]),
        .I5(rx_flag),
        .O(\uart_data_out[1]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hFFEFFFFF00200000)) 
    \uart_data_out[2]_i_1 
       (.I0(p_7_in),
        .I1(\uart_data_out[2]_i_2_n_0 ),
        .I2(\uart_data_out[2]_i_3_n_0 ),
        .I3(clk_cnt_reg__0[7]),
        .I4(\uart_data_out[2]_i_4_n_0 ),
        .I5(D[2]),
        .O(\uart_data_out[2]_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair11" *) 
  LUT2 #(
    .INIT(4'h7)) 
    \uart_data_out[2]_i_2 
       (.I0(clk_cnt_reg__0[1]),
        .I1(clk_cnt_reg__0[0]),
        .O(\uart_data_out[2]_i_2_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT4 #(
    .INIT(16'h0800)) 
    \uart_data_out[2]_i_3 
       (.I0(clk_cnt_reg__0[6]),
        .I1(rx_flag),
        .I2(clk_cnt_reg__0[5]),
        .I3(clk_cnt_reg__0[4]),
        .O(\uart_data_out[2]_i_3_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT2 #(
    .INIT(4'h2)) 
    \uart_data_out[2]_i_4 
       (.I0(clk_cnt_reg__0[2]),
        .I1(clk_cnt_reg__0[3]),
        .O(\uart_data_out[2]_i_4_n_0 ));
  LUT6 #(
    .INIT(64'hFBFFFFFF08000000)) 
    \uart_data_out[3]_i_1 
       (.I0(p_7_in),
        .I1(\uart_data_out[3]_i_2_n_0 ),
        .I2(clk_cnt_reg__0[0]),
        .I3(clk_cnt_reg__0[5]),
        .I4(clk_cnt_reg__0[4]),
        .I5(D[3]),
        .O(\uart_data_out[3]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0001000000000000)) 
    \uart_data_out[3]_i_2 
       (.I0(clk_cnt_reg__0[3]),
        .I1(clk_cnt_reg__0[2]),
        .I2(clk_cnt_reg__0[1]),
        .I3(clk_cnt_reg__0[7]),
        .I4(clk_cnt_reg__0[6]),
        .I5(rx_flag),
        .O(\uart_data_out[3]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hFFFFBFFF00008000)) 
    \uart_data_out[4]_i_1 
       (.I0(p_7_in),
        .I1(\uart_data_out[4]_i_2_n_0 ),
        .I2(clk_cnt_reg__0[0]),
        .I3(clk_cnt_reg__0[3]),
        .I4(clk_cnt_reg__0[2]),
        .I5(D[4]),
        .O(\uart_data_out[4]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000001000000000)) 
    \uart_data_out[4]_i_2 
       (.I0(clk_cnt_reg__0[4]),
        .I1(clk_cnt_reg__0[5]),
        .I2(clk_cnt_reg__0[7]),
        .I3(clk_cnt_reg__0[1]),
        .I4(clk_cnt_reg__0[6]),
        .I5(rx_flag),
        .O(\uart_data_out[4]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hFFFFBFFF00008000)) 
    \uart_data_out[5]_i_1 
       (.I0(p_7_in),
        .I1(\uart_data_out[5]_i_2_n_0 ),
        .I2(clk_cnt_reg__0[1]),
        .I3(clk_cnt_reg__0[7]),
        .I4(clk_cnt_reg__0[4]),
        .I5(D[5]),
        .O(\uart_data_out[5]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000010000000000)) 
    \uart_data_out[5]_i_2 
       (.I0(clk_cnt_reg__0[3]),
        .I1(clk_cnt_reg__0[2]),
        .I2(clk_cnt_reg__0[0]),
        .I3(clk_cnt_reg__0[5]),
        .I4(clk_cnt_reg__0[6]),
        .I5(rx_flag),
        .O(\uart_data_out[5]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hFBFFFFFF08000000)) 
    \uart_data_out[6]_i_1 
       (.I0(p_7_in),
        .I1(\uart_data_out[6]_i_2_n_0 ),
        .I2(clk_cnt_reg__0[2]),
        .I3(clk_cnt_reg__0[3]),
        .I4(clk_cnt_reg__0[7]),
        .I5(D[6]),
        .O(\uart_data_out[6]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000800000000000)) 
    \uart_data_out[6]_i_2 
       (.I0(clk_cnt_reg__0[5]),
        .I1(clk_cnt_reg__0[4]),
        .I2(clk_cnt_reg__0[1]),
        .I3(clk_cnt_reg__0[0]),
        .I4(clk_cnt_reg__0[6]),
        .I5(rx_flag),
        .O(\uart_data_out[6]_i_2_n_0 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \uart_data_out[7]_i_1 
       (.I0(p_7_in),
        .I1(uart_rec0_out),
        .I2(D[7]),
        .O(\uart_data_out[7]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'h0000000000200000)) 
    \uart_data_out[7]_i_2 
       (.I0(clk_cnt_reg__0[7]),
        .I1(clk_cnt_reg__0[1]),
        .I2(\uart_data_out[2]_i_3_n_0 ),
        .I3(clk_cnt_reg__0[0]),
        .I4(clk_cnt_reg__0[2]),
        .I5(clk_cnt_reg__0[3]),
        .O(uart_rec0_out));
  FDCE \uart_data_out_reg[0] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(\uart_data_out[0]_i_1_n_0 ),
        .Q(D[0]));
  FDCE \uart_data_out_reg[1] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(\uart_data_out[1]_i_1_n_0 ),
        .Q(D[1]));
  FDCE \uart_data_out_reg[2] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(\uart_data_out[2]_i_1_n_0 ),
        .Q(D[2]));
  FDCE \uart_data_out_reg[3] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(\uart_data_out[3]_i_1_n_0 ),
        .Q(D[3]));
  FDCE \uart_data_out_reg[4] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(\uart_data_out[4]_i_1_n_0 ),
        .Q(D[4]));
  FDCE \uart_data_out_reg[5] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(\uart_data_out[5]_i_1_n_0 ),
        .Q(D[5]));
  FDCE \uart_data_out_reg[6] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(\uart_data_out[6]_i_1_n_0 ),
        .Q(D[6]));
  FDCE \uart_data_out_reg[7] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\keyin[3] ),
        .D(\uart_data_out[7]_i_1_n_0 ),
        .Q(D[7]));
  LUT5 #(
    .INIT(32'hBBBF8880)) 
    uart_rec_i_1
       (.I0(uart_rec0_out),
        .I1(keyin),
        .I2(uart_rec_i_2_n_0),
        .I3(uart_rec_i_3_n_0),
        .I4(uart_rec),
        .O(uart_rec_i_1_n_0));
  LUT6 #(
    .INIT(64'hFFFFFFFFAAAAFEAB)) 
    uart_rec_i_2
       (.I0(uart_rec_i_4_n_0),
        .I1(clk_cnt_reg__0[3]),
        .I2(clk_cnt_reg__0[2]),
        .I3(clk_cnt_reg__0[4]),
        .I4(clk_cnt_reg__0[1]),
        .I5(uart_rec_i_5_n_0),
        .O(uart_rec_i_2_n_0));
  LUT6 #(
    .INIT(64'hFFFFFFFF0FC40C4C)) 
    uart_rec_i_3
       (.I0(clk_cnt_reg__0[0]),
        .I1(clk_cnt_reg__0[2]),
        .I2(clk_cnt_reg__0[4]),
        .I3(clk_cnt_reg__0[3]),
        .I4(clk_cnt_reg__0[5]),
        .I5(uart_rec_i_6_n_0),
        .O(uart_rec_i_3_n_0));
  LUT6 #(
    .INIT(64'hFF00FF00FF4545FF)) 
    uart_rec_i_4
       (.I0(clk_cnt_reg__0[3]),
        .I1(clk_cnt_reg__0[0]),
        .I2(clk_cnt_reg__0[5]),
        .I3(clk_cnt_reg__0[7]),
        .I4(clk_cnt_reg__0[6]),
        .I5(clk_cnt_reg__0[2]),
        .O(uart_rec_i_4_n_0));
  LUT6 #(
    .INIT(64'hFF7F5D5D55555D5D)) 
    uart_rec_i_5
       (.I0(rx_flag),
        .I1(clk_cnt_reg__0[4]),
        .I2(clk_cnt_reg__0[5]),
        .I3(clk_cnt_reg__0[0]),
        .I4(clk_cnt_reg__0[2]),
        .I5(clk_cnt_reg__0[3]),
        .O(uart_rec_i_5_n_0));
  LUT6 #(
    .INIT(64'hCFCCDFDD3F3CFC3C)) 
    uart_rec_i_6
       (.I0(clk_cnt_reg__0[0]),
        .I1(clk_cnt_reg__0[6]),
        .I2(clk_cnt_reg__0[4]),
        .I3(clk_cnt_reg__0[1]),
        .I4(clk_cnt_reg__0[2]),
        .I5(clk_cnt_reg__0[3]),
        .O(uart_rec_i_6_n_0));
  FDRE uart_rec_reg
       (.C(clk),
        .CE(1'b1),
        .D(uart_rec_i_1_n_0),
        .Q(uart_rec),
        .R(1'b0));
  FDPE uart_rxd_last1_reg
       (.C(clk),
        .CE(1'b1),
        .D(uart_rxd),
        .PRE(\keyin[3] ),
        .Q(uart_rxd_last1));
  FDPE uart_rxd_last2_reg
       (.C(clk),
        .CE(1'b1),
        .D(uart_rxd_last1),
        .PRE(\keyin[3] ),
        .Q(p_7_in));
endmodule

(* ORIG_REF_NAME = "uart_send_hs" *) 
module design_1_test_0_0_uart_send_hs
   (\clk_cnt_reg[1]_0 ,
    uart_txd,
    uart_send,
    clk,
    keyin,
    \uart_data_in_reg[7] );
  output \clk_cnt_reg[1]_0 ;
  output uart_txd;
  input uart_send;
  input clk;
  input [0:0]keyin;
  input [7:0]\uart_data_in_reg[7] ;

  wire clk;
  wire \clk_cnt[7]_i_2_n_0 ;
  wire \clk_cnt_reg[1]_0 ;
  wire [7:0]clk_cnt_reg__0;
  wire data1;
  wire data2;
  wire data3;
  wire data4;
  wire data5;
  wire data6;
  wire data7;
  wire [0:0]keyin;
  wire [7:0]p_0_in;
  wire \tx_data[7]_i_1_n_0 ;
  wire \tx_data_reg_n_0_[0] ;
  wire tx_flag_i_1_n_0;
  wire tx_flag_i_2_n_0;
  wire tx_flag_i_3_n_0;
  wire tx_flag_reg_n_0;
  wire [7:0]\uart_data_in_reg[7] ;
  wire uart_send;
  wire uart_send_last1;
  wire uart_send_last2;
  wire uart_txd;
  wire uart_txd_i_10_n_0;
  wire uart_txd_i_11_n_0;
  wire uart_txd_i_12_n_0;
  wire uart_txd_i_13_n_0;
  wire uart_txd_i_14_n_0;
  wire uart_txd_i_15_n_0;
  wire uart_txd_i_16_n_0;
  wire uart_txd_i_1_n_0;
  wire uart_txd_i_2_n_0;
  wire uart_txd_i_3_n_0;
  wire uart_txd_i_4_n_0;
  wire uart_txd_i_5_n_0;
  wire uart_txd_i_6_n_0;
  wire uart_txd_i_7_n_0;
  wire uart_txd_i_8_n_0;
  wire uart_txd_i_9_n_0;

  (* SOFT_HLUTNM = "soft_lutpair19" *) 
  LUT2 #(
    .INIT(4'h2)) 
    \clk_cnt[0]_i_1 
       (.I0(tx_flag_reg_n_0),
        .I1(clk_cnt_reg__0[0]),
        .O(p_0_in[0]));
  (* SOFT_HLUTNM = "soft_lutpair19" *) 
  LUT3 #(
    .INIT(8'h48)) 
    \clk_cnt[1]_i_1 
       (.I0(clk_cnt_reg__0[0]),
        .I1(tx_flag_reg_n_0),
        .I2(clk_cnt_reg__0[1]),
        .O(p_0_in[1]));
  (* SOFT_HLUTNM = "soft_lutpair15" *) 
  LUT4 #(
    .INIT(16'h7080)) 
    \clk_cnt[2]_i_1 
       (.I0(clk_cnt_reg__0[1]),
        .I1(clk_cnt_reg__0[0]),
        .I2(tx_flag_reg_n_0),
        .I3(clk_cnt_reg__0[2]),
        .O(p_0_in[2]));
  (* SOFT_HLUTNM = "soft_lutpair15" *) 
  LUT5 #(
    .INIT(32'h7F008000)) 
    \clk_cnt[3]_i_1__0 
       (.I0(clk_cnt_reg__0[0]),
        .I1(clk_cnt_reg__0[1]),
        .I2(clk_cnt_reg__0[2]),
        .I3(tx_flag_reg_n_0),
        .I4(clk_cnt_reg__0[3]),
        .O(p_0_in[3]));
  LUT6 #(
    .INIT(64'h7FFF000080000000)) 
    \clk_cnt[4]_i_1 
       (.I0(clk_cnt_reg__0[2]),
        .I1(clk_cnt_reg__0[1]),
        .I2(clk_cnt_reg__0[0]),
        .I3(clk_cnt_reg__0[3]),
        .I4(tx_flag_reg_n_0),
        .I5(clk_cnt_reg__0[4]),
        .O(p_0_in[4]));
  (* SOFT_HLUTNM = "soft_lutpair13" *) 
  LUT4 #(
    .INIT(16'hD020)) 
    \clk_cnt[5]_i_1__0 
       (.I0(clk_cnt_reg__0[4]),
        .I1(\clk_cnt[7]_i_2_n_0 ),
        .I2(tx_flag_reg_n_0),
        .I3(clk_cnt_reg__0[5]),
        .O(p_0_in[5]));
  (* SOFT_HLUTNM = "soft_lutpair13" *) 
  LUT5 #(
    .INIT(32'hDF002000)) 
    \clk_cnt[6]_i_1__0 
       (.I0(clk_cnt_reg__0[5]),
        .I1(\clk_cnt[7]_i_2_n_0 ),
        .I2(clk_cnt_reg__0[4]),
        .I3(tx_flag_reg_n_0),
        .I4(clk_cnt_reg__0[6]),
        .O(p_0_in[6]));
  LUT6 #(
    .INIT(64'hDFFF000020000000)) 
    \clk_cnt[7]_i_1__0 
       (.I0(clk_cnt_reg__0[4]),
        .I1(\clk_cnt[7]_i_2_n_0 ),
        .I2(clk_cnt_reg__0[5]),
        .I3(clk_cnt_reg__0[6]),
        .I4(tx_flag_reg_n_0),
        .I5(clk_cnt_reg__0[7]),
        .O(p_0_in[7]));
  (* SOFT_HLUTNM = "soft_lutpair17" *) 
  LUT4 #(
    .INIT(16'h7FFF)) 
    \clk_cnt[7]_i_2 
       (.I0(clk_cnt_reg__0[2]),
        .I1(clk_cnt_reg__0[1]),
        .I2(clk_cnt_reg__0[0]),
        .I3(clk_cnt_reg__0[3]),
        .O(\clk_cnt[7]_i_2_n_0 ));
  FDCE \clk_cnt_reg[0] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(p_0_in[0]),
        .Q(clk_cnt_reg__0[0]));
  FDCE \clk_cnt_reg[1] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(p_0_in[1]),
        .Q(clk_cnt_reg__0[1]));
  FDCE \clk_cnt_reg[2] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(p_0_in[2]),
        .Q(clk_cnt_reg__0[2]));
  FDCE \clk_cnt_reg[3] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(p_0_in[3]),
        .Q(clk_cnt_reg__0[3]));
  FDCE \clk_cnt_reg[4] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(p_0_in[4]),
        .Q(clk_cnt_reg__0[4]));
  FDCE \clk_cnt_reg[5] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(p_0_in[5]),
        .Q(clk_cnt_reg__0[5]));
  FDCE \clk_cnt_reg[6] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(p_0_in[6]),
        .Q(clk_cnt_reg__0[6]));
  FDCE \clk_cnt_reg[7] 
       (.C(clk),
        .CE(1'b1),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(p_0_in[7]),
        .Q(clk_cnt_reg__0[7]));
  LUT1 #(
    .INIT(2'h1)) 
    flg_i_2
       (.I0(keyin),
        .O(\clk_cnt_reg[1]_0 ));
  LUT2 #(
    .INIT(4'h2)) 
    \tx_data[7]_i_1 
       (.I0(uart_send_last2),
        .I1(uart_send_last1),
        .O(\tx_data[7]_i_1_n_0 ));
  FDCE \tx_data_reg[0] 
       (.C(clk),
        .CE(\tx_data[7]_i_1_n_0 ),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(\uart_data_in_reg[7] [0]),
        .Q(\tx_data_reg_n_0_[0] ));
  FDCE \tx_data_reg[1] 
       (.C(clk),
        .CE(\tx_data[7]_i_1_n_0 ),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(\uart_data_in_reg[7] [1]),
        .Q(data1));
  FDCE \tx_data_reg[2] 
       (.C(clk),
        .CE(\tx_data[7]_i_1_n_0 ),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(\uart_data_in_reg[7] [2]),
        .Q(data2));
  FDCE \tx_data_reg[3] 
       (.C(clk),
        .CE(\tx_data[7]_i_1_n_0 ),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(\uart_data_in_reg[7] [3]),
        .Q(data3));
  FDCE \tx_data_reg[4] 
       (.C(clk),
        .CE(\tx_data[7]_i_1_n_0 ),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(\uart_data_in_reg[7] [4]),
        .Q(data4));
  FDCE \tx_data_reg[5] 
       (.C(clk),
        .CE(\tx_data[7]_i_1_n_0 ),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(\uart_data_in_reg[7] [5]),
        .Q(data5));
  FDCE \tx_data_reg[6] 
       (.C(clk),
        .CE(\tx_data[7]_i_1_n_0 ),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(\uart_data_in_reg[7] [6]),
        .Q(data6));
  FDCE \tx_data_reg[7] 
       (.C(clk),
        .CE(\tx_data[7]_i_1_n_0 ),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(\uart_data_in_reg[7] [7]),
        .Q(data7));
  LUT6 #(
    .INIT(64'hFFFF7FFFFFFF0000)) 
    tx_flag_i_1
       (.I0(tx_flag_i_2_n_0),
        .I1(tx_flag_i_3_n_0),
        .I2(clk_cnt_reg__0[6]),
        .I3(clk_cnt_reg__0[5]),
        .I4(\tx_data[7]_i_1_n_0 ),
        .I5(tx_flag_reg_n_0),
        .O(tx_flag_i_1_n_0));
  (* SOFT_HLUTNM = "soft_lutpair17" *) 
  LUT4 #(
    .INIT(16'h4000)) 
    tx_flag_i_2
       (.I0(clk_cnt_reg__0[1]),
        .I1(clk_cnt_reg__0[0]),
        .I2(clk_cnt_reg__0[3]),
        .I3(clk_cnt_reg__0[2]),
        .O(tx_flag_i_2_n_0));
  (* SOFT_HLUTNM = "soft_lutpair12" *) 
  LUT2 #(
    .INIT(4'h2)) 
    tx_flag_i_3
       (.I0(clk_cnt_reg__0[7]),
        .I1(clk_cnt_reg__0[4]),
        .O(tx_flag_i_3_n_0));
  FDCE tx_flag_reg
       (.C(clk),
        .CE(1'b1),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(tx_flag_i_1_n_0),
        .Q(tx_flag_reg_n_0));
  FDCE uart_send_last1_reg
       (.C(clk),
        .CE(1'b1),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(uart_send),
        .Q(uart_send_last1));
  FDCE uart_send_last2_reg
       (.C(clk),
        .CE(1'b1),
        .CLR(\clk_cnt_reg[1]_0 ),
        .D(uart_send_last1),
        .Q(uart_send_last2));
  LUT6 #(
    .INIT(64'hEEEEEEEFEEEEEEE0)) 
    uart_txd_i_1
       (.I0(uart_txd_i_2_n_0),
        .I1(uart_txd_i_3_n_0),
        .I2(uart_txd_i_4_n_0),
        .I3(uart_txd_i_5_n_0),
        .I4(uart_txd_i_6_n_0),
        .I5(uart_txd),
        .O(uart_txd_i_1_n_0));
  LUT6 #(
    .INIT(64'h0000B800FFFFFFFF)) 
    uart_txd_i_10
       (.I0(data4),
        .I1(clk_cnt_reg__0[4]),
        .I2(data3),
        .I3(clk_cnt_reg__0[2]),
        .I4(clk_cnt_reg__0[7]),
        .I5(tx_flag_reg_n_0),
        .O(uart_txd_i_10_n_0));
  LUT4 #(
    .INIT(16'h0008)) 
    uart_txd_i_11
       (.I0(clk_cnt_reg__0[3]),
        .I1(clk_cnt_reg__0[6]),
        .I2(clk_cnt_reg__0[5]),
        .I3(clk_cnt_reg__0[4]),
        .O(uart_txd_i_11_n_0));
  (* SOFT_HLUTNM = "soft_lutpair14" *) 
  LUT5 #(
    .INIT(32'h00020000)) 
    uart_txd_i_12
       (.I0(clk_cnt_reg__0[6]),
        .I1(clk_cnt_reg__0[1]),
        .I2(clk_cnt_reg__0[2]),
        .I3(clk_cnt_reg__0[4]),
        .I4(clk_cnt_reg__0[7]),
        .O(uart_txd_i_12_n_0));
  (* SOFT_HLUTNM = "soft_lutpair18" *) 
  LUT3 #(
    .INIT(8'h24)) 
    uart_txd_i_13
       (.I0(clk_cnt_reg__0[2]),
        .I1(clk_cnt_reg__0[5]),
        .I2(clk_cnt_reg__0[7]),
        .O(uart_txd_i_13_n_0));
  (* SOFT_HLUTNM = "soft_lutpair16" *) 
  LUT5 #(
    .INIT(32'h00040000)) 
    uart_txd_i_14
       (.I0(clk_cnt_reg__0[6]),
        .I1(clk_cnt_reg__0[1]),
        .I2(clk_cnt_reg__0[3]),
        .I3(clk_cnt_reg__0[0]),
        .I4(clk_cnt_reg__0[4]),
        .O(uart_txd_i_14_n_0));
  (* SOFT_HLUTNM = "soft_lutpair18" *) 
  LUT4 #(
    .INIT(16'h4001)) 
    uart_txd_i_15
       (.I0(clk_cnt_reg__0[7]),
        .I1(clk_cnt_reg__0[2]),
        .I2(clk_cnt_reg__0[5]),
        .I3(clk_cnt_reg__0[6]),
        .O(uart_txd_i_15_n_0));
  (* SOFT_HLUTNM = "soft_lutpair16" *) 
  LUT4 #(
    .INIT(16'h0081)) 
    uart_txd_i_16
       (.I0(clk_cnt_reg__0[4]),
        .I1(clk_cnt_reg__0[3]),
        .I2(clk_cnt_reg__0[0]),
        .I3(clk_cnt_reg__0[1]),
        .O(uart_txd_i_16_n_0));
  LUT6 #(
    .INIT(64'hFFFFFFFFBAAAAAAA)) 
    uart_txd_i_2
       (.I0(uart_txd_i_7_n_0),
        .I1(clk_cnt_reg__0[4]),
        .I2(data2),
        .I3(clk_cnt_reg__0[1]),
        .I4(uart_txd_i_8_n_0),
        .I5(uart_txd_i_9_n_0),
        .O(uart_txd_i_2_n_0));
  (* SOFT_HLUTNM = "soft_lutpair12" *) 
  LUT5 #(
    .INIT(32'hAEAAAAAA)) 
    uart_txd_i_3
       (.I0(uart_txd_i_10_n_0),
        .I1(clk_cnt_reg__0[7]),
        .I2(clk_cnt_reg__0[4]),
        .I3(clk_cnt_reg__0[2]),
        .I4(data6),
        .O(uart_txd_i_3_n_0));
  LUT6 #(
    .INIT(64'h02000000FFFFFFFF)) 
    uart_txd_i_4
       (.I0(uart_txd_i_11_n_0),
        .I1(clk_cnt_reg__0[2]),
        .I2(clk_cnt_reg__0[7]),
        .I3(clk_cnt_reg__0[0]),
        .I4(clk_cnt_reg__0[1]),
        .I5(tx_flag_reg_n_0),
        .O(uart_txd_i_4_n_0));
  LUT6 #(
    .INIT(64'hFFFF240024002400)) 
    uart_txd_i_5
       (.I0(clk_cnt_reg__0[0]),
        .I1(clk_cnt_reg__0[3]),
        .I2(clk_cnt_reg__0[5]),
        .I3(uart_txd_i_12_n_0),
        .I4(uart_txd_i_13_n_0),
        .I5(uart_txd_i_14_n_0),
        .O(uart_txd_i_5_n_0));
  LUT6 #(
    .INIT(64'hFFFF100010001000)) 
    uart_txd_i_6
       (.I0(\clk_cnt[7]_i_2_n_0 ),
        .I1(clk_cnt_reg__0[6]),
        .I2(clk_cnt_reg__0[5]),
        .I3(tx_flag_i_3_n_0),
        .I4(uart_txd_i_15_n_0),
        .I5(uart_txd_i_16_n_0),
        .O(uart_txd_i_6_n_0));
  LUT6 #(
    .INIT(64'h808080808C8C8C80)) 
    uart_txd_i_7
       (.I0(data5),
        .I1(clk_cnt_reg__0[7]),
        .I2(clk_cnt_reg__0[4]),
        .I3(clk_cnt_reg__0[5]),
        .I4(data7),
        .I5(clk_cnt_reg__0[2]),
        .O(uart_txd_i_7_n_0));
  (* SOFT_HLUTNM = "soft_lutpair14" *) 
  LUT2 #(
    .INIT(4'h1)) 
    uart_txd_i_8
       (.I0(clk_cnt_reg__0[7]),
        .I1(clk_cnt_reg__0[2]),
        .O(uart_txd_i_8_n_0));
  LUT6 #(
    .INIT(64'h0000000000008A80)) 
    uart_txd_i_9
       (.I0(clk_cnt_reg__0[4]),
        .I1(data1),
        .I2(clk_cnt_reg__0[1]),
        .I3(\tx_data_reg_n_0_[0] ),
        .I4(clk_cnt_reg__0[2]),
        .I5(clk_cnt_reg__0[7]),
        .O(uart_txd_i_9_n_0));
  FDPE uart_txd_reg
       (.C(clk),
        .CE(1'b1),
        .D(uart_txd_i_1_n_0),
        .PRE(\clk_cnt_reg[1]_0 ),
        .Q(uart_txd));
endmodule
`ifndef GLBL
`define GLBL
`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;
    wire FCSBO_GLBL;
    wire [3:0] DO_GLBL;
    wire [3:0] DI_GLBL;
   
    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (strong1, weak0) GSR = GSR_int;
    assign (strong1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule
`endif

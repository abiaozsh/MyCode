//Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2018.1 (win64) Build 2188600 Wed Apr  4 18:40:38 MDT 2018
//Date        : Sat Apr 25 16:32:58 2020
//Host        : zsh-PC running 64-bit Service Pack 1  (build 7601)
//Command     : generate_target design_1.bd
//Design      : design_1
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "design_1,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=design_1,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=2,numReposBlks=2,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=1,numPkgbdBlks=0,bdsource=USER,\"da_clkrst_cnt\"=1,synth_mode=OOC_per_IP}" *) (* HW_HANDOFF = "design_1.hwdef" *) 
module design_1
   (DDR_0_addr,
    DDR_0_ba,
    DDR_0_cas_n,
    DDR_0_ck_n,
    DDR_0_ck_p,
    DDR_0_cke,
    DDR_0_cs_n,
    DDR_0_dm,
    DDR_0_dq,
    DDR_0_dqs_n,
    DDR_0_dqs_p,
    DDR_0_odt,
    DDR_0_ras_n,
    DDR_0_reset_n,
    DDR_0_we_n,
    FIXED_IO_0_ddr_vrn,
    FIXED_IO_0_ddr_vrp,
    FIXED_IO_0_mio,
    FIXED_IO_0_ps_clk,
    FIXED_IO_0_ps_porb,
    FIXED_IO_0_ps_srstb,
    debug,
    keyin,
    led,
    uart_rxd,
    uart_txd);
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 ADDR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR_0, AXI_ARBITRATION_SCHEME TDM, BURST_LENGTH 8, CAN_DEBUG false, CAS_LATENCY 11, CAS_WRITE_LATENCY 11, CS_ENABLED true, DATA_MASK_ENABLED true, DATA_WIDTH 8, MEMORY_TYPE COMPONENTS, MEM_ADDR_MAP ROW_COLUMN_BANK, SLOT Single, TIMEPERIOD_PS 1250" *) inout [14:0]DDR_0_addr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 BA" *) inout [2:0]DDR_0_ba;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 CAS_N" *) inout DDR_0_cas_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 CK_N" *) inout DDR_0_ck_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 CK_P" *) inout DDR_0_ck_p;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 CKE" *) inout DDR_0_cke;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 CS_N" *) inout DDR_0_cs_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 DM" *) inout [3:0]DDR_0_dm;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 DQ" *) inout [31:0]DDR_0_dq;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 DQS_N" *) inout [3:0]DDR_0_dqs_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 DQS_P" *) inout [3:0]DDR_0_dqs_p;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 ODT" *) inout DDR_0_odt;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 RAS_N" *) inout DDR_0_ras_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 RESET_N" *) inout DDR_0_reset_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 DDR_0 WE_N" *) inout DDR_0_we_n;
  (* X_INTERFACE_INFO = "xilinx.com:display_processing_system7:fixedio:1.0 FIXED_IO_0 DDR_VRN" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME FIXED_IO_0, CAN_DEBUG false" *) inout FIXED_IO_0_ddr_vrn;
  (* X_INTERFACE_INFO = "xilinx.com:display_processing_system7:fixedio:1.0 FIXED_IO_0 DDR_VRP" *) inout FIXED_IO_0_ddr_vrp;
  (* X_INTERFACE_INFO = "xilinx.com:display_processing_system7:fixedio:1.0 FIXED_IO_0 MIO" *) inout [53:0]FIXED_IO_0_mio;
  (* X_INTERFACE_INFO = "xilinx.com:display_processing_system7:fixedio:1.0 FIXED_IO_0 PS_CLK" *) inout FIXED_IO_0_ps_clk;
  (* X_INTERFACE_INFO = "xilinx.com:display_processing_system7:fixedio:1.0 FIXED_IO_0 PS_PORB" *) inout FIXED_IO_0_ps_porb;
  (* X_INTERFACE_INFO = "xilinx.com:display_processing_system7:fixedio:1.0 FIXED_IO_0 PS_SRSTB" *) inout FIXED_IO_0_ps_srstb;
  output [7:0]debug;
  input [3:0]keyin;
  output [3:0]led;
  input uart_rxd;
  output uart_txd;

  wire [3:0]keyin_0_1;
  wire [14:0]processing_system7_0_DDR_ADDR;
  wire [2:0]processing_system7_0_DDR_BA;
  wire processing_system7_0_DDR_CAS_N;
  wire processing_system7_0_DDR_CKE;
  wire processing_system7_0_DDR_CK_N;
  wire processing_system7_0_DDR_CK_P;
  wire processing_system7_0_DDR_CS_N;
  wire [3:0]processing_system7_0_DDR_DM;
  wire [31:0]processing_system7_0_DDR_DQ;
  wire [3:0]processing_system7_0_DDR_DQS_N;
  wire [3:0]processing_system7_0_DDR_DQS_P;
  wire processing_system7_0_DDR_ODT;
  wire processing_system7_0_DDR_RAS_N;
  wire processing_system7_0_DDR_RESET_N;
  wire processing_system7_0_DDR_WE_N;
  wire processing_system7_0_FCLK_CLK0;
  wire processing_system7_0_FIXED_IO_DDR_VRN;
  wire processing_system7_0_FIXED_IO_DDR_VRP;
  wire [53:0]processing_system7_0_FIXED_IO_MIO;
  wire processing_system7_0_FIXED_IO_PS_CLK;
  wire processing_system7_0_FIXED_IO_PS_PORB;
  wire processing_system7_0_FIXED_IO_PS_SRSTB;
  wire [31:0]test_0_M00_AXI_ARADDR;
  wire [1:0]test_0_M00_AXI_ARBURST;
  wire [3:0]test_0_M00_AXI_ARLEN;
  wire test_0_M00_AXI_ARREADY;
  wire [2:0]test_0_M00_AXI_ARSIZE;
  wire test_0_M00_AXI_ARVALID;
  wire [31:0]test_0_M00_AXI_AWADDR;
  wire [1:0]test_0_M00_AXI_AWBURST;
  wire [3:0]test_0_M00_AXI_AWLEN;
  wire test_0_M00_AXI_AWREADY;
  wire [2:0]test_0_M00_AXI_AWSIZE;
  wire test_0_M00_AXI_AWVALID;
  wire test_0_M00_AXI_BREADY;
  wire [1:0]test_0_M00_AXI_BRESP;
  wire test_0_M00_AXI_BVALID;
  wire [31:0]test_0_M00_AXI_RDATA;
  wire test_0_M00_AXI_RLAST;
  wire test_0_M00_AXI_RREADY;
  wire [1:0]test_0_M00_AXI_RRESP;
  wire test_0_M00_AXI_RVALID;
  wire [31:0]test_0_M00_AXI_WDATA;
  wire test_0_M00_AXI_WLAST;
  wire test_0_M00_AXI_WREADY;
  wire [3:0]test_0_M00_AXI_WSTRB;
  wire test_0_M00_AXI_WVALID;
  wire [7:0]test_0_debug;
  wire [3:0]test_0_led;
  wire test_0_uart_txd;
  wire uart_rxd;

  assign debug[7:0] = test_0_debug;
  assign keyin_0_1 = keyin[3:0];
  assign led[3:0] = test_0_led;
  assign uart_txd = test_0_uart_txd;
  design_1_processing_system7_0_0 processing_system7_0
       (.DDR_Addr(DDR_0_addr[14:0]),
        .DDR_BankAddr(DDR_0_ba[2:0]),
        .DDR_CAS_n(DDR_0_cas_n),
        .DDR_CKE(DDR_0_cke),
        .DDR_CS_n(DDR_0_cs_n),
        .DDR_Clk(DDR_0_ck_p),
        .DDR_Clk_n(DDR_0_ck_n),
        .DDR_DM(DDR_0_dm[3:0]),
        .DDR_DQ(DDR_0_dq[31:0]),
        .DDR_DQS(DDR_0_dqs_p[3:0]),
        .DDR_DQS_n(DDR_0_dqs_n[3:0]),
        .DDR_DRSTB(DDR_0_reset_n),
        .DDR_ODT(DDR_0_odt),
        .DDR_RAS_n(DDR_0_ras_n),
        .DDR_VRN(FIXED_IO_0_ddr_vrn),
        .DDR_VRP(FIXED_IO_0_ddr_vrp),
        .DDR_WEB(DDR_0_we_n),
        .FCLK_CLK0(processing_system7_0_FCLK_CLK0),
        .MIO(FIXED_IO_0_mio[53:0]),
        .PS_CLK(FIXED_IO_0_ps_clk),
        .PS_PORB(FIXED_IO_0_ps_porb),
        .PS_SRSTB(FIXED_IO_0_ps_srstb),
        .S_AXI_GP0_ACLK(processing_system7_0_FCLK_CLK0),
        .S_AXI_GP0_ARADDR(test_0_M00_AXI_ARADDR),
        .S_AXI_GP0_ARBURST(test_0_M00_AXI_ARBURST),
        .S_AXI_GP0_ARCACHE({1'b0,1'b0,1'b1,1'b1}),
        .S_AXI_GP0_ARID({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_GP0_ARLEN(test_0_M00_AXI_ARLEN),
        .S_AXI_GP0_ARLOCK({1'b0,1'b0}),
        .S_AXI_GP0_ARPROT({1'b0,1'b0,1'b0}),
        .S_AXI_GP0_ARQOS({1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_GP0_ARREADY(test_0_M00_AXI_ARREADY),
        .S_AXI_GP0_ARSIZE(test_0_M00_AXI_ARSIZE),
        .S_AXI_GP0_ARVALID(test_0_M00_AXI_ARVALID),
        .S_AXI_GP0_AWADDR(test_0_M00_AXI_AWADDR),
        .S_AXI_GP0_AWBURST(test_0_M00_AXI_AWBURST),
        .S_AXI_GP0_AWCACHE({1'b0,1'b0,1'b1,1'b1}),
        .S_AXI_GP0_AWID({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_GP0_AWLEN(test_0_M00_AXI_AWLEN),
        .S_AXI_GP0_AWLOCK({1'b0,1'b0}),
        .S_AXI_GP0_AWPROT({1'b0,1'b0,1'b0}),
        .S_AXI_GP0_AWQOS({1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_GP0_AWREADY(test_0_M00_AXI_AWREADY),
        .S_AXI_GP0_AWSIZE(test_0_M00_AXI_AWSIZE),
        .S_AXI_GP0_AWVALID(test_0_M00_AXI_AWVALID),
        .S_AXI_GP0_BREADY(test_0_M00_AXI_BREADY),
        .S_AXI_GP0_BRESP(test_0_M00_AXI_BRESP),
        .S_AXI_GP0_BVALID(test_0_M00_AXI_BVALID),
        .S_AXI_GP0_RDATA(test_0_M00_AXI_RDATA),
        .S_AXI_GP0_RLAST(test_0_M00_AXI_RLAST),
        .S_AXI_GP0_RREADY(test_0_M00_AXI_RREADY),
        .S_AXI_GP0_RRESP(test_0_M00_AXI_RRESP),
        .S_AXI_GP0_RVALID(test_0_M00_AXI_RVALID),
        .S_AXI_GP0_WDATA(test_0_M00_AXI_WDATA),
        .S_AXI_GP0_WID({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_GP0_WLAST(test_0_M00_AXI_WLAST),
        .S_AXI_GP0_WREADY(test_0_M00_AXI_WREADY),
        .S_AXI_GP0_WSTRB(test_0_M00_AXI_WSTRB),
        .S_AXI_GP0_WVALID(test_0_M00_AXI_WVALID));
  design_1_test_0_0 test_0
       (.M00_AXI_araddr(test_0_M00_AXI_ARADDR),
        .M00_AXI_arburst(test_0_M00_AXI_ARBURST),
        .M00_AXI_arlen(test_0_M00_AXI_ARLEN),
        .M00_AXI_arready(test_0_M00_AXI_ARREADY),
        .M00_AXI_arsize(test_0_M00_AXI_ARSIZE),
        .M00_AXI_arvalid(test_0_M00_AXI_ARVALID),
        .M00_AXI_awaddr(test_0_M00_AXI_AWADDR),
        .M00_AXI_awburst(test_0_M00_AXI_AWBURST),
        .M00_AXI_awlen(test_0_M00_AXI_AWLEN),
        .M00_AXI_awready(test_0_M00_AXI_AWREADY),
        .M00_AXI_awsize(test_0_M00_AXI_AWSIZE),
        .M00_AXI_awvalid(test_0_M00_AXI_AWVALID),
        .M00_AXI_bready(test_0_M00_AXI_BREADY),
        .M00_AXI_bresp(test_0_M00_AXI_BRESP),
        .M00_AXI_bvalid(test_0_M00_AXI_BVALID),
        .M00_AXI_rdata(test_0_M00_AXI_RDATA),
        .M00_AXI_rlast(test_0_M00_AXI_RLAST),
        .M00_AXI_rready(test_0_M00_AXI_RREADY),
        .M00_AXI_rresp(test_0_M00_AXI_RRESP),
        .M00_AXI_rvalid(test_0_M00_AXI_RVALID),
        .M00_AXI_wdata(test_0_M00_AXI_WDATA),
        .M00_AXI_wlast(test_0_M00_AXI_WLAST),
        .M00_AXI_wready(test_0_M00_AXI_WREADY),
        .M00_AXI_wstrb(test_0_M00_AXI_WSTRB),
        .M00_AXI_wvalid(test_0_M00_AXI_WVALID),
        .clk(processing_system7_0_FCLK_CLK0),
        .debug(test_0_debug),
        .keyin(keyin_0_1),
        .led(test_0_led),
        .uart_rxd(uart_rxd),
        .uart_txd(test_0_uart_txd));
endmodule

module emptyProj_Top
(
    input         sys_clk,
    
    inout  [15:0] mcb3_dram_dq,
    output [14:0] mcb3_dram_a,
    output [2:0]  mcb3_dram_ba,
    output        mcb3_dram_ras_n,
    output        mcb3_dram_cas_n,
    output        mcb3_dram_we_n,
    output        mcb3_dram_odt,
    output        mcb3_dram_reset_n,
    output        mcb3_dram_cke,
    output        mcb3_dram_dm,
    inout         mcb3_dram_udqs,
    inout         mcb3_dram_udqs_n,
    inout         mcb3_rzq,
    inout         mcb3_zio,
    output        mcb3_dram_udm,
    inout         mcb3_dram_dqs,
    inout         mcb3_dram_dqs_n,
    output        mcb3_dram_ck,
    output        mcb3_dram_ck_n,
 
  input key0,//SW2
  inout key1,//SW3 + led

//×ó×ó
	 input A14,
	 input C13,
	 input B12,
	 input C11,
	 input B10,
	 input C9 ,
	 input B8 ,
	 input C7 ,
	 input B6 ,
	 input B5 ,
	 input E10,
	 input E11,
	 input F9 ,
	 input C8 ,
	 input E7 ,
	 input F7 ,
	 input D6 ,
	 input M7 ,
	 input N8 ,
	 input P9 ,
	 input T5 ,
	 input T6 ,
	 input N9 ,
	 input L8 ,
	 input L10,
	 input P12,
	 output R9Led,

//×óÖĞ
	 input B14,
	 input A13,
	 input A12,
	 input A11,
	 input A9 ,
	 input A8 ,
	 input A7 ,
	 input A6 ,
	 input A5 ,
	 input A4 ,
	 input C10,
	 input F10,
	 input D9 ,
	 input D8 ,
	 input E6 ,
	 input C6 ,
	 input N6 ,
	 input P6 ,
	 input L7 ,
	 input T4 ,
	 input R5 ,
	 input T7 ,
	 input M9 ,
	 input M10,
	 input P11,
	 input M11,
	 input T9Led,

//ÓÒÖĞ
   input E12,
	 input B15,
	 input C15,
	 input D14,
	 input E15,
	 input F15,
	 input G11,
	 input F14,
	 input G16,
	 input H15,
	 input G12,
	 input H13,
	 input J14,
	 input J11,
	 input K14,
	 input K15,
	 input L16,
	 input K11,
	 input M15,
	 input N14,
	 input M13,
	 input L12,
	 input P15,
	 input R15,
	 input R14,
	 input T13,
	 input T12,

//ÓÒÓÒ
	 input E13,
	 input B16,
	 input C16,
	 input D16,
	 input E16,
	 input F16,
	 input F12,
	 input F13,
	 input G14,
	 input H16,
	 input H11,
	 input H14,
	 input J16,
	 input J12,
	 input J13,
	 input K16,
	 input L14,
	 input K12,
	 input M16,
	 input N16,
	 input M14,
	 input L13,
	 input P16,
	 input R16,
	 input T15,
	 input T14,
	 input R12
	 	
    );

wire reset_n = key0;
wire ledKey1 = flg;
assign key1 = ledKey1 ? 1'b0 : 1'bz;

assign R9Led = key1 == 0 ? 1'b0 : 1'bz;

reg [31:0] cnt;
reg       flg;
always @(posedge sys_clk or negedge reset_n) begin
  if (!reset_n) begin
     cnt <= 0;
    flg <= 0;
  end else begin
	cnt <= cnt+1'b1;
    if(cnt==50000000)begin
		cnt <= 0;
		flg<=~flg;
	 end

  end
end

endmodule


module flow_led(
  input sys_clk,
  //input sys_rst_n,


input R2 ,
input P2 ,
input N2 ,
input M2 ,
input J2 ,
input H2 ,
input F2 ,
input D2 ,

input C2 ,
input B2 ,
input A3 ,
input A4 ,
input C3 ,
input A5 ,
input A6 ,
input A7 ,

input A8 ,
input A9 ,
input A10,
input A13,
input A14,
input A15,
input A16,
input A17,

input A18,
input A19,
input A20,
//////////////////////////////

input R1 ,
input P1 ,
input N1 ,
input M1 ,
input J1 ,
input H1 ,
input F1 ,
input E1 ,

input C1 ,
input B1 ,
input B3 ,
input B4 ,
input C4 ,
input B5 ,
input B6 ,
input B7 ,

input B8 ,
input B9 ,
input B10,
input B13,
input B14,
input B15,
input B16,
input B17,

input B18,
input B19,
input B20,
//////////////////////////////////

input AB13,
input AB14,
input AB15,
input AB16,
input AB17,
input AB18,
input AB19,
input AB20,

input Y21 ,
input W21 ,
input V21 ,
input U21 ,
input R21 ,
input P21 ,
input N21 ,
input M21 ,

input L21 ,
input K21 ,
input J21 ,
input H21 ,
input F21 ,
input E21 ,
input D21 ,
input C21 ,

output B21 ,
output N19 ,
output M19 ,
/////////////////////////////////
input AA13,
input AA14,
input AA15,
input AA16,
input AA17,
input AA18,
input AA19,
input AA20,

input Y22 ,
input W22 ,
input V22 ,
input U22 ,
input R22 ,
input P22 ,
input N22 ,
input M22 ,

input L22 ,
input K22 ,
input J22 ,
input H22 ,
input F22 ,
input E22 ,
input D22 ,
input C22 ,

output B22 ,
output N20 ,
output M20 ,
///////////////////////////////////
  input key1,
  input key2,
  output reg led

); 
assign B21 = shift[0];
assign N19 = shift[1];
assign M19 = shift[2];
//assign H22 = shift[3];
assign B22 = shift[4];
assign N20 = shift[5];
assign M20 = shift[6];
//assign C22 = shift[7];



reg [7:0] shift;
reg [23:0] timer;
always @(posedge sys_clk or negedge key1) begin
  if (!key1) begin
    timer <= 0;
    shift <= 1;
  end else begin
    timer<=timer+1;
    if(timer==0)begin
      led = !led;
      shift<={shift[0],shift[7:1]};
    end
  end
end


endmodule




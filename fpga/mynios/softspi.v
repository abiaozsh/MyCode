module softspi (
    input  clk,                // clock.clk
    input  clk_50M,
    input  reset_n,               // reset.reset
    
    input   [29:0] avs_s0_address,     //    s0.address
    input          avs_s0_read,        //      .read
    input          avs_s0_write,       //      .write
    output  [31:0] avs_s0_readdata,    //      .readdata
    input   [31:0] avs_s0_writedata,   //      .writedata
    output         avs_s0_waitrequest, //      .waitrequest
    input   [3:0]  avs_s0_byteenable,    //      .readdata

    output [7:0]   debug8,
    
    input             MISO,
    output reg        MOSI,
    output reg        SCLK,
    output reg [2:0]  SS_n
    
);

  reg [1:0] clk_count;
  parameter DELAY = 3;
  reg [7:0] read_data;
  reg read_req_buff;
  reg read_ack;
  reg write_req_buff;
  reg write_ack;
  
  assign debug8[0]=read_req_buff;  // 0  00101100
  assign debug8[1]=read_ack;       // 0
  assign debug8[2]=write_req_buff; // 1
  assign debug8[3]=write_ack;      // 1
  
  reg [3:0] count;
  reg       state;
  always @ (posedge clk_50M or negedge reset_n) begin
    if (!reset_n) begin
      MOSI <= 1;
      SCLK <= 0;
      read_req_buff <= 0;
      read_ack <= 0;
      write_req_buff <= 0;
      write_ack <= 0;
      count <= 0;
      state <= 0;
      clk_count <= 0;
    end else begin
      read_req_buff <= read_req;
      write_req_buff <= write_req;
      
      
      if(read_req_buff && !read_ack)begin
        if(         state==0)begin
          MOSI <= 1;
          SCLK <= 1;
          clk_count <= clk_count+1'b1;
          if(clk_count==DELAY)begin
            state <= 1;
            clk_count<=0;
          end
        end else if(state==1)begin
          SCLK <= 0;
          clk_count <= clk_count+1'b1;
          if(clk_count==DELAY)begin
            state <= 0;
            clk_count<=0;
            count <= count + 1'b1;
            if(count == 7)begin
              count <= 0;
              read_ack <= 1;
            end
          end
          if(clk_count==0)begin
            read_data[count] <= MISO;
          end
        end
      end
      
      if(!read_req_buff && read_ack)begin
        read_ack <= 0;
      end

      if(write_req_buff && !write_ack)begin
        if(         state==0)begin
          SCLK <= 1;
          if(clk_count==0)begin
            MOSI <= write_data[count];
          end
          clk_count <= clk_count+1'b1;
          if(clk_count==DELAY)begin
            clk_count<=0;
            count <= count + 1'b1;
            state <= 1;
          end
        end else if(state==1)begin
          SCLK <= 0;
          clk_count <= clk_count+1'b1;
          if(clk_count==DELAY)begin
            clk_count<=0;
            if(count == 8)begin
              count <= 0;
              write_ack <= 1;
            end
            state <= 0;
          end
        end
      end

      if(!write_req_buff && write_ack)begin
        write_ack <= 0;
      end

    end
  end

  assign avs_s0_waitrequest = 1'b0;
  
  assign avs_s0_readdata = avs_s0_address == 0 ? {read_data_valid,read_data[0],read_data[1],read_data[2],read_data[3],read_data[4],read_data[5],read_data[6],read_data[7]} : 
                           avs_s0_address == 1 ? {write_data_done,8'b0} : 0;

  
  assign debug8[4]=read_req;        // 0  00101100
  assign debug8[5]=write_req;       // 1
  assign debug8[6]=read_data_valid; // 0
  assign debug8[7]=write_data_done; // 0

  reg       read_req;
  reg       write_req;
  reg       read_data_valid;
  reg [7:0] write_data;
  reg       write_data_done;
  reg read_ack_buff;
  reg write_ack_buff;
  reg read_ack_buff2;
  reg write_ack_buff2;
  always @ (posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      SS_n <= 3'b111;
      read_req <= 0;
      write_req <= 0;
      read_ack_buff <= 0;
      write_ack_buff <= 0;
      read_ack_buff2 <= 0;
      write_ack_buff2 <= 0;
      write_data_done <= 0;
      read_data_valid <= 0;
    end else begin
      read_ack_buff <= read_ack;
      write_ack_buff <= write_ack;
      read_ack_buff2 <= read_ack_buff;
      write_ack_buff2 <= write_ack_buff;
      
      
      if(avs_s0_read) begin
        if(avs_s0_address==0) begin
          read_data_valid <= 0;
        end
        if(avs_s0_address==1) begin
          write_data_done <= 0;
        end
      end

      if(avs_s0_write) begin
        if(avs_s0_address==0) begin
          read_req<=1;
        end
        if(avs_s0_address==1) begin
          write_data<={avs_s0_writedata[0],avs_s0_writedata[1],avs_s0_writedata[2],avs_s0_writedata[3],avs_s0_writedata[4],avs_s0_writedata[5],avs_s0_writedata[6],avs_s0_writedata[7]};
          write_req<=1;
        end
        if(avs_s0_address==2) begin
          SS_n<=avs_s0_writedata[2:0];
        end
      end

      if(!read_ack_buff2 && read_ack_buff)begin
        read_req<=0;
        read_data_valid <= 1;
      end
      
      
      if(!write_ack_buff2 && write_ack_buff)begin
        write_req<=0;
        write_data_done <= 1;
      end

    end
  end

endmodule

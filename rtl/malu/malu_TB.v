`timescale 1ns/1ps



module combination_uut(input [3:0]a, input [3:0] b, output wire [3:0] c);
   assign c = (a & b) ^ 4'b0110;
   endmodule

module malu_TB();

   reg clk;

   reg [3:0] a;
   reg [3:0] b;

   wire [3:0] c; // output


   initial clk = 0;
   always #5 clk = ~clk;


   combination_uut uut(.a(a), .b(b), .c(c));


   // Main loop
   initial begin
      a = 4'b0111; b = 4'b1111; #10;
      a = 4'b0100; b = 4'b1100; #10;
      a = 4'b0110; b = 4'b0100; #10;
      a = 4'b1001; b = 4'b0110; #10;
      $finish;
   end



   initial begin: TEST
      $dumpfile("malu.vcd");
      $dumpvars(0, malu_TB);

   end
endmodule

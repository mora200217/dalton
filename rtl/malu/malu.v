

module malu(
            input a,
            input b,
            input c,
);


   reg [7:0] values;
   reg [3:0] more_values;

   localparam STATE = 8'b0000000;


   always @(*) begin
      switch(STATE) begin
         case INIT:

           break;


           case READ:

             break

        end


   end



endmodule // malu

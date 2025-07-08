module quadrature_decoder_tb ();

localparam PPR = 960;
localparam CLK_T = 2;
localparam SIGNAL_T_H = CLK_T * 6;
// SIGNAL_T_H*2 time units (a period of the pulse signal) are equivalent to
// SIGNAL_T_H clock cycles
localparam UPDATE_RATE = SIGNAL_T_H*1;

reg clk = 0;
always #1 clk <= ~clk;

reg rst_n = 1;
reg a;
reg b;
wire [$clog2(PPR*4*2)-1:0] p_cnt;


quadrature_decoder #(
    .UPDATE_RATE(UPDATE_RATE),
    .PPR(PPR)
) quad_deco_inst0 (
    .clk(clk),
    .rst(~rst_n),
    .a(a),
    .b(b),
    .p_cnt(p_cnt)
);


// Generate inputs according to the selected direction
reg [1:0] sf = 0;
reg dir_test;
always #(SIGNAL_T_H) sf[0] <= ~sf[0];
always #(SIGNAL_T_H/2) begin
    sf[1] <= sf[0];

    if (dir_test) begin
        a <= sf[0];
        b <= sf[1];
    end
    else begin
        a <= sf[1];
        b <= sf[0];
    end

end

task send_reset;
    rst_n <= 1'b1;
    @(posedge clk);
    rst_n <= 1'b0;
    @(posedge clk);
    rst_n <= 1'b1;
endtask


initial begin
    $dumpfile("quadrature_decoder_tb.vcd"); $dumpvars();
    send_reset();
    dir_test <= 1'b1;
    #(CLK_T);
    #(SIGNAL_T_H*2*2);
    #(CLK_T);
    dir_test <= 1'b0;
    #(SIGNAL_T_H*2*2);
    #(SIGNAL_T_H*2*1)
    $finish();
end

endmodule


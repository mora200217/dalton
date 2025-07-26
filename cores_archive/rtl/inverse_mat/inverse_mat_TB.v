`timescale 1ns/1ps

module inverse_tb;

    reg clk = 0;
    always #5 clk = ~clk; // 100 MHz

    reg rst = 1;
    reg start = 0;

    reg signed [31:0] A11, A12, A13;
    reg signed [31:0] A21, A22, A23;
    reg signed [31:0] A31, A32, A33;

    wire done;
    wire signed [31:0] Inv11, Inv12, Inv13;
    wire signed [31:0] Inv21, Inv22, Inv23;
    wire signed [31:0] Inv31, Inv32, Inv33;

    inverse_mat uut (
        .clk(clk), .rst(rst), .start(start),
        .A11(A11), .A12(A12), .A13(A13),
        .A21(A21), .A22(A22), .A23(A23),
        .A31(A31), .A32(A32), .A33(A33),
        .done(done),
        .Inv11(Inv11), .Inv12(Inv12), .Inv13(Inv13),
        .Inv21(Inv21), .Inv22(Inv22), .Inv23(Inv23),
        .Inv31(Inv31), .Inv32(Inv32), .Inv33(Inv33)
    );

    initial begin
        $display("=== Inverse 3x3 Q16.16 FSM TB ===");
        $dumpfile("inverse3x3_tb.vcd");
        $dumpvars(0, inverse_tb);

        #10 rst = 0;

        // Test matrix:
        // | 1 0 0 |
        // | 0 1 0 |
        // | 0 0 1 |
        // Should invert to identity

        A11 = 32'sh0001_0000; // 1.0 in Q16.16
        A12 = 0;
        A13 = 0;

        A21 = 0;
        A22 = 32'sh0001_0000; // 1.0
        A23 = 0;

        A31 = 0;
        A32 = 0;
        A33 = 32'sh0001_0000; // 1.0

        start = 1;

        #10 start = 0;

        wait(done);

        $display("Inv11=%d Inv12=%d Inv13=%d", Inv11, Inv12, Inv13);
        $display("Inv21=%d Inv22=%d Inv23=%d", Inv21, Inv22, Inv23);
        $display("Inv31=%d Inv32=%d Inv33=%d", Inv31, Inv32, Inv33);

        $finish;
    end

endmodule

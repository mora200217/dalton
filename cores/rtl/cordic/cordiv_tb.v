`timescale 1ns/1ps

module cordic_tb;

  // DUT ports
  reg [31:0] angle;
  reg [1:0] opt;
  reg enable;
  reg reset;
  reg clk;

  wire signed [31:0] sin_out;
  wire signed [31:0] cos_out;
  wire done;

  // Instantiate DUT
  cordic uut (
    .angle(angle),
    .opt(opt),
    .enable(enable),
    .reset(reset),
    .clk(clk),
    .sin_out(sin_out),
    .cos_out(cos_out),
    .done(done)
  );

  // Clock generator: 100 MHz
  always #5 clk = ~clk;

  initial begin
    $display("=== CORDIC Q16.16 Testbench ===");
    $dumpfile("cordic_tb.vcd");
    $dumpvars(0, cordic_tb);

    clk = 0;
    reset = 1;
    enable = 0;
    angle = 0;
    opt = 2'b10; // both: cos and sin

    #20;
    reset = 0;

    // 45° ≈ pi/4 rad ≈ 0.785398 rad
    // In Q16.16: 0.785398 * 65536 ≈ 51472
    angle = 32'd102944;

    enable = 1;
    #10;

    wait (done == 1);
    enable = 0;

    #20;
    $display("Angle (deg): 45");
    $display("COS (float) = %f", $itor($signed(cos_out)) / 65536.0);
    $display("SIN (float) = %f", $itor($signed(sin_out)) / 65536.0);

    $finish;
  end

endmodule

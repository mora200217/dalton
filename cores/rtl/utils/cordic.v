// CORDIC - Coordinate transformation (Rotation Mode)
// Computes sin and cos for an input angle in fixed-point Q16.16

module cordic(
    // INPUTS =================
    input wire [31:0]   angle,   // Input angle in Q16.16, e.g. [-π, π]
    input wire [1:0]    opt,     // 00 = sin, 01 = cos, 10 = both
    input wire          enable, 
    input wire          reset, 
    input wire          clk,

    // OUTPUTS =================
    output reg signed [31:0] sin_out,  // Q16.16
    output reg signed [31:0] cos_out,  // Q16.16
    output reg               done 
);

    // Parameters
    parameter ITER = 16;
    parameter signed [31:0] K = 32'd39797; // ~0.607252935 * 65536 ≈ 39797

    // Internal registers
    reg signed [31:0] x, y, z;
    reg [4:0] i;

    // Arctan lookup table (Q16.16)
    reg signed [31:0] atan_table [0:15];

    initial begin
        atan_table[0]  = 32'd51472;  // atan(2^-0) = π/4
        atan_table[1]  = 32'd30385;  // atan(2^-1)
        atan_table[2]  = 32'd16055;  // atan(2^-2)
        atan_table[3]  = 32'd8145;   // atan(2^-3)
        atan_table[4]  = 32'd4090;   // atan(2^-4)
        atan_table[5]  = 32'd2045;
        atan_table[6]  = 32'd1023;
        atan_table[7]  = 32'd512;
        atan_table[8]  = 32'd256;
        atan_table[9]  = 32'd128;
        atan_table[10] = 32'd64;
        atan_table[11] = 32'd32;
        atan_table[12] = 32'd16;
        atan_table[13] = 32'd8;
        atan_table[14] = 32'd4;
        atan_table[15] = 32'd2;
    end

    // FSM
    always @(posedge clk or posedge reset) begin
        if (reset) begin
            x <= 0;
            y <= 0;
            z <= 0;
            i <= 0;
            done <= 0;
            sin_out <= 0;
            cos_out <= 0;
        end else if (enable) begin
            if (i == 0) begin
                x <= K;     // Pre-scale by K
                y <= 0;
                z <= angle;
                done <= 0;
                sin_out <= 0;
                cos_out <= 0;
                i <= i + 1;
            end else if (i <= ITER) begin
                if (z[31] == 0) begin // z >= 0
                    x <= x - (y >>> (i-1));
                    y <= y + (x >>> (i-1));
                    z <= z - atan_table[i-1];
                end else begin
                    x <= x + (y >>> (i-1));
                    y <= y - (x >>> (i-1));
                    z <= z + atan_table[i-1];
                end
                i <= i + 1;
            end else begin
                done <= 1'b1;
                case (opt)
                    2'b00: begin
                        sin_out <= y;  // sin only
                        cos_out <= x;
                    end
                    2'b01: begin
                        cos_out <= x;  // cos only
                        sin_out <= 0;
                    end
                    2'b10: begin
                        cos_out <= x;  // both
                        sin_out <= y;
                    end
                    default: begin
                        cos_out <= 0;
                        sin_out <= 0;
                    end
                endcase
            end
        end else begin
            done <= 1'b0;
        end
    end

endmodule

`timescale 1ns/1ps

module inverse_mat (
    input  wire clk,
    input  wire rst,
    input  wire start,

    input  wire signed [31:0] A11, A12, A13,
    input  wire signed [31:0] A21, A22, A23,
    input  wire signed [31:0] A31, A32, A33,

    output reg  done,

    output reg  signed [31:0] Inv11, Inv12, Inv13,
    output reg  signed [31:0] Inv21, Inv22, Inv23,
    output reg  signed [31:0] Inv31, Inv32, Inv33
);

    // === FSM States ===
    localparam IDLE      = 3'd0;
    localparam CALC_DET  = 3'd1;
    localparam CHECK_DET = 3'd2;
    localparam CALC_COF  = 3'd3;
    localparam DIVIDE    = 3'd4;
    localparam DONE      = 3'd5;

    reg [2:0] state, next_state;

    reg signed [63:0] det, inv_det;

    reg signed [63:0] C11, C12, C13;
    reg signed [63:0] C21, C22, C23;
    reg signed [63:0] C31, C32, C33;

    // === FSM ===
    always @(posedge clk or posedge rst) begin
        if (rst)
            state <= IDLE;
        else
            state <= next_state;
    end

    always @(*) begin
        case (state)
            IDLE:      next_state = start ? CALC_DET : IDLE;
            CALC_DET:  next_state = CHECK_DET;
            CHECK_DET: next_state = (det != 0) ? CALC_COF : IDLE;
            CALC_COF:  next_state = DIVIDE;
            DIVIDE:    next_state = DONE;
            DONE:      next_state = IDLE;
            default:   next_state = IDLE;
        endcase
    end

    always @(posedge clk) begin
        if (rst) begin
            done <= 0;
        end else begin
            case (state)
                IDLE: begin
                    done <= 0;
                end

                CALC_DET: begin
                    det <= 
                        (A11 * (A22 * A33 - A23 * A32)) -
                        (A12 * (A21 * A33 - A23 * A31)) +
                        (A13 * (A21 * A32 - A22 * A31));
                end

                CHECK_DET: begin
                    if (det == 0) done <= 1;  // Singular
                end

                CALC_COF: begin
                    C11 <= (A22 * A33 - A23 * A32);
                    C12 <= -(A21 * A33 - A23 * A31);
                    C13 <= (A21 * A32 - A22 * A31);
                    C21 <= -(A12 * A33 - A13 * A32);
                    C22 <= (A11 * A33 - A13 * A31);
                    C23 <= -(A11 * A32 - A12 * A31);
                    C31 <= (A12 * A23 - A13 * A22);
                    C32 <= -(A11 * A23 - A13 * A21);
                    C33 <= (A11 * A22 - A12 * A21);
                end

                DIVIDE: begin
                    inv_det <= (64'sh1_0000_0000 << 16) / det;  // Q32.32

                    Inv11 <= (C11 * inv_det) >>> 16;
                    Inv12 <= (C21 * inv_det) >>> 16;
                    Inv13 <= (C31 * inv_det) >>> 16;

                    Inv21 <= (C12 * inv_det) >>> 16;
                    Inv22 <= (C22 * inv_det) >>> 16;
                    Inv23 <= (C32 * inv_det) >>> 16;

                    Inv31 <= (C13 * inv_det) >>> 16;
                    Inv32 <= (C23 * inv_det) >>> 16;
                    Inv33 <= (C33 * inv_det) >>> 16;
                end

                DONE: begin
                    done <= 1;
                end
            endcase
        end
    end

endmodule

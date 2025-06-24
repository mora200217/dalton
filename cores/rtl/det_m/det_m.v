module det3x3 (
    input wire clk,
    input wire reset,
    input wire start,
    input wire [31:0] M [0:8],  // Matriz 3x3
    output reg [31:0] det,      // Determinante
    output reg busy,
    output reg done
);

    typedef enum logic [1:0] {
        IDLE = 2'b00,
        CALC = 2'b01,
        DONE = 2'b10
    } state_t;

    state_t state;

    reg signed [63:0] temp_det;
    reg signed [31:0] a, b, c, d, e, f, g, h, i;

    always @(posedge clk or posedge reset) begin
        if (reset) begin
            state <= IDLE;
            busy <= 0;
            done <= 0;
            det <= 0;
        end else begin
            case (state)
                IDLE: begin
                    done <= 0;
                    busy <= 0;
                    if (start) begin
                        busy <= 1;

                        a <= M[0]; b <= M[1]; c <= M[2];
                        d <= M[3]; e <= M[4]; f <= M[5];
                        g <= M[6]; h <= M[7]; i <= M[8];

                        state <= CALC;
                    end
                end

                CALC: begin
                    temp_det <= a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g);
                    state <= DONE;
                end

                DONE: begin
                    det <= temp_det[31:0]; // truncado, si hay overflow se pierde
                    busy <= 0;
                    done <= 1;
                    state <= IDLE;
                end
            endcase
        end
    end

endmodule

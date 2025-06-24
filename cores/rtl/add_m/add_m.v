module add_m (
    input wire clk,
    input wire reset,
    input wire start,
    input wire [31:0] A [0:8],  // Matriz A de 3x3
    input wire [31:0] B [0:8],  // Matriz B de 3x3
    output reg [31:0] C [0:8],  // Matriz C = A + B
    output reg busy,
    output reg done
);

    typedef enum logic [1:0] {
        IDLE = 2'b00,
        PROCESS = 2'b01,
        DONE = 2'b10
    } state_t;

    state_t state;
    integer i;

    always @(posedge clk or posedge reset) begin
        if (reset) begin
            state <= IDLE;
            busy <= 0;
            done <= 0;
            i <= 0;
        end else begin
            case (state)
                IDLE: begin
                    busy <= 0;
                    done <= 0;
                    if (start) begin
                        busy <= 1;
                        i <= 0;
                        state <= PROCESS;
                    end
                end

                PROCESS: begin
                    C[i] <= A[i] + B[i];
                    i <= i + 1;
                    if (i == 8) begin
                        state <= DONE;
                    end
                end

                DONE: begin
                    busy <= 0;
                    done <= 1;
                    state <= IDLE;
                end
            endcase
        end
    end

endmodule

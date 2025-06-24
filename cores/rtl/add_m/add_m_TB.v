`timescale 1ns / 1ps

module add_m_TB;

    reg clk;
    reg reset;
    reg start;
    reg [31:0] A [0:8];
    reg [31:0] B [0:8];
    wire [31:0] C [0:8];
    wire busy;
    wire done;

    // Instancia del módulo
    add_m uut (
        .clk(clk),
        .reset(reset),
        .start(start),
        .A(A),
        .B(B),
        .C(C),
        .busy(busy),
        .done(done)
    );

    // Generador de reloj
    always #5 clk = ~clk;

    initial begin
        // Inicialización
        clk = 0;
        reset = 1;
        start = 0;
        #10;

        reset = 0;

        // Cargar valores
        A[0] = 1; A[1] = 2; A[2] = 3;
        A[3] = 4; A[4] = 5; A[5] = 6;
        A[6] = 7; A[7] = 8; A[8] = 9;

        B[0] = 9; B[1] = 8; B[2] = 7;
        B[3] = 6; B[4] = 5; B[5] = 4;
        B[6] = 3; B[7] = 2; B[8] = 1;

        // Comenzar la suma
        start = 1;
        #10;
        start = 0;

        // Esperar a que done se active
        wait(done);

        // Mostrar resultados
        $display("Resultado de la suma:");
        $display("%d %d %d", C[0], C[1], C[2]);
        $display("%d %d %d", C[3], C[4], C[5]);
        $display("%d %d %d", C[6], C[7], C[8]);

        // Verificar que todos sean 10
        for (int i = 0; i < 9; i++) begin
            if (C[i] !== 10) begin
                $display("Error en la celda %0d: %0d", i, C[i]);
            end
        end

        $finish;
    end

endmodule

`timescale 1ns / 1ps

module tb_det3x3;

    reg clk;
    reg reset;
    reg start;
    reg [31:0] M [0:8];
    wire [31:0] det;
    wire busy;
    wire done;

    // Instanciar el módulo del determinante
    det3x3 uut (
        .clk(clk),
        .reset(reset),
        .start(start),
        .M(M),
        .det(det),
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

        // Cargar la matriz con determinante 10
        M[0] = -5; M[1] = -5; M[2] = -5;
        M[3] = -5; M[4] = -5; M[5] = -4;
        M[6] = -5; M[7] = -3; M[8] = -5;

        // Iniciar cálculo
        start = 1;
        #10;
        start = 0;

        // Esperar resultado
        wait(done);

        // Mostrar el determinante
        $display("Determinante calculado: %0d", det);

        // Verificar si es 10
        if (det !== 10) begin
            $display("ERROR: El determinante deberia ser 10 pero fue %0d", det);
        end else begin
            $display("OK: Determinante correcto.");
        end

        $finish;
    end

endmodule

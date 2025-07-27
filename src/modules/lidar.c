
#include "lidar.h" 

#include <stdio.h>
#include "irq.h"
#include <uart.h>


// extern volatile int index;
// extern volatile int distance;
// extern volatile int strength;
// extern volatile int en_pausa;
// extern char packet[PACKET_LEN];

volatile int index       = 0;
volatile int distance    = 0;
volatile int strength    = 0;
volatile int en_pausa    = 0;
char packet[PACKET_LEN]  = {0};


// ------------------ Lógica de recepción de datos ------------------
int recibir_paquete(void) {
    if (uart_txempty_read()) return 0;

    uart_rxtx_write(1);
    uart_rxtx_write(0);

    
    char b = uart_rxtx_read();


    if (index == 0 && b != 0x59) return 0;
    if (index == 1 && b != 0x59) { index = 0; return 0; }

    packet[index++] = b;

    if (index == PACKET_LEN) {
        index = 0;
        return 1;
    }

    return 0;
}

void procesar_distancia(void) {
    distance = (packet[3] << 8) | packet[2];
    strength = (packet[5] << 8) | packet[4];
}

void manejar_estado_por_distancia(void) {
    if (distance <= UMBRAL_CM) {
        if (!en_pausa) {
            printf("Distancia: %d cm | Señal: %d\n", distance, strength);
            printf("¡Estás en menos de 10 centímetros! Pausando...\n");
            en_pausa = 1;
        }
    } else {
        if (en_pausa) {
            printf("Distancia: %d cm | Señal: %d\n", distance, strength);
            printf("Distancia segura, reanudando...\n");
            en_pausa = 0;
        } else {
            printf("Distancia: %d cm | Señal: %d\n", distance, strength);
        }
    }
}

// ------------------ Lazo principal ------------------
void loop_principal(void) {
    while (1) {
        if (recibir_paquete()) {
            procesar_distancia();
            manejar_estado_por_distancia();
        }
    }
}


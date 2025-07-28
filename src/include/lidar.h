// sensor_distancia.h

#ifndef SENSOR_DISTANCIA_H
#define SENSOR_DISTANCIA_H

#include <stdint.h>

// ------------------ Constantes ------------------
#define PACKET_LEN 9
#define UMBRAL_CM 10

// ------------------ Variables globales ------------------
extern volatile int index;
extern volatile int distance;
extern volatile int strength;
extern volatile int en_pausa;
extern char packet[PACKET_LEN];

// ------------------ Declaración de funciones ------------------
int recibir_paquete(void);
void procesar_distancia(void);
void manejar_estado_por_distancia(void);
void loop_principal(void);
void iniciar_lidar(void);  

#endif // SENSOR_DISTANCIA_H

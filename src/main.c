
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <uart.h>
#include <i2c.h>

#include <math.h>

#include <generated/csr.h>
#include <generated/soc.h>

#include "fpgaio.h"
#include "utils.h"
#include "lidar.h"
#include "imu.h"

#include "kalman_filter.h"

KalmanState kf;
const int32_t dt_q16 = 13107; // 200 ms = 0.2 s



int main(void)
{
    fpgaio_enable_interrupts();
    uart_init();

    printf("Welcome to Dalton! \n");
    printf("Testing...");

    // Iniciar UART con configuración directa [ LIDAR ]

    uint8_t frame[9];
    imu_init();

    int16_t ox, oy, oz;
    imu_calibrate(&ox, &oy, &oz);
    imu_set_offsets(ox, oy, oz);

    int16_t ax, ay, az;
    int16_t gz;

    // iniciar_lidar();
kalman_init(&kf); 
    
    while (1)
    {
        my_wait(300);
        // IMU

        // Lecturas
        imu_read_acc(&ax, &ay, &az); // Aceleracion en xy
        gz = imu_read_gyro_z();

        printf("Acc: (%d, %d)  GyroZ: %d\n", ax, ay, gz);
        // LIDAR

        kalman_predict(&kf, ax, ay, gz, dt_q16); 
        kalman_print(&kf); 
        recibir_paquete(); 

        printf("Frame: ");
        for (int i = 0; i < 9; i++)
        {
            printf("%02X ", frame[i]);
        }
        printf("\n");
    }

    //    printf("...\n");
    // if (recibir_paquete()) {
    //     procesar_distancia();
    //     manejar_estado_por_distancia();
    // }

    return 0;
}
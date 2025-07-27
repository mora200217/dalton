#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <irq.h>
#include <uart.h>
#include <i2c.h>

#include "include/kalman_filter.h"
#include <math.h> 

#include <generated/csr.h>
#include <generated/soc.h>

// #include "irobot2_serial/irobot2.h"

uint8_t posx; 
uint8_t posy; 

uint8_t velx; 
uint8_t vely;  

uint8_t theta = 0; // Agulo polar 
KalmanXYThetaVO kf;

void my_wait(unsigned int ms)
{
    timer0_en_write(0);
    timer0_reload_read();
    timer0_load_write(CONFIG_CLOCK_FREQUENCY/1000*ms);
    timer0_en_write(1);
    timer0_update_value_write(1);
    while (timer0_value_read()) timer0_update_value_write(1);
}

int main(void)
{
#ifdef CONFIG_CPU_HAS_INTERRUPT
    irq_setmask(0);
    irq_setie(1);
#endif

    uart_init();

    /// Init commadn to irobot2 

    
    uart_write(7); 
    while(1){
    my_wait(1000); 

    uart_write(128);  // start 
    my_wait(1000)  ; 

 
    uart_write(131); 
    my_wait(1000)  ; 
    
    uart_write(137);
	uart_write(0);
	uart_write(20);
	uart_write(0);
	uart_write(20);

    my_wait(1000)  ; 
    }

    
    // uart_write(DRIVE);
    // uart_write(vel_high);
    // uart_write(vel_low);
    // uart_write(rad_high);
    // uart_write(rad_low);
    
    
    printf("Welcome to Dalton!\n");
    
    while(1); 




    // if(!cordic_done_read())
    //     printf("Cordic not ended! \n"); 

     
    // cordic_opt_write(0x02);
    
    // cordic_angle_write(23242);  // 75º in Q15
    // cordic_enable_write(1);  // init cordic
    // while(!cordic_done_read()){
    //      printf("calculando..."); 
    //  }


    // uint32_t cos_fixed = cordic_cos_out_read(); 
    // uint32_t sin_fixed = cordic_sin_out_read();  
    

    // // printf("cos:  %ld ", sin); 
    // // printf("sin:  %ld ", sin); 
 
    // // Aproximar valor real sin float:
    // int32_t cos_int = cos_fixed >> 16;
    // uint32_t cos_frac = ((cos_fixed & 0xFFFF) * 1000) >> 16;

    // int32_t sin_int = sin_fixed >> 16;
    // uint32_t sin_frac = ((sin_fixed & 0xFFFF) * 1000) >> 16;

    // printf("cos ≈ %ld.%03u\n", cos_int, cos_frac);
    // printf("sin ≈ %ld.%03u\n", sin_int, sin_frac);
    
    // // while(1){}
    // // ==== temp 

    // unsigned char data = 0x11;
    // i2c_write(0x69, 0x7E, &data, 1, 1); // Activación acc 
    // printf("IMU activation command sent\n");

    // data = 0x15;
    // i2c_write(0x69, 0x7E, &data, 1, 1); // Activación gyro 

    // printf("IMU activation      command sent\n"); 

    // uint8_t data_in[6];
    // uint8_t gyro_z[2]; 

    // int16_t raw_x, raw_y, raw_z;
    // int32_t acc_x, acc_y;

    // int64_t sum_x = 0, sum_y = 0, sum_z = 0;
    // int16_t offset_x = 0, offset_y = 0, offset_z = 0;


    // printf("Calibrating for 2 seconds. Keep IMU still...\n");

    // // ~2 seconds: 100 samples @ 20ms
    // const int samples = 100;

    // for (int i = 0; i < samples; i++) {
    //     i2c_read(0x69, 0x12, data_in, 6, true, 1);

    //     raw_x = (int16_t)((data_in[1] << 8) | data_in[0]);
    //     raw_y = (int16_t)((data_in[3] << 8) | data_in[2]);
    //     raw_z = (int16_t)((data_in[5] << 8) | data_in[4]);

    //     sum_x += raw_x;
    //     sum_y += raw_y;
    //     sum_z += raw_z;

    //     my_wait(20);
    // }

    // offset_x = sum_x / samples;
    // offset_y = sum_y / samples;
    // offset_z = (sum_z / samples) - 16384;  // subtract +1g for Z if lying flat

    // printf("Calibration done. Offsets:\n");
    // printf("offset_x = %d, offset_y = %d, offset_z = %d\n", offset_x, offset_y, offset_z);

    // printf("Starting loop...\n");

    // kalman_init(&kf);

    //     int32_t dt = 0.01 * Q16_ONE;
    //     // int32_t acc_x = 0.1 * Q16_ONE;
    //     // int32_t gyro_z = 0.01 * Q16_ONE;
    //     int32_t v_enc = 1.0 * Q16_ONE;
    //     int32_t omega_enc = 0.01 * Q16_ONE;



    // while (1) {
    //     i2c_read(0x69, 0x12, data_in, 6, true, 1);

    //     raw_x = (int16_t)((data_in[1] << 8) | data_in[0]);
    //     raw_y = (int16_t)((data_in[3] << 8) | data_in[2]);
    //     raw_z = (int16_t)((data_in[5] << 8) | data_in[4]);

    //     // Apply offset
    //     raw_x -= offset_x;
    //     raw_y -= offset_y;
    //     raw_z -= offset_z;

    //     // Scale to mm/s², fixed point
    //     acc_x = (raw_x * 9807) / 16384;
    //     acc_y = (raw_y * 9807) / 16384;
    //     // acc_z = (raw_z * 9807) / 16384;

    //     printf("acc = ( %ld, %ld) mm/s^2 \t", acc_x, acc_y);
    //     // Gyro

    //     i2c_read(0x69, 0x10, gyro_z, 2, true, 1);         
    //     uint32_t gyro_raw = (int16_t)((gyro_z[1] << 8) | gyro_z[0]); 

    //     printf("\t gyro = %ld \n", gyro_raw);


    //     // ------- prueba de Filtro de Kalman ------

        

    // kalman_predict(&kf, acc_x, *gyro_z, dt);
    // kalman_update(&kf, v_enc, omega_enc, *gyro_z);
    
    // printf("estimacion con filtro de kalman: "); 
    // printf("(%ld, %ld, %ld) \n" , kf.x, kf.y, kf.theta); 






   // ------- prueba de Filtro de Kalman ------
// Variables de estado Q16.16
// static int32_t pos_x = 0;   // posición X en metros Q16.16
// static int32_t pos_y = 0;   // posición Y en metros Q16.16
// static int32_t vel_x = 0;   // velocidad X en m/s Q16.16
// static int32_t vel_y = 0;   // velocidad Y en m/s Q16.16
// static int32_t theta = 0;   // ángulo en radianes Q16.16

// // dt en Q16.16: 200 ms = 0.2 s => 0.2 * 65536 = 13107
// const int32_t dt = 13107;

// // Gyro: suponer gyro_raw en deg/s --> rad/s: rad = deg * pi/180
// // Aproxima pi/180 en Q16.16: pi/180 ≈ 0.017453 * 65536 ≈ 1147
int32_t gyro_z_q16 = gyro_raw * 1147; // Q16.16

// // Integrar giro: theta += gyro_z_rad * dt >> 16
// theta += ( ( (int64_t)gyro_z_q16 * dt ) >> 16 );

// // Aceleración acc_x, acc_y en mm/s² --> m/s²: divide entre 1000
// // acc_x Q16.16 = acc_x * 65536 / 1000
// int32_t acc_x_q16 = (acc_x * 65536) / 1000;
// int32_t acc_y_q16 = (acc_y * 65536) / 1000;

// // Integrar aceleración para velocidad: vel += a*dt
// vel_x += ( ( (int64_t)acc_x_q16 * dt ) >> 16 );
// vel_y += ( ( (int64_t)acc_y_q16 * dt ) >> 16 );

// // Integrar velocidad para posición: pos += v*dt
// pos_x += ( ( (int64_t)vel_x * dt ) >> 16 );
// pos_y += ( ( (int64_t)vel_y * dt ) >> 16 );

// // Limitar plano para dibujar en caracteres
// int draw_x = pos_x >> 16; // parte entera
// int draw_y = pos_y >> 16;

// if (draw_x < 0) draw_x = 0;
// if (draw_x > 40) draw_x = 40;
// if (draw_y < 0) draw_y = 0;
// if (draw_y > 10) draw_y = 10;

// // Dibuja plano cartesiano simple
// printf("\033[2J"); // Limpia pantalla
// for (int y = 10; y >= 0; y--) {
//     for (int x = 0; x <= 40; x++) {
//         if (x == draw_x && y == draw_y) {
//             printf("O");
//         } else if (y == 0 && x == 0) {
//             printf("+");
//         } else if (y == 0) {
//             printf("-");
//         } else if (x == 0) {
//             printf("|");
//         } else {
//             printf(" ");
//         }
//     }
//     printf("\n");
// }

// // Muestra valores Q16.16 como int.frac
// int32_t pos_x_int = pos_x >> 16;
// uint32_t pos_x_frac = ((pos_x & 0xFFFF) * 1000) >> 16;

// int32_t pos_y_int = pos_y >> 16;
// uint32_t pos_y_frac = ((pos_y & 0xFFFF) * 1000) >> 16;

// printf("Pos: (%ld.%03u, %ld.%03u) m  Angulo: %ld\n",
//     pos_x_int, pos_x_frac,
//     pos_y_int, pos_y_frac,
//     theta >> 16); 

//         my_wait(200);
//     }
// }

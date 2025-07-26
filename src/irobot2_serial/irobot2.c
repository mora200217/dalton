#include <libbase/uart.h>
#include "delay.h"

#include "irobot2.h"


unsigned char single_byte_packets[22] = { 7, 8, 9, 10, 11, 12, 13, 14, 15, 17, 18,
    21, 24, 34, 35, 36, 37, 38, 45, 52, 53, 58};


void roomba_start(void){
	uart_write(128);
}

void roomba_reset(void){
	uart_write(7);
}

void roomba_turn_off(void){
	uart_write(133);
}

void roomba_safe_mode(void){
	uart_write(131);
}

void roomba_full_mode(void){
    uart_write(132);
}

void roomba_passive_mode(void){
	roomba_start();
}

void roomba_clean_mode(void){
	uart_write(135);
}

// linear movement
void roomba_move(short speed){
	clamp(speed, -500, 500);
	uart_write(145);
    // right wheel
	uart_write(speed >> 8);
	uart_write(speed);
    // left wheel
	uart_write(speed >> 8);
	uart_write(speed);
}

void roomba_move_forward_forever(void) {
    roomba_move(ROOMBA_LINEAR_SPEED);
}

void roomba_move_backward_forever(void) {
    roomba_move(-ROOMBA_LINEAR_SPEED);
}

// distance in cm
void roomba_move_forward(unsigned int distance) {
    roomba_move(ROOMBA_LINEAR_SPEED);
    delay(distance*100/ROOMBA_LINEAR_SPEED);
    roomba_move_stop();
}

void roomba_move_backward(unsigned int distance) {
    roomba_move(-ROOMBA_LINEAR_SPEED);
    delay(distance*100/ROOMBA_LINEAR_SPEED);
    roomba_move_stop();
}


// rotational movement
void roomba_turn(int degrees){
    unsigned short lin_speed = ROOMBA_ANGULAR_SPEED/RAD_TO_DEG * ROOMBA_RADIUS;
    short r_lin_speed;
    short l_lin_speed;

    clamp(lin_speed, 0, 500);
    if (degrees > 0) {
        // clockwise (left)
        r_lin_speed = lin_speed;
        l_lin_speed = -lin_speed;
    }
    else {
        // counter clockwise (right)
        l_lin_speed = lin_speed;
        r_lin_speed = -lin_speed;
    }
    uart_write(145);
	uart_write(r_lin_speed >> 8);
    uart_write(r_lin_speed);
	uart_write(l_lin_speed >> 8);
    uart_write(l_lin_speed);
}

void roomba_turn_left(unsigned int degrees) {
    roomba_turn(degrees);
    delay(degrees/ROOMBA_ANGULAR_SPEED);
    roomba_move_stop();
}

void roomba_turn_right(unsigned int degrees) {
    roomba_turn(-degrees);
    delay(degrees/ROOMBA_ANGULAR_SPEED);
    roomba_move_stop();
}

// doesn't matter the exact angle in here as long as its > 0 for left and
// < 0 for right
void roomba_turn_left_forever(void) {
    roomba_turn(1);
}

void roomba_turn_right_forever(void) {
    roomba_turn(-1);
}

void roomba_move_stop(void) {
    roomba_move(0);
}

// move the thing in 1 meter square loop
void roomba_motor_square_test(void) {
    for (int i = 0; i < 4; ++i) {
        roomba_move_forward(50); // half meter
        roomba_move_stop();
        roomba_turn_right(90);
    }
}

// blocking sensor read data functions
int roomba_get_sensor_data(unsigned char sensorID){
	int returnVal;
	unsigned char packetID = 0;
	if (sensorID > 100){
		switch (sensorID){
		case 101:
		case 102:
		case 103:
		case 104:
			packetID = 7;
			break;
		case 105:
		case 106:
		case 107:
		case 108:
			packetID = 14;
			break;
		case 109:
		case 110:
		case 111:
		case 112:
		case 113:
		case 114:
		case 115:
		case 116:
			packetID = 18;
			break;
		case 117:
		case 118:
		case 119:
		case 120:
		case 121:
		case 122:
			packetID = 45;
			break;
		}

	}
	else {
		packetID = sensorID;
	}
	unsigned char MSB = 0;
	unsigned char LSB = 0;

	uart_write(142);
	uart_write(packetID);
	if(roomba_is_single_byte(packetID)){
		returnVal = uart_read();
	} else {
		MSB = uart_read();
		LSB = uart_read();
		returnVal = (MSB << 7) | LSB;
	}
	return returnVal;
}

int roomba_get_speed(void) {
    int l_wheel_counts = roomba_get_sensor_data(43);
    int r_wheel_counts = roomba_get_sensor_data(44);
    int l_distance = l_wheel_counts/DISTANCE_TO_COUNTS;
    int r_distance = r_wheel_counts/DISTANCE_TO_COUNTS;
    int speed = (l_distance + r_distance)/2.0;
    return speed;
}

int roomba_get_angle(void) {
    int l_wheel_counts = roomba_get_sensor_data(43);
    int r_wheel_counts = roomba_get_sensor_data(44);
    int l_distance = l_wheel_counts/DISTANCE_TO_COUNTS;
    int r_distance = r_wheel_counts/DISTANCE_TO_COUNTS;
    int rads_angle = (r_distance - l_distance) / ROOMBA_RADIUS;
    return rads_angle * RAD_TO_DEG;
}

char roomba_is_single_byte(unsigned char packetID){
    for (int i = 0; i < 22; ++i) {
        if (packetID == single_byte_packets[i]){
            return 1;
        }
    }
    return 0;
}


#include <libbase/uart.h>
#include "delay.h"

#include "irobot2.h"

//available as passive safe or full modes

iRobotCreate2::iRobotCreate2() {
}

void iRobotCreate2::start(){
	uart_write(128);
}

void iRobotCreate2::reset(){
	uart_write(7);
}

void iRobotCreate2::turnOff(){
	uart_write(133);
}

void iRobotCreate2::safeMode(){
	uart_write(131);
}

void iRobotCreate2::fullMode(){
    uart_write(132);
}

void iRobotCreate2::passiveMode(){
	start();
}

void iRobotCreate2::clean(){
	uart_write(135);
}

//wheel motors
void iRobotCreate2::drive(int velocity, int radius){
	clamp(velocity, -500, 500);
	clamp(radius, -2000, 2000);
	uart_write(137);
	uart_write(velocity >> 8);
	uart_write(velocity);
	uart_write(radius >> 8);
	uart_write(radius);
}

void iRobotCreate2::turnCW(unsigned short velocity, unsigned short degrees){
	drive(velocity, -1);
	clamp(velocity, 0, 500);
	delay(2700);
	//delay((1580 + 2.25*velocity)/velocity*degrees);
	//delay((-0.03159720835 * velocity + 21.215270835) * degrees);
	drive(0,0);
}

void iRobotCreate2::turnCCW(unsigned short velocity, unsigned short degrees){
	drive(velocity, 1);
	clamp(velocity, 0, 500);
	delay(6600);
	drive(0,0);
}

void iRobotCreate2::driveStop(void) {
    drive(0, 0);
}

void iRobotCreate2::motorSquareTest(void) {
    for (int i = 0; i < 4; ++i) {
        drive(60, 0);
        delay(5000);
        driveStop();
        turnCCW(30, 90);
    }
}


// blocking sensor data request
int iRobotCreate2::getSensorData(unsigned char sensorID){
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
	if(is_single_byte(packetID)){
		returnVal = uart_read();
	} else {
		MSB = uart_read();
		LSB = uart_read();
		returnVal = (MSB << 7) | LSB;
	}
	return returnVal;
}

#define COUNTS_TO_DISTANCE 0.445265100
float iRobotCreate2::getSpeed(void) {
    int l_wheel_counts = getSensorData(43);
    int r_wheel_counts = getSensorData(44);
    // long float distance = counts * (PI * 72.0/508.8);
    int l_distance = l_wheel_counts * COUNTS_TO_DISTANCE;
    int r_distance = r_wheel_counts * COUNTS_TO_DISTANCE;
    int speed = (l_distance + r_distance)/2.0;
    return speed;
}

float iRobotCreate2::getAngle(void) {
    int l_wheel_counts = getSensorData(43);
    int r_wheel_counts = getSensorData(44);
    int l_distance = l_wheel_counts * COUNTS_TO_DISTANCE;
    int r_distance = r_wheel_counts * COUNTS_TO_DISTANCE;
    int rads_angle = (r_distance - l_distance) / 300;
    return rads_angle;
}

bool iRobotCreate2::is_single_byte(unsigned char packetID){
    for (int i = 0; i < 22; ++i) {
        if (packetID == single_byte_packets[i]){
            return true;
        }
    }
    return false;
}


// Functions to control the iRobot Create 2.
// Partial copy of the required functions from https://github.com/brinnLabs/Create2
#ifndef IROBOT2_H
#define IROBOT2_H

#define clamp(value, min, max) (value = value < min ? min : value > max ? max : value)

class iRobotCreate2 {
public:
    iRobotCreate2();
	void start(void);
	void reset(void);
    bool is_in_array(unsigned char val);
    // set operation modes
	void fullMode(void);
	void passiveMode(void);
	void clean(void);
	void maxClean(void);
	void spotClean(void);
	void seekDock(void);
	void turnOff(void);
    void safeMode(void);

	// available is safe or full modes
	// movement control
	void drive(int velocity, int radius);
	void driveWheels(int right, int left);
	void driveLeft(int left);
	void driveRight(int right);
	void driveWheelsPWM(int rightPWM, int leftPWM);
	void turnCW(unsigned short velocity,unsigned short degrees);
	void turnCCW(unsigned short velocity, unsigned short degrees);
    void motorSquareTest(void);
    void driveStop(void);

    // blocking sensfsor functions
	int getSensorData(unsigned char packetID);
    float getSpeed(void);
    float getAngle(void);
private:
    unsigned char single_byte_packets[22] = { 7, 8, 9, 10, 11, 12, 13, 14, 15, 17, 18,
        21, 24, 34, 35, 36, 37, 38, 45, 52, 53, 58};
    bool is_single_byte(unsigned char packetID);
};

#endif

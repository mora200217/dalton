// Functions to control the iRobot Create 2.
// Partial copy of the required functions from https://github.com/brinnLabs/Create2
#ifndef IROBOT2_H
#define IROBOT2_H

#define clamp(value, min, max) (value = value < min ? min : value > max ? max : value)

#define ROOMBA_LINEAR_SPEED 500
#define ROOMBA_RADIUS 117.5
#define RAD_TO_DEG 57.29577
#define ROOMBA_ANGULAR_SPEED 90 // degrees per second
#define DISTANCE_TO_COUNTS 7.06667

void roomba_start(void);
void roomba_reset(void);
// set operation modes
void roomba_full_mode(void);
void roomba_passive_mode(void);
void roomba_clean_mode(void);
void roomba_turn_off(void);
void roomba_safe_mode(void);

// linear movement
void roomba_move(short l_speed);
void roomba_move_forward(unsigned int distance);
void roomba_move_forward_forever(void);
void roomba_move_backward(unsigned int distance);
void roomba_move_backward_forever(void);

// rotational movement
void roomba_turn(int degrees);
void roomba_turn_left(unsigned int degrees);
void roomba_turn_left_forever(void);
void roomba_turn_right(unsigned int degrees);
void roomba_turn_right_forever(void);

void roomba_move_stop(void);

void roomba_motor_square_test(void);

// blocking sensor reading
int roomba_get_sensor_data(unsigned char packetID);
int roomba_get_speed(void);
int roomba_get_angle(void);
char roomba_is_single_byte(unsigned char packetID);

#endif


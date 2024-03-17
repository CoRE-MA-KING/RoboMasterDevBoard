/*
 * machine_variable.h
 *
 *  Created on: Jan 20, 2024
 *      Author: mmaakkyyii
 */

#ifndef MACHINE_VARIABLE_H_
#define MACHINE_VARIABLE_H_

#include "motor.h"
#include "encorder.h"
#include "controller.h"
#include "core_wireless_control_rx.hpp"
#include "state.h"
#include "buzzer.h"
#include "servo.h"

extern CanInterface can1_bus;
extern CanInterface can2_bus;
extern C620 motor1;
extern C620 motor2;
extern C620 motor3;
extern C620 motor4;

extern PID m1_pid;
extern PID m2_pid;
extern PID m3_pid;
extern PID m4_pid;

extern C610 loading_motor;
extern C620 pitch_motor;
extern PID loading_motor_pid;
extern PID pitch_motor_pid;
extern DJIServo pitch_servo;

extern SabertoothDual rollerR;
extern SabertoothDual rollerL;

extern Encorder roller_enc_R;
extern Encorder roller_enc_L;

extern uint8_t rxed_byte_data;
extern UsartBuffer ub;
extern CoreWirelessControlRx cwcr;

extern Buzzer buzzer;

extern State state;

extern bool rec;
extern bool reboot_flag;
extern int video_id;
#endif /* MACHINE_VARIABLE_H_ */

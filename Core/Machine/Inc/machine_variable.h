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

extern CanInterface can1_bus;
extern C620 motor1;
extern C620 motor2;
extern C620 motor3;
extern C620 motor4;

extern PID m1_pid;
extern PID m2_pid;
extern PID m3_pid;
extern PID m4_pid;

extern SabertoothDual rollerR;
extern SabertoothDual rollerL;

extern Encorder roller_enc_R;
extern Encorder roller_enc_L;
#endif /* MACHINE_VARIABLE_H_ */

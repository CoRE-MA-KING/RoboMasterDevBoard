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

extern CanInterface can1_bus;
extern C620 motor1;
extern C620 motor2;
extern C620 motor3;
extern C620 motor4;

extern SabertoothDual rollerR;
extern SabertoothDual rollerL;

extern Encorder roller_enc_R;
extern Encorder roller_enc_L;
#endif /* MACHINE_VARIABLE_H_ */

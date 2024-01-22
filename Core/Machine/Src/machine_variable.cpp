/*
 * machine_variable.cpp
 *
 *  Created on: Jan 20, 2024
 *      Author: mmaakkyyii
 */

#include "machine_variable.h"
#include "machine_constant.hpp"

#include "can.h"

CanInterface can1_bus(&hcan1);
C620 motor1(&can1_bus,1,-1);
C620 motor2(&can1_bus,2,-1);
C620 motor3(&can1_bus,3,1);
C620 motor4(&can1_bus,4,1);

PID m1_pid(1.0, 1, 0, control_period_ms);
PID m2_pid(1.0, 0.01, 0, control_period_ms);
PID m3_pid(1.0, 0.01, 0, control_period_ms);
PID m4_pid(1.0, 0.01, 0, control_period_ms);


SabertoothDual rollerL(1,1);
SabertoothDual rollerR(2,-1);

Encorder roller_enc_R(2,1024*2,1,1,1);
Encorder roller_enc_L(8,1024*2,1,1,1);

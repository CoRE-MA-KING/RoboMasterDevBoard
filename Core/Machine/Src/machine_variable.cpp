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
CanInterface can2_bus(&hcan2);
C620 motor1(&can1_bus,1,1,kWheelRadius_mm);
C620 motor2(&can1_bus,2,-1,kWheelRadius_mm);
C620 motor3(&can1_bus,3,1,kWheelRadius_mm);
C620 motor4(&can1_bus,4,-1,kWheelRadius_mm);

C610 loading_motor(&can2_bus,1,1,1);
C620 pitch_motor(&can2_bus,2,1,1);


PID m1_pid(4.0, 0.1, 0, kControlPeriod_ms);
PID m2_pid(4.0, 0.1, 0, kControlPeriod_ms);
PID m3_pid(4.0, 0.1, 0, kControlPeriod_ms);
PID m4_pid(4.0, 0.1, 0, kControlPeriod_ms);

PID loading_motor_pid(5.0, 0.05, 0, kControlPeriod_ms);
PID pitch_motor_pos_pid(1,0.01,0,kControlPeriod_ms);
PID pitch_motor_vel_pid(1,0.01,0,kControlPeriod_ms);

DJIServo pitch_servo(&pitch_motor,&pitch_motor_pos_pid,&pitch_motor_vel_pid,kPitchTheta2Position);

SabertoothDual rollerL(1,-1);
SabertoothDual rollerR(2,-1);

Encorder roller_enc_R(2,1024*2,kControlPeriod_ms,kRollerRadius_mm,1);
Encorder roller_enc_L(8,1024*2,kControlPeriod_ms,kRollerRadius_mm,-1);

uint8_t rxed_byte_data;
UsartBuffer ub;
CoreWirelessControlRx cwcr(&ub);

Buzzer buzzer;
State state;

bool rec;
bool reboot_flag;



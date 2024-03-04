/*
 * esw_mode.cpp
 *
 *  Created on: Feb 25, 2024
 *      Author: mmaakkyyii
 */

#include "state.h"
#include "machine_variable.h"
#include "stdio.h"

ESWMode::ESWMode(){}

void ESWMode::Init(){
	rollerL.SetVoltage_V(0);
	rollerR.SetVoltage_V(0);

}

void ESWMode::Update(){
	motor1.SetCurrent_mA(0);
	motor2.SetCurrent_mA(0);
	motor3.SetCurrent_mA(0);
	motor4.SetCurrent_mA(0);
	loading_motor.SetCurrent_mA(0);
	pitch_motor.SetCurrent_mA(0);
	rollerL.SetVoltage_V(0);
	rollerR.SetVoltage_V(0);
	printf("!!ESW!!\r\n");

}



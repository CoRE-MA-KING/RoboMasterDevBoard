/*
 * communication_error_mode.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: mmaakkyyii
 */

#include "state.h"
#include "machine_variable.h"

CommunicationErrorMode::CommunicationErrorMode(){

}

void CommunicationErrorMode::Init(){
	rollerL.SetVoltage_V(0);
	rollerR.SetVoltage_V(0);
}

void CommunicationErrorMode::Update(){
	motor1.SetCurrent_mA(0);
	motor2.SetCurrent_mA(0);
	motor3.SetCurrent_mA(0);
	motor4.SetCurrent_mA(0);
	loading_motor.SetCurrent_mA(0);
	pitch_motor.SetCurrent_mA(0);
	rollerL.SetVoltage_V(0);
	rollerR.SetVoltage_V(0);
}


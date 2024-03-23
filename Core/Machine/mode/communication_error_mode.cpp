/*
 * communication_error_mode.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: mmaakkyyii
 */

#include "state.h"
#include "machine_variable.h"
#include "stdio.h"
CommunicationErrorMode::CommunicationErrorMode(){

}

void CommunicationErrorMode::Init(){
	rollerL.SetVoltage_V(0);
	rollerR.SetVoltage_V(0);
	cwcr.ClearBuffer();
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

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);

}

void CommunicationErrorMode::Update_10ms(){
	static int clear_flag=0;

	clear_flag++;
	if(clear_flag>10){
		cwcr.ClearBuffer();
		clear_flag=0;
	}

	printf("%d,%d,%d,%d,%d,%d,%d,%d,\n",
			(int)Mode::kCommunicationError,
			0,
			(int)(pitch_servo.GetPosition()*10),
			0,
			rec,
			reboot_flag,
			0,
			0
			);
}



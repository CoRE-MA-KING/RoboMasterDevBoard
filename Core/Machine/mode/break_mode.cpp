/*
 * break_mode.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: mmaakkyyii
 */

#include "state.h"
#include "stdio.h"
#include "machine_variable.h"

BreakMode::BreakMode(){

}

void BreakMode::Init(){
	rollerL.SetVoltage_V(0);
	rollerR.SetVoltage_V(0);
	buzzer.SetFrequency(400,1000);
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);

}

void BreakMode::Update(){

	if(cwcr.button(15)==1){
		buzzer.SetFrequency(400,10);
	}

	// change video
	if(cwcr.button(2)==1){
		video_id=0;
	}else if(cwcr.button(3)==1){
		video_id=2;
	}else{
		video_id=1;
	}


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

void BreakMode::Update_10ms(){
	printf("%d,%d,%d,%d,%d,%d,%d,%d,\n",
			(int)Mode::kMachineBreak,
			0,
			(int)pitch_servo.GetPosition(),
			0,
			rec,
			reboot_flag,
			0,
			video_id
			);

}

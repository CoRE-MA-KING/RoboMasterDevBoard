/*
 * machine_init_mode.cpp
 *
 *  Created on: Feb 25, 2024
 *      Author: mmaakkyyii
 */
#include "state.h"
#include "gpio.h"
#include "machine_variable.h"
#include "stdio.h"
MachineInitMode::MachineInitMode(){
};

void MachineInitMode::Init(){
	finish_flag_=false;
	power_on_timer_=0;
	pitch_dir_=-1;

	rollerL.SetVoltage_V(0);
	rollerR.SetVoltage_V(0);

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
	pitch_servo.Reset();

}
void MachineInitMode::Update(){

	motor1.SetCurrent_mA(0);
	motor2.SetCurrent_mA(0);
	motor3.SetCurrent_mA(0);
	motor4.SetCurrent_mA(0);

	pitch_servo.SetReferenceVelocity(0);
	if(cwcr.button(7)==1){//up
		pitch_servo.SetReferenceVelocity(2);

	}
	if(cwcr.button(5)==1){//down
		pitch_servo.SetReferenceVelocity(-2);
	}

	if(power_on_timer_<kPowerOnTime_ms_){
		power_on_timer_++;

		return;
	}else if(reset_flag_){
		finish_flag_=true;
	}else{
		pitch_servo.SetReferenceVelocity(pitch_dir_*2);
	}

	if(HAL_GPIO_ReadPin(PHOTO_SENS2_GPIO_Port, PHOTO_SENS2_Pin)==GPIO_PIN_RESET){
		pitch_dir_=1;
		pitch_servo.SetReferenceVelocity(0);
		pitch_servo.ResetPosition(0);
		reset_flag_=true;
	}
	if(reset_flag_ && pitch_servo.GetPosition()>10.0){
		finish_flag_=true;

	}


	//printf("%d,%d,%d,\n",HAL_GPIO_ReadPin(PHOTO_SENS2_GPIO_Port, PHOTO_SENS2_Pin),(int)pitch_servo.GetPosition(),(int)(pitch_servo.GetVelocity()));


}

void MachineInitMode::Update_10ms(){
	printf("%d,%d,%d,%d,%d,%d,%d,%d,\n",
			(int)Mode::kMachineInit,
			0,
			(int)(pitch_servo.GetPosition()*10),
			0,
			rec,
			reboot_flag,
			0,
			video_id
			);

}

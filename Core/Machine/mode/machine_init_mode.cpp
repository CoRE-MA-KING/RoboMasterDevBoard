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

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
	pitch_servo.Reset();

}
void MachineInitMode::Update(){

	// change video
	if(cwcr.button(2)==1){
		video_id=0;
	}else if(cwcr.button(3)==1){
		video_id=2;
	}else{
		video_id=1;
	}


	if(power_on_timer_<kPowerOnTime_ms_){
		power_on_timer_++;

		//return;
	}else{
		finish_flag_=true; //TODO あとではずす
		pitch_dir_=-1;
	}

	pitch_servo.SetReferenceVelocity(pitch_dir_*1.0);
	pitch_servo.Update();

	if(HAL_GPIO_ReadPin(PHOTO_SENS2_GPIO_Port, PHOTO_SENS2_Pin)==GPIO_PIN_SET){
		pitch_dir_=1;
	}
	if(pitch_dir_==1 && HAL_GPIO_ReadPin(PHOTO_SENS2_GPIO_Port, PHOTO_SENS2_Pin)==GPIO_PIN_RESET){
		finish_flag_=true;
	}


	motor1.SetCurrent_mA(0);
	motor2.SetCurrent_mA(0);
	motor3.SetCurrent_mA(0);
	motor4.SetCurrent_mA(0);

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

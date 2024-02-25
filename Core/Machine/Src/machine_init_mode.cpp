/*
 * machine_init_mode.cpp
 *
 *  Created on: Feb 25, 2024
 *      Author: mmaakkyyii
 */
#include "state.h"
#include "gpio.h"
#include "machine_variable.h"

MachineInitMode::MachineInitMode(){
};

void MachineInitMode::Init(){
	finish_flag=false;
	power_on_timer=0;

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);

}
void MachineInitMode::Update(){
	if(power_on_timer<power_on_time_ms){
		power_on_timer++;
		pitch_dir=-1;
		return;
	}

	float pitch_vel=pitch_motor.GetVelocity_rad_s();
	pitch_motor_pid.SetReference(pitch_dir*1);

	int pitch_current_mA=(int)pitch_motor.GetCurrent_mA();
	float target_pitch_current_mA=pitch_motor_pid.Update(pitch_vel);

	if(HAL_GPIO_ReadPin(PHOTO_SENS2_GPIO_Port, PHOTO_SENS2_Pin)==GPIO_PIN_SET){
		pitch_dir=1;
	}
	if(pitch_dir==1 && HAL_GPIO_ReadPin(PHOTO_SENS2_GPIO_Port, PHOTO_SENS2_Pin)==GPIO_PIN_RESET){
		target_pitch_current_mA=0;
		finish_flag=true;
	}
	pitch_motor.SetCurrent_mA(target_pitch_current_mA);


	motor1.SetCurrent_mA(0);
	motor2.SetCurrent_mA(0);
	motor3.SetCurrent_mA(0);
	motor4.SetCurrent_mA(0);



}


/*
 * normal_mode.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: mmaakkyyii
 */

#include "state.h"
#include "stdio.h"
#include "machine_constant.hpp"
#include "machine_variable.h"
#include "motion.h"

void NromalMode::Init(){
	printf("NromalMode!\r\n");
	m1_pid.Reset();
	m2_pid.Reset();
	m3_pid.Reset();
	m4_pid.Reset();
	loading_motor_pid.Reset();
	pitch_motor_pid.Reset();
}
void NromalMode::Update(){

	//READ Wireless Controller
	static int loading_motor_ref=0;

	static float vx_mm_s=0;
	static float vy_mm_s=0;
	static float omega_rad_s=0;

	vx_mm_s=cwcr.axis(2)*50;
	vy_mm_s=cwcr.axis(1)*50;
	omega_rad_s=cwcr.axis(4)*0.0001;

	if(cwcr.button(1)==1){
		loading_motor_ref=5000;
	}

	static float roller_Voltage;
	const float roller_Voltage_max=5.0;
	const float delta_V=0.01;
	if(cwcr.button(2)==1){//TODO check button
		roller_Voltage+=delta_V;
		if(roller_Voltage<=roller_Voltage_max){
			roller_Voltage=roller_Voltage_max;
		}
	}else{
		roller_Voltage-=delta_V;
		if(roller_Voltage>=0){
			roller_Voltage=0;
		}
	}

	rollerL.SetVoltage_V(roller_Voltage);
	rollerR.SetVoltage_V(roller_Voltage*0.2);

	//loading motor control
	int photo1=HAL_GPIO_ReadPin(PHOTO_SENS1_GPIO_Port, PHOTO_SENS1_Pin);
	static int pre_hoto1;
	if(photo1==1 && pre_hoto1==0){
		loading_motor_ref=0;
	}
	pre_hoto1=photo1;

	int loading_current_mA = loading_motor.GetCurrent_mA();
	float v_loading_rpm=loading_motor.GetVelocity_rad_s()/(2*3.14)*60;

	loading_motor_pid.SetReference(loading_motor_ref);
	loading_motor_pid.Update(v_loading_rpm);

	float target_current_loading = loading_motor_pid.Update(v_loading_rpm);
	loading_motor.SetCurrent_mA(target_current_loading);

	//wheel motor control
	float v1,v2,v3,v4;
	MecanumWheelJacobian(vx_mm_s,vy_mm_s,omega_rad_s,&v1,&v2,&v3,&v4);

	float vel1=motor1.GetVelocity_mm_s();
	float vel2=motor2.GetVelocity_mm_s();
	float vel3=motor3.GetVelocity_mm_s();
	float vel4=motor4.GetVelocity_mm_s();

	m1_pid.SetReference(v1);
	m2_pid.SetReference(v2);
	m3_pid.SetReference(v3);
	m4_pid.SetReference(v4);

	float target_current1 = m1_pid.Update(vel1);
	float target_current2 = m2_pid.Update(vel2);
	float target_current3 = m3_pid.Update(vel3);
	float target_current4 = m4_pid.Update(vel4);

	motor1.SetCurrent_mA(target_current1);
	motor2.SetCurrent_mA(target_current2);
	motor3.SetCurrent_mA(target_current3);
	motor4.SetCurrent_mA(target_current4);

	printf("%d,%d,%d\r\n",(int)(vx_mm_s),(int)vy_mm_s,(int)(omega_rad_s*1000));

}



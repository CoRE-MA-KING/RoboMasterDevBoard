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
	loading_motor_ref_=0;

	vx_mm_s_=0;
	vy_mm_s_=0;
	omega_rad_s_=0;
	photo1_=0;
	pre_hoto1_=0;
	roller_voltage_V_=0;


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

	vx_mm_s_=cwcr.axis(0)*20;
	vy_mm_s_=cwcr.axis(1)*20;
	omega_rad_s_=cwcr.axis(2)*0.01;

	if(cwcr.button(1)==1){
		loading_motor_ref_=4000;
	}

	const float kDelta_V=0.01;
	if(cwcr.button(2)==1){//TODO check button
		roller_voltage_V_+=kDelta_V;
		if(roller_voltage_V_>=roller_voltage_max_V){
			roller_voltage_V_=roller_voltage_max_V;
		}
	}else{
		roller_voltage_V_-=kDelta_V;
		if(roller_voltage_V_<=0){
			roller_voltage_V_=0;
		}
	}

	rollerL.SetVoltage_V(roller_voltage_V_);
	rollerR.SetVoltage_V(roller_voltage_V_*0.2);

	//loading motor control
	photo1_=HAL_GPIO_ReadPin(PHOTO_SENS1_GPIO_Port, PHOTO_SENS1_Pin);
	if(photo1_==1 && pre_hoto1_==0){
		loading_motor_ref_=0;
	}
	pre_hoto1_=photo1_;

	int loading_current_mA = loading_motor.GetCurrent_mA();
	float v_loading_rpm=loading_motor.GetVelocity_rad_s()/(2*3.14)*60;

	loading_motor_pid.SetReference(loading_motor_ref_);
	loading_motor_pid.Update(v_loading_rpm);

	float target_current_loading = loading_motor_pid.Update(v_loading_rpm);
	loading_motor.SetCurrent_mA(target_current_loading);

	//wheel motor control
	float v1,v2,v3,v4;
	MecanumWheelJacobian(vx_mm_s_,vy_mm_s_,omega_rad_s_,&v1,&v2,&v3,&v4);

	float vel1=motor1.GetVelocity_mm_s();
	float vel2=motor2.GetVelocity_mm_s();
	float vel3=motor3.GetVelocity_mm_s();
	float vel4=motor4.GetVelocity_mm_s();

//	v1=4000;
//	v2=4000;
//	v3=4000;
//	v4=4000;
	m1_pid.SetReference(v1);
	m2_pid.SetReference(v2);
	m3_pid.SetReference(v3);
	m4_pid.SetReference(v4);

	float target_current1 = m1_pid.Update(vel1);
	float target_current2 = m2_pid.Update(vel2);
	float target_current3 = m3_pid.Update(vel3);
	float target_current4 = m4_pid.Update(vel4);

//	target_current1=0;
//	target_current2=0;
//	target_current3=0;
//	target_current4=0;
	motor1.SetCurrent_mA(target_current1);
	motor2.SetCurrent_mA(target_current2);
	motor3.SetCurrent_mA(target_current3);
	motor4.SetCurrent_mA(target_current4);


	printf("%d,%d,%d,%d,%d,\r\n",(int)(roller_voltage_V_*1000),(int)v1,(int)v2,(int)v3,(int)v4);
	//printf("%d,%d,%d,%d,\r\n",(int)v1,(int)v2,(int)v3,(int)v4);
	//printf("%d,%d,%d,%d,\r\n",(int)vel1,(int)vel2,(int)vel3,(int)vel4);
	//printf("%d,%d,%d\r\n",(int)(vx_mm_s_),(int)vy_mm_s_,(int)(omega_rad_s_*1000));

}



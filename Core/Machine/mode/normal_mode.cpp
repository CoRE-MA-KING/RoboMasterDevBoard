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

	shoot_enable_=true;

	m1_pid.Reset();
	m2_pid.Reset();
	m3_pid.Reset();
	m4_pid.Reset();
	loading_motor_pid.Reset();
	pitch_servo.Reset();
	buzzer.SetFrequency(400,500);
}
void NromalMode::Update(){

	//READ Wireless Controller
	float machine_velocity_max=1000;
	float machine_omega_max=0.5;
	vx_mm_s_=(float)cwcr.axis(0)/127.0*machine_velocity_max;
	vy_mm_s_=(float)cwcr.axis(1)/127.0*machine_velocity_max;
	vx_mm_s_=filter_x_.Update(vx_mm_s_);
	vy_mm_s_=filter_y_.Update(vy_mm_s_);
	omega_rad_s_=cwcr.axis(2)/127.0*machine_omega_max;

	if(cwcr.button(1)==1 && roller_voltage_V_==roller_voltage_max_V_){//R1
		loading_motor_ref_=4000;
	}

	//buzzer check
	if(cwcr.button(15)==1){//△
		buzzer.SetFrequency(400,10);
	}
	//pitch control
	const float kDeltaPos=1.0;
	const float kPitchPosMax=20.0;
	if(cwcr.button(7)==1){
		target_pitch_pos_+=kDeltaPos;
		if(target_pitch_pos_>kPitchPosMax)target_pitch_pos_=kPitchPosMax;
	}
	if(cwcr.button(5)==1){
		target_pitch_pos_-=kDeltaPos;
		if(target_pitch_pos_<-kPitchPosMax)target_pitch_pos_=-kPitchPosMax;
	}
	pitch_servo.SetReferencePotition(target_pitch_pos_);
	//roller control
	if(cwcr.button(6)==1){
		roller_voltage_max_V_+=0.001;
		if(roller_voltage_max_V_>15)roller_voltage_max_V_=15.0;
	}
	if(cwcr.button(4)==1){
		roller_voltage_max_V_-=0.001;
		if(roller_voltage_max_V_<0)roller_voltage_max_V_=0;
	}

	const float kDelta_V=0.02;
	if(cwcr.button(0)==1){//R2
		roller_voltage_V_+=kDelta_V;
		if(roller_voltage_V_>=roller_voltage_max_V_){
			roller_voltage_V_=roller_voltage_max_V_;
		}
	}else{
		roller_voltage_V_-=kDelta_V;
		if(roller_voltage_V_<=0){
			roller_voltage_V_=0;
		}
	}

	rollerL.SetVoltage_V(roller_voltage_V_);
	rollerR.SetVoltage_V(0);

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


	printf("%d,%d,%d,%d,%d,\r\n",(int)(roller_voltage_max_V_*1000),(int)v1,(int)v2,(int)v3,(int)v4);
//	printf("%d,%d,%d,%d,%d,\r\n",(int)cwcr.axis(0),(int)v1,(int)vel1,(int)v2,(int)vel2);
	//printf("%d,%d,%d,%d,\r\n",(int)vel1,(int)vel2,(int)vel3,(int)vel4);
//	printf("%d,%d,%d\r\n",(int)(vx_mm_s_),(int)vy_mm_s_,(int)(omega_rad_s_*1000));

/*	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
		cwcr.button(0),
		cwcr.button(1),
		cwcr.button(2),
		cwcr.button(3),
		cwcr.button(4),
		cwcr.button(5),
		cwcr.button(6),
		cwcr.button(7),
		cwcr.button(8),
		cwcr.button(9),
		cwcr.button(10),
		cwcr.button(11),
		cwcr.button(12),
		cwcr.button(13),
		cwcr.button(14),
		cwcr.button(15)
		);

*/

//	printf("%d,%d,%d,%d,%d,%d,%d,\n",2,1,);

}

void NromalMode::Update_10ms(){
	printf("%d,%d,%d,%d,%d,%d,%d,\r\n",
			Mode::kNormal,
			shoot_enable_,
			pitch_servo.GetPosition(),
			roller_voltage_max_V_,
			rec,
			reboot_flag,
			0,
			0
			);

}


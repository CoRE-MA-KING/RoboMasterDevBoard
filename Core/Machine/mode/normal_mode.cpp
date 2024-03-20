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
	roller_l_pid.Reset();
	roller_r_pid.Reset();
	pitch_servo.Reset();
	filter_x_.ClearBuffer();
	filter_y_.ClearBuffer();

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

	// change video
	if(cwcr.button(2)==1){
		video_id=0;
	}else if(cwcr.button(3)==1){
		video_id=2;
	}else{
		video_id=1;
	}
	//buzzer check
	if(cwcr.button(15)==1){//△
		buzzer.SetFrequency(400,10);
	}
	//reload fresbee
	if(cwcr.button(10)==1){//share
		buzzer.SetFrequency(350,20);
		rec=true;
	}
	if(cwcr.button(11)==1){//option
		buzzer.SetFrequency(350,20);
		rec=false;
	}
	if(cwcr.button(12)==1){//sqare
		buzzer.SetFrequency(350,10);
		frisbee_num_=kMaxFrisbeeNum;

	}

	//pitch control
	const float kDeltaPos=1.0;
	const float kPitchPosMax=20.0;
	pitch_servo.SetReferenceVelocity(0);

	if(cwcr.button(7)==1){//up
		target_pitch_pos_+=kDeltaPos;
		if(target_pitch_pos_>kPitchPosMax)target_pitch_pos_=kPitchPosMax;
		pitch_servo.SetReferenceVelocity(5);

	}
	if(cwcr.button(5)==1){//down
		target_pitch_pos_-=kDeltaPos;
		if(target_pitch_pos_<-kPitchPosMax)target_pitch_pos_=-kPitchPosMax;
		pitch_servo.SetReferenceVelocity(-5);
	}

	//pitch_servo.SetReferencePotition(target_pitch_pos_);

	//roller control
	const float kDelta_V=20	;
	if(cwcr.button(0)==1){//R2
		roller_l_target_velocity_mm_s_+=kDelta_V;
		if(roller_l_target_velocity_mm_s_>=roller_target_velocity_max_mm_s_){
			roller_l_target_velocity_mm_s_=roller_target_velocity_max_mm_s_;
		}
	}else{
		roller_l_target_velocity_mm_s_-=kDelta_V;
		if(roller_l_target_velocity_mm_s_<=0){
			roller_l_target_velocity_mm_s_=0;
		}
	}

	roller_l_pid.SetReference(roller_l_target_velocity_mm_s_);
	float roller_l_voltage_V=roller_l_pid.Update(roller_enc_L.GetVelicty_mm_s());

	if(roller_l_voltage_V>15)roller_l_voltage_V=18;

//	rollerL.SetVoltage_V(0);
	rollerL.SetVoltage_V(roller_l_voltage_V);
	rollerR.SetVoltage_V(0);

	if(cwcr.button(6)==1){
		roller_target_velocity_max_mm_s_+=10;
		if(roller_target_velocity_max_mm_s_>kRollerVelocityLimit_mm_s){
			roller_target_velocity_max_mm_s_=kRollerVelocityLimit_mm_s;
		}
	}
	if(cwcr.button(4)==1){
		roller_target_velocity_max_mm_s_-=10;
		if(roller_target_velocity_max_mm_s_<kRollerVelocityUnderLimit_mm_s){
			roller_target_velocity_max_mm_s_=kRollerVelocityUnderLimit_mm_s;
		}
	}



	if(cwcr.button(1)==1 && roller_l_target_velocity_mm_s_*0.8 < roller_enc_L.GetVelicty_mm_s()){//R1
		loading_motor_ref_=5000;
	}

	//loading motor control
	photo1_=HAL_GPIO_ReadPin(PHOTO_SENS1_GPIO_Port, PHOTO_SENS1_Pin);
	if(photo1_==1 && pre_hoto1_==0){
		loading_motor_ref_=0;
		frisbee_num_--;
		if(frisbee_num_<0)frisbee_num_=0;
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

	printf("%d,%d,%d,%d,%d\r\n",
			(int)((int)pitch_servo.GetPosition()),
			(int)roller_enc_L.GetVelicty_mm_s(),
			(int)roller_l_target_velocity_mm_s_,
			(int)(roller_l_voltage_V*1000),
			(int)(roller_target_velocity_max_mm_s_)
	);


//	printf("%d,%d,%d\n",(int)(v1),(int)vel1,(int)target_current1);

	//	printf("%d,%d,%d,%d,%d,\r\n",(int)(roller_voltage_max_V_*1000),(int)v1,(int)v2,(int)v3,(int)v4);

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
/*
	printf("%d,%d,%d,%d,%d,%d,%d,%d,\n",
			(int)Mode::kNormal,
			shoot_enable_,
			(int)pitch_servo.GetPosition(),
			(int)roller_voltage_max_V_,
			rec,
			reboot_flag,
			frisbee_num_,
			video_id
			);
//*/
}


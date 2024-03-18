/*
 * servo.cpp
 *
 *  Created on: Mar 12, 2024
 *      Author: mmaakkyyii
 */

#include "servo.h"

DJIServo::DJIServo(DJI* dji,PID* pos_pid,PID* vel_pid, float motor_theta_to_pos)
:dji_(dji),
 pos_pid_(pos_pid),
 vel_pid_(vel_pid),
 motor_theta_to_pos_(motor_theta_to_pos),
 control_mode_(1){

}

void DJIServo::ResetPosition(float x){
	zero_theta_rad_=dji_->GetPotion_rad();
	rotate_num=0;
	x_=x;
}

void DJIServo::SetReferencePotition(float x_ref){
	pos_pid_->SetReference(x_ref);
	control_mode_=2;
}
void DJIServo::SetReferenceVelocity(float vel_ref){
	dji_->SetCurrent_mA(vel_ref);

//	vel_pid_->SetReference(vel_ref);
	control_mode_=1;
}
float DJIServo::Update(){
	float theta_rad_data=dji_->GetPotion_rad();
	if(theta_rad_data-pre_theta_rad_data_ < -3.141592){
		rotate_num++;
	}
	if(theta_rad_data-pre_theta_rad_data_ > 3.141592){
		rotate_num--;
	}
	theta_rad_=rotate_num*2*3.141592 + theta_rad_data;

	pre_theta_rad_data_=theta_rad_data;


	x_=theta_rad_*motor_theta_to_pos_;


	switch(control_mode_){
	case 1:
		UpdateVelocityControl();
		break;
	case 2:
		UpdatePositionControl();
		break;
	}
	return x_;
}

void DJIServo::Reset(){
	pos_pid_->Reset();
	vel_pid_->Reset();
}

float DJIServo::UpdatePositionControl(){
	float input=pos_pid_->Update(x_);
//	dji_->SetCurrent_mA(input);

}

float DJIServo::UpdateVelocityControl(){
	float input=vel_pid_->Update(dji_->GetVelocity_rad_s()*motor_theta_to_pos_);
//	dji_->SetCurrent_mA(input);

}




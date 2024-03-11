/*
 * servo.cpp
 *
 *  Created on: Mar 12, 2024
 *      Author: mmaakkyyii
 */

#include "servo.h"

DJIServo::DJIServo(DJI* dji,PID* pid, float motor_theta_to_pos)
:dji_(dji),
 pid_(pid),
 motor_theta_to_pos_(motor_theta_to_pos){

}

void DJIServo::ResetPosition(float x){
	zero_theta_rad_=dji_->GetPotion_rad();
	rotate_num=0;
	x_=x;
}

void DJIServo::SetReference(float x_ref){
	x_ref_=x_ref;
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

	float input=pid_->Update(x_);
	dji_->SetCurrent_mA(input);

	return theta_rad_;


}



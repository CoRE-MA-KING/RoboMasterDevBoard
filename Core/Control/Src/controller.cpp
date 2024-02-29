/*
 * controller.cpp
 *
 *  Created on: Jan 23, 2024
 *      Author: mmaakkyyii
 */

#include "controller.h"

PID::PID(float kp,float ki,float kd,float period_ms):
kp_(kp),ki_(ki),kd_(kd),period_ms_(period_ms),reference_(0),e_sum_(0),pre_input_(0){

}
float PID::Update(float input){
	float output;
	float error= reference_- input;
	e_sum_ += error* period_ms_*0.001;

	output = kp_*error + ki_*e_sum_ + kd_*(input-pre_input_)/(period_ms_*0.001);

	pre_input_=input;

	return output;
}
void PID::SetReference(float ref){
	reference_ = ref;
}




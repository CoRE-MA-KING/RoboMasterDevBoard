/*
 * controller.cpp
 *
 *  Created on: Jan 23, 2024
 *      Author: mmaakkyyii
 */

#include "controller.h"

PID::PID(float _Kp,float _Ki,float _Kd,float _period_ms):
Kp(_Kp),Ki(_Ki),Kd(_Kd),period_ms(_period_ms),reference(0),e_sum(0),pre_input(0){

}
float PID::Update(float input){
	float output;
	float error= reference- input;
	e_sum += error* period_ms*0.001;

	output = Kp*error + Ki*e_sum + Kd*(input-pre_input)/(period_ms*0.001);

	pre_input=input;

	return output;
}
void PID::SetReference(float ref){
	reference = ref;
}




/*
 * motion.cpp
 *
 *  Created on: Jan 23, 2024
 *      Author: mmaakkyyii
 */

#include "machine_constant.hpp"

void MecanumWheelJacobian(float vx, float vy, float omega, float* v1,float* v2,float* v3,float* v4){
	float r=kMachineLength_mm+kMachineWidth_mm;

	*v1= -vx + vy - r*omega;
	*v2=  vx + vy + r*omega;
	*v3=  vx - vy - r*omega;
	*v4= -vx - vy + r*omega;

}

float PitchLiner2Angle_deg(float pos){
	float offset=10;
	float x=pos-offset;
	return 5e-5*x*x*x+0.0072*x*x+0.5277*x-0.0487;
}


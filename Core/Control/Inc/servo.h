/*
 * servo.h
 *
 *  Created on: Mar 12, 2024
 *      Author: mmaakkyyii
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "motor.h"
#include "controller.h"

class DJIServo{
private:
	DJI* dji_;
	PID* pid_;
	float motor_theta_to_pos_;
	int rotate_num;
	float zero_theta_rad_;
	float theta_rad_;
	float pre_theta_rad_data_;
	float x_;
	float pre_x_;
	float x_ref_;

public:
	DJIServo(DJI* dji,PID* pid,float motor_theta_to_pos);
	void ResetPosition(float x);
	void SetReference(float x_ref);
	float GetPosition(){return x_;}
	float Update();
};



#endif /* CONTROL_INC_SERVO_H_ */

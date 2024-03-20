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
	PID* pos_pid_;
	PID* vel_pid_;
	float motor_theta_to_pos_;
	int rotate_num;
	float zero_theta_rad_;
	float theta_rad_;
	float pre_theta_rad_data_;
	float x_;
	int control_mode_;//1:vel 2:pos
	float UpdatePositionControl();
	float UpdateVelocityControl();

public:
	DJIServo(DJI* dji,PID* pos_pid,PID* vel_pid,float motor_theta_to_pos);
	void ResetPosition(float x);
	void SetReferencePotition(float x_ref);
	void SetReferenceVelocity(float vel_ref);
	float GetVelocity(){return dji_->GetVelocity_rad_s()*motor_theta_to_pos_;}
	float GetPosition(){return x_;}
	float Update();
	void Reset();
};



#endif /* CONTROL_INC_SERVO_H_ */

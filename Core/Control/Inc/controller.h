/*
 * controller.h
 *
 *  Created on: Jan 23, 2024
 *      Author: mmaakkyyii
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_


class PID{
private:
	float kp_,ki_,kd_;
	float period_ms_;
	float reference_;
	float e_sum_;
	float pre_input_;
public:
	PID(float kp,float ki,float kd,float period_ms);
	float Update(float input);
	void SetReference(float ref);
	void Reset(){e_sum_=0;pre_input_=0;}
	void SetKp(float kp){kp_=kp;}
	void SetKi(float ki){ki_=ki;}
	void SetKd(float kd){kd_=kd;}
};


#endif /* CONTROLLER_H_ */

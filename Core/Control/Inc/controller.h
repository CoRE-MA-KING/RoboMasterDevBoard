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
	float Kp,Ki,Kd;
	float period_ms;
	float reference;
	float e_sum;
	float pre_input;
public:
	PID(float _Kp,float _Ki,float _Kd,float _period_ms);
	float Update(float input);
	void SetReference(float ref);
	void Reset(){e_sum=0;pre_input=0;}
	void SetKp(float _Kp){Kp=_Kp;}
	void SetKi(float _Ki){Ki=_Ki;}
	void SetKd(float _Kd){Kd=_Kd;}
};


#endif /* CONTROLLER_H_ */

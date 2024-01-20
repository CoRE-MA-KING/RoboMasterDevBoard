/*
 * motor.h
 *
 *  Created on: Jan 10, 2024
 *      Author: mmaakkyyii
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "stdint.h"
#include "can.h"

class MotorBase{
protected:
	int dir;
public:
	MotorBase(int _dir);
};

class CanInterface{
private:

public:
	CAN_HandleTypeDef* hcanx;
	uint16_t current_data[8];

	uint16_t current_raw[8];
	uint16_t position_raw[8];
	uint16_t velocity_raw[8];
	uint16_t temp_raw[8];

	CanInterface(CAN_HandleTypeDef* _hcanx);
	void Start();
	void SendData();
};

class C620 :public MotorBase{
private:
	const int max_current_data=16384;
	const float max_current_value_mA=20000;
	const float max_position_value_rad=3.141592;
	const int max_position_data=8191;

	CanInterface* can_bus;
	int id;
public:
	C620(CanInterface* _can_bus,int _id,int _dir);
	void Init();
	void SetCurrent_mA(float I_mA);
	float GetCurrent_mA();
	float GetPotion_rad();
	float GetVelocity_rad_s();
	float GetTemp_degC();
};

void SabertoothDualInit();
void SabertoothDualSetVoltage(int ch, float Voltage_V);

#endif /* MOTOR_H_ */

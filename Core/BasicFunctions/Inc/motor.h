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
	void Init();
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

class DJI :public MotorBase{
private:
	const int max_current_data=16384;
	const float max_current_value_mA=20000;
	const float max_position_value_rad=3.141592;
	const int max_position_data=8191;
	const float gera_ratio=1;
	float radius_mm;

	CanInterface* can_bus;
	int id;
public:
	DJI(CanInterface* _can_bus,int _id,int _dir, float _radius_mm);
	void Init();
	void SetCurrent_mA(float I_mA);
	float GetCurrent_mA();
	float GetPotion_rad();
	float GetVelocity_rad_s();
	float GetTemp_degC();
	float GetVelocity_mm_s();
};

class C620 :public DJI{
private:
	const int max_current_data=16384;
	const float max_current_value_mA=20000;
	const float max_position_value_rad=3.141592;
	const int max_position_data=8191;
	const float gera_ratio=3591.0/187.0;

	CanInterface* can_bus;
	int id;
public:
	C620(CanInterface* _can_bus,int _id,int _dir, float _radius_mm):DJI(_can_bus, _id, _dir,_radius_mm){};
};

class C610 :public DJI{
private:
	const int max_current_data=16384;
	const float max_current_value_mA=10000;
	const float max_position_value_rad=3.141592;
	const int max_position_data=8191;
	float radius_mm;

	CanInterface* can_bus;
	int id;
public:
	C610(CanInterface* _can_bus,int _id,int _dir,float _radius_mm):DJI(_can_bus, _id, _dir, _radius_mm){};
};

class SabertoothDual: public MotorBase{
private:
	float supply_voltage_V=24;

	int ch;
public:
	SabertoothDual(int _ch, int _dir);
	void Init();
	void SetVoltage_V(float Voltage_V);
};

#endif /* MOTOR_H_ */

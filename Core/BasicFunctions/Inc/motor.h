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
	int dir_;
public:
	MotorBase(int dir);
	void Init();
};

class CanInterface{
private:

public:
	CAN_HandleTypeDef* hcanx_;
	uint16_t current_data_[8];

	uint16_t current_raw_[8];
	uint16_t position_raw_[8];
	uint16_t velocity_raw_[8];
	uint16_t temp_raw_[8];

	CanInterface(CAN_HandleTypeDef* hcanx);
	void Start();
	void SendData();
};

class DJI :public MotorBase{
private:
	CanInterface* can_bus_;

	const int kMaxCurrentData=16384;
	const float kMaxCurrentValue_mA=20000;
	const float kMaxPositionValue_rad=3.141592;
	const int kMaxPositionData=8191;
	const float gera_ratio_=1;

	float radius_mm_;
	int id_;
public:
	DJI(CanInterface* _can_bus,int _id,int _dir, float _radius_mm);
	void Init();
	void SetCurrent_mA(float i_mA);
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
	float supply_voltage_V_=24;

	int ch_;
public:
	SabertoothDual(int ch, int dir);
	void Init();
	void SetVoltage_V(float voltage_V);
};

#endif /* MOTOR_H_ */

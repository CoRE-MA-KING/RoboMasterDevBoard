/*
 * state.hpp
 *
 *  Created on: Feb 23, 2024
 *      Author: mmaakkyyii
 */

#ifndef _STATE_H_
#define _STATE_H_

#include "filter.h"


enum class Event{
kNone,
kFinishInitMPU,
kFinishInitMachine,
kEswPush,
kEswRelease,
kBreakSignal,
kRevibal,
kCommunicationError,
kCommunicationOk
};

enum class Mode{
kMPUInit=-1,
kMPUInitEswPushed=-2,
kMachineInit=1,
kNormal=2,
kMachineBreak=3,
kEmergencyStop=4,
kCommunicationError=5
};

class MachineMode{
public:
	MachineMode(){};
	virtual void Init(){};
	virtual void Update(){};
	virtual void Update_10ms(){};
};


class State{
private:
	MachineMode* machine_mode_;
	Event event_;
	Mode mode_;

	int pre_esw_;
public:
	State();
	void Init();
	void SetMode(Mode mode);
	void ChaekEvent();
	void Update(){machine_mode_->Update();};
	void Update10ms(){machine_mode_->Update_10ms();};
};

class MachineInitMode: public MachineMode{
private:
	bool finish_flag_=false;
	const int kPowerOnTime_ms_=4000;
	int power_on_timer_=0;
	int pitch_dir_=-1;
public:
	MachineInitMode();
	void Init();
	void Update();
	void Update_10ms();
	bool isFinishInit(){return finish_flag_;};

};

class NromalMode: public MachineMode{
private:
	int loading_motor_ref_=0;

	float vx_mm_s_=0;
	float vy_mm_s_=0;
	float omega_rad_s_=0;

	int photo1_=0;
	int pre_hoto1_=0;
	float roller_voltage_V_=0;
	float roller_voltage_max_V_=10.0;
	float target_pitch_pos_=0;

	MovingAverage filter_x_;
	MovingAverage filter_y_;

	bool shoot_enable_=true;

public:
	NromalMode():filter_x_(50),filter_y_(50){}
	void Init();
	void Update();
	void Update_10ms();
};

class ESWMode: public MachineMode{
public:
	ESWMode();
	void Init();
	void Update();
	void Update_10ms();
};

class BreakMode: public MachineMode{
public:
	BreakMode();
	void Init();
	void Update();
	void Update_10ms();

};

class CommunicationErrorMode: public MachineMode{
public:
	CommunicationErrorMode();
	void Init();
	void Update();
	void Update_10ms();

};


#endif /* _STATE_H_ */

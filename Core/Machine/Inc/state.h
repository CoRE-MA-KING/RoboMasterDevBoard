/*
 * state.hpp
 *
 *  Created on: Feb 23, 2024
 *      Author: mmaakkyyii
 */

#ifndef _STATE_H_
#define _STATE_H_



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
kMPUInit,
kMPUInitEswPushed,
kMachineInit,
kNormal,
kMachineBreak,
kEmergencyStop,
kCommunicationError
};

class MachineMode{
public:
	MachineMode(){};
	virtual void Init(){};
	virtual void Update(){};
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
	float roller_voltage_max_V=10.0;

	float moving_average_vx;

public:
	NromalMode(){}
	void Init();
	void Update();
};

class ESWMode: public MachineMode{
public:
	ESWMode();
	void Init();
	void Update();
};

class BreakMode: public MachineMode{
public:
	BreakMode();
	void Init();
	void Update();

};

class CommunicationErrorMode: public MachineMode{
public:
	CommunicationErrorMode();
	void Init();
	void Update();

};


#endif /* _STATE_H_ */

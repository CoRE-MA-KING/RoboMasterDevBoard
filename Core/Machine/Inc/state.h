/*
 * state.hpp
 *
 *  Created on: Feb 23, 2024
 *      Author: mmaakkyyii
 */

#ifndef _STATE_H_
#define _STATE_H_



enum class Event{
none,
finish_init_MPU,
finish_init_machine,
esw_push,
esw_release,
break_signal,
revibal,
communication_error,
communication_ok
};

enum class Mode{
MPU_init,
MPU_init_ESW_pushed,
machine_init,
normal,
machine_break,
ESW_pushed,
communication_error
};

class MachineMode{
public:
	MachineMode(){};
	void Init(){};
	void Update(){};
};


class State{
private:
	MachineMode machine_mode_;
	Event event_;
	Mode mode_;

	int pre_esw_;
public:
	State();
	void Init(){};
	void SetMode(Mode mode){mode_=mode;};
	void ChaekEvent();
	void Update(){machine_mode_.Update();};
};

class MachineInitMode: public MachineMode{
private:
	bool finish_flag_=false;
	const int power_on_time_ms_=2000;
	int power_on_timer_=0;
	int pitch_dir_=-1;
public:
	MachineInitMode();
	void Init();
	void Update();
	bool isFinishInit(){return finish_flag_;};

};

class NromalMode: public MachineMode{
public:
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

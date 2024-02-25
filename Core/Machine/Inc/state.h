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
	MachineMode machine_mode;
	Event event;
	Mode mode;

	int pre_esw;
public:
	State();
	void Init(){};
	void SetMode(Mode _mode){mode=_mode;};
	void ChaekEvent();
	void Update(){machine_mode.Update();};
};

class MachineInitMode: public MachineMode{
private:
	bool finish_flag=false;
	const int power_on_time_ms=2000;
	int power_on_timer=0;
	int pitch_dir=-1;
public:
	MachineInitMode();
	void Init();
	void Update();
	bool isFinishInit(){return finish_flag;};

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
	void Update();

};

class CommunicationErrorMode: public MachineMode{
	void Update();

};


#endif /* _STATE_H_ */

/*
 * state.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: mmaakkyyii
 */

#include "state.h"
#include "gpio.h"
#include "machine_variable.h"

MachineInitMode machine_init_mode;
NromalMode normal_mode;
ESWMode esw_mode;
BreakMode break_mode;
CommunicationErrorMode communication_error_mode;

State::State(){

}

void State::ChaekEvent(){
	event_=Event::none;

	if(machine_init_mode.isFinishInit()){
		event_=Event::finish_init_machine;
	}

	const GPIO_PinState kMachneBrake=GPIO_PIN_RESET;
	const GPIO_PinState kMachineAlive=GPIO_PIN_SET;
	static GPIO_PinState pre_machine_vitale=kMachineAlive;
	GPIO_PinState machine_vitale=HAL_GPIO_ReadPin(BREAK_SIGNAL_GPIO_Port, BREAK_SIGNAL_Pin);
	if(pre_machine_vitale==kMachineAlive
		&& machine_vitale==kMachneBrake){
		event_=Event::break_signal;
	}
	if(pre_machine_vitale==kMachneBrake
		&& machine_vitale==kMachineAlive){
		event_=Event::revibal;
	}
	pre_machine_vitale=machine_vitale;


	bool commu_status=cwcr.isCommunicationError();
	static bool pre_commu_status;
	if(commu_status==true && pre_commu_status==false){
		event_=Event::communication_error;
	}else if(commu_status==false && pre_commu_status==true){
		event_=Event::communication_ok;
	}
	pre_commu_status=commu_status;


	const GPIO_PinState kEswPushed=GPIO_PIN_RESET;
	const GPIO_PinState kEswRelesed=GPIO_PIN_SET;
	static GPIO_PinState pre_ESW=kEswRelesed;
	GPIO_PinState ESW=HAL_GPIO_ReadPin(ESW_GPIO_Port, ESW_Pin);
	if(pre_ESW==kEswRelesed
		&& ESW==kEswPushed){
		event_=Event::esw_push;
	}
	if(pre_ESW==kEswPushed
		&& ESW==kEswRelesed){
		event_=Event::esw_release;
	}
	pre_ESW=ESW;


//////////
//
//////////
	switch(mode_){
	case Mode::MPU_init:
		if(event_==Event::finish_init_MPU){
			mode_=Mode::machine_init;
			machine_mode_=machine_init_mode;
			machine_mode_.Init();
		}
		break;
	case Mode::MPU_init_ESW_pushed:
		if(event_==Event::finish_init_MPU){
			mode_=Mode::ESW_pushed;
			machine_mode_=esw_mode;
			machine_mode_.Init();
		}
		break;
	case Mode::machine_init:
		if(event_==Event::esw_push){
			mode_=Mode::ESW_pushed;
			machine_mode_=esw_mode;
			machine_mode_.Init();
		}
		else if(event_==Event::break_signal){
			mode_=Mode::machine_break;
			machine_mode_=break_mode;
			machine_mode_.Init();
		}
		else if(event_==Event::communication_error){
			mode_=Mode::communication_error;
			machine_mode_=communication_error_mode;
			machine_mode_.Init();
		}
		else if(event_==Event::finish_init_machine){
			mode_=Mode::normal;
			machine_mode_=normal_mode;
			machine_mode_.Init();
		}
		break;
	case Mode::normal:
		if(event_==Event::esw_push){
			mode_=Mode::ESW_pushed;
			machine_mode_=esw_mode;
			machine_mode_.Init();
		}
		else if(event_==Event::break_signal){
			mode_=Mode::machine_break;
			machine_mode_=break_mode;
			machine_mode_.Init();
		}
		else if(event_==Event::communication_error){
			mode_=Mode::communication_error;
			machine_mode_=communication_error_mode;
			machine_mode_.Init();
		}
		break;
	case Mode::machine_break:
		if(event_==Event::esw_push){
			mode_=Mode::ESW_pushed;
			machine_mode_=esw_mode;
			machine_mode_.Init();
		}
		else if(event_==Event::communication_error){
			mode_=Mode::communication_error;
			machine_mode_=communication_error_mode;
			machine_mode_.Init();
		}
		else if(event_==Event::revibal){
			mode_=Mode::machine_init;
			machine_mode_=machine_init_mode;
			machine_mode_.Init();
		}
		break;
	case Mode::ESW_pushed:
		if(event_==Event::esw_release){
			mode_=Mode::machine_init;
			machine_mode_=machine_init_mode;
			machine_mode_.Init();
		}
		break;
	case Mode::communication_error:
		if(event_==Event::esw_push){
			mode_=Mode::ESW_pushed;
			machine_mode_=esw_mode;
			machine_mode_.Init();
		}
		else if(event_==Event::communication_ok){
			mode_=Mode::machine_init;
			machine_mode_=machine_init_mode;
			machine_mode_.Init();
		}
		break;
	}

}

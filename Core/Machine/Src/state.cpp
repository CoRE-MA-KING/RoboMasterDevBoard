/*
 * state.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: mmaakkyyii
 */

#include "state.h"
#include "gpio.h"
#include "machine_variable.h"

State::State(){

}

void State::ChaekEvent(){
	event=Event::none;

	if(machine_init_mode.isFinishInit()){
		event=Event::finish_init_machine;
	}

	const GPIO_PinState MACHINE_BREAK=GPIO_PIN_RESET;
	const GPIO_PinState MACHINE_ALIVE=GPIO_PIN_SET;
	static GPIO_PinState pre_machine_vitale=MACHINE_ALIVE;
	GPIO_PinState machine_vitale=HAL_GPIO_ReadPin(BREAK_SIGNAL_GPIO_Port, BREAK_SIGNAL_Pin);
	if(pre_machine_vitale==MACHINE_ALIVE
		&& machine_vitale==MACHINE_BREAK){
		event=Event::break_signal;
	}
	if(pre_machine_vitale==MACHINE_BREAK
		&& machine_vitale==MACHINE_ALIVE){
		event=Event::revibal;
	}
	pre_machine_vitale=machine_vitale;


	bool commu_status=cwcr.isCommunicationError();
	static bool pre_commu_status;
	if(commu_status==true && pre_commu_status==false){
		event=Event::communication_error;
	}else if(commu_status==false && pre_commu_status==true){
		event=Event::communication_ok;
	}
	pre_commu_status=commu_status;


	const GPIO_PinState ESW_PUSHED=GPIO_PIN_RESET;
	const GPIO_PinState ESW_RELESED=GPIO_PIN_SET;
	static GPIO_PinState pre_ESW=ESW_RELESED;
	GPIO_PinState ESW=HAL_GPIO_ReadPin(ESW_GPIO_Port, ESW_Pin);
	if(pre_ESW==ESW_RELESED
		&& ESW==ESW_PUSHED){
		event=Event::esw_push;
	}
	if(pre_ESW==ESW_PUSHED
		&& ESW==ESW_RELESED){
		event=Event::esw_release;
	}
	pre_ESW=ESW;


//////////
//
//////////
	switch(mode){
	case Mode::MPU_init:
		if(event==Event::finish_init_MPU){
			mode=Mode::machine_init;
			machine_mode=machine_init_mode;
			machine_mode.Init();
		}
		break;
	case Mode::MPU_init_ESW_pushed:
		if(event==Event::finish_init_MPU){
			mode=Mode::ESW_pushed;
			machine_mode=esw_mode;
			machine_mode.Init();
		}
		break;
	case Mode::machine_init:
		if(event==Event::esw_push){
			mode=Mode::ESW_pushed;
			machine_mode=esw_mode;
			machine_mode.Init();
		}
		else if(event==Event::break_signal){
			mode=Mode::machine_break;
			machine_mode=break_mode;
			machine_mode.Init();
		}
		else if(event==Event::communication_error){
			mode=Mode::communication_error;
			machine_mode=communication_error_mode;
			machine_mode.Init();
		}
		else if(event==Event::finish_init_machine){
			mode=Mode::normal;
			machine_mode=normal_mode;
			machine_mode.Init();
		}
		break;
	case Mode::normal:
		if(event==Event::esw_push){
			mode=Mode::ESW_pushed;
			machine_mode=esw_mode;
			machine_mode.Init();
		}
		else if(event==Event::break_signal){
			mode=Mode::machine_break;
			machine_mode=break_mode;
			machine_mode.Init();
		}
		else if(event==Event::communication_error){
			mode=Mode::communication_error;
			machine_mode=communication_error_mode;
			machine_mode.Init();
		}
		break;
	case Mode::machine_break:
		if(event==Event::esw_push){
			mode=Mode::ESW_pushed;
			machine_mode=esw_mode;
			machine_mode.Init();
		}
		else if(event==Event::communication_error){
			mode=Mode::communication_error;
			machine_mode=communication_error_mode;
			machine_mode.Init();
		}
		else if(event==Event::revibal){
			mode=Mode::machine_init;
			machine_mode=machine_init_mode;
			machine_mode.Init();
		}
		break;
	case Mode::ESW_pushed:
		if(event==Event::esw_release){
			mode=Mode::machine_init;
			machine_mode=machine_init_mode;
			machine_mode.Init();
		}
		break;
	case Mode::communication_error:
		if(event==Event::esw_push){
			mode=Mode::ESW_pushed;
			machine_mode=esw_mode;
			machine_mode.Init();
		}
		else if(event==Event::communication_ok){
			mode=Mode::machine_init;
			machine_mode=machine_init_mode;
			machine_mode.Init();
		}
		break;
	}

}

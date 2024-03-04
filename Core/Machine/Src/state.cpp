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

void State::SetMode(Mode mode){
	mode_=mode;
	switch(mode_){
	case Mode::kEmergencyStop:
		machine_mode_=&esw_mode;
		machine_mode_->Init();
		break;
	case Mode::kMachineBreak:
		machine_mode_=&break_mode;
		machine_mode_->Init();
		break;
	case Mode::kMachineInit:
		machine_mode_=&machine_init_mode;
		machine_mode_->Init();
		break;
	case Mode::kNormal:
		machine_mode_=&normal_mode;
		machine_mode_->Init();
		break;
	case Mode::kCommunicationError:
		machine_mode_=&communication_error_mode;
		machine_mode_->Init();
		break;
	}
}
void State::Init(){
	machine_mode_=&machine_init_mode;
}
void State::ChaekEvent(){
	event_=Event::kNone;

	if(machine_init_mode.isFinishInit()){
		event_=Event::kFinishInitMachine;
	}

	const GPIO_PinState kMachneBrake=GPIO_PIN_RESET;
	const GPIO_PinState kMachineAlive=GPIO_PIN_SET;
	static GPIO_PinState pre_machine_vitale=kMachineAlive;
	GPIO_PinState machine_vitale=HAL_GPIO_ReadPin(BREAK_SIGNAL_GPIO_Port, BREAK_SIGNAL_Pin);
	if(pre_machine_vitale==kMachineAlive
		&& machine_vitale==kMachneBrake){
	//	event_=Event::kBreakSignal;
	}
	if(pre_machine_vitale==kMachneBrake
		&& machine_vitale==kMachineAlive){
	//	event_=Event::kRevibal;
	}
	pre_machine_vitale=machine_vitale;


	bool commu_status=cwcr.isCommunicationError();
	static bool pre_commu_status;
	if(commu_status==true && pre_commu_status==false){
//		event_=Event::kCommunicationError;
	}else if(commu_status==false && pre_commu_status==true){
		event_=Event::kCommunicationOk;
	}
	pre_commu_status=commu_status;


	const GPIO_PinState kEswPushed=GPIO_PIN_RESET;
	const GPIO_PinState kEswRelesed=GPIO_PIN_SET;
	static GPIO_PinState pre_ESW=kEswRelesed;
	GPIO_PinState ESW=HAL_GPIO_ReadPin(ESW_GPIO_Port, ESW_Pin);
	if(pre_ESW==kEswRelesed
		&& ESW==kEswPushed){
		event_=Event::kEswPush;
	}
	if(pre_ESW==kEswPushed
		&& ESW==kEswRelesed){
		event_=Event::kEswRelease;
	}
	pre_ESW=ESW;


//////////
//
//////////
	switch(mode_){
	case Mode::kMPUInit:
		if(event_==Event::kFinishInitMPU){
			mode_=Mode::kMachineInit;
			machine_mode_=&machine_init_mode;
			machine_mode_->Init();
		}
		break;
	case Mode::kMPUInitEswPushed:
		if(event_==Event::kFinishInitMPU){
			mode_=Mode::kEmergencyStop;
			machine_mode_=&esw_mode;
			machine_mode_->Init();
		}
		break;
	case Mode::kMachineInit:
		if(event_==Event::kEswPush){
			mode_=Mode::kEmergencyStop;
			machine_mode_=&esw_mode;
			machine_mode_->Init();
		}
		else if(event_==Event::kBreakSignal){
			mode_=Mode::kMachineBreak;
			machine_mode_=&break_mode;
			machine_mode_->Init();
		}
		else if(event_==Event::kCommunicationError){
			mode_=Mode::kCommunicationError;
			machine_mode_=&communication_error_mode;
			machine_mode_->Init();
		}
		else if(event_==Event::kFinishInitMachine){
			mode_=Mode::kNormal;
			machine_mode_=&normal_mode;
			machine_mode_->Init();
		}
		break;
	case Mode::kNormal:
		if(event_==Event::kEswPush){
			mode_=Mode::kEmergencyStop;
			machine_mode_=&esw_mode;
			machine_mode_->Init();
		}
		else if(event_==Event::kBreakSignal){
			mode_=Mode::kMachineBreak;
			machine_mode_=&break_mode;
			machine_mode_->Init();
		}
		else if(event_==Event::kCommunicationError){
			mode_=Mode::kCommunicationError;
			machine_mode_=&communication_error_mode;
			machine_mode_->Init();
		}
		break;
	case Mode::kMachineBreak:
		if(event_==Event::kEswPush){
			mode_=Mode::kEmergencyStop;
			machine_mode_=&esw_mode;
			machine_mode_->Init();
		}
		else if(event_==Event::kCommunicationError){
			mode_=Mode::kCommunicationError;
			machine_mode_=&communication_error_mode;
			machine_mode_->Init();
		}
		else if(event_==Event::kRevibal){
			mode_=Mode::kMachineInit;
			machine_mode_=&machine_init_mode;
			machine_mode_->Init();
		}
		break;
	case Mode::kEmergencyStop:
		if(event_==Event::kEswRelease){
			mode_=Mode::kMachineInit;
			machine_mode_=&machine_init_mode;
			machine_mode_->Init();
		}
		break;
	case Mode::kCommunicationError:
		if(event_==Event::kEswPush){
			mode_=Mode::kEmergencyStop;
			machine_mode_=&esw_mode;
			machine_mode_->Init();
		}
		else if(event_==Event::kCommunicationOk){
			mode_=Mode::kMachineInit;
			machine_mode_=&machine_init_mode;
			machine_mode_->Init();
		}
		break;
	}

}

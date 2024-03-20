/*
 * interrupt_func.cpp
 *
 *  Created on: Jan 8, 2024
 *      Author: mmaakkyyii
 */
#include "interrupt_func.h"
#include "stdint.h"
#include "stdio.h"

//#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "motor.h"
#include "encorder.h"

#include "machine_variable.h"
#include "machine_constant.hpp"
#include "core_wireless_control_rx.hpp"

#include "buzzer.h"
#include "state.h"

void Interrupt1ms(){

	buzzer.Update();
	roller_enc_L.Update();
	roller_enc_R.Update();

	pitch_servo.Update();

	cwcr.Update();

	// change video
	if(cwcr.button(2)==1){
		video_id=0;
	}else if(cwcr.button(3)==1){
		video_id=2;
	}else{
		video_id=1;
	}

	//buzzer check
	if(cwcr.button(15)==1){//△
		buzzer.SetFrequency(400,10);
	}
	//reload fresbee
	if(cwcr.button(10)==1){//share
		buzzer.SetFrequency(350,20);
		rec=true;
	}
	if(cwcr.button(11)==1){//option
		buzzer.SetFrequency(350,20);
		rec=false;
	}

	if(cwcr.button(12)==1 && cwcr.button(13)==1 && cwcr.button(13)==1 && cwcr.button(15)==1){//o x

		buzzer.SetFrequency(350,100);
	}



	state.Update();

	can1_bus.SendData();
	can2_bus.SendData();

	state.ChaekEvent();

	static int timer=0;
	timer++;
	if(timer>100){
		timer=0;
		state.Update10ms();
	}

}




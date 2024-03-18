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




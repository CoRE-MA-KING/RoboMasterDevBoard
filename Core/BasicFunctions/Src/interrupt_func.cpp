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

	roller_enc_L.Update();
	roller_enc_R.Update();

	state.Update();

	can1_bus.SendData();
	can2_bus.SendData();

	state.ChaekEvent();

}




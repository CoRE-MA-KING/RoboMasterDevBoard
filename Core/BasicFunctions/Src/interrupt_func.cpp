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

char s[128];

void Interrupt1ms(){
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);;

	SetC620Current_mA(1,400);
	SetC620Current_mA(2,400);

	C620Update();
	int deg1=(GetC620Potion_rad(1)*180/3.14);
	int deg2=(GetC620Potion_rad(2)*180/3.14);
	int current1=(int)GetC620Current_mA(1);
	int current2=(int)GetC620Current_mA(2);
	int n=sprintf(s,"%d,%d,%d,%d\r\n",deg1,current1,deg2,current2);
	CDC_Transmit_FS((uint8_t*)s, n);
}



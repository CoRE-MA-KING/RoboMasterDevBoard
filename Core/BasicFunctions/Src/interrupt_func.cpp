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

char s[128];

float v=0;
int dir=1;
void Interrupt1ms(){
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

	float r=160.0/2.0;

	float vel1=GetC620Velocity_rad_s(1)*r;
	float vel2=GetC620Velocity_rad_s(2)*r;
	float vel3=GetC620Velocity_rad_s(3)*r;
	float vel4=GetC620Velocity_rad_s(4)*r;

	float v_ref=2000;
	float Kp=1;

	SetC620Current_mA(1,Kp*(v_ref-vel1));
//	SetC620Current_mA(2,Kp*(-v_ref-vel2));
//	SetC620Current_mA(1,0);
	SetC620Current_mA(2,0);
	SetC620Current_mA(3,0);
	SetC620Current_mA(4,0);

	v+=dir*0.01;
	if(v>3){dir=-1;}
	if(v<-3){dir=1;}
	SabertoothDualSetVoltage(1,v);
	C620Update();
	int deg1=(GetC620Potion_rad(1)*180/3.14);
	int deg2=(GetC620Potion_rad(2)*180/3.14);
	int deg3=(GetC620Potion_rad(3)*180/3.14);
	int deg4=(GetC620Potion_rad(4)*180/3.14);
	int current1=(int)GetC620Current_mA(1);
	int current2=(int)GetC620Current_mA(2);
	int current3=(int)GetC620Current_mA(3);
	int current4=(int)GetC620Current_mA(4);
//	int n=sprintf(s,"%d,%d,%d,%d,%d\r\n",(int)(v*1000),deg1,current1,Encorder1Pulse(),Encorder2Pulse());
//	int n=sprintf(s,"%d,%d,%d,%d,%d,%d,%d,%d\r\n",deg1,deg2,deg3,deg4,current1,current2,current3,current4);
//	int n=sprintf(s,"%d,%d,%d,%d,%d,%d,%d,%d\r\n",(int)(vel1),(int)(vel2),(int)(vel3),(int)(vel4),current1,current2,current3,current4);
//	CDC_Transmit_FS((uint8_t*)s, n);
	  printf("%d,%d,%d,%d,%d,%d,%d,%d\r\n",(int)(vel1),(int)(vel2),(int)(vel3),(int)(vel4),current1,current2,current3,current4);

}



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

char s[128];

float v=0;
int dir=1;
void Interrupt1ms(){
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

	roller_enc_L.Update();
	roller_enc_R.Update();

	float r=160.0/2.0;

	float vel1=motor1.GetVelocity_rad_s()*r;
	float vel2=motor2.GetVelocity_rad_s()*r;
	float vel3=motor3.GetVelocity_rad_s()*r;
	float vel4=motor4.GetVelocity_rad_s()*r;

	float v_ref=500;
	float Kp=1;

	v+=dir*0.01;
	if(v>3){dir=-1;}
	if(v<-3){dir=1;}
	rollerL.SetVoltage_V(v);
	rollerR.SetVoltage_V(v);


	int deg1=(motor1.GetPotion_rad()*180/3.14);
	int deg2=(motor2.GetPotion_rad()*180/3.14);
	int deg3=(motor3.GetPotion_rad()*180/3.14);
	int deg4=(motor4.GetPotion_rad()*180/3.14);

	int current1=(int)motor1.GetCurrent_mA();
	int current2=(int)motor2.GetCurrent_mA();
	int current3=(int)motor3.GetCurrent_mA();
	int current4=(int)motor4.GetCurrent_mA();

	motor1.SetCurrent_mA(Kp*(v_ref-vel1));
	motor2.SetCurrent_mA(Kp*(v_ref-vel2));
	motor3.SetCurrent_mA(Kp*(v_ref-vel3));
	motor4.SetCurrent_mA(Kp*(v_ref-vel4));


	can1_bus.SendData();

	static int pulseL=0;
	pulseL+=roller_enc_L.GetPulse();
	//	int n=sprintf(s,"%d,%d,%d,%d,%d\r\n",(int)(v*1000),deg1,current1,Encorder1Pulse(),Encorder2Pulse());
//	int n=sprintf(s,"%d,%d,%d,%d,%d,%d,%d,%d\r\n",deg1,deg2,deg3,deg4,current1,current2,current3,current4);
//	int n=sprintf(s,"%d,%d,%d,%d,%d,%d,%d,%d\r\n",(int)(vel1),(int)(vel2),(int)(vel3),(int)(vel4),current1,current2,current3,current4);
//	CDC_Transmit_FS((uint8_t*)s, n);
//	  printf("%d,%d,%d,%d,%d,%d,%d,%d\r\n",(int)(vel1),(int)(vel2),(int)(vel3),(int)(vel4),current1,current2,current3,current4);
//	  printf("%d,%d,%d,%d,\r\n",(int)(vel1),current1,0,0);
	printf("%4d,%3d,%3d\r\n",pulseL,roller_enc_L.GetPulse(),roller_enc_R.GetPulse());
}



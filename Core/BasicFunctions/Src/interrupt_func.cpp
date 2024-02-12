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

void Interrupt1ms(){
	if(ESW!=HAL_GPIO_ReadPin(ESW_GPIO_Port, ESW_Pin)){
		HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
		setBuzzerFrequency(500);
		return;
	}
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

	roller_enc_L.Update();
	roller_enc_R.Update();
	static int loading_motor_ref=0;

	static int x=0;
	static int y=0;
	if(cwcr.parse() == true){
		x=cwcr.axis(3)*50;
		y=cwcr.axis(1)*50;
		if(x<0)x=0;
		if(y<0)y=0;
		if(cwcr.button(1)==1){
			loading_motor_ref=5000;
		}
//		printf("!-- %d, %d, %d, %d, %d\r\n"
//			, cwcr.axis(0), cwcr.axis(1), cwcr.axis(2), cwcr.axis(3), cwcr.axis(4)
//		);
//		printf("!--\r\n%d, %d, %d, %d, %d\r\n%d, %d, %d, %d\r\n%d, %d, %d, %d\r\n%d, %d, %d, %d\r\n%d, %d, %d, %d\r\n"
//			, cwcr.axis(0), cwcr.axis(1), cwcr.axis(2), cwcr.axis(3), cwcr.axis(4)
//			, cwcr.button(0), cwcr.button(1), cwcr.button(2), cwcr.button(3)
//			, cwcr.button(4), cwcr.button(5), cwcr.button(6), cwcr.button(7)
//			, cwcr.button(8), cwcr.button(9), cwcr.button(10), cwcr.button(11)
//			, cwcr.button(12), cwcr.button(13), cwcr.button(14), cwcr.button(15)
//		);
	}

	rollerL.SetVoltage_V(y*6.0/2000.0);
	rollerR.SetVoltage_V(x*6.0/2000.0);

	///////////////

	float vel1=motor1.GetVelocity_mm_s();
	float vel2=motor2.GetVelocity_mm_s();
	float vel3=motor3.GetVelocity_mm_s();
	float vel4=motor4.GetVelocity_mm_s();


	m1_pid.SetReference(0);
	m2_pid.SetReference(500);
	m3_pid.SetReference(0);
	m4_pid.SetReference(0);

/*
	int deg1=(motor1.GetPotion_rad()*180/3.14);
	int deg2=(motor2.GetPotion_rad()*180/3.14);
	int deg3=(motor3.GetPotion_rad()*180/3.14);
	int deg4=(motor4.GetPotion_rad()*180/3.14);
*/
	int current1=(int)motor1.GetCurrent_mA();
	int current2=(int)motor2.GetCurrent_mA();
	int current3=(int)motor3.GetCurrent_mA();
	int current4=(int)motor4.GetCurrent_mA();

	float target_current1 = m1_pid.Update(vel1);
	float target_current2 = m2_pid.Update(vel2);
	float target_current3 = m3_pid.Update(vel3);
	float target_current4 = m4_pid.Update(vel4);
	target_current2=300;
	motor1.SetCurrent_mA(target_current1);
	motor2.SetCurrent_mA(target_current2);
	motor3.SetCurrent_mA(target_current3);
	motor4.SetCurrent_mA(target_current4);


	int photo1=HAL_GPIO_ReadPin(PHOTO_SENS1_GPIO_Port, PHOTO_SENS1_Pin);
	static int pre_hoto1;
	if(photo1==1 && pre_hoto1==0){
		loading_motor_ref=0;
	}
	pre_hoto1=photo1;

	int loading_current_mA = loading_motor.GetCurrent_mA();
	float v_loading_rpm=loading_motor.GetVelocity_rad_s()/(2*3.14)*60;

	loading_motor_pid.SetReference(loading_motor_ref);
	loading_motor_pid.Update(v_loading_rpm);

	float target_current_loading = loading_motor_pid.Update(v_loading_rpm);
	loading_motor.SetCurrent_mA(target_current_loading);


	can1_bus.SendData();
	can2_bus.SendData();

	//	int n=sprintf(s,"%d,%d,%d,%d,%d\r\n",(int)(v*1000),deg1,current1,Encorder1Pulse(),Encorder2Pulse());
//	int n=sprintf(s,"%d,%d,%d,%d,%d,%d,%d,%d\r\n",deg1,deg2,deg3,deg4,current1,current2,current3,current4);
	printf("%d,%d,%d\r\n",(int)(vel2),current2,(int)target_current2);
//	CDC_Transmit_FS((uint8_t*)s, n);

//	printf("%d,%d,%d,%d,%d,%d\r\n",x,y,(int)(roller_enc_L.GetVelicty_mm_s()),(int)(roller_enc_R.GetVelicty_mm_s()),(int)target_current_loading,(int)v_loading_rpm,loading_current_mA);
//	  printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",x,y,(int)(vel1),(int)(vel2),(int)(vel3),(int)(vel4),current1,current2,current3,current4);
//	  printf("%d,%d,%d,%d,\r\n",(int)(vel1),current1,0,0);
//	printf("%4d,%3d,%3d\r\n",pulseL,roller_enc_L.GetPulse(),roller_enc_R.GetPulse());
}




/*
 * normal_mode.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: mmaakkyyii
 */

#include "state.h"
#include "stdio.h"
#include "machine_constant.hpp"
#include "machine_variable.h"


void NromalMode::Init(){
	printf("NromalMode!\r\n");
	m1_pid.Reset();
	m2_pid.Reset();
	m3_pid.Reset();
	m4_pid.Reset();
	loading_motor_pid.Reset();
	pitch_motor_pid.Reset();
}
void NromalMode::Update(){
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

	motor1.SetCurrent_mA(target_current1);
	motor2.SetCurrent_mA(target_current2);
	motor3.SetCurrent_mA(target_current3);
	motor4.SetCurrent_mA(target_current4);

	printf("%d,%d,%d\r\n",(int)(vel2),current2,(int)target_current2);



}



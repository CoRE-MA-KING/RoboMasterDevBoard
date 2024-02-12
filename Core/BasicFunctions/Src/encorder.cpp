/*
 * encorder.cpp
 *
 *  Created on: Jan 14, 2024
 *      Author: mmaakkyyii
 */

#include "encorder.h"
#include "tim.h"

Encorder::Encorder(int _ch, int _ppr,int _period_ms,int _radius_mm,int _dir):
ch(_ch),ppr(_ppr),period_ms(_period_ms),radius_mm(_radius_mm),dir(_dir),pulse(0)
{}
void Encorder::Init(){
	switch(ch){
	case 2:
		TIM2->CNT=ENC_ZERO;
		HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
		break;
	case 8:
		TIM8->CNT=ENC_ZERO;
		HAL_TIM_Encoder_Start(&htim8,TIM_CHANNEL_ALL);
		break;
	default:
		break;
	}

}
void Encorder::Update(){
	switch(ch){
	case 2:
		pulse=TIM2->CNT-ENC_ZERO;
		pulse*=dir;
		TIM2->CNT=ENC_ZERO;
		break;
	case 8:
		pulse=TIM8->CNT-ENC_ZERO;
		pulse*=dir;
		TIM8->CNT=ENC_ZERO;
		break;
	default:
		break;
	}

}
int Encorder::GetPulse(){
	return pulse;

}
float Encorder::GetVelocity_rpm(){
	return (float)pulse/ppr/period_ms*60000;

}
float Encorder::GetVelocity_rad_s(){
	return (float)pulse/ppr*(2*3.1415)/(period_ms*0.001);
}

float Encorder::GetVelicty_mm_s(){
	return GetVelocity_rad_s()*radius_mm;
}

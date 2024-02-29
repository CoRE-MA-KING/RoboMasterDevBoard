/*
 * encorder.cpp
 *
 *  Created on: Jan 14, 2024
 *      Author: mmaakkyyii
 */

#include "encorder.h"
#include "tim.h"

Encorder::Encorder(int ch, int ppr, int period_ms,int radius_mm,int dir):
ch_(ch),ppr_(ppr),period_ms_(period_ms),radius_mm_(radius_mm),dir_(dir),pulse_(0)
{}
void Encorder::Init(){
	switch(ch_){
	case 2:
		TIM2->CNT=kEncZero;
		HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
		break;
	case 8:
		TIM8->CNT=kEncZero;
		HAL_TIM_Encoder_Start(&htim8,TIM_CHANNEL_ALL);
		break;
	default:
		break;
	}

}
void Encorder::Update(){
	switch(ch_){
	case 2:
		pulse_=TIM2->CNT-kEncZero;
		pulse_*=dir_;
		TIM2->CNT=kEncZero;
		break;
	case 8:
		pulse_=TIM8->CNT-kEncZero;
		pulse_*=dir_;
		TIM8->CNT=kEncZero;
		break;
	default:
		break;
	}

}
int Encorder::GetPulse(){
	return pulse_;

}
float Encorder::GetVelocity_rpm(){
	return (float)pulse_/ppr_/period_ms_*60000;

}
float Encorder::GetVelocity_rad_s(){
	return (float)pulse_/ppr_*(2*3.1415)/(period_ms_*0.001);
}

float Encorder::GetVelicty_mm_s(){
	return GetVelocity_rad_s()*radius_mm_;
}

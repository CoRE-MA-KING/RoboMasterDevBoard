/*
 * encorder.cpp
 *
 *  Created on: Jan 14, 2024
 *      Author: mmaakkyyii
 */

#include "encorder.h"
#include "tim.h"

static const int16_t ENC_ZERO=0x7FFF;

void Encorder1Init(){
	TIM2->CNT=ENC_ZERO;
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
}
void Encorder2Init(){
	TIM8->CNT=ENC_ZERO;
	HAL_TIM_Encoder_Start(&htim8,TIM_CHANNEL_ALL);

}

int Encorder1Pulse(){
	return TIM2->CNT;
}
int Encorder2Pulse(){
	return TIM8->CNT;

}

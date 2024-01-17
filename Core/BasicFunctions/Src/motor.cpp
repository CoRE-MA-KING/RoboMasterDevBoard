/*
 * motor.cpp
 *
 *  Created on: Jan 10, 2024
 *      Author: mmaakkyyii
 */

#include "motor.h"
#include "can.h"
#include "tim.h"

uint16_t current_data[8];

uint16_t current_raw[8];
uint16_t position_raw[8];
uint16_t velocity_raw[8];
uint16_t temp_raw[8];


const int max_current_data=16384;
const float max_current_value_mA=20000;
const float max_position_value_rad=3.141592;
const int max_position_data=8191;
void SetC620Current_mA(int id,float I_mA){
	if(0<id && id <=8){
		current_data[id-1]=(int16_t)(I_mA/max_current_value_mA *max_current_data);
	}
}
float GetC620Current_mA(int id){
	return (float)(int16_t)current_raw[id-1]/max_current_data*max_current_value_mA;
}

float GetC620Potion_rad(int id){
	return (float)position_raw[id-1]/max_position_data*max_position_value_rad;

}
float GetC620Velocity_rad_s(int id){
	return (float)(int16_t)velocity_raw[id-1]*3.141592/60.0;

}
float GetC620Temp_degC(int id){
	return temp_raw[id-1];

}
void C620Init(){
	CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x201 << 5; // ID 0x201 を左に5ビットシフト
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000; // マスクは使用しない
	sFilterConfig.FilterMaskIdLow = 0x0000;  // マスクは使用しない
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;

	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);

	sFilterConfig.FilterBank = 1;
	sFilterConfig.FilterIdHigh = 0x202 << 5; // ID 2
	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
	sFilterConfig.FilterBank = 2;
	sFilterConfig.FilterIdHigh = 0x203 << 5; // ID 3
	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
	sFilterConfig.FilterBank = 3;
	sFilterConfig.FilterIdHigh = 0x204 << 5; // ID 4
	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);


	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Start(&hcan1);

	HAL_GPIO_WritePin(POWER1_CTRL_GPIO_Port, POWER1_CTRL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POWER2_CTRL_GPIO_Port, POWER2_CTRL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POWER3_CTRL_GPIO_Port, POWER3_CTRL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POWER4_CTRL_GPIO_Port, POWER4_CTRL_Pin, GPIO_PIN_SET);


}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[8];

	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK){
		int id = RxHeader.StdId & 0x00F; //C620 ID : 0x200| id
		position_raw[id-1]  =((uint16_t)RxData[0])<<8 | (uint16_t)RxData[1];
		velocity_raw[id-1]  =((uint16_t)RxData[2])<<8 | (uint16_t)RxData[3];
		current_raw[id-1]	=((uint16_t)RxData[4])<<8 | (uint16_t)RxData[5];
		temp_raw[id-1]   	=RxData[6];
	}
}
void C620Update(){
	CAN_TxHeaderTypeDef TxHeadrt;
	uint8_t TxData[8];
	uint32_t TxMailbox;
	TxHeadrt.StdId=0x200;
	TxHeadrt.ExtId=0x0;
	TxHeadrt.RTR=CAN_RTR_DATA;
	TxHeadrt.IDE=CAN_ID_STD;
	TxHeadrt.DLC=8;
	for(int i=0;i<4;i++){
		TxData[2*i]  = (current_data[i]>>8)&0xff;
		TxData[2*i+1]=  current_data[i]&0xff;
	}

	HAL_CAN_AddTxMessage(&hcan1, &TxHeadrt, TxData, &TxMailbox);
}


void SabertoothDualInit(){
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 1500);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);

}
float supply_voltage_V=24;

void SabertoothDualSetVoltage(int ch, float Voltage_V){
	if(Voltage_V> supply_voltage_V)Voltage_V = supply_voltage_V;
	if(Voltage_V<-supply_voltage_V)Voltage_V =-supply_voltage_V;

	switch(ch){
	case 1:
	    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500+500*Voltage_V/supply_voltage_V);
	    break;
	case 2:
	    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 1500+500*Voltage_V/supply_voltage_V);
	    break;
	}


}



/*
 * motor.cpp
 *
 *  Created on: Jan 10, 2024
 *      Author: mmaakkyyii
 */

#include "motor.h"
#include "can.h"
#include "tim.h"
#include "machine_variable.h"

MotorBase::MotorBase(int _dir):dir(_dir){

}
CanInterface::CanInterface(CAN_HandleTypeDef* _hcanx):hcanx(_hcanx){

}

void CanInterface::Start(){
	HAL_CAN_ActivateNotification(hcanx, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Start(hcanx);
}

void CanInterface::SendData(){
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

	HAL_CAN_AddTxMessage(hcanx, &TxHeadrt, TxData, &TxMailbox);
}

DJI::DJI(CanInterface* _can_bus,int _id,int _dir):MotorBase(_dir),can_bus(_can_bus),id(_id){
}

void DJI::Init(){
	CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterBank = id-1;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = (0x200 + id) << 5; //
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000; // マスクは使用しない
	sFilterConfig.FilterMaskIdLow = 0x0000;  // マスクは使用しない
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;

	HAL_CAN_ConfigFilter(can_bus->hcanx, &sFilterConfig);

}

void DJI::SetCurrent_mA(float I_mA){
	if(0<id && id <=8){
		can_bus->current_data[id-1]= dir * (int16_t)(I_mA/max_current_value_mA *max_current_data);
	}
}
float DJI::GetCurrent_mA(){
	return dir * (float)(int16_t)can_bus->current_raw[id-1]/max_current_data*max_current_value_mA;
}
float DJI::GetPotion_rad(){
	return (float)can_bus->position_raw[id-1]/max_position_data*max_position_value_rad;
}
float DJI::GetVelocity_rad_s(){
	return dir * (float)(int16_t)can_bus->velocity_raw[id-1]*3.141592/60.0;
}
float DJI::GetTemp_degC(){
	return can_bus->temp_raw[id-1];
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[8];

	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK){
		int id = RxHeader.StdId & 0x00F; //C620 ID : 0x200| id
		can1_bus.position_raw[id-1]  =((uint16_t)RxData[0])<<8 | (uint16_t)RxData[1];
		can1_bus.velocity_raw[id-1]  =((uint16_t)RxData[2])<<8 | (uint16_t)RxData[3];
		can1_bus.current_raw[id-1]	=((uint16_t)RxData[4])<<8 | (uint16_t)RxData[5];
		can1_bus.temp_raw[id-1]   	=RxData[6];
	}
}

SabertoothDual::SabertoothDual(int _ch, int _dir):
MotorBase(_dir),ch(_ch)
{

}

void SabertoothDual::Init(){
	switch(ch){
	case 1:
	    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500);
	    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
		break;
	case 2:
	    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 1500);
	    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
		break;
	default:
		break;
	}
}
void SabertoothDual::SetVoltage_V(float Voltage_V){
	if(Voltage_V> supply_voltage_V)Voltage_V = supply_voltage_V;
	if(Voltage_V<-supply_voltage_V)Voltage_V =-supply_voltage_V;

	Voltage_V*=dir;

	switch(ch){
	case 1:
	    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500+500*Voltage_V/supply_voltage_V);
	    break;
	case 2:
	    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 1500+500*Voltage_V/supply_voltage_V);
	    break;
	}
}



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

MotorBase::MotorBase(int dir):dir_(dir){

}
CanInterface::CanInterface(CAN_HandleTypeDef* hcanx):hcanx_(hcanx){

}

void CanInterface::Start(){
	HAL_CAN_ActivateNotification(hcanx_, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Start(hcanx_);
}

void CanInterface::SendData(){
	CAN_TxHeaderTypeDef tx_headrt;
	uint8_t tx_data[8];
	uint32_t tx_mailbox;
	tx_headrt.StdId=0x200;
	tx_headrt.ExtId=0x0;
	tx_headrt.RTR=CAN_RTR_DATA;
	tx_headrt.IDE=CAN_ID_STD;
	tx_headrt.DLC=8;
	for(int i=0;i<4;i++){
		tx_data[2*i]  = (current_data_[i]>>8)&0xff;
		tx_data[2*i+1]=  current_data_[i]&0xff;
	}

	HAL_CAN_AddTxMessage(hcanx_, &tx_headrt, tx_data, &tx_mailbox);
}

DJI::DJI(CanInterface* can_bus,int id, int dir, float radius_mm, float _gera_ratio):
	MotorBase(dir),can_bus_(can_bus),radius_mm_(radius_mm),id_(id),gera_ratio_(_gera_ratio){
}

void DJI::Init(){
	CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterBank = id_-1;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = (0x200 + id_) << 5; //
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000; // マスクは使用しない
	sFilterConfig.FilterMaskIdLow = 0x0000;  // マスクは使用しない
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;

	HAL_CAN_ConfigFilter(can_bus_->hcanx_, &sFilterConfig);

}

void DJI::SetCurrent_mA(float i_mA){
	if(0<id_ && id_ <=8){
		can_bus_->current_data_[id_-1]= dir_ * (int16_t)(i_mA/kMaxCurrentValue_mA *kMaxCurrentData);
	}
}
float DJI::GetCurrent_mA(){
	return dir_ * (float)(int16_t)can_bus_->current_raw_[id_-1]/kMaxCurrentData*kMaxCurrentValue_mA;
}
float DJI::GetPotion_rad(){
	return (float)can_bus_->position_raw_[id_-1]/kMaxPositionData*kMaxPositionValue_rad;
}
float DJI::GetVelocity_rad_s(){//todo rpm*2*pi/60
	return dir_ * (float)(int16_t)can_bus_->velocity_raw_[id_-1]*3.141592/60.0/gera_ratio_;
}
float DJI::GetVelocity_mm_s(){
	return dir_ * (float)(int16_t)can_bus_->velocity_raw_[id_-1]*2*3.141592/60.0/gera_ratio_*radius_mm_;
}

float DJI::GetTemp_degC(){
	return can_bus_->temp_raw_[id_-1];
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	if(hcan==&hcan1){
		CAN_RxHeaderTypeDef rx_header;
		uint8_t rx_data[8];
		if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data) == HAL_OK){
			int id = rx_header.StdId & 0x00F; //C620 ID : 0x200| id
			can1_bus.position_raw_[id-1]  =((uint16_t)rx_data[0])<<8 | (uint16_t)rx_data[1];
			can1_bus.velocity_raw_[id-1]  =((uint16_t)rx_data[2])<<8 | (uint16_t)rx_data[3];
			can1_bus.current_raw_[id-1]	=((uint16_t)rx_data[4])<<8 | (uint16_t)rx_data[5];
			can1_bus.temp_raw_[id-1]   	=rx_data[6];
		}
	}
	if(hcan==&hcan2){
		CAN_RxHeaderTypeDef rx_header;
		uint8_t rx_data[8];
		if (HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &rx_header, rx_data) == HAL_OK){
			int id = rx_header.StdId & 0x00F; //C620 ID : 0x200| id
			can2_bus.position_raw_[id-1]  =((uint16_t)rx_data[0])<<8 | (uint16_t)rx_data[1];
			can2_bus.velocity_raw_[id-1]  =((uint16_t)rx_data[2])<<8 | (uint16_t)rx_data[3];
			can2_bus.current_raw_[id-1]	=((uint16_t)rx_data[4])<<8 | (uint16_t)rx_data[5];
			can2_bus.temp_raw_[id-1]   	=rx_data[6];
		}
	}

}

SabertoothDual::SabertoothDual(int ch, int dir):
MotorBase(dir),ch_(ch)
{

}

void SabertoothDual::Init(){
	switch(ch_){
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
void SabertoothDual::SetVoltage_V(float voltage_V){
	if(voltage_V> supply_voltage_V_)voltage_V = supply_voltage_V_;
	if(voltage_V<-supply_voltage_V_)voltage_V =-supply_voltage_V_;

	voltage_V*=dir_;

	switch(ch_){
	case 1:
	    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500+500*voltage_V/supply_voltage_V_);
	    break;
	case 2:
	    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 1500+500*voltage_V/supply_voltage_V_);
	    break;
	}
}



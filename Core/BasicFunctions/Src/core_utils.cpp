/*
 * core_utils.cpp
 *
 *  Created on: Jan 8, 2024
 *      Author: mmaakkyyii
 */

#include "core_utils.h"
#include "stdint.h"
#include "stdio.h"
#include "gpio.h"
#include "tim.h"

#include "usb_device.h"
#include "usbd_cdc_if.h"

#include "usart.h"

#include "motor.h"
#include "encorder.h"

#include "machine_variable.h"

#include "buzzer.h"


void Init(){
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET); HAL_Delay(100);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET); HAL_Delay(100);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET); HAL_Delay(100);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); HAL_Delay(100);
	HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET); HAL_Delay(100);
	HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, GPIO_PIN_SET); HAL_Delay(100);
	HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, GPIO_PIN_SET); HAL_Delay(100);
	HAL_GPIO_WritePin(LED8_GPIO_Port, LED8_Pin, GPIO_PIN_SET); HAL_Delay(100);


	char s[128];
	buzzer.Init();
	rollerL.Init();
	rollerR.Init();
	rollerL.SetVoltage_V(0);
	rollerR.SetVoltage_V(0);
	state.Init();

	buzzer.On(400);
	HAL_Delay(200);
	buzzer.Off();


	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POWER1_CTRL_GPIO_Port, POWER1_CTRL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POWER2_CTRL_GPIO_Port, POWER2_CTRL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POWER3_CTRL_GPIO_Port, POWER3_CTRL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POWER4_CTRL_GPIO_Port, POWER4_CTRL_Pin, GPIO_PIN_SET);

	HAL_Delay(2000);
	for(int f=300;f<=500;f+=50){
		buzzer.On(f);
		HAL_Delay(100);
	}
	setBuzzerFrequency(-1);

	motor1.Init();
	motor2.Init();
	motor3.Init();
	motor4.Init();
	can1_bus.Start();
	can2_bus.Start();

	roller_enc_R.Init();
	roller_enc_L.Init();

	HAL_UART_Receive_IT(&huart8, &rxed_byte_data, 1);

	if(HAL_GPIO_ReadPin(ESW_GPIO_Port, ESW_Pin)==GPIO_PIN_SET){
		state.SetMode(Mode::kMachineInit);
	}else{
		state.SetMode(Mode::kEmergencyStop);
	}
	HAL_TIM_Base_Start_IT(&htim14);

}


void Loop(){


	  HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin); HAL_Delay(100);
//	  HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin); HAL_Delay(100);
//	  HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin); HAL_Delay(100);
//	  HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin); HAL_Delay(100);
//	  HAL_GPIO_TogglePin(LED6_GPIO_Port, LED6_Pin); HAL_Delay(100);
//	  HAL_GPIO_TogglePin(LED7_GPIO_Port, LED7_Pin); HAL_Delay(100);
//	  HAL_GPIO_TogglePin(LED8_GPIO_Port, LED8_Pin); HAL_Delay(100);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == UART8)
  {
	  bool res = ub.enqueue(rxed_byte_data);
	  if(!res){
		  printf("Failed to enqueue\r\n");
	  }
	  HAL_UART_Receive_IT(&huart8, &rxed_byte_data, 1);
  }
}

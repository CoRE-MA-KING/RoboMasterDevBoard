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

#include "motor.h"

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
	  int n=sprintf(s,"MA-KING RoboMaster Board\r\n");
	  CDC_Transmit_FS((uint8_t*)s, n);

	  C620Init();
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


/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usart.h
  * @brief          : Header library for uart data receive.
  ******************************************************************************
  * @attention
  *
  * @Chama1176 All rights reserved.
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CORE_WIRELESS_CONTROL_RX_H_
#define __CORE_WIRELESS_CONTROL_RX_H_

#include "usart.hpp"

class CoreWirelessControlRx
{
private:
	UsartBuffer *usart_;
	int16_t axis_[5] = {};
	bool buttons_[16] = {};
	char test_;

	bool communication_error;
	int cwcr_counter_ms=0;


	// original func for minimum flash size
	// input char should be a-f,0-9,A-F
	inline uint8_t char2int(char & c) {
		int8_t ans = 0;
		if('a' <= c && c <= 'f'){
			ans = c - 'a' + 10;
		} else if('A' <= c && c <= 'F'){
			ans = c - 'A' + 10;
		} else if('0' <= c && c <= '9'){
			ans = c - '0';
		}
		return ans;
	}

public:
  CoreWirelessControlRx(UsartBuffer * usart)
  {
	  usart_ = usart;
	  communication_error=false;
	  cwcr_counter_ms=0;
  }
  bool isCommunicationError(){
	  return communication_error;
  }
  bool CheckESW(){
	  if(button(13) && button(14)){
		  return true;
	  }else{
		  return false;
	  }
  }
  void Update(){
		const int cwcr_counter_threshold_ms=500;
		if(parse() == true){
			cwcr_counter_ms=0;
			communication_error=false;
		}else{
			cwcr_counter_ms++;
			if(cwcr_counter_ms>cwcr_counter_threshold_ms){
				communication_error=true;
			}
		}

  }
  void ClearBuffer(){
	  usart_->clear();
	  memset(buttons_,0,sizeof(buttons_));
	  memset(axis_,0,sizeof(axis_));
	}

  bool parse(){

    // parse when buffer has new line code
    if(usart_->nl_count() <= 0){
    	return false;
    }
    char data[40] = {};
    uint32_t len = usart_->get_line(data, 40);
    if(len < 35){
     return false;
    }
    uint32_t idx = 0;
    while(1){
	  if(len < idx + 35){
		 return false;
	  }
	  if(data[idx] == '0' && data[idx+1] == '0' && data[idx+34] == '\r') {

		for(int i = 0; i < 5; ++i){
			axis_[i] = (int8_t)((char2int(data[idx+14+i*3]) << 4) | char2int(data[idx+15+i*3]));
		}
		for(int i = 0; i < 2; ++i){
			uint8_t a1 = char2int(data[idx+29+i*3]);
			uint8_t a2 = char2int(data[idx+30+i*3]);
			buttons_[0+i*8] = (a1 & 0x1);
			buttons_[1+i*8] = (a1 & 0x2) >> 1;
			buttons_[2+i*8] = (a1 & 0x4) >> 2;
			buttons_[3+i*8] = (a1 & 0x8) >> 3;

			buttons_[4+i*8] = (a2 & 0x1);
			buttons_[5+i*8] = (a2 & 0x2) >> 1;
			buttons_[6+i*8] = (a2 & 0x4) >> 2;
			buttons_[7+i*8] = (a2 & 0x8) >> 3;
		}
	    return true;
	  } else{
		++idx;
		continue;
	  }
	}
	return true;
  }
  uint8_t test(){
	  char tmp = test_;
	  test_ = '\r';
	  return tmp;
  }
  int16_t axis(uint8_t i){
	  if(i < 5) return axis_[i];
	  return 0;
  }
  /*
0 R2
1 R1
2 L2
3 L1
4 left
5 down
6 right
7 up
8 right push
9 left push
10 share
11 option
12 sqare
13 x
14 o
15 triangle
   *
   */
  bool button(uint8_t i){
	  if(i < 16) return buttons_[i];
	  return 0;
  }

};


#endif /* __CORE_WIRELESS_CONTROL_RX_H_ */


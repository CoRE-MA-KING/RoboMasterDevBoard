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
#ifndef __USART_H_
#define __USART_H_

class UsartBuffer
{
private:
	static constexpr uint32_t BUFFER_SIZE = 128;	// 2
	static constexpr char NL_CODE = '\r';
	unsigned char buffer_[BUFFER_SIZE] = {};
	uint32_t rp_;
	uint32_t wp_;
	uint32_t nl_count_;

public:
  UsartBuffer()
  : rp_(0)
  , wp_(0)
  , nl_count_(0)
  {
  }
  bool enqueue(unsigned char data){
	  if(((wp_ - rp_) & (BUFFER_SIZE-1)) == (BUFFER_SIZE-1)){
		  return false;
	  }
	  if(data == NL_CODE){
		  ++nl_count_;
	  }
	  buffer_[wp_] = data;
	  inc_wp();
	  return true;
  }
  void inc_wp(){
	  wp_ = (++wp_) & (BUFFER_SIZE-1);
  }
  void inc_rp(){
	  rp_ = (++rp_) & (BUFFER_SIZE-1);
  }
  bool dequeue(unsigned char & data){
	  if(rp_ == wp_){
		  return false;
	  }
	  if(buffer_[rp_] == NL_CODE){
		  --nl_count_;
	  }
	  data = buffer_[rp_];
	  inc_rp();
	  return true;
  }
  uint32_t get_line(char* buff, uint32_t len){
	  uint32_t i = 0;
	  while(1){
		  if(i == len) break;
		  uint8_t data = ' ';
		  bool res = dequeue(data);
		  if(!res) break;
		  else buff[i] = data;
		  ++i;
		  if(data == NL_CODE) break;
	  }
	  return i;
  }
  uint32_t nl_count(){
	  return nl_count_;
  }

};


#endif /* __USART_H_ */


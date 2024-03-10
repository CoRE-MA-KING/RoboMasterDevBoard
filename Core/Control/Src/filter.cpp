/*
 * filter.cpp
 *
 *  Created on: 2024/03/10
 *      Author: mmaakkyyii
 */

#include "filter.h"
#include "string.h"

MovingAverage::MovingAverage(int length):buffer_length_(length){
	memset(buffer_,0,sizeof(buffer_));
}

float MovingAverage::Update(float input){
	sum_=0;
	for(int i=0;i<buffer_length_-1;i++){
		buffer_[i]=buffer_[i+1];
		sum_+=buffer_[i];
	}
	buffer_[buffer_length_-1]=input;
	sum_+=input;
	return sum_/buffer_length_;
}




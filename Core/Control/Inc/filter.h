/*
 * filter.h
 *
 *  Created on: 2024/03/10
 *      Author: mmaakkyyii
 */

#ifndef FILTER_H_
#define FILTER_H_

class MovingAverage{
private:
	float buffer_[1000];
	int buffer_length_;
	float sum_;
public:
	MovingAverage(int length);
	float Update(float input);

};




#endif /* CONTROL_INC_FILTER_H_ */

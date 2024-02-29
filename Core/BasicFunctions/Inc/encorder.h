/*
 * encorder.h
 *
 *  Created on: Jan 14, 2024
 *      Author: mmaakkyyii
 */

#ifndef ENCORDER_H_
#define ENCORDER_H_

#include "stdint.h"

class Encorder{
private:
	const int16_t kEncZero=0x7FFF;
	int ch_;
	int ppr_;
	int period_ms_;
	int radius_mm_;
	int dir_;
	int16_t pulse_;

public:
	Encorder(int ch, int ppr,int period_ms,int radius_mm,int dir);
	void Init();
	void Update();//call every period
	int GetPulse();
	float GetVelocity_rpm();
	float GetVelocity_rad_s();
	float GetVelicty_mm_s();
};

#endif /* ENCORDER_H_ */

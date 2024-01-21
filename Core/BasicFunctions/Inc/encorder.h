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
	const int16_t ENC_ZERO=0x7FFF;
	int ch;
	int ppr;
	int period_ms;
	int radius_mm;
	int dir;
	int16_t pulse;

public:
	Encorder(int _ch, int _ppr,int _period_ms,int _radius_mm,int _dir);
	void Init();
	void Update();//call every period
	int GetPulse();
	float GetVelocity_rpm();
	float GetVelocity_rad_s();
	float GetVelicty_mm_s();
};

void Encorder1Init();
void Encorder2Init();

int Encorder1Pulse();
int Encorder2Pulse();
#endif /* ENCORDER_H_ */

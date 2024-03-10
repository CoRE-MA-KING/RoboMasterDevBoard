/*
 * buzzer.h
 *
 *  Created on: Feb 12, 2024
 *      Author: mmaakkyyii
 */

#ifndef BUZZER_H_
#define BUZZER_H_


class Buzzer{
private:
	int set_time_ms_;
	int timer_ms_;
public:

	Buzzer();
	void Init();
	void Update();
	void On(float f);
	void Off();
	void SetFrequency(float f, int time_ms);

};

void setBuzzerFrequency(float frequency);



#endif /* BUZZER_H_ */

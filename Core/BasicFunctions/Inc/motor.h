/*
 * motor.h
 *
 *  Created on: Jan 10, 2024
 *      Author: mmaakkyyii
 */

#ifndef MOTOR_H_
#define MOTOR_H_

void SetC620Current_mA(int id,float I_mA);
float GetC620Current_mA(int id);
float GetC620Potion_rad(int id);
float GetC620Velocity_rad_s(int id);
float GetC620Temp_degC(int id);
void C620Init();
void C620Update();

void SabertoothDualInit();
void SabertoothDualSetVoltage(int ch, float Voltage_V);

#endif /* MOTOR_H_ */

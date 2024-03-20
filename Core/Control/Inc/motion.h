/*
 * motion.h
 *
 *  Created on: Jan 23, 2024
 *      Author: mmaakkyyii
 */

#ifndef MOTION_H_
#define MOTION_H_

void MecanumWheelJacobian(float vx, float vy, float omega, float* v1,float* v2,float* v3,float* v4);
float PitchLiner2Angle_deg(float pos);

#endif /* MOTION_H_ */

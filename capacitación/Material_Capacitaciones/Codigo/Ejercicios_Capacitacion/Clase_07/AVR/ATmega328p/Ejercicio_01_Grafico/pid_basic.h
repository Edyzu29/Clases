/*
 * pid_basic.h
 *
 *  Created on: Feb 22, 2020
 *      Author: mtorres
 */

#ifndef PID_BASIC_H_
#define PID_BASIC_H_

#define T_SAMPLE_SEC		0.5
#define PID_MAX_OUTPUT		1999.0
#define PID_MIN_OUTPUT		0.0


extern volatile double kp, ti, kd;
extern volatile double pid_setpoint;
extern volatile double error[3];
extern volatile double pid_output;
extern volatile double pid_feedback;

void PID_Basic(void);


#endif /* PID_BASIC_H_ */

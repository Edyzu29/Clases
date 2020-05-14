#include <stdint.h>
#include "PID_basico.h"

/* Sintonizar el PID variando estos valores */
volatile double kp = 1.0, ti = 1.0,  kd = 300.0;

volatile double pid_setpoint;
volatile double pid_error[3];
volatile double pid_feedback;
volatile double pid_output;

void PID_Basic(void){

	double p_comp, i_comp, d_comp, aux;

	pid_error[2] = pid_error[1];
	pid_error[1] = pid_error[0];
	pid_error[0] = pid_setpoint - pid_feedback;

	p_comp = pid_error[0] - pid_error[1];
	i_comp = pid_error[0];
	d_comp = (pid_error[0] - (2 * pid_error[1]) + pid_error[2]) / T_SAMPLE_SEC;


	aux = pid_output + (kp * p_comp) + ((kp * i_comp) / ti) + (kd * d_comp);

	if(aux > PID_MAX_OUTPUT){
		aux = PID_MAX_OUTPUT;
	}else if (aux < PID_MIN_OUTPUT){
		aux = PID_MIN_OUTPUT;
	}else{
		// aux no sufre cambios
	}

	pid_output = aux;

}

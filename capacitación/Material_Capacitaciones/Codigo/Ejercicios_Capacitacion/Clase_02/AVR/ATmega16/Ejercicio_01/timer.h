/*
 * Timer.h
 *
 *  Created on: 26 ene. 2018
 *      Author: MiguelAngel
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <avr/io.h>


/* Fuente de reloj de un temporizador */
typedef enum{
	Timer_Clk_Disabled = 0,
	Timer_Clk_PS1,
	Timer_Clk_PS8,
	Timer_Clk_PS64,
	Timer_Clk_PS256,
	Timer_Clk_PS1024,
	Timer_Clk_Ext_Fall_Edge,
	Timer_Clk_Ext_Rise_Edge
}Timer_Clock_Source_t;


/* Modos de operaci�n de un temporizador de 8 bits */
typedef enum{
	Timer_8b_Normal_Mode = 0,
	Timer_8b_Phase_Correct_PWM_FixPeriod_Mode = 1,
	Timer_8b_CTC_Mode = 2,
	Timer_8b_Fast_PWM_FixPeriod_Mode = 3,
}Timer_8b_Mode_t;


/* Modos de operaci�n de un temporizador de 16 bits */
typedef enum{
	Timer_16b_Normal_Mode = 0,
	Timer_16b_Phase_Correct_PWM_8bit_Mode = 1,
	Timer_16b_Phase_Correct_PWM_9bit_Mode = 2,
	Timer_16b_Phase_Correct_PWM_10bit_Mode = 3,
	Timer_16b_CTC_OCRA_Mode = 4,
	Timer_16b_Fast_PWM_8bit_Mode = 5,
	Timer_16b_Fast_PWM_9bit_Mode = 6,
	Timer_16b_Fast_PWM_10bit_Mode = 7,
	Timer_16b_Phase_Freq_Correct_ICR_Mode = 8,
	Timer_16b_Phase_Freq_Correct_OCRA_Mode = 9,
	Timer_16b_Phase_Correct_PWM_ICR_Mode = 10,
	Timer_16b_Phase_Correct_PWM_OCRA_Mode = 11,
	Timer_16b_CTC_ICR_Mode = 12,
	Timer_16b_Fast_PWM_ICR_Mode = 14,
	Timer_16b_Fast_PWM_OCRA_Mode = 15
}Timer_16b_Mode_t;


/* Modos de operaci�n de una salida por comparaci�n */
typedef enum{
	OC_Disabled = 0,
	OC_Toggle_Mode,
	OC_Clear_Non_Inverting_Mode,
	OC_Set_Inverting_Mode,
}Timer_Output_Compare_Mode_t;



/* Estado del cancelador de ruido en la entrada de captura */
typedef enum{
	Timer_ICNC_Disabled = 0,
	Timer_ICNC_Enabled
}Timer_IC_Noise_Canceler_t;


/* Flanco empleado en la entrada de captura */
typedef enum{
	Timer_IC_Falling_Edge = 0,
	Timer_IC_Rising_Edge
}Timer_IC_Edge_Selector_t;


/* Estructura que representa un Timer de 8 bits (para fines de configuraci�n) */
typedef struct{
    uint8_t OCR;
	Timer_Clock_Source_t clock;
	Timer_8b_Mode_t mode;
	Timer_Output_Compare_Mode_t OC;
}Timer_8b_t;


/* Estructura que representa un Timer de 16 bits (para fines de configuraci�n) */
typedef struct{
        uint16_t OCRA;
        uint16_t OCRB;
        uint16_t ICR;
	Timer_Clock_Source_t clock;
	Timer_16b_Mode_t mode;
	Timer_Output_Compare_Mode_t OCA;
	Timer_Output_Compare_Mode_t OCB;
	Timer_IC_Noise_Canceler_t ic_noise_canceler;
	Timer_IC_Edge_Selector_t ic_edge_selector;
}Timer_16b_t;


/* Prototipos de las funciones que llevar�n a cabo la configuraci�n de los temporizadores */
void Timer0_Configurar(Timer_8b_t *timer_param);
void Timer2_Configurar(Timer_8b_t *timer_param);
void Timer1_Configurar(Timer_16b_t *timer_param);



#endif /* TIMER_H_ */

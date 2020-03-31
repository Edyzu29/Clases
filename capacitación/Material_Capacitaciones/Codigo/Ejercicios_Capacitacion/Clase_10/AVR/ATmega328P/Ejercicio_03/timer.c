/*
 * Timer.c
 *
 *  Created on: 26 ene. 2018
 *      Author: MiguelAngel
 */

#include <stdint.h>
#include <avr/io.h>
#include "timer.h"

/* Definición de funciones */


/****************************************************************************
* Nombre de la función: Timer0_Configurar
* retorna : Nada
* arg : timer_param
* Descripción : Recibe un puntero hacia una estructura de tipo Timer_8b_t,
*               (la cual contiene los parámetros de configuración) y configura
* 				el módulo Timer0.
* Notas : Si se desea emplear el generador de señales con los pines OC0A u
* 		  OC0B, antes es necesario configurar estos pines como salida.
*****************************************************************************/
void Timer0_Configurar(Timer_8b_t *timer_param){

	/* Inicializar los registros de configuraci�n */
	TCCR0A = 0;
	TCCR0B = 0;
	TIMSK0 = 0;
    /* Cargar los valores de OCRA y OCRB */
    OCR0A = timer_param->OCRA;
    OCR0B = timer_param->OCRB;
	/* Configurar la fuente de reloj del temporizador */
	TCCR0B |= (timer_param->clock) & 0x07;
	/* Configurar el modo de operaci�n del temporizador */
	TCCR0B |= ((timer_param->mode) << 1) & (1 << WGM02);
	TCCR0A |= (timer_param->mode) & 0x03;
	/* Configurar las salida por comparaci�n OC0A */
	TCCR0A |= (timer_param->OCA) << COM0A0;
	/* Configurar la salida por comparaci�n OC0B */
	TCCR0A |= (timer_param->OCB) << COM0B0;
	/* Habilitar las interrupciones que sean necesarias */
	TIMSK0 |= timer_param->interrupt_mask;
}


/****************************************************************************
* Nombre de la función: Timer2_Configurar
* retorna : Nada
* arg : timer_param
* Descripción : Recibe un puntero hacia una estructura de tipo Timer_8b_t,
*               (la cual contiene los parámetros de configuración) y configura
* 				el módulo Timer2.
* Notas : Si se desea emplear el generador de señales con los pines OC2A u
* 		  OC2B, antes es necesario configurar estos pines como salida.
*****************************************************************************/
void Timer2_Configurar(Timer_8b_t *timer_param){

	/* Inicializar los registros de configuraci�n */
	TCCR2A = 0;
	TCCR2B = 0;
	TIMSK2 = 0;
    /* Cargar los valores de OCRA y OCRB */
    OCR2A = timer_param->OCRA;
    OCR2B = timer_param->OCRB;
	/* Configurar la fuente de reloj del temporizador */
	TCCR2B |= (timer_param->clock) & 0x07;
	/* Configurar el modo de operaci�n del temporizador */
	TCCR2B |= ((timer_param->mode) << 1) & (1 << WGM22);
	TCCR2A |= (timer_param->mode) & 0x03;
	/* Configurar las salida por comparaci�n OC0A */
	TCCR2A |= (timer_param->OCA) << COM2A0;
	/* Configurar la salida por comparaci�n OC0B */
	TCCR2A |= (timer_param->OCB) << COM2B0;
	/* Habilitar las interrupciones que sean necesarias */
	TIMSK2 |= timer_param->interrupt_mask;
}


/****************************************************************************
* Nombre de la función: Timer1_Configurar
* retorna : Nada
* arg : timer_param
* Descripción : Recibe un puntero hacia una estructura de tipo Timer_16b_t,
*               (la cual contiene los parámetros de configuración) y configura
* 				el módulo Timer1.
* Notas : Si se desea emplear el generador de señales con los pines OC1A,
* 		  OC1B u OC1C, antes es necesario configurar estos pines como salida.
*****************************************************************************/
void Timer1_Configurar(Timer_16b_t *timer_param){

	/* Inicializar los registros de configuraci�n */
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
    /* Cargar los valores de OCRA, OCRB, OCRC e ICR */
    OCR1A = timer_param->OCRA;
    OCR1B = timer_param->OCRB;
    ICR1 =  timer_param->ICR;
	/* Configurar la fuente de reloj del temporizador */
	TCCR1B |= (timer_param->clock) & 0x07;
	/* Configurar el modo de operaci�n del temporizador */
	TCCR1B |= ((timer_param->mode) << 1) & ((1 << WGM12) | (1 << WGM13));
	TCCR1A |= (timer_param->mode) & 0x03;
	/* Configurar las salida por comparaci�n OC0A */
	TCCR1A |= (timer_param->OCA) << COM1A0;
	/* Configurar la salida por comparaci�n OC0B */
	TCCR1A |= (timer_param->OCB) << COM1B0;
	/* Habilitar el cancelador de ruido de la entrada de captura */
	TCCR1B |= (timer_param->ic_noise_canceler) << ICNC1;
	/* Configurar selector de flancos de la entrada de captura */
	TCCR1B |= (timer_param->ic_edge_selector) << ICES1;
	/* Habilitar las interrupciones que sean necesarias */
	TIMSK1 |= timer_param->interrupt_mask;
}


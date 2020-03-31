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
* Notas : Si se desea emplear el generador de señales con el pin OC0,
* 		  antes es necesario configurar estos pines como salida.
*****************************************************************************/
void Timer0_Configurar(Timer_8b_t *timer_param){

	/* Inicializar el registro de configuraci�n */
	TCCR0 = 0;
    /* Cargar los valores de OCRA y OCRB */
    OCR0 = timer_param->OCR;
	/* Configurar la fuente de reloj del temporizador */
	TCCR0 |= (timer_param->clock) & 0x07;
	/* Configurar el modo de operaci�n del temporizador */
	TCCR0 |= ((timer_param->mode) & 0x01) << WGM00;
	TCCR0 |= ((timer_param->mode) & 0x02) << (WGM01 - 1);
	/* Configurar las salida por comparaci�n OC0A */
	TCCR0 |= (timer_param->OC) << COM00;
}


/****************************************************************************
* Nombre de la función: Timer2_Configurar
* retorna : Nada
* arg : timer_param
* Descripción : Recibe un puntero hacia una estructura de tipo Timer_8b_t,
*               (la cual contiene los parámetros de configuración) y configura
* 				el módulo Timer2.
* Notas : Si se desea emplear el generador de señales con el pin OC2,
* 		  antes es necesario configurar estos pines como salida.
*****************************************************************************/
void Timer2_Configurar(Timer_8b_t *timer_param){

	/* Inicializar el registro de configuraci�n */
	TCCR2 = 0;
    /* Cargar los valores de OCRA y OCRB */
    OCR2 = timer_param->OCR;
	/* Configurar la fuente de reloj del temporizador */
	TCCR2 |= (timer_param->clock) & 0x07;
	/* Configurar el modo de operaci�n del temporizador */
	TCCR2 |= ((timer_param->mode) & 0x01) << WGM00;
	TCCR2 |= ((timer_param->mode) & 0x02) << (WGM01 - 1);
	/* Configurar las salida por comparaci�n OC0A */
	TCCR2 |= (timer_param->OC) << COM20;
}


/****************************************************************************
* Nombre de la función: Timer1_Configurar
* retorna : Nada
* arg : timer_param
* Descripción : Recibe un puntero hacia una estructura de tipo Timer_16b_t,
*               (la cual contiene los parámetros de configuración) y configura
* 				el módulo Timer1.
* Notas : Si se desea emplear el generador de señales con los pines OC1A,
* 		  OC1B, antes es necesario configurar estos pines como salida.
*****************************************************************************/
void Timer1_Configurar(Timer_16b_t *timer_param){

	/* Inicializar los registros de configuraci�n */
	TCCR1A = 0;
	TCCR1B = 0;
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
}


/*
 *  eplimin_temp_control_module_avr_defines.h
 *
 *  Este archivo contiene definiciones de las constantes, registros y pines
 *  empleados para el manejo de los distintos componentes incluidos en el
 *  M?dulo de Control de Temperatura de EPLIMIN, empleando el microcontrolador
 *  PIC16F886.
 *
 *  Se recomienda incluir este archivo cada vez que desarrolle un proyecto
 *  con el m?dulo de control de temperatura y el microcontrolador PIC16F886.
 *
 *  ADVERTENCIA: ?No modifique este archivo bajo ninguna circunstancia!
 *
 *	NOTA: Si desea utilizar el m?dulo de control de temperatura con el
 *	microcontrolador ATmega328P, en su lugar debe incluir el archivo
 *	eplimin_iot_module_avr_defines.h
 *
 */
#include <xc.h>

#ifndef EPLIMIN_TEMP_CONTROL_MODULE_PIC_DEFINES_H
#define	EPLIMIN_TEMP_CONTROL_MODULE_PIC_DEFINES_H

/* Pin para controlar la potencia disipada por la resistencia cer?mica */
#define TRIS_RES_OUT				TRISB
#define PORT_RES_OUT			PORTB
#define RES_OUT					1

/* Entrada de la se?al del detector de cruce por cero */
#define PORT_SYNC_ZCD			PORTD
#define SYNC_ZCD				2

/* Se?al de disparo para el TRIAC */
#define TRIS_TRIAC_OUT			TRISB
#define PORT_TRIAC_OUT			PORTB
#define TRIAC_OUT				0

/* Chip Select para el conversor de termocupla */
#define TRIS_CS_MAX6675			TRISB
#define PORT_CS_MAX6675			PORTB
#define CS_MAX6675				2

/* Pin RW para la pantalla LCD */
#define TRIS_LCD_RW				TRISC
#define PORT_LCD_RW			PORTC
#define LCD_RW					4

/* Canales ADC: LM35 (sensor temperatura) */
#define ADC_CANAL_LM35			0

#endif	/* EPLIMIN_TEMP_CONTROL_MODULE_PIC_DEFINES_H */


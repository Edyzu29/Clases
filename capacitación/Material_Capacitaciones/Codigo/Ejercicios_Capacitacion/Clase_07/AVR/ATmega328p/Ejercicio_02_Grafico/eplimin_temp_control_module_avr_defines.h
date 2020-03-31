/*
 *  eplimin_temp_control_module_avr_defines.h
 *
 *  Este archivo contiene definiciones de las constantes, registros y pines
 *  empleados para el manejo de los distintos componentes incluidos en el
 *  Módulo de Control de Temperatura de EPLIMIN, empleando el microcontrolador
 *  ATmega328P.
 *
 *  Se recomienda incluir este archivo cada vez que desarrolle un proyecto
 *  con el módulo de control de temperatura y el microcontrolador ATmega328P.
 *
 *  ADVERTENCIA: ¡No modifique este archivo bajo ninguna circunstancia!
 *
 *	NOTA: Si desea utilizar el módulo de control de temperatura con el
 *	microcontrolador PIC16F886, en su lugar debe incluir el archivo
 *	eplimin_iot_module_pic_defines.h
 *
 */

#ifndef EPLIMIN_TEMP_CONTROL_MODULE_AVR_DEFINES_H_
#define EPLIMIN_TEMP_CONTROL_MODULE_AVR_DEFINES_H_

/* Pin para controlar la potencia disipada por la resistencia cerámica */
#define DDR_RES_OUT				DDRB
#define PORT_RES_OUT			PORTB
#define RES_OUT					1

/* Entrada de la señal del detector de cruce por cero */
#define PIN_SYNC_ZCD			PIND
#define SYNC_ZCD				2

/* Señal de disparo para el TRIAC */
#define DDR_TRIAC_OUT			DDRB
#define PORT_TRIAC_OUT			PORTB
#define TRIAC_OUT				0

/* Chip Select para el conversor de termocupla */
#define DDR_CS_MAX6675			DDRB
#define PORT_CS_MAX6675			PORTB
#define CS_MAX6675				2

/* Pin RW para la pantalla LCD */
#define DDR_LCD_RW				DDRC
#define PORT_LCD_RW				PORTC
#define LCD_RW					4

/* Canales ADC: LM35 (sensor temperatura) */
#define ADC_CANAL_LM35			0

#endif /* EPLIMIN_TEMP_CONTROL_MODULE_AVR_DEFINES_H_ */

/*
 *  eplimin_servo_module_avr_defines.h
 *
 *  Este archivo contiene definiciones de las constantes, registros y pines
 *  empleados para el manejo de los distintos componentes incluidos en el
 *  Módulo de Servomotores de EPLIMIN, empleando el microcontrolador ATmega328P.
 *
 *  Se recomienda incluir este archivo cada vez que desarrolle un proyecto
 *  con el módulo de servomotores y el microcontrolador ATmega328P.
 *
 *  ADVERTENCIA: ¡No modifique este archivo bajo ninguna circunstancia!
 *
 *	NOTA: Si desea utilizar el módulo de servomotores con el microcontrolador
 *	PIC16F886, en su lugar debe incluir el archivo eplimin_servo_module_pic_defines.h
 *
 */

#ifndef EPLIMIN_SERVO_MODULE_AVR_DEFINES_H_
#define EPLIMIN_SERVO_MODULE_AVR_DEFINES_H_

/* Pines de salida de los servomotores */
#define DDR_SERVO_1			DDRD
#define PORT_SERVO_1		PORTD
#define SERVO_1				6
#define DDR_SERVO_2			DDRB
#define PORT_SERVO_2		PORTB
#define SERVO_2				1
#define DDR_SERVO_3			DDRD
#define PORT_SERVO_3		PORTD
#define SERVO_3				7

/* Pulsadores: S1, S2, S3, S4, S5, S6 */
#define PIN_S1				PIND
#define S1					5
#define PIN_S2				PINB
#define S2					0
#define PIN_S3				PINB
#define S3					2
#define PIN_S4				PINC
#define S4					5
#define PIN_S5				PIND
#define S5					3
#define PIN_S6				PIND
#define S6					4

#define S1_PRESIONADO			(PIN_S1 & (1 << S1))
#define S2_PRESIONADO			(PIN_S2 & (1 << S2))
#define S3_PRESIONADO			(PIN_S3 & (1 << S3))
#define S4_PRESIONADO			(PIN_S4 & (1 << S4))
#define S5_PRESIONADO			(PIN_S5 & (1 << S5))
#define S6_PRESIONADO			(PIN_S6 & (1 << S6))

/* Entradas select del joystick */
#define DDR_JOYSTICK_SELECT		DDRD
#define PORT_JOYSTICK_SELECT	PORTD
#define JOYSTICK_SELECT			2

/* Canales ADC: potenciómetros POT1, POT2 y POT3 */
#define ADC_CANAL_JOY_Y			4
#define ADC_CANAL_JOY_X			3
#define ADC_CANAL_POT1			2
#define ADC_CANAL_POT2			1
#define ADC_CANAL_POT3			0



#endif /* EPLIMIN_SERVO_MODULE_AVR_DEFINES_H_ */

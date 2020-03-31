/*
 *  eplimin_iot_module_avr_defines.h
 *
 *  Este archivo contiene definiciones de las constantes, registros y pines
 *  empleados para el manejo de los distintos componentes incluidos en el
 *  Módulo de IoT de EPLIMIN, empleando el microcontrolador ATmega328P.
 *
 *  Se recomienda incluir este archivo cada vez que desarrolle un proyecto
 *  con el módulo IoT y el microcontrolador ATmega328P.
 *
 *  ADVERTENCIA: ¡No modifique este archivo bajo ninguna circunstancia!
 *
 *	NOTA: Si desea utilizar el módulo IoT con el microcontrolador PIC16F886,
 *	en su lugar debe incluir el archivo eplimin_iot_module_pic_defines.h
 *
 */

#ifndef EPLIMIN_IOT_MODULE_AVR_DEFINES_H_
#define EPLIMIN_IOT_MODULE_AVR_DEFINES_H_

/* Pin habilitador del módulo Bluetooth y Wifi */
#define DDR_BT_ESP_ENABLE		DDRB
#define PORT_BT_ESP_ENABLE		PORTB
#define BT_ESP_ENABLE			0

/* LEDS: LED1, LED2*/
#define DDR_LEDS				DDRD
#define PORT_LEDS				PORTD
#define LED1					5
#define LED2					4

/* Salidas relay: Q1, Q2 */
#define DDR_RELAYS				DDRD
#define PORT_RELAYS				PORTD
#define Q1						6
#define Q2						7

/* Pulsadores: S1, S2 */
#define PIN_BOTONES				PINC
#define S1						3
#define S2						2

#define S1_PRESIONADO			(PIN_BOTONES & (1 << S1))
#define S2_PRESIONADO			(PIN_BOTONES & (1 << S2))

/* Canales ADC: LM35 (sensor temperatura), LDR (sensor luz) y potenciómetro */
#define ADC_CANAL_LDR			5
#define ADC_CANAL_TEMP			4
#define ADC_CANAL_POT			0


#endif /* EPLIMIN_IOT_MODULE_AVR_DEFINES_H_ */

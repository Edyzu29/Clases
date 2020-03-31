/*
 * push_buttons.h
 *
 *  Created on: Feb 7, 2020
 *      Author: mtorres
 */


/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>

/* Constantes y macros */

#ifndef PUSH_BUTTONS_H_
#define PUSH_BUTTONS_H_

#define PORT_BOTONES	PORTB
#define PIN_BOTONES		PINB

#define BOTON1			0
#define BOTON2			1
#define BOTON3			2
#define BOTON4			3

#define MASK_BOTONES	((1 << BOTON1) | (1 << BOTON2) | (1 << BOTON3) | (1 << BOTON4))

/* Variables globales */

extern volatile uint8_t boton_estado;
extern volatile uint8_t boton_pulsado;

/* Declaración de funciones */
void Botones_Escanear(void);
uint8_t Boton_Leer(uint8_t pos);

#endif /* PUSH_BUTTONS_H_ */

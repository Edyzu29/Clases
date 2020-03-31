/*
 * matrix_keypad_4x4.h
 *
 *  Created on: Feb 6, 2020
 *      Author: mtorres
 */

#ifndef MATRIX_KEYPAD_4X4_H_PORT
#define MATRIX_KEYPAD_4X4_H_

/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>

/* Constantes y macros */
#define DDR_KP_ROWS		DDRD
#define PORT_KP_ROWS	PORTD
#define PIN_KP_ROWS		PIND
#define KP_R1			0
#define KP_R2			1
#define KP_R3			2
#define KP_R4			3

#define DDR_KP_COLUMNS	DDRD
#define PORT_KP_COLUMNS	PORTD
#define PIN_KP_COLUMNS	PIND
#define KP_C1			4
#define KP_C2			5
#define KP_C3			6
#define KP_C4			7

#define MASK_KP_ROWS ((1 << KP_R1) | (1 << KP_R2) | (1 << KP_R3) | (1 << KP_R4))

#define MASK_KP_COLUMNS ((1 << KP_C1) | (1 << KP_C2) | (1 << KP_C3) | (1 << KP_C4))

#define N_ROWS			4
#define N_COLUMNS		4

/* Símbolos impresos en el teclado */
#define K_1			0x11
#define K_2			0x21
#define K_3			0x31
#define K_4			0x12
#define K_5			0x22
#define K_6			0x32
#define K_7			0x13
#define K_8			0x23
#define K_9			0x33
#define K_0			0x24
#define K_A			0x41
#define K_B			0x42
#define K_C			0x43
#define K_D			0x44
#define K_AST		0x14
#define K_HASH		0x34


/* Variables globales */
extern uint8_t col_pos[N_COLUMNS];
extern uint8_t raw_pos[N_ROWS];

/* Declaración de funciones */
uint8_t Keypad_Leer_Tecla_Pulsada(void);


#endif /* MATRIX_KEYPAD_4X4_H_ */

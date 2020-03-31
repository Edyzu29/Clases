/*
 * display_7_segmentos.h
 *
 *  Created on: 17 dic. 2017
 *      Author: MiguelAngel
 */

#ifndef DISPLAY_7_SEGMENTOS_H_
#define DISPLAY_7_SEGMENTOS_H_

/* Inclusiones */
#include <xc.h>
#include <stdint.h>

/* Constantes y macros */

// Cantidad de displays
#define	N_DISPLAYS	4

// Definici�n de pines
#define TRIS_DISPLAY		TRISD
#define PORT_DISPLAY	PORTD
#define	TRIS_ENABLES		TRISC
#define PORT_ENABLES	PORTC
#define EN_0			0
#define EN_1			1
#define EN_2			2
#define EN_3			3

// M�scara que cubre los bits que no son habilitadores
#define MASCARA_ENABLES	((1 << EN_0) | (1 << EN_1) | (1 << EN_2) | (1 << EN_3))

/* Variables globales */
extern uint8_t display_buffer[N_DISPLAYS];

/* Declaraci�n de funciones (prototipos) */
void Display_Inicializar(void);
uint8_t Display_Decodificar_Digito(uint8_t num_binario);
void Display_Decodificar_Entero_Decimal(uint16_t num_binario);
void Display_Gestionar_Habilitadores(void);

#endif /* DISPLAY_7_SEGMENTOS_H_ */

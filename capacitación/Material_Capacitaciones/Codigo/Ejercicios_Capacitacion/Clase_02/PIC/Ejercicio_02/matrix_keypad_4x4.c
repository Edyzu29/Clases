/*
 * matrix_keypad_4x4.c
 *
 *  Created on: Feb 6, 2020
 *      Author: mtorres
 */


#include <stdint.h>
#include "matrix_keypad_4x4.h"

/* Variables globales */

uint8_t col_pos[N_COLUMNS] = {KP_C1, KP_C2, KP_C3, KP_C4};
uint8_t row_pos[N_ROWS] = {KP_R1, KP_R2, KP_R3, KP_R4};

/* Definición de funciones */


/****************************************************************************
* Nombre de la función: Keypad_Ubicar_Tecla
* retorna : el valor (ubicación) de la tecla presionada
* arg : Ninguno
* Descripción : Realiza un barrido de todas las teclas y retorna la ubicación
* 				de la tecla presionada en forma de un número de 8 bits.
* 				Los 4 bits más significativos indican la columna (0, 1, 2, 3)
* 				y los 4 bits menos significativos indican la fila (0, 1, 2, 3)
* 				Por ejemplo, si se presiona la tecla ubicada en la columna 2
* 				y fila 3, esta función retornará el valor 0x23.
* 				Si ninguna tecla está presionada esta función retorna 0.
*
* Notas : 		Esta función no ha sido diseñada para ser llamada por el
* 				usuario, pero es necesaria para que sea utilizada en la
* 				función Keypad_Leer_Tecla_Pulsada, la cual se describe en
* 				líneas más abajo.
*****************************************************************************/
static uint8_t Keypad_Ubicar_Tecla(void){

	uint8_t index;
	uint8_t col = 0, row = 0;
	uint8_t salida = 0;


	/* Configurar como salida todos los pines de columnas y entregar '0' en todos ellos */
	TRIS_KP_COLUMNS &= ~MASK_KP_COLUMNS;
	PORT_KP_COLUMNS &= ~MASK_KP_COLUMNS;

	/* Realizar un primer barrido para obtener la fila (row) */
	for(index = 0; index < N_ROWS; index++){
		/* Determinar la columna */
		if(!(PORT_KP_ROWS & (1 << row_pos[index]))){
			row = index + 1;
		}
	}

	/* Si alguna tecla es presionada (row diferente de 0) ... */
	if(row != 0){
		/* Realizar un segundo barrido para obtener la columna */
		for(index = 0; index < N_COLUMNS; index++){
			/* Configurar como salida y enviar '0', un pin de fila a la vez */
			TRIS_KP_COLUMNS |= MASK_KP_COLUMNS;
			TRIS_KP_COLUMNS &= ~(1 << col_pos[index]);
			PORT_KP_COLUMNS &= ~(1 << col_pos[index]);
			/* Determinar la fila */
			if(!(PORT_KP_ROWS & (1 << row_pos[row-1]))){
				col = index + 1;
			}
		}
	}


	if(col != 0){
		salida = ((col << 4) | row);
	}

	return salida;
}


/****************************************************************************
* Nombre de la función: Keypad_Leer_Tecla_Pulsada
* retorna : el valor de la tecla pulsada
* arg : Ninguno
* Descripción : Identifica si ha ocurrido el evento USUARIO PULSA TECLA
* 				y retorna el valor (ubicación) de la tecla solamente
* 				cuando ha ocurrido este evento. Si no ha ocurrido este
* 				evento, la función retorna 0.
*
* 				Por ejemplo: si el usuario presiona, y mantiene presionada,
* 				la tecla en la columna 3 y la fila 2, esta función retorna
* 				0x32 solamente cuando detecta que la tecla acaba de ser
* 				pulsada. Durante el tiempo que la tecla permaneza presionada
* 				la función retornará 0.
*
* 				Asimismo, si ninguna tecla es pulsada esta función retorna '0'.
*
* Notas : 		Se debe llamar a esta función periódicamente para monitorear
* 				el teclado matricial. Se recomienda emplear periodos entre
* 				20ms y 100 ms.
*****************************************************************************/
uint8_t Keypad_Leer_Tecla_Pulsada(void){

	static uint8_t estado_anterior = 0;
	uint8_t estado_actual;
	uint8_t salida = 0;

	estado_actual = Keypad_Ubicar_Tecla();

	if((estado_actual != 0) && (estado_anterior == 0)){
		salida = estado_actual;
	}

	estado_anterior = estado_actual;

	return salida;
}


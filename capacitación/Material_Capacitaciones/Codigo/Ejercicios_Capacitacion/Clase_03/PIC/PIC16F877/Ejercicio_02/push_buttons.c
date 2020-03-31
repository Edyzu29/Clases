/*
 * push_buttons.c
 *
 *  Created on: Feb 7, 2020
 *      Author: mtorres
 */

#include <stdint.h>
#include "push_buttons.h"

/* Variables globales */
volatile uint8_t boton_estado = 0;
volatile uint8_t boton_pulsado = 0;


/* Definición de funciones */

/****************************************************************************
* Nombre de la función: Botones_Escanear
* retorna : Nada
* arg : Ninguno
* Descripción : Esta función mide los valores de los botones, elimina los
* 				efectos de los rebotes mecánicos, y determina si un botón
* 				ha sido pulsado.
*
* 				La variable boton_pulsado indica los botones que han sido
* 				pulsados: el valor de cada bit en esta variable representa
* 				el estado de un botón. Por ejemplo: si el bit 3 de la
* 				variable boton_pulsado es '1', eso indica que el BOTON3
* 				del la entrada PORT_BOTONES ha sido pulsado.
*
* 				De forma análoga, el estado del botón (PRESIONADO --> 1,
* 				SUELTO --> 0), con los rebotes filtrados, se indica en
* 				cada uno de los bits de la variable boton_Estado
*
* Notas : 		Esta función debe ser llamada, periódicamente. Es necesario
* 				llamar a esta función antes de utilizar la función de nombre
* 				Botones_Escanear.
*****************************************************************************/
void Botones_Escanear(void){

	static uint8_t estado_01 = 0;
	static uint8_t estado_02 = 0;
	uint8_t estado_03;
	uint8_t estado_estable;

	/* Filtrar los rebotes */

	// Si emplea resistencia pull-down
	estado_03 = PORT_BOTONES & MASK_BOTONES;

	// Si emplea resistencia pull-up
	//estado_03 = ~PORT_BOTONES & MASK_BOTONES;

	estado_estable = estado_01 & estado_02 & estado_03;
	estado_01 = estado_02;
	estado_02 = estado_03;
	/* Detectar si hubo un cambio */
	boton_pulsado |= (~boton_estado & estado_estable);
	boton_estado = estado_estable;

}




/****************************************************************************
* Nombre de la función: Boton_Leer
* retorna : Verdadero (botón pulsado) o Falso (botón no pulsado)
* arg : pos (Posición del botón que se desea leer)
* Descripción : Esta función lee un bit específico de la variable
* 				boton_pulsado y determina si el botón correspondiente
* 				ha sido pulsado.
*
* 				Si el botón ha sido pulsado, la función limpia el bit
* 				correspondiente al botón (en la variable boton_pulsado) y
* 				retorna un valor diferente de cero (Veradero).
*
* 				Por otro lado, si el botón indicado no ha sido pulsado,
* 				entonces la función retorna 0 (Falso).
*
* Notas : 		Esta función solo puede leer un botón a la vez.
*****************************************************************************/
uint8_t Boton_Leer(uint8_t pos){

	uint8_t salida;
	salida = (boton_pulsado & (1 << pos));
	boton_pulsado &= ~(1 << pos);

	return salida;
}


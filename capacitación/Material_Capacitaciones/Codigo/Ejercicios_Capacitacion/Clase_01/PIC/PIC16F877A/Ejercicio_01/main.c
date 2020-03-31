/*
 * main.c
 *
 *  Created on: Feb 7, 2020
 *      Author: mtorres
 */


/* Inclusión de archivos */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>

/* Constantes y macros */

// Definición de pines
#define TRIS_LEDS           TRISD
#define PORT_LEDS           PORTD

#define TRIS_DISPLAY		TRISC
#define PORT_DISPLAY        PORTC

/* Declaración de funciones */
uint8_t Display_Decodificar_Digito(uint8_t num_binario);

/* Función principal */
int main(void){

	uint8_t contador;

	/* Configurar los pines de los LEDs como salida */
	TRIS_LEDS = 0xF0;
	/* Configurar los pines del display como salida */
	TRIS_DISPLAY = 0x00;
	/* Inicializar los LEDs y el display apagados */
	/* (Este paso no es necesario, ya que, en los AVRs las
	 * salidas valen '0' al momento del arranque */
	PORT_LEDS = 0;
	PORT_DISPLAY = 0;

	/* Repetir indefinidamente ... */
	while(1){
		/* Contar de forma ascendente del 0 al 9 */
		for(contador = 0; contador <= 9; contador++){
			/* Mostrar la cuenta en los LEDs */
			PORT_LEDS = contador;
			/* Mostrar la cuenta en el display */
			PORT_DISPLAY = Display_Decodificar_Digito(contador);
			/* Esperar un segundo */
			__delay_ms(1000);
		}
	}

	return 0;
}


/* Definición de funciones */

uint8_t Display_Decodificar_Digito(uint8_t num_binario){

	uint8_t salida;

	/* Tabla de búsqueda para decodificar los dígitos */
	uint8_t tabla_display[10] = {
									0x3F, // 0
									0x06, // 1
									0x5B, // 2
									0x4F, // 3
									0x66, // 4
									0x6D, // 5
									0x7D, // 6
									0x07, // 7
									0x7F, // 8
									0x6F, // 9
								};

	/* Retorna el valor del dígito decodificado */
	if(num_binario < 10){
		salida = tabla_display[num_binario];
	}else{
		salida = 0;
	}

	return salida;
}


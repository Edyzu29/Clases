/*
 * display_7_segmentos.c
 *
 *  Created on: 17 dic. 2017
 *      Author: MiguelAngel
 */

#include <avr/io.h>
#include <stdint.h>
#include "display_7_segmentos.h"

/* Variables globales */
uint8_t display_buffer[N_DISPLAYS];

/****************************************************************************
* Nombre de la función: Display_Inicializar
* retorna : Nada
* Descripción : Configura los pines conectados a los segmentos de los display
* 				y los pines conectados a los habilitadores de cada display
* 				como salidas digitales. Además, inicializa los displays en
* 				estado apagado y los habilitadores desactivados.
* Notas : Se asume que un puerto contiene 8 bits.
*****************************************************************************/
void Display_Inicializar(void){
	/* Configurar el puerto de los segmentos como salida */
	DDR_DISPLAY = 0xFF;
	/* Inicializar con todos los segmentos apagados*/
	PORT_DISPLAY = 0x00;
	/* Configurar los pines de los habilitadores como salida */
	DDR_ENABLES |= MASCARA_ENABLES;
	/* Inicializar con todos los habilitadores desactivados*/
	PORT_ENABLES &= ~MASCARA_ENABLES;
}

/****************************************************************************
* Nombre de la función: Display_Decodificar_Digito
* retorna : dígito decodificado (uint8_t)
* arg : num_binario
* Descripción : Recibe un dígito (número de 0 a 9), lo decodifica, y retorna
* 				la secuencia de bits correspondiente para mostrar el valor
* 				numérico del dígito en un display de 7 segmentos.
* Notas : Debe asegurarse que el valor recibido se encuentra entre 0 y 9.
* 		  Cualquier valor fuera de este rango puede generar errores en la
* 		  ejecución del programa.
*****************************************************************************/
uint8_t Display_Decodificar_Digito(uint8_t num_binario){

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
	return tabla_display[num_binario];
}

/****************************************************************************
* Nombre de la función: Display_Decodificar_Entero_Decimal
* retorna : Nada
* arg : num_binario (uint16_t)
* Descripción : Recibe un número entero (uint16_t), extrae los N últimos
* 				dígitos, los decodifica y almacena los valores decodificados
* 				en el arreglo display_buffer.
* 				El número de dígitos extraídos se encuentra determinado por
* 				el número displays, definido en la constante N_DISPLAYS.
* Notas : La cantidad de dígitos almacenados en el buffer depende solo de
* 		  la constante N_DISPLAYS, sin importar la cantidad de dígitos
* 		  significativos que contenga el parámetro num_binario.
*****************************************************************************/
void Display_Decodificar_Entero_Decimal(uint16_t num_binario){

	uint8_t pos_digito;

	/* Extraer los dígitos (en base decimal)del número deseado*/
	for(pos_digito = 0; pos_digito < N_DISPLAYS; pos_digito++){
		/* Extraer el dígito de las unidades, decodificarlo y almacenarlo */
		display_buffer[pos_digito] = Display_Decodificar_Digito(num_binario % 10);
		/* Desplazar el número un dígito decimal hacia la derecha */
		num_binario /= 10;
	}
}

/****************************************************************************
* Nombre de la función: Display_Gestionar_Habilitadores
* retorna : Nada
* Descripción : Desactiva todos los habilitadores, actualiza el valor que se
* 				mostrará sobre los displays (segmentos encendidos) y activa
* 				el habilitador correspondiente.
* Notas : Esta función debe ser invocada periódicamente con una frecuencia
* 		  lo suficientemente alta como para que no se perciban interrupciones
* 		  en la visualización de los dígitos.
*****************************************************************************/
void Display_Gestionar_Habilitadores(void){

	uint8_t posicion_enable[N_DISPLAYS] = {EN_0, EN_1, EN_2, EN_3};

	static uint8_t pos_digito = 0;

	// Apagar todos los displays
	PORT_ENABLES &= ~MASCARA_ENABLES;
	// Cargar el digito correspondiente a los segmentos
	PORT_DISPLAY = display_buffer[pos_digito];
	// Habilitar el display correspondiente
	PORT_ENABLES |= (1 << posicion_enable[pos_digito]);
	// Actualizar el valor de la posición
	pos_digito++;
	if(pos_digito >= N_DISPLAYS){
		pos_digito = 0;
	}
}

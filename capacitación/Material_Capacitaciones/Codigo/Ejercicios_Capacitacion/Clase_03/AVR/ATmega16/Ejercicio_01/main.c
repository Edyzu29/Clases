/*
 * main.c
 *
 *  Created on: 17 dic. 2017
 *      Author: MiguelAngel
 */

/* Inclusiones */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "display_7_segmentos.h"

/* Funci�n principal */
int main(void){

	uint16_t numero;
	uint16_t conta;

	/* Configurar el display */
	Display_Inicializar();

	/* Repetir permanentemente */
	while(1){
		for(numero = 1000; numero <= 1020; numero++){
			Display_Decodificar_Entero_Decimal(numero);
			/* Mostrar el mismo número durante 1 segundo */
			for(conta = 0; conta < 500; conta++ ){
				Display_Gestionar_Habilitadores();
				_delay_ms(2);
			}
		}
	}

}



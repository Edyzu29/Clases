/*
 * main.c
 *
 *  Created on: 17 dic. 2017
 *      Author: MiguelAngel
 */

/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "display_7_segmentos.h"

/* Constantes y macros */
#define TIMER0_OFFSET   131


/* Declaración de funciones (prototipos) */
void Timer0_Inicializar(void);

/* Función principal */
int main(void){

	uint16_t numero;
	uint16_t conta;

	/* Configurar el display */
	Display_Inicializar();
    /* Configurar el display para generar una interrupción cada 2ms */
    Timer0_Inicializar();
    /* Habilitar las interrupciones */
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

	/* Repetir permanentemente */
	while(1){
		for(numero = 1000; numero <= 1020; numero++){
			Display_Decodificar_Entero_Decimal(numero);
            __delay_ms(1000);
		}
	}

}


/* Rutinas de servicio a interrupción */
__interrupt() void ISR(void){
    /* Limpiar la bandera de desbordamiento del Timer0 */
    INTCONbits.T0IF = 0;
    /* Cargar el valor de OFFSET al Timer 0*/
    TMR0 = TIMER0_OFFSET;
    /* Gestionar los habilitadores*/
    Display_Gestionar_Habilitadores();
    
}

/* Definición de funciones */
void Timer0_Inicializar(void){
    
    /* Timer0 en Modo Temporizador*/
    OPTION_REGbits.T0CS = 0;
    /* Incrementar con cada flanco de subida (solo para modo contador)*/
    OPTION_REGbits.T0SE = 0;
    /* Activar el Pre-Scaler para el Timer0 */
    OPTION_REGbits.PSA = 0;
    /* Definir el valor del Prescaler = 64 */
    OPTION_REGbits.PS = 0b101;
    /* Cargar el valor de OFFSET */
	TMR0 = TIMER0_OFFSET;
    /* Habilitar la interrupción por desbordamiento del Timer0 */
    INTCONbits.T0IE = 1;
}
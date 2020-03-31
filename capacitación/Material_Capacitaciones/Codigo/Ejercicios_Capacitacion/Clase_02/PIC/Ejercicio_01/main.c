
/* Inclusiones */
#include <xc.h>
#include <stdint.h>
#include <pic16f877a.h>
#include "push_buttons.h"

/* Definici�n de pines */
#define TRIS_ALARM		TRISD
#define PORT_ALARM		PORTD
#define ALARM			7

#define TRIS_LED		TRISD
#define PORT_LED		PORTD
#define LED             5

#define TIMER0_OFFSET	100

/* Variables globales */
uint8_t estado_alarma;

/* Declaraci�n de funciones (prototipos) */
void Timer0_Inicializar(void);

int main(void){

	uint8_t conta_4ms = 0;

	/* Configurar el pin del buzzer como salida e inicializar la alarma apagada */
	TRIS_ALARM &= ~(1 << ALARM);
	PORT_ALARM &= ~(1 << ALARM);
	/* Configurar el pin del LED como salida e inicializar el LED apagado */
	TRIS_LED &= ~(1 << LED);
	PORT_LED &= ~(1 << LED);
	/* Configurar el Timer0 para contar 4ms */
	Timer0_Inicializar();

	/* Repetir indefinidamente */
	while(1){
		/* Si ha transcurrido 4ms ... */
		if(INTCONbits.T0IF == 1){
			/* Limpiar la bandera de desbordamiento */
			INTCONbits.T0IF = 0;
			/* Cargar el valor de OFFSET */
			TMR0 = TIMER0_OFFSET;
			/* Escanear el pulsador para activar/desactivar la alarma */
			Botones_Escanear();
			if(Boton_Leer(BOTON1)){
				estado_alarma = !estado_alarma;
				PORT_LED ^= (1 << LED);
			}
			/* Incrementar el contador de periodos de 4ms */
			conta_4ms++;
			/* Si ha transcurrido medio segundo */
			if(conta_4ms == 125){
				/* Conmutar la alarma (de ser el caso) */
				if(estado_alarma == 1){
					PORT_ALARM ^= (1 << ALARM);
				}else{
					PORT_ALARM &= ~(1 << ALARM);
				}
				/* Limpiar el contador de periodos de 4ms */
				conta_4ms = 0;
			}
		}

	}

	return 0;
}


void Timer0_Inicializar(void){
    
    /* Timer0 en Modo Temporizador*/
    OPTION_REGbits.T0CS = 0;
    /* Incrementar con cada flanco de subida (solo para modo contador)*/
    OPTION_REGbits.T0SE = 0;
    /* Activar el Pre-Scaler para el Timer0 */
    OPTION_REGbits.PSA = 0;
    /* Definir el valor del Prescaler = 128 */
    OPTION_REGbits.PS = 0b110;
    /* Cargar el valor de OFFSET */
	TMR0 = TIMER0_OFFSET;
    
}




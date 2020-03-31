
/* Inclusiones */
#include <avr/io.h>
#include <stdint.h>
#include "timer.h"
#include "push_buttons.h"

/* Definici�n de pines */
#define DDR_ALARM		DDRD
#define PORT_ALARM		PORTD
#define ALARM			7

#define DDR_LED			DDRD
#define PORT_LED		PORTD
#define LED				5

#define TIMER0_OFFSET	131

/* Variables globales */
uint8_t estado_alarma;

/* Declaraci�n de funciones (prototipos) */
void Timer0_Inicializar(void);

int main(void){

	uint8_t conta_4ms = 0;

	/* Configurar el pin del buzzer como salida e inicializar la alarma apagada */
	DDR_ALARM |= (1 << ALARM);
	PORT_ALARM &= ~(1 << ALARM);
	/* Configurar el pin del LED como salida e inicializar el LED apagado */
	DDR_LED |= (1 << LED);
	PORT_LED &= ~(1 << LED);
	/* Configurar el Timer0 para contar 4ms */
	Timer0_Inicializar();

	/* Repetir indefinidamente */
	while(1){
		/* Si ha transcurrido 4ms ... */
		if(TIFR & (1 << TOV0)){
			/* Limpiar la bandera de desbordamiento */
			TIFR |= (1 << TOV0);
			/* Cargar el valor de OFFSET */
			TCNT0 = TIMER0_OFFSET;
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

	Timer_8b_t timer0;

	/* Establecer los par�metros de configuraci�n del Timer0 */
	timer0.clock = Timer_Clk_PS256;
	timer0.mode = Timer_8b_Normal_Mode;
	timer0.OC = OC_Disabled;
	/* Configurar el Timer0 */
	Timer0_Configurar(&timer0);
	TCNT0 = TIMER0_OFFSET;
}




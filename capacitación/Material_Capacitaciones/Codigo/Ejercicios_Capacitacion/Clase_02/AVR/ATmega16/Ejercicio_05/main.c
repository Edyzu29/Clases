
/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "timer.h"

/* Constantes y macros */
#define DDR_PWM			DDRD
#define SERVO			5

/* Variables globales */

/* Declaración de funciones */
void Timer1_Inicializar(void);

/* Función principal */
int main(void){

	uint16_t ciclo_trabajo;

	/* Configurar el Timer1 en Modo PWM con periodo ajustable (Periodo 20ms) */
	Timer1_Inicializar();

	while(1){
		for(ciclo_trabajo = 499; ciclo_trabajo < 2500; ciclo_trabajo++){
			OCR1A = ciclo_trabajo;
			_delay_ms(5);
		}

	}


	return 0;
}

void Timer1_Inicializar(void){

	Timer_16b_t timer1;

	/* Configurar como salida el pin OC1A (salida PWM) */
	DDR_PWM |= (1 << SERVO);

	/* Establecer los par�metros de configuraci�n del Timer0 */
	timer1.clock = Timer_Clk_PS8;
	timer1.mode = Timer_16b_Fast_PWM_ICR_Mode;
	timer1.OCA = OC_Clear_Non_Inverting_Mode;
	timer1.OCB = OC_Disabled;
	timer1.ic_edge_selector = Timer_IC_Falling_Edge;
	timer1.ic_noise_canceler = Timer_ICNC_Disabled;
	timer1.OCRA = 499;
	timer1.ICR = 19999;
	/* Configurar el Timer1 */
	Timer1_Configurar(&timer1);
}





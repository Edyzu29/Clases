
/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "timer.h"

/* Constantes y macros */
#define DDR_PWM1		DDRD
#define LED1			5
#define DDR_PWM2		DDRD
#define LED2			4


/* Variables globales */

/* Declaración de funciones */
void Timer1_Inicializar(void);

/* Función principal */
int main(void){

	int16_t ciclo_trabajo;

	/* Configurar el Timer1 en Modo PWM con periodo ajustable (Periodo 10us) */
	Timer1_Inicializar();

	while(1){
		for(ciclo_trabajo = 0; ciclo_trabajo < 80; ciclo_trabajo++){
			OCR1A = ciclo_trabajo;
			OCR1B = 79 - ciclo_trabajo;
			_delay_ms(50);
		}

		for(ciclo_trabajo = 79; ciclo_trabajo >= 0; ciclo_trabajo--){
			OCR1A = ciclo_trabajo;
			OCR1B = 79 - ciclo_trabajo;
			_delay_ms(50);
		}

	}


	return 0;
}

void Timer1_Inicializar(void){

	Timer_16b_t timer1;

	/* Configurar como salida los pines OC1A y OC1B (salida PWM) */
	DDR_PWM1 |= (1 << LED1);
	DDR_PWM2 |= (1 << LED2);
	/* Establecer los par�metros de configuraci�n del Timer0 */
	timer1.clock = Timer_Clk_PS1;
	timer1.mode = Timer_16b_Fast_PWM_ICR_Mode;
	timer1.OCA = OC_Clear_Non_Inverting_Mode;
	timer1.OCB = OC_Clear_Non_Inverting_Mode;
	timer1.ic_edge_selector = Timer_IC_Falling_Edge;
	timer1.ic_noise_canceler = Timer_ICNC_Disabled;
	timer1.OCRA = 1;
	timer1.OCRB = 1;
	timer1.ICR = 79;
	/* Configurar el Timer1 */
	Timer1_Configurar(&timer1);
}





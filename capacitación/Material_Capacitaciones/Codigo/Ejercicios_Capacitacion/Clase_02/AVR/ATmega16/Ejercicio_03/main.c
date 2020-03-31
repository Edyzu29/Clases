
/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "timer.h"

/* Constantes y macros */
#define DDR_LED			DDRB
#define LED1			3

/* Variables globales */

/* Declaración de funciones */
void Timer0_Inicializar(void);

/* Función principal */
int main(void){

	uint8_t brillo = 0;

	Timer0_Inicializar();

	while(1){
		OCR0 = brillo;
		_delay_ms(20);
		brillo++;
	}


	return 0;
}

void Timer0_Inicializar(void){

	Timer_8b_t timer0;

	/* COnfigurar como salida el pin OC0 (salida PWM) */
	DDR_LED |= (1 << LED1);

	/* Establecer los par�metros de configuraci�n del Timer0 */
	timer0.clock = Timer_Clk_PS1;
	timer0.mode = Timer_8b_Fast_PWM_FixPeriod_Mode;
	timer0.OC = OC_Clear_Non_Inverting_Mode;
	timer0.OCR = 12;
	/* Configurar el Timer0 */
	Timer0_Configurar(&timer0);
}





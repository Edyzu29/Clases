
/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "adc.h"
#include "display_7_segmentos.h"

/* Variables globales */
volatile uint16_t valor_adc = 1023;

/* Declaración de funciones */
void ADC_Inicializar(void);
void Timer1_Inicializar(void);

/* Función principal */
int main(void){
	/* Configurar los pines del display */
	Display_Inicializar();
	/* Configurar el TImer1 para generar una interrupción cada 1ms */
	Timer1_Inicializar();
	/* Configurar el módulo ADC */
	ADC_Inicializar();
	/* Habilitar las interrupciones */
	sei();
	/* Repetir indefinidamente */
	while(1){
		/* Iniciar una vonversión */
		ADCSRA |= (1 << ADSC);
		while(!(ADCSRA & (1 << ADIF))){
			// Esperar hasta que la conversión se complete
		}
		/* Leer el valor adquirido */
		valor_adc = ADC;
		/* Mostrar el valor ADC en los display */
		Display_Decodificar_Entero_Decimal(valor_adc);
		/* Esperar 100ms antes de realizar la siguiente conversión */
		_delay_ms(100);
	}

	return 0;
}

/* Rutinas de servicio a interrupciones */

ISR(TIMER1_COMPA_vect){
	Display_Gestionar_Habilitadores();
}


/* Definición de funciones */

void Timer1_Inicializar(void){

	Timer_16b_t timer1;

	/* Establecer los par�metros de configuraci�n del Timer0 */
	timer1.clock = Timer_Clk_PS64;
	timer1.mode = Timer_16b_CTC_OCRA_Mode;
	timer1.OCA = OC_Disabled;
	timer1.OCB = OC_Disabled;
	timer1.ic_edge_selector = Timer_IC_Falling_Edge;
	timer1.ic_noise_canceler = Timer_ICNC_Disabled;
	timer1.OCRA = 124;

	/* Configurar el Timer1 */
	Timer1_Configurar(&timer1);

	/* Activar la interrupción por comparación con OCR1A */
	TIMSK |= (1 << OCIE1A);
}


void ADC_Inicializar(void){
	ADC_t my_adc;

	my_adc.prescaler = ADC_Prescaler_32;
	my_adc.vref = AVCC;
	my_adc.channel = ADC_Channel_0;
	my_adc.auto_trigger = ADC_Autotrigger_Disabled;
	my_adc.trigger_source = ADC_Free_Running_Mode;
	my_adc.interrupt_mask = ADC_Interrupt_Disabled;

	ADC_Configurar(&my_adc);
}

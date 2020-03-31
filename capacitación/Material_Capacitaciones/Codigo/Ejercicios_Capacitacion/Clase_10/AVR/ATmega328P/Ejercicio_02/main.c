
/* Inclusion de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "eplimin_servo_module_avr_defines.h"
#include "timer.h"
#include "adc.h"


/* Constantes y macros */

/* Variables globales */
volatile uint16_t valor_adc;


/* Declaración de funciones */
void Timer0_Inicializar(void);
void Timer1_Inicializar(void);
void ADC_Inicializar(void);

/* Función principal */
int main(void){

	/* Configurar el ADC */
	ADC_Inicializar();
	/* Configurar el Timer0 para interrumpir cada 10 ms (aprox)*/
	Timer0_Inicializar();
	/* Configurar el Timer1 para generar una PWM de periodo 20ms */
	Timer1_Inicializar();
	/* Habilitar las interrupciones */
	sei();

	/* Repetir indefinidamente */
	while(1){

	}
	return 0;
}


/* Rutinas de servicio a interrupción */
ISR(TIMER0_COMPA_vect){
	/* Leer el ADC cada 10ms */
	ADCSRA |= (1 << ADSC);
}


ISR(ADC_vect){

	valor_adc = 1023 - ADC;
	OCR1A = 124 + (((uint32_t)valor_adc * 500) / 1023);

}

/* Definición de funciones */

void Timer0_Inicializar(void){

	Timer_8b_t timer0;

	/* Establecer los par�metros de configuraci�n del Timer1 */
	timer0.clock = Timer_Clk_PS1024;
	timer0.mode = Timer_8b_CTC_Mode;
	timer0.OCA = OC_Disabled;
	timer0.OCB = OC_Disabled;
	timer0.interrupt_mask = Timer_CompA_IE;
	timer0.OCRA = 155;

	/* Configurar el Timer1 */
	Timer0_Configurar(&timer0);

}


void Timer1_Inicializar(void){

	Timer_16b_t timer1;

	DDR_SERVO_2 |= (1 << SERVO_2);

	/* Establecer los par�metros de configuraci�n del Timer1 */
	timer1.clock = Timer_Clk_PS64;
	timer1.mode = Timer_16b_Fast_PWM_ICR_Mode;
	timer1.OCA = OC_Clear_Non_Inverting_Mode;
	timer1.OCB = OC_Disabled;
	timer1.ic_edge_selector = Timer_IC_Falling_Edge;
	timer1.ic_noise_canceler = Timer_ICNC_Disabled;
	timer1.interrupt_mask = Timer_Interrupts_Disabled;
	timer1.OCRA = 124;
	timer1.ICR = 4999;

	/* Configurar el Timer1 */
	Timer1_Configurar(&timer1);

}


void ADC_Inicializar(void){
	ADC_t my_adc;

	my_adc.prescaler = ADC_Prescaler_32;
	my_adc.vref = AVCC;
	my_adc.channel = ADC_Channel_1;
	my_adc.auto_trigger = ADC_Autotrigger_Disabled;
	my_adc.trigger_source = ADC_Free_Running_Mode;
	my_adc.interrupt_mask = ADC_Interrupt_Enabled;

	ADC_Configurar(&my_adc);
}


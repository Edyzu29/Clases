
/* Inclusión de archivos */
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "adc.h"
#include "lcd.h"

/* Constantes y macros */
#define N_CANALES	2

/* Variables globales */
volatile uint16_t valor_adc[N_CANALES];
volatile uint8_t bandera_actualizar_display = 0;

/* Declaración de funciones */
void ADC_Inicializar(void);
void Timer1_Inicializar(void);

/* Función principal */
int main(void){

	uint16_t voltaje[N_CANALES];
	char buffer[17];

	/* Configurar la pantalla LCD */
	LCD_Init_4_Bits();
	/* Configurar el TImer1 para generar una interrupción cada 50ms */
	Timer1_Inicializar();
	/* Configurar el módulo ADC */
	ADC_Inicializar();
	/* Habilitar las interrupciones */
	sei();
	/* Imprimir los encabezados */
	LCD_Goto_XY(1,1);
	LCD_Put_String("POT1: ");
	LCD_Goto_XY(1,2);
	LCD_Put_String("POT2: ");

	/* Repetir indefinidamente */
	while(1){
		/* Si ha transcurrido 100ms ... */
		if(bandera_actualizar_display == 1){
			/* Convertir los valores binarios a voltajes */
			voltaje[0] = ((uint32_t)valor_adc[0] * 500) / 1024;
			voltaje[1] = ((uint32_t)valor_adc[1] * 500) / 1024;
			/* Imprimir voltaje del potenciómetro 1 */
			sprintf(buffer,"%d.%d V", (voltaje[0] / 100), (voltaje[0] % 100));
			LCD_Goto_XY(7, 1);
			LCD_Put_String("      ");
			LCD_Goto_XY(7, 1);
			LCD_Put_String(buffer);
			/* Imprimir voltaje del potenciómetro 2 */
			sprintf(buffer,"%d.%d V", (voltaje[1] / 100), (voltaje[1] % 100));
			LCD_Goto_XY(7, 2);
			LCD_Put_String("      ");
			LCD_Goto_XY(7, 2);
			LCD_Put_String(buffer);

			bandera_actualizar_display = 0;
		}
	}

	return 0;
}

/* Rutinas de servicio a interrupciones */

ISR(TIMER1_COMPA_vect){
	ADCSRA |= (1 << ADSC);
}


ISR(ADC_vect){

	static uint8_t indice_canal = 0;

	valor_adc[indice_canal] = ADC;
	indice_canal++;
	if(indice_canal == N_CANALES){
		bandera_actualizar_display = 1;
		indice_canal = 0;
	}
	ADC_Seleccionar_Canal(indice_canal);
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
	timer1.OCRA = 6249;

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
	my_adc.interrupt_mask = ADC_Interrupt_Enabled;

	ADC_Configurar(&my_adc);
}

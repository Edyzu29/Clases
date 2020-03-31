
/* Inclusion de archivos */
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "eplimin_temp_control_module_avr_defines.h"
#include "timer.h"
#include "adc.h"
#include "lcd.h"
#include "uart.h"
#include "pid_basic.h"



/* Constantes y macros */
#define N_MUESTRAS		32
#define TEMP_SETPOINT	40
#define ADC_SETPOINT	(((TEMP_SETPOINT * 1024.0)/500.0) + 102.0)

/* Variables globales */
volatile uint8_t muestra_filtrada;
volatile uint16_t valor_adc = 0;
uint16_t valor_adc_anterior = 0;
uint8_t temperatura = 0;
char buffer[17];

/* Declaración de funciones */
void ADC_Inicializar(void);
void Timer0_Inicializar(void);
void Timer1_Inicializar(void);
void UART0_Inicializar(void);

/* Función principal */
int main(void){

	/* Forzar a 0 el pin RW de la LCD */
	DDR_LCD_RW |= (1 << LCD_RW);
	PORT_LCD_RW &= ~(1 << LCD_RW);
	/* Configurar la pantalla LCD */
	LCD_Init_4_Bits();
	/* Impresión del mensaje de bienvenida */
	LCD_Goto_XY(1,1);
	LCD_Put_String(" Modulo Control ");
	LCD_Goto_XY(1,2);
	LCD_Put_String(" de Temperatura ");

	_delay_ms(2000);

	/* Impresión de las cabeceras */
	LCD_Tx_Command(LCD_CLEAR);
	LCD_Goto_XY(1,1);
	LCD_Put_String("Setpoint:      C");
	LCD_Goto_XY(15,1);
	LCD_Put_Char(0xDF);
	LCD_Goto_XY(1,2);
	LCD_Put_String("Medicion:      C");
	LCD_Goto_XY(15,2);
	LCD_Put_Char(0xDF);
	sprintf(buffer, "%d", TEMP_SETPOINT);
	LCD_Goto_XY(11,1);
	LCD_Put_String(buffer);

	/* Configurar el ADC */
	ADC_Inicializar();
	/* Configurar el UART */
	UART0_Inicializar();
	/* Configurar el Timer0 para generar una interrupción cada 1/64 segundos */
	Timer0_Inicializar();
	/* Configurar el Timer 1 para generar una señal PWM */
	Timer1_Inicializar();
	/* Establecer el setpoint del PID */
	pid_setpoint = ADC_SETPOINT;
	/* Habilitar las interrupciones */
	sei();

	/* Repetir indefinidamente */
	while(1){
		if(muestra_filtrada){
			muestra_filtrada = 0;
			pid_feedback = (double)valor_adc;
			PID_Basic();
			OCR1A = (uint16_t)pid_output;

			if(valor_adc != valor_adc_anterior){
				valor_adc_anterior = valor_adc;
				temperatura = ((uint32_t)(valor_adc - 102) * 500) / 1024;
				sprintf(buffer, "%d", temperatura);
				LCD_Goto_XY(11,2);
				LCD_Put_String("   ");
				LCD_Goto_XY(11,2);
				LCD_Put_String(buffer);
				sprintf(buffer, "%d,%d\r\n", TEMP_SETPOINT, temperatura);
			}
			UARTn_Tx_String(UART0, buffer);
		}
	}

	return 0;
}

/* Rutinas de servicio a interrupción */

ISR(TIMER0_COMPA_vect){
	/* Iniciar una nueva conversión */
	ADCSRA |= (1 << ADSC);
}


ISR(ADC_vect){

	static uint32_t acumulador = 0;
	static uint8_t indice_muestra = 0;

	acumulador += ADC;
	indice_muestra++;

	if(indice_muestra == N_MUESTRAS){
		valor_adc = acumulador / N_MUESTRAS;
		acumulador = 0;
		indice_muestra = 0;
		muestra_filtrada = 1;
	}

}


/* Definición de funciones */

void Timer0_Inicializar(void){

	Timer_8b_t timer0;

	/* Establecer los par�metros de configuraci�n del Timer0 */
	timer0.clock = Timer_Clk_PS1024;
	timer0.mode = Timer_8b_CTC_Mode;
	timer0.OCA = OC_Disabled;
	timer0.OCB = OC_Disabled;
	timer0.interrupt_mask = Timer_CompA_IE;
	timer0.OCRA = 243;

	/* Configurar el Timer0 */
	Timer0_Configurar(&timer0);

}

void Timer1_Inicializar(void){

	Timer_16b_t timer1;

	/* Configurar como salida el pin de la señal PWM */
	DDR_RES_OUT |= (1 << RES_OUT);

	/* Establecer los par�metros de configuraci�n del Timer1 */
	timer1.clock = Timer_Clk_PS64;
	timer1.mode = Timer_16b_Fast_PWM_ICR_Mode;
	timer1.OCA = OC_Clear_Non_Inverting_Mode;
	timer1.OCB = OC_Disabled;
	timer1.ic_edge_selector = Timer_IC_Falling_Edge;
	timer1.ic_noise_canceler = Timer_ICNC_Disabled;
	timer1.interrupt_mask = Timer_Interrupts_Disabled;
	timer1.OCRA = 1000;
	timer1.ICR = 1999;

	/* Configurar el Timer1 */
	Timer1_Configurar(&timer1);

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

void UART0_Inicializar(void){

	UART_t my_uart;

	my_uart.u2x = U2X_Enabled;
	my_uart.baud_rate = Baud_115200_bps;
	my_uart.tx_status = UART_Tx_Enabled;
	my_uart.rx_status = UART_Rx_Disabled;
	my_uart.data_bits = UART_8_Data_Bits;
	my_uart.stop_bits = UART_1_Stop_Bit;
	my_uart.parity_mode = UART_Parity_Disabled;
	my_uart.interrupt_mask = UART_Interrupts_Disabled;

	UART0_Configurar(&my_uart);
}

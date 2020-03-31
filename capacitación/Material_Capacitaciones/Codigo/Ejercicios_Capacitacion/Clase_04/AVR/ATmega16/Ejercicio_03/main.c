
/* Inclusiones */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "adc.h"

/* Constantes y macros */
#define N_MUESTRAS	20

/* Variables globales */
volatile uint8_t transmitir_datos_flag = 0;
volatile int16_t muestra[N_MUESTRAS];

/* Declaraci�n de funciones */
void ADC_Inicializar(void);
void UART0_Inicializar(void);
void Imprimir_Vector(void);


/* Funci�n principal */
int main(void){

	uint8_t indice_muestra =0;

	/* Configurar el UART */
	UART0_Inicializar();
	/* Configurar el ADC */
	ADC_Inicializar();
	/* Repetir indefinidamente */
	while(1){
		/* Imprimir mensaje inicial */
		UARTn_Tx_String(UART0, "Iniciando muestreo...\n\n\r");
		_delay_ms(1000);
		/* Almacenar las N muestras del ADC */
		for(indice_muestra = 0; indice_muestra < N_MUESTRAS; indice_muestra++){
			/* Iniciar una conversión */
			ADCSRA |= (1 << ADSC);
			while(!(ADCSRA & (1 << ADIF))){
				/* No hacer nada hasta que la conversión finalice */
			}
			/* Almacenar el valor ADC en un buffer */
			muestra[indice_muestra] = ADC;
			/* Esperar un segundo antes de la siguiente conversión */
			_delay_ms(1000);
		}

		/* Transmitir el vector por el puerto serial */
		UARTn_Tx_String(UART0, " ... muestreo finalizado.\n\n\r");
		Imprimir_Vector();
	}

	return 0;
}


/* Definici�n de funciones */

void Imprimir_Vector(void){

	uint8_t indice;

        /* Imprimir el inicio del vector */
	UARTn_Tx_String(UART0, "A=[");

        /* Para cada uno de  los valores del vector ... */
	for(indice = 0; indice < N_MUESTRAS; indice++){
                /* Imprimir el valor de la muestra */
		UARTn_Tx_Integer(UART0, (int32_t)muestra[indice]);
		/* Imprimir coma o cerrar llaves */
		if(indice == (N_MUESTRAS - 1)){
			UARTn_Tx_String(UART0, "];\n\n\r");
		}else{
			UARTn_Tx_String(UART0, ", ");
		}
	}
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



void UART0_Inicializar(void){

	UART_t my_uart;

	my_uart.u2x = U2X_Enabled;
	my_uart.baud_rate = Baud_2400_bps;
	my_uart.tx_status = UART_Tx_Enabled;
	my_uart.rx_status = UART_Rx_Disabled;
	my_uart.data_bits = UART_8_Data_Bits;
	my_uart.stop_bits = UART_1_Stop_Bit;
	my_uart.parity_mode = UART_Parity_Disabled;
	my_uart.interrupt_mask = UART_Interrupts_Disabled;

	UART0_Configurar(&my_uart);
}



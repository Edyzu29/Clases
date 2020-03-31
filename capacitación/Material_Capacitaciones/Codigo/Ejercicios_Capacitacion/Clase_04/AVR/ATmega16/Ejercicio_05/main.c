

/* Inclusiones */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "uart.h"

/* Constantes y macros */
#define DDR_DISPLAY		DDRC
#define PORT_DISPLAY	PORTC

/* Variables globales */
volatile uint8_t evaluar_comando_flag = 0;
volatile char comando;

/* Declaraci�n de funciones (prototipos) */
void UART0_Inicializar(void);
void Evaluar_Comando(void);

/* Funci�n principal */
int main(void){
	/* Configurar los pines de los segmentos como salida */
	DDR_DISPLAY = 0xFF;
	/* Configurar el UART */
	UART0_Inicializar();
	/* Activar el habilitador global de interrupciones */
	sei();

	while(1){
		/* Si la bandera para evaluar el comando est� en ALTO */
		if(evaluar_comando_flag){
			/* Limpiar la bandera */
			evaluar_comando_flag = 0;
			/* Evaluar la acci�n a tomar con el comando recibido */
			Evaluar_Comando();
		}
	}

	return 0;
}

/* Rutinas de servicio a interrupciones */
ISR(USART_RXC_vect){
	/* Almacenar el caracter recibido */
	comando = UDR;
	/* Indicar que debe evaluarse el comando recibido */
	evaluar_comando_flag = 1;
}

/* Definici�n de funciones */

void UART0_Inicializar(void){

	UART_t my_uart;

	my_uart.u2x = U2X_Enabled;
	my_uart.baud_rate = Baud_2400_bps;
	my_uart.tx_status = UART_Tx_Enabled;
	my_uart.rx_status = UART_Rx_Enabled;
	my_uart.data_bits = UART_8_Data_Bits;
	my_uart.stop_bits = UART_1_Stop_Bit;
	my_uart.parity_mode = UART_Parity_Disabled;
	my_uart.interrupt_mask = UART_Rx_Complete_IE;

	UART0_Configurar(&my_uart);
}


void Evaluar_Comando(void){

	switch(comando){
		case 'a':
			PORT_DISPLAY ^= (1 << 0);
			break;
		case 'b':
			PORT_DISPLAY ^= (1 << 1);
			break;
		case 'c':
			PORT_DISPLAY ^= (1 << 2);
			break;
		case 'd':
			PORT_DISPLAY ^= (1 << 3);
			break;
		case 'e':
			PORT_DISPLAY ^= (1 << 4);
			break;
		case 'f':
			PORT_DISPLAY ^= (1 << 5);
			break;
		case 'g':
			PORT_DISPLAY ^= (1 << 6);
			break;
		case 'p':
			PORT_DISPLAY ^= (1 << 7);
			break;
		default:
			break;
	}


}

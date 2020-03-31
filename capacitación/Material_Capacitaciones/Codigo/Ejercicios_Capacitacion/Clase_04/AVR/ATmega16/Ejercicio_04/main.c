

/* Inclusiones */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "uart.h"

/* Constantes y macros */

/* Variables globales */
volatile char caracter;

/* Declaraci�n de funciones (prototipos) */
void UART0_Inicializar(void);

/* Funci�n principal */
int main(void){
	/* Configurar el UART */
	UART0_Inicializar();
	/* Activar el habilitador global de interrupciones */
	sei();

	/* Repetir indefinidamente */
	while(1){

	}

	return 0;
}

/* Rutinas de servicio a interrupciones */
ISR(USART_RXC_vect){
	/* Almacenar el caracter recibido */
	caracter = UDR;
	/* Transmitir el caracter recibido (eco) */
	UARTn_Tx_Byte(UART0, caracter);
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

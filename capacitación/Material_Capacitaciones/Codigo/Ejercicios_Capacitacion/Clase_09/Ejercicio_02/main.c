/*
EJERCICIO 02: CONTROL DE HARDWARE CON TELÉFONO MÓVIL MEDIANTE BLUETOOTH

Este programa se utiliza controlar los leds (LED1, LED2) y los relays (Q1, Q2),
incorporados en la tarjeta de desarrollo Módulo IoT, mediante el envío de
comandos desde el teléfono móvil, empleando una aplicación que permita
establecer una comunicación bluetooth.

 */

/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "eplimin_iot_module_avr_defines.h"


volatile char comando = 0;
volatile uint8_t bandera_leer_comando = 0;

/* Declaraci�n de funciones (prototipos) */
void UART0_Inicializar(void);

int main(void){

	/* Habilitar el módulo bluetooth */
	DDR_BT_ESP_ENABLE |= (1 << BT_ESP_ENABLE);
	PORT_BT_ESP_ENABLE |= (1 << BT_ESP_ENABLE);
	/* Inicializar los puertos de los LEDs */
	DDR_LEDS |= (1 << LED1) | (1 << LED2);
	PORT_LEDS &= ~((1 << LED1) | (1 << LED2));
	/* Inicializar los puertos de los relays */
	DDR_RELAYS |= (1 << Q1) | (1 << Q2);
	PORT_RELAYS &= ~((1 << Q1) | (1 << Q2));
	/* Configurar el módulo UART con una tasa de 9600 bps */
	UART0_Inicializar();
	/* Habilitar las interrupciones */
	sei();

	/* Repetir indefinidamente ... */
	while(1){

		/* Si se debe leer un comando ... */
		if(bandera_leer_comando == 1){

			/* Llevar a cavo la acción requerida para cada comando */
			if(comando == '1'){
				PORT_LEDS ^= (1 << LED1);
			}else if(comando == '2'){
				PORT_LEDS ^= (1 << LED2);
			}else if(comando == '3'){
				PORT_RELAYS ^= (1 << Q1);
			}else if(comando == '4'){
				PORT_RELAYS ^= (1 << Q2);
			}else{
				// Cualquier otro comando no hace nada
			}

			/* Indicar que el comando ha sido leído */
			bandera_leer_comando = 0;
		}

	}

	return 0;
}

void UART0_Inicializar(void){

	UART_t my_uart;

	my_uart.u2x = U2X_Enabled;
	my_uart.baud_rate = Baud_9600_bps;
	my_uart.tx_status = UART_Tx_Disabled;
	my_uart.rx_status = UART_Rx_Enabled;
	my_uart.data_bits = UART_8_Data_Bits;
	my_uart.stop_bits = UART_1_Stop_Bit;
	my_uart.parity_mode = UART_Parity_Disabled;
	my_uart.interrupt_mask = UART_Rx_Complete_IE;

	UART0_Configurar(&my_uart);
}

ISR(USART_RX_vect){

	comando = UDR0;
	bandera_leer_comando = 1;

}


/*
Ejercicio 09:
Verificar el correcto funcionamiento de las funciones creadas en el Ejercicio 07
y el Ejercicio 08, empleándolas para escribir un mensaje (por ejemplo “Hola”) en
una memoria EEPROM 24C32. Luego, intercambiar de módulo con su compañero para
leer lo que él ha escrito en su chip de memoria.
 */

/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "twi_master.h"
#include "uart.h"
#include "eeprom_i2c.h"


/* Constantes y macros */
#define EEPROM1_SLAVE_ADDRESS		0x50

/* Variables globales */
uint8_t buffer_entrada[100];
uint8_t buffer_salida[] = "Esta es la historia de un muchacho que perdio a su amada\n\r";


/* Declaración de funciones */
void UART0_Inicializar(void);

/* Función principal */
int main(void){

	uint8_t salida;

	TWI_Master_Inicializar();
	UART0_Inicializar();

	/* Escribir datos en la EEPROM */
	salida = EEPROM_Write(EEPROM1_SLAVE_ADDRESS, 0x0000, 59, buffer_salida);
	/* Esperar un ciclo de escritura (10 ms) */
	_delay_ms(10);
	/* Leer datos desde la EEPROM */
	salida = EEPROM_Read(EEPROM1_SLAVE_ADDRESS, 0x0000, 59, buffer_entrada);
	while(1){
		/* Imprimir el mensaje leido desde la EEPROM */
		UARTn_Tx_String(UART0, buffer_entrada);
		PORTD |= (1 << 7);
		_delay_ms(1000);
	}

	return 0;
}


void UART0_Inicializar(void){

	UART_t my_uart;

	my_uart.u2x = U2X_Enabled;
	my_uart.baud_rate = Baud_9600_bps;
	my_uart.tx_status = UART_Tx_Enabled;
	my_uart.rx_status = UART_Rx_Disabled;
	my_uart.data_bits = UART_8_Data_Bits;
	my_uart.stop_bits = UART_1_Stop_Bit;
	my_uart.parity_mode = UART_Parity_Disabled;
	my_uart.interrupt_mask = UART_Interrupts_Disabled;

	UART0_Configurar(&my_uart);
}





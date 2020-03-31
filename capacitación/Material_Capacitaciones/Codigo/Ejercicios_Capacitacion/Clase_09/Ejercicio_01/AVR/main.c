/*
EJERCICIO 01: CONFIGURACIÓN DEL MÓDULO BLUETOOTH MLT-BT05

Este programa se utiliza para configurar el nombre y el password del módulo
bluetooth incorporado en la tarjeta de desarrollo Modulo IoT de EPLIMIN.

Verifique que el módulo bluetooth de la tarjeta de desarrollo no se encuentra
conectado con ningún dispositivo (PC, celular, tablet, etc.) antes de iniciar
la ejecución del programa. Puede verificar que el bluetooth está desconectado
porque el LED 6 comenzará a parpadear, de lo contrario, permanecerá encendido.

Una vez que el dispositivo se encuentre desconectado, la configuración se
llevará a cabo en cuanto se presione el pulsador S2 de la tarjeta.

Una vez que la configuración se haya llevado a cabo, el LED1 comenzará a
parpadear y el módulo transmitirá un mensaje, el cual puede ser visualizado
desde un teléfono móvil al conectarse con el módulo.

 */

/* Inclusiones */
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include "uart.h"
#include "eplimin_iot_module_avr_defines.h"


/* Variables globales */

char buffer_salida[30];

char *name = "SENATI"; 	// Nombre (18 caracteres como máximo)
char *password = "611535";  // Password (de 6 dígitos)
char mode = '0'; // Modo de operación (dejarlo en modo esclavo)

/* Declaración de funciones (prototipos) */
void UART0_Inicializar(void);

int main(void){

	/* Habilitar el funcionamiento del módulo Bluetooth */
	DDR_BT_ESP_ENABLE |= (1 << BT_ESP_ENABLE);
	PORT_BT_ESP_ENABLE |= (1 << BT_ESP_ENABLE);
	/* Configurar como salida el pin del LED1 y apagarlo */
	DDR_LEDS |= (1 << LED1);
	PORT_LEDS &= ~(1 << LED1);
	/* Habilitar el módulo UART 0 con un baud rate de 38400*/
	UART0_Inicializar();

	while(!S2_PRESIONADO){
		/* Esperar hasta que el pulsador S2 sea presionado */
	}

	/* Configurar el nombre del dispositivo */
	sprintf(buffer_salida, "AT+NAME%s\r\n", name);
	UARTn_Tx_String(UART0, buffer_salida);
	_delay_ms(100);
	/* Configurar la clave de acceso */
	sprintf(buffer_salida, "AT+PIN%s\r\n", password);
	UARTn_Tx_String(UART0, buffer_salida);
	_delay_ms(100);
	/* Configurar el dispositivo en modo esclavo */
	sprintf(buffer_salida, "AT+ROLE%c\r\n", mode);
	UARTn_Tx_String(UART0, buffer_salida);
	_delay_ms(100);

	/* Repetir indefinidamente ... */
	while(1){
		/* Transmitir un texto al teléfono celular ... */
		UARTn_Tx_String(UART0, "Módulo configurado\n\r");
		/* Hacer parpadear el LED1 */
		PORT_LEDS ^= (1 << LED1);
		_delay_ms(1000);
	}

	return 0;
}

/* Definición de funciones */

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




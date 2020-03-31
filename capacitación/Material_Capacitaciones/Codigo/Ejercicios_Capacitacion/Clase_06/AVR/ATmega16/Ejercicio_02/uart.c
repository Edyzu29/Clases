/*
 * uart.c
 *
 *  Created on: 18 mar. 2018
 *      Author: MiguelAngel
 */

/* Inclusiones */
#include <avr/io.h>
#include <stdint.h>
#include "uart.h"


/****************************************************************************
* Nombre de la funci�n: UARTn_Configurar
* retorna : Nada
* arg : uart_param
* Descripci�n : Recibe un puntero hacia una estructura de tipo UART_t (la
* 				cual contiene los par�metros de configuraci�n) y configura
* 				el m�dulo USARTn.
* Notas : La funcionalidad de la velocidad doble (modo 2X) no se habilita
* 		  en esta funci�n, sino que debe definirse en el archivo cabecera
* 		  (#define MODOn_2X). Esta funci�n solo sirve para configurar el
* 		  USARTn para trabajar en modo as�ncrono (como UART).
*****************************************************************************/
void UART0_Configurar(UART_t *uart_param){

	uint32_t my_ubrr;

	/* Inicializar los registros de configuraci�n */
	UCSRA = 0;
	UCSRB = 0;
	UCSRC = 0;

	/* Operaci�n del USART en Modo As�ncrono (UART) */
	UCSRC &= ~(1 << UMSEL);

	if(uart_param->u2x){
		/* Calcular el valor de UBRR */
		my_ubrr = ((F_OSC / 8) / uart_param->baud_rate) - 1;
		/* Habilitar el modo r�pido 2X */
		UCSRA |= (1 << U2X);
	}else{
		/* Calcular el valor de UBRR */
		my_ubrr = ((F_OSC / 16) / uart_param->baud_rate) - 1;
		/* Deshabilitar el modo r�pido 2X */
		UCSRA &= ~(1 << U2X);
	}

	/* Configurar la tasa de transferencia de bits */
	UBRRH = (((uint16_t)my_ubrr) >> 8) & 0x0F;
	UBRRL = ((uint16_t)my_ubrr) & 0xFF;

	/* Configurar el tama�o del frame (bits de datos) */
	UCSRC |= ((uart_param->data_bits & 0x03) << UCSZ0)  | (1 << URSEL);
	UCSRB |= ((uart_param->data_bits >> 2) << UCSZ2);
	/* Configurar la cantidad de bits de parada */
	UCSRC |= (uart_param->stop_bits << USBS);
	/* Configurar los bits de paridad */
	UCSRC |= (uart_param->parity_mode << UPM0);
	/* Habilitar/deshabilitar el transmisor del USART */
	UCSRB |= (uart_param->tx_status << TXEN);
	/* Habilitar/deshabilitar el receptor del USART */
	UCSRB |= (uart_param->rx_status << RXEN);
	/* Habilitar las interrupciones */
	UCSRB |= uart_param->interrupt_mask;
}


/****************************************************************************
* Nombre de la funci�n: UARTn_Tx_Byte
* retorna : Nada
* arg : mod, byte
* Descripci�n : Se indica el m�dulo USART por el cual se desea transmitir un
* 				byte, este byte se considera un caracter (tipo char).
* 				La funci�n recibe un caracter y lo transmite de manera serial
* 				por el m�dulo USART correspondiente, de forma as�ncrona.
* Notas : La funci�n no lleva a cabo una nueva transmisi�n antes de que se
* 		  haya completado la transmisi�n anterior.
*****************************************************************************/
void UARTn_Tx_Byte(UART_Module_t uart_n, char byte){

	switch(uart_n){
	case UART0:
		/* No empezar la transmisi�n hasta que se haya completado la anterior */
		while(!(UCSRA & (1 << UDRE))){
			/* no avanzar hata que el registro de datos se encuentre vac�o */
		}
		/* Iniciar la transmisi�n del byte */
		UDR = byte;
		break;
	default:
		break;

	}
}


/****************************************************************************
* Nombre de la funci�n: UARTn_Tx_String
* retorna : Nada
* arg : str
* Descripci�n : Recibe un puntero a una cadena de caracteres y la transmite
* 				por el USARTn de forma as�ncrona.
* Notas : La funci�n primero transmite el caracter indicado por el puntero
* 		  str, y contin�a transmitiendo los siguientes caracteres hasta
* 		  encontrar el '\0' (fin de string).
*****************************************************************************/
void UARTn_Tx_String(UART_Module_t uart_n, char *str){

	/* Mientras no se alcance el final de la cadena */
	while(*str != '\0'){
		/* Transmitir el caracter correspondiente */
		UARTn_Tx_Byte(uart_n, *str);
		/* Incrementar el valor del puntero (apuntar al siguiente caracter
		en el arreglo) */
		str++;
	}
}


/****************************************************************************
* Nombre de la funci�n: UARTn_Tx_Integer
* retorna : Nada
* arg : num
* Descripci�n : Recibe un n�mero entero de 32 bits (con signo) y lo transmite
* 				por el USARTn de forma as�ncrona.
* Notas : La funci�n primero determina el signo del n�mero, luego procede a
* 		  extraer los d�gitos y almacenarlos en un buffer, y finalmente
* 		  transmite el signo y los d�gitos.
*****************************************************************************/
void UARTn_Tx_Integer(UART_Module_t uart_n, int32_t num){

	char signo_y_digitos[12];
	uint8_t signo = 0;
	int32_t digito;
	int8_t indice = 11;

	/* Determinar el signo del n�mero */
	if(num < 0){
		signo = 1;
		num = -num;
	}

	/* Indicar el fin de la cadena de caracteres */
	signo_y_digitos[indice--] = '\0';

	/* Extraer los d�gitos uno a uno, empezando por las unidades */
	do{
		digito = (num % 10) + '0';
		signo_y_digitos[indice--] = (char)digito;
		num /= 10;
	}while(num);

	/* Agregar el signo (de ser necesario) */
	if(signo){
		signo_y_digitos[indice] = '-';
	}else{
		indice++;
	}

	/* Transmitir el n�mero */
	UARTn_Tx_String(uart_n, &signo_y_digitos[indice]);
}


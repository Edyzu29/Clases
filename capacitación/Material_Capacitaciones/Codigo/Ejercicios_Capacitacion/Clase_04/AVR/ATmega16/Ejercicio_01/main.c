

/* Inclusiones */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

/* Constantes y macros */
#define DDR_SERIAL_TX	DDRD
#define PORT_SERIAL_TX	PORTD
#define SERIAL_TX		1

#define BIT_DURATION_US	417

/* Variables globales */

/* Declaraci�n de funciones */
void Serial_Tx_Byte(char byte);

/* Funci�n principal */
int main(void){

	/* Configurar el pin de transmisi�n como salida digital */
	DDR_SERIAL_TX |= (1 << SERIAL_TX);
	/* Inicializar el pin en ALTO */
	PORT_SERIAL_TX |= (1 << SERIAL_TX);

	/* Transmitir un texto indefinidamente */
	while(1){
		/* Transmitir mensaje */
		Serial_Tx_Byte('H');
		Serial_Tx_Byte('o');
		Serial_Tx_Byte('l');
		Serial_Tx_Byte('a');
		Serial_Tx_Byte(' ');
		Serial_Tx_Byte('m');
		Serial_Tx_Byte('u');
		Serial_Tx_Byte('n');
		Serial_Tx_Byte('d');
		Serial_Tx_Byte('o');
		Serial_Tx_Byte('.');
		Serial_Tx_Byte('\n');
		Serial_Tx_Byte('\r');
		/* Esperar un segundo */
		_delay_ms(1000);
	}

	return 0;
}


/* Definici�n de funciones */
void Serial_Tx_Byte(char byte){

	int8_t bit_pos;

	/* Transmitir el bit de inicio (en BAJO) */
	PORT_SERIAL_TX &= ~(1 << SERIAL_TX);
	_delay_us(BIT_DURATION_US);

	/* Transmitir los bits de datos */
	for(bit_pos = 0; bit_pos < 8; bit_pos++){
		// Si el bit en la posici�n correspondiente es '1' ...
		if(byte & (1 << bit_pos)){
			// Colocar el pin de transmisi�n en ALTO
			PORT_SERIAL_TX |= (1 << SERIAL_TX);
		// De lo contrario ...
		}else{
			// Colocar el pin de transmisi�n en BAJO
			PORT_SERIAL_TX &= ~(1 << SERIAL_TX);
		}
		/* Esperar un tiempo equivalente a la duraci�n del bit */
		_delay_us(BIT_DURATION_US);
	}

	/* Transmitir el bit de parada (en ALTO) */
	PORT_SERIAL_TX |= (1 << SERIAL_TX);
	_delay_us(BIT_DURATION_US);
}

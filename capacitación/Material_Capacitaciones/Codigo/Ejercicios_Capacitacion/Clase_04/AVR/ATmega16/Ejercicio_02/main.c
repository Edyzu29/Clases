

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
void Serial_Tx_String(char *str);
void Serial_Tx_Integer(int32_t num);


/* Funci�n principal */
int main(void){

	uint8_t num_01 = 123;
	int16_t num_02 = -350;
	int32_t num_03 = 0;

	/* Configurar el pin de transmisi�n como salida digital */
	DDR_SERIAL_TX |= (1 << SERIAL_TX);
	/* Inicializar el pin en ALTO */
	PORT_SERIAL_TX |= (1 << SERIAL_TX);

	/* Transmitir un texto indefinidamente */
	while(1){
		 Serial_Tx_String("Numero 01:");
		 Serial_Tx_Integer((int32_t)num_01);
		 Serial_Tx_String("\n\r");
		 _delay_ms(1000);
		 Serial_Tx_String("Numero 02:");
		 Serial_Tx_Integer((int32_t)num_02);
		 Serial_Tx_String("\n\r");
		 _delay_ms(1000);
		 Serial_Tx_String("Numero 03:");
		 Serial_Tx_Integer(num_03);
		 Serial_Tx_String("\n\r");
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


void Serial_Tx_String(char *str){

	/* Mientras no se alcance el final de la cadena */
	while(*str != '\0'){
		/* Transmitir el caracter correspondiente */
		Serial_Tx_Byte(*str);
		/* Incrementar el valor del puntero (apuntar al siguiente caracter
		en el arreglo) */
		str++;
	}
}


void Serial_Tx_Integer(int32_t num){

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
	}while(num != 0);

	/* Agregar el signo (de ser necesario) */
	if(signo){
		signo_y_digitos[indice] = '-';
	}else{
		indice++;
	}

	/* Transmitir el n�mero */
	Serial_Tx_String(&signo_y_digitos[indice]);
}



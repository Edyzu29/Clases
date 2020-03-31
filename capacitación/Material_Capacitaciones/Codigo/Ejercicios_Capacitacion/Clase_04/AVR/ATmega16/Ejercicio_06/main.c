/*
Ejercicio 09:
Modificar el programa del ejercicio anterior, de modo que el microcontrolador
reciba el comando �toggle�, seguido de un espacio, la posici�n del LED que se
desea manipular (de 0 a 7), y un salto de l�nea (tecla ENTER). Si se desea
manipular todos los LEDs a la vez, entonces debe escribirse �A� en lugar de la
posici�n. Por ejemplo: �toggle 2� conmuta el LED 2 y "toggle A" conmuta todos
los LEDs. Si se escribe un comando inv�lido, debe imprimirse un mensaje que lo
indique en la consola serial.
 */


/* Inclusiones */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "uart.h"

/* Constantes y macros */
#define DDR_DISPLAY		DDRC
#define PORT_DISPLAY	PORTC

// Tama�o del buffer
#define RX_BUFFER_SIZE	10


/* Variables globales */

// "Banderas" del sistema
volatile uint8_t evaluar_comando_flag = 0;
volatile uint8_t comando_invalido_flag = 0;
// Cadenas constantes
char *mensaje_error = "Comando o parametro invalido\n\r";
char *cmd_01 = "toggle ";
// Buffer de recepci�n
volatile char rx_buffer[RX_BUFFER_SIZE];


/* Declaraci�n de funciones (prototipos) */
void UART0_Inicializar(void);
void Evaluar_Comando(void);
void Evaluar_Parametro_Posicion(char *pos);
uint8_t String_Comparar(char *str1, char *str2, uint8_t n);

/* Funci�n principal */
int main(void){
	/* Configurar los pines de los segmentos como salida */
	DDR_DISPLAY = 0xFF;
	/* Configurar el UART */
	UART0_Inicializar();
	/* Activar el habilitador global de interrupciones */
	sei();

	while(1){
		/* Si la bandera para leer el comando est� en ALTO */
		if(evaluar_comando_flag){
			/* Limpiar la bandera */
			evaluar_comando_flag = 0;
			/* Evaluar la acci�n a tomar con el comando recibido */
			Evaluar_Comando();
			/* Si el comando es inv�lido, mostrar un mensaje de error */
			if(comando_invalido_flag){
				comando_invalido_flag = 0;
				UARTn_Tx_String(UART0, mensaje_error);
			}
		}
	}

	return 0;
}

/* Rutinas de servicio a interrupciones */
ISR(USART_RXC_vect){

	static uint8_t indice = 0;
	/* Almacenar el caracter recibido en el buffer */
	rx_buffer[indice] = UDR;
	/* Mostrar el caracter recibido en pantalla (eco) */
	UARTn_Tx_Byte(UART0, rx_buffer[indice]);
	/* Si se recibi� la tecla ENTER, indicar que se debe evaluar el comando,
	 * y volver al inicio del buffer */
	if(rx_buffer[indice] == '\r'){
		evaluar_comando_flag = 1;
		indice = 0;
	/* De lo contrario, actualizar el indice del buffer de manera normal */
	}else{
		indice++;
		if(indice >= RX_BUFFER_SIZE){
			indice = 0;
		}
	}
}

/* Definici�n de funciones */

void UART0_Inicializar(void){

	UART_t my_uart;

	my_uart.u2x = U2X_Enabled;
	my_uart.baud_rate = Baud_9600_bps;
	my_uart.tx_status = UART_Tx_Enabled;
	my_uart.rx_status = UART_Rx_Enabled;
	my_uart.data_bits = UART_8_Data_Bits;
	my_uart.stop_bits = UART_1_Stop_Bit;
	my_uart.parity_mode = UART_Parity_Disabled;
	my_uart.interrupt_mask = UART_Rx_Complete_IE;

	UART0_Configurar(&my_uart);
}


uint8_t String_Comparar(char *str1, char *str2, uint8_t n){

	uint8_t indice;
	uint8_t son_iguales = 1;

	for(indice = 0; indice < n; indice++){
		if(*(str1 + indice) != *(str2 + indice)){
			son_iguales = 0;
		}
	}
	return son_iguales;
}


void Evaluar_Comando(void){

	/* Si el nombre del comando ("toggle" + espacio) est� bien escrito ...*/
	if(String_Comparar(cmd_01, rx_buffer, 7)){
		/* Evaluar si la posici�n y el ENTER fueron ingresados correctamente */
		Evaluar_Parametro_Posicion(&rx_buffer[7]);
	/* De lo contrario, el comando se considera inv�lido inv�lido */
	}else{
		comando_invalido_flag = 1;
	}

	UARTn_Tx_Byte(UART0, '\n');
}

void Evaluar_Parametro_Posicion(char *pos){

	switch(*pos){
		case '0':
			if(*(pos + 1) == '\r'){
				PORT_DISPLAY ^= (1 << 0);
			}else{
				comando_invalido_flag = 1;
			}
			break;
		case '1':
			if(*(pos + 1) == '\r'){
				PORT_DISPLAY ^= (1 << 1);
			}else{
				comando_invalido_flag = 1;
			}
			break;
		case '2':
			if(*(pos + 1) == '\r'){
				PORT_DISPLAY ^= (1 << 2);
			}else{
				comando_invalido_flag = 1;
			}
			break;
		case '3':
			if(*(pos + 1) == '\r'){
				PORT_DISPLAY ^= (1 << 3);
			}else{
				comando_invalido_flag = 1;
			}
			break;
		case '4':
			if(*(pos + 1) == '\r'){
				PORT_DISPLAY ^= (1 << 4);
			}else{
				comando_invalido_flag = 1;
			}
			break;
		case '5':
			if(*(pos + 1) == '\r'){
				PORT_DISPLAY ^= (1 << 5);
			}else{
				comando_invalido_flag = 1;
			}
			break;
		case '6':
			if(*(pos + 1) == '\r'){
				PORT_DISPLAY ^= (1 << 6);
			}else{
				comando_invalido_flag = 1;
			}
			break;
		case '7':
			if(*(pos + 1) == '\r'){
				PORT_DISPLAY ^= (1 << 7);
			}else{
				comando_invalido_flag = 1;
			}
			break;
		case 'A':
			if(*(pos + 1) == '\r'){
				PORT_DISPLAY ^= 0xFF;
			}else{
				comando_invalido_flag = 1;
			}
			break;
		default:
			comando_invalido_flag = 1;
	}

}



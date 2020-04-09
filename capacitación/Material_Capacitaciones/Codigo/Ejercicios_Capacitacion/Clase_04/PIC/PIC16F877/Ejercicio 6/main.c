/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "Usart_PIC16F877A.h"
#include "Interrupciones_PIC16F877A.h"

/* Constantes y macros */
#define DISPLAY_PORT   PORTD
#define DISPLAY_TRIS    TRISD

// Definición de variables
char *cmd_01="toggle ";

/* Constantes y macros */
uint8_t String_Comparar(char *str1, char *str2, uint8_t n);
void Corroborar_Posicion(void);

/* Función principal */
int main(void) {
    INPUT_TRIS(RX, TRISC);
    OUTPUT_TRIS(TX, TRISC);
    DISPLAY_TRIS = 0x00;
    DISPLAY_PORT= 0x00;
    Config_Usart();
    Inicializar_Interrupciones();
    Activar_Uart_Tx_Rx_Interrupciones();

    /*Mandar informacion al usuario sobre que mensaje debe
     mandar para conmutar un segmento o todo el Display*/
    UART0_Tx_String("Para conmutar un segmento debes mandar 'toggle X' siendo \r\n"
                                    "X un valor del 1 al 8 para conmutar el valor correspondiente \r\n"
                                    "pero si quieres conmutar todo el Display mandar 'toggle A' \r\n"
                                    "simpre enviar cada comanda seguido por un enter A' \r\n");

    /* Repetir indefinidamente */
    while (1) {
        UART0_Rx_String();
        Corroborar_Posicion();        
    }
    return 0;
}


/* Definición de funciones */
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

void Corroborar_Posicion(void) {

    if ( String_Comparar(cmd_01, rx_buffer, 7) ) {
        
        switch (rx_buffer[7]) {
            case '1':
                if (rx_buffer[8] == '\r') DISPLAY_PORT ^= (1 << 0);
                else UART0_Tx_String("El código enviado es incorrecto");
                break;

            case '2':
                if (rx_buffer[8]== '\r') DISPLAY_PORT ^= (1 << 1);
                else UART0_Tx_String("El código enviado es incorrecto");
                break;

            case '3':
                if (rx_buffer[8]== '\r') DISPLAY_PORT ^= (1 << 2);
                else UART0_Tx_String("El código enviado es incorrecto");
                break;

            case '4':
                if (rx_buffer[8]== '\r') DISPLAY_PORT ^= (1 << 3);
                else UART0_Tx_String("El código enviado es incorrecto");
                break;

            case '5':
                if (rx_buffer[8]== '\r') DISPLAY_PORT ^= (1 << 4);
                else UART0_Tx_String("El código enviado es incorrecto");
                break;

            case '6':
                if (rx_buffer[8]== '\r') DISPLAY_PORT ^= (1 << 5);
                else UART0_Tx_String("El código enviado es incorrecto");
                break;
            
            case '7':
                if(rx_buffer[8]== '\r') DISPLAY_PORT ^=(1<<6);
                else UART0_Tx_String("El código enviado es incorrecto");
            break;
            
            case '8':
                if(rx_buffer[8]== '\r') DISPLAY_PORT ^=(1<<7);
                else UART0_Tx_String("El código enviado es incorrecto");
            break;
            
            case 'A':
                if(rx_buffer[8]== '\r') DISPLAY_PORT ^=0xff;
                else UART0_Tx_String("El código enviado es incorrecto");
            break;
        }
        
        UART0_Tx_String("Segmento conmutado\r\n\r\n");

    }else{
        UART0_Tx_String("El código enviado es incorrecto\r\n");
    }
}
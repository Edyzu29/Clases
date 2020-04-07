/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "Usart_PIC16F877A.h"

/* Constantes y macros */
#define DISPLAY_PORT   PORTD
#define DISPLAY_TRIS    TRISD

// Definición de variables
char      letras[]={'a','b','c','d','e','f','g','p'};
char      valor_recibido;
uint8_t indice;

/* Función principal */
int main(void){
    INPUT_TRIS(RX,TRISC);
    OUTPUT_TRIS(TX,TRISC);
    Config_Usart();
    DISPLAY_TRIS=0x00;      //configuramos los pines que se conectaran con el display como salida
    DISPLAY_PORT=0x00; 
    UART0_Tx_String("Hola mundo");

/* Repetir indefinidamente */
    while (1) {
        valor_recibido=Rx();            //Guardamos el valore recibido para comprobarlo luego
        for(indice=0;indice<8;indice++){
            if(valor_recibido==letras[indice]){         //Controbamos si el valor recibio es igual a alguna letra
                DISPLAY_PORT =(1<<indice);               //SI es verdadera la consigna, usamos el  indice para correr el valor
            }
        }
    }
    return 0;
}




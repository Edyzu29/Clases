/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "Usart_PIC16F887A.h"

/* Declaración de variables*/
char msj_char[]={"Pepito"};     //Edita un el mensaje para probar la capacidad de la funcion
int32_t valor_entero= -52;        //Valor entero con signo

/* Función principal */
int main(void){
    OUTPUT_TRIS(TX,PORTC);
    Config_Usart();

/* Repetir indefinidamente */
	while(1){
                UART0_Tx_String(msj_char);
                UART0_Tx_String("\r\n");
                __delay_ms(250);
                UART0_Tx_Integer(valor_entero);
                UART0_Tx_String("\r\n");
                __delay_ms(250);
	}
	
	return 0;
}


/* Definición de funciones */




/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "Usart_PIC16F887A.h"

/* Constantes y macros */
 #define     INPUT_TRIS(A, B)        B   |=    (1<<A)   
 #define     OUTPUT_TRIS(A, B)     B &= ~(1<<A)
 /*En la variable A, debes poner que pin quieres modificar
 * En la varible B, debes poner el puerto al que usaras
 * Ej: 
 *      - Quiero usar el pin 8 del puerto D para prender un Led
 *              OUTPUT_TRIS(7,TRISD)
 *      - Quiero usar el pin 4 del puerto B para leer un bit
 *              INTPUT_TRIS(3,TRISB)
 */

// Definición de pines



/* Declaración de variables*/
char msj_char[]={"Pepito"};
int32_t valor_entero= -52;


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




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
/*Varibles Globales*/
char msj[]={"Hola Mundo\r\n"};
uint8_t indice;

/* Función principal */
int main(void){
    // Configuramos el pin de transmision como salida
    INPUT_TRIS(TX,TRISC);
    //Configuramos el modulo serial
    Config_Usart();

/* Repetir indefinidamente */
	while(1){
                //Como el Msj tiene 14 elementos, usamos la funcion for para mandar toda informacion
                for(indice=0;indice<15;indice++){
                    //Como el msj es un arreglo, donde cada elemto es una letra, usamos el indice para saltar de letra en letra
                    Tx_char( msj[indice] );     
                    __delay_us(20);
                }
                __delay_ms(500);
	}	
	return  0;
}

/* OjO
    \r\n sirven para que cambiar de linea. Osea que para que cada cadena de mensajes enviados este en una linea
 * diferente y asi poder distinguir mejor asi el mensaje en una terminal  */



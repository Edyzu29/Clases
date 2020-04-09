/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "Usart_PIC16F887A.h"

char msj[] = {"Hola Mundo\r\n"};
uint8_t indice;

/* Función principal */
int main(void) {
    INPUT_TRIS(TX, TRISC); // Configuramos el pin de transmision como salida
    Config_Usart(); //Configuramos el modulo serial

    /* Repetir indefinidamente */
    while (1) {
        //Como el Msj tiene 14 elementos, usamos la funcion for para mandar toda informacion
        for (indice = 0; indice < 15; indice++) {

            Tx_char(msj[indice]); //Como el msj es un arreglo, donde cada elemto es una letra, usamos el indice para saltar de letra en letra
            __delay_us(20);
        }
        __delay_ms(500);
    }
    return 0;
}
/* Luego edita el valor baudiaje, usa la formula que esta en la libreria para hallar el valor adecuado */

/* OjO
    \r\n sirven para que cambiar de linea. Osea que para que cada cadena de mensajes enviados este en una linea
 * diferente y asi poder distinguir mejor asi el mensaje en una terminal  */



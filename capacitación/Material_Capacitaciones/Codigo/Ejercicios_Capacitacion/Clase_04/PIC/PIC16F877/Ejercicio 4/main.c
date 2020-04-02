
/* Inclusiones */
#include "system_config.h"
#include "Usart_PIC16F877A.h"
#include <xc.h>
#include <stdint.h>

/* Constantes y macros */


// Definici�n de variables
char valor_recibido;


/* Declaraci�n de funciones (prototipos) */


/* Funci�n principal */
int main(void){
    INPUT_TRIS(RX,PORTC);
    OUTPUT_TRIS(TX,PORTC);
    Config_Usart();

/* Repetir indefinidamente */
	while(1){
                valor_recibido=Rx();
                Tx_char(valor_recibido);
	}
	return 0;
}


/* Definici�n de funciones */



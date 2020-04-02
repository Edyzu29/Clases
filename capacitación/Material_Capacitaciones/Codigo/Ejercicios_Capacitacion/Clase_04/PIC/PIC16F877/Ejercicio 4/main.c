
/* Inclusiones */
#include "system_config.h"
#include "Usart_PIC16F877A.h"
#include <xc.h>
#include <stdint.h>

/* Constantes y macros */


// Definición de variables
char valor_recibido;


/* Declaración de funciones (prototipos) */


/* Función principal */
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


/* Definición de funciones */



/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "Usart_PIC16F877A.h"
#include "Interrupciones_PIC16F877A.h"

// Definición de variables
char valor_recibido;

/* Función principal */
int main(void){
    INPUT_TRIS(RX,PORTC);
    OUTPUT_TRIS(TX,PORTC);
    Config_Usart();
    Inicializar_Interrupciones();
    Activar_Uart_Tx_Rx_Interrupciones();
    UART0_Tx_String("Hola mundo\r\n");

/* Repetir indefinidamente */
	while(1){
                if(RX_FLAG==1){                            //La bandera nos indica cuando ha llegado un nuevo valor
                    RX_FLAG=0;                              //Limpiar la bandera RX_FLAG 
                    valor_recibido=Rx();  
                    Tx_char(valor_recibido);        //Mandamos el valor recivido
                    UART0_Tx_String("\r\n");      //Este mensaje se manda para que sea más legible en el terminal
                }                
	}
	return 0;
}
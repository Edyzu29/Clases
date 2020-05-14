/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "SPI_PIC16F877A.h"
#include "Usart_PIC16F877A.h"
#include "MAX6675.h"

/* Constantes y macros */
#define ALARMA                  0
#define ALARMA_TRIS      TRISD
#define ALARMA_PORT    PORTD

#define ALARMA_ON()      ALARMA_PORT |=     (1<<ALARMA)
#define ALARMA_OFF()    ALARMA_PORT &= ~(1<<ALARMA)


#define VALOR_MAX       30
#define VALOR_MIN        10

#define AJUSTE              24
// Definición de variables
uint16_t esclavo1;

/* Declaración de funciones (prototipos) */

/* Función principal */
int main(void) {
    //Configuración de pines
    OUTPUT_TRIS(RX, TRISC);
    
    OUTPUT_TRIS(CS, TRISC);
    OUTPUT_TRIS(SDK, TRISC);
    OUTPUT_TRIS(SDO, TRISC);
    INPUT_TRIS(SDI, TRISC);
    
    OUTPUT_TRIS(ALARMA, ALARMA_TRIS);
    
    //Configuración de perifericos
    Config_Usart();
    Config_SPI();
    
    UART0_Tx_String("Saludos, el proyecto realizado podra mostrara la temperatura \r\n"
                                    "medida por la termocupla \r\n");
    
    UART0_Tx_String("Los valores extremos son: \r\n"
                                            "Temperatura maxima  "); UART0_Tx_Integer(VALOR_MAX);
    UART0_Tx_String("\r\nTemperatura mínima   "); UART0_Tx_Integer(VALOR_MIN);
    UART0_Tx_String("\r\n");


    /* Repetir indefinidamente */
    while (1) {
        __delay_ms(1000);
        /*Para recibir el dato por el SPI necesitamos activar el enable y luego desactivarlo*/
        CS_ON();
        esclavo1=Max6675_Val()-AJUSTE;
        CS_OFF();
        
        /*Enviamos en valor por el Puerto Serial y luego comprobamos en que rango está*/
        UART0_Tx_Integer(esclavo1); UART0_Tx_String("\r\n");       
        if(esclavo1<VALOR_MIN | esclavo1>VALOR_MAX){
            ALARMA_ON();
        }else{
            ALARMA_OFF();
        }
    }
    return 0;
}


/* Definición de funciones */


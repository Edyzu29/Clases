/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "SPI_PIC16F877A.h"
#include "Usart_PIC16F877A.h"
#include "MAX6675.h"

/* Constantes y macros */
#define ALARMA_CONTINUA                          0
#define ALARMA_INTERMITENTE                  1

#define ALARMAS_TRIS      TRISD
#define ALARMAS_PORT    PORTD

#define ALARMA_ON(A)      ALARMAS_PORT |=     (1<<A)
#define ALARMA_OFF(A)    ALARMAS_PORT &= ~(1<<A)

#define VALOR_MAX       30
#define VALOR_MIN        10

#define AJUSTE              24
 
// Definición de variables
uint16_t esclavo1;
uint16_t esclavo2;

/* Declaración de funciones (prototipos) */

/* Función principal */
int main(void) {
    //Configuración de pines
    OUTPUT_TRIS(RX, TRISC);
    
    OUTPUT_TRIS(CS, TRISC);
    OUTPUT_TRIS(SDK, TRISC);
    OUTPUT_TRIS(SDO, TRISC);
    INPUT_TRIS(SDI, TRISC);
    
    OUTPUT_TRIS(ALARMA_CONTINUA, ALARMAS_TRIS);
    OUTPUT_TRIS(ALARMA_INTERMITENTE, ALARMAS_TRIS);
    
    //Configuración de perifericos
    Config_Usart();
    Config_SPI();
    
    UART0_Tx_String("Saludos, el proyecto realizado podra mostrara la temperatura medida por las termocuplas \r\n");
    
    UART0_Tx_String("Los valores extremos son: \r\n"
                                            "Temperatura maxima  "); UART0_Tx_Integer(VALOR_MAX);
    UART0_Tx_String("\r\nTemperatura mínima   "); UART0_Tx_Integer(VALOR_MIN);
    UART0_Tx_String("\r\n");
   

    /* Repetir indefinidamente */
    while (1) {
        __delay_ms(1000);
        /*Para recibir el dato por el SPI necesitamos activar el enable y luego desactivarlo para ambos casos*/
        CS1_ON();
        esclavo1=Max6675_Val() - AJUSTE;
        CS2_ON();
        
        CS2_ON();
        esclavo2=Max6675_Val() - AJUSTE;
        CS1_ON();
        
        /*Luego enviamos dichos valores por el puerto serial y finalmente comprobamos en que rango estan cada valor*/
        UART0_Tx_String("La primera Termocupla ");    UART0_Tx_Integer(esclavo1);    UART0_Tx_String(" C \r\n");
        UART0_Tx_String("La segunda Termocupla ");   UART0_Tx_Integer(esclavo2);    UART0_Tx_String(" C \r\n");
                
        if(esclavo1<VALOR_MIN | esclavo1>VALOR_MAX){
            ALARMA_ON(ALARMA_CONTINUA);
        }else{
            ALARMA_OFF(ALARMA_CONTINUA);
        }
        
        if(esclavo2<VALOR_MIN | esclavo2>VALOR_MAX){
            ALARMA_ON(ALARMA_INTERMITENTE);
            __delay_ms(500);
            ALARMA_OFF(ALARMA_INTERMITENTE);
            __delay_ms(250);
        }else{
            ALARMA_OFF(ALARMA_INTERMITENTE);
        }
    }
    return 0;
}


/* Definición de funciones */


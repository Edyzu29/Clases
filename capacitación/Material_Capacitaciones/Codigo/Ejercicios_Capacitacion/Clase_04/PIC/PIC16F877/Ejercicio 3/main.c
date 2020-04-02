/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "ADC_PIC16F877A.h"
#include "Usart_PIC16F887A.h"

// Definición de variables
uint16_t valor_potenciometro[20];
uint8_t indice;

/* Función principal */
int main(void){
    Config_ADC();
    Config_Usart();
    OUTPUT_TRIS(TX,PORTC);
    OUTPUT_TRIS(0,PORTA);

/* Repetir indefinidamente */
	while(1){
            for(indice=0;indice<21;indice++){
                valor_potenciometro[indice]=Lectura_ADC(0);
                __delay_us(125);
            }		
            for(indice=0;indice<21;indice++){
                UART0_Tx_Integer(valor_potenciometro[indice]);
                UART0_Tx_String("\r\n");
            }
	}
	return 0;
}
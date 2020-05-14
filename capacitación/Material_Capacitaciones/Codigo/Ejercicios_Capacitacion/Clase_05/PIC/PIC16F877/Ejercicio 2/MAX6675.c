#include <xc.h>
#include <stdint.h>
#include "MAX6675.h"
#include "SPI_PIC16F877A.h"

/****************************************************************************
* Nombre de la función: Max6675_Bin_Val
* retorna : valor entero medido
* arg : Ninguno
* Descripción : Lleva a cabo la lectura de los dos bytes porporcionados por
* 				el dispositivo, y retorna el valor binario (de 12 bits) que
* 				sera convertido a entero.
*
* Notas : El valor binario de la medición es empleado cuando se requiere una
* 		 mayor precisión para monitorear y controlar temperatura.
*****************************************************************************/
uint16_t Max6675_Val(void){
    uint8_t primer_byte; 
    uint8_t segundo_byte;    
    uint16_t valor_temp;
    
    primer_byte = Leer_Spi();       //Leemos los 2 bytes traducidos por la Termocupla 
    segundo_byte = Leer_Spi();
    valor_temp=  ((primer_byte << 3) + (segundo_byte >> 5));    // Concatenar los bits recibidos para reconstruir el valor de la temperatura 
    return valor_temp;    
}
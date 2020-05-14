#include <xc.h>
#include <stdint.h>
#include "ADC_Inicializar.h"

void ADC_inicializacion(int Activar_ADC) {
    /*32 TOSC para trabajar con el cristal de 20MHZ*/
    ADCON0bits.ADCS = 0b010;
    /*Se activa el conversor según sea necesario*/
    if (Activar_ADC == 1) {
        ADCON0bits.ADON = 1; /*Activamos el conversor*/
    } else {
        ADCON0bits.ADON = 0; /*Desactivamos el conversor*/
    }
    /*Se justifican los datos hacia a la izquierda para tomar los bits más significativos*/
    ADCON1bits.ADFM = 0;
    /*Los pines son todos analógicos*/
    ADCON1bits.PCFG = 0x00;
}

uint16_t Leer_ADC(char Puerto_Analogico) {
    uint16_t Valor_De_Retorno_ADC;
    ADRESH = 0; /*Limpiamos los registros de la conversión del ADC*/
    ADRESL = 0;
    ADCON0bits.CHS = Puerto_Analogico; /*Seleccionamos el puerto análogo que se desea leer*/
    ADCON0bits.GO = 1; /*Iniciamos la conversión del timer0*/
    while (ADCON0bits.GO); /*Hacemo un bucle mientras se da la conversión del ADC*/

    Valor_De_Retorno_ADC = ADRESH; /*Retornamos solo los 8 bits más significativos*/

    return Valor_De_Retorno_ADC; /*Retornamos el valor de retorno del ADC*/
}


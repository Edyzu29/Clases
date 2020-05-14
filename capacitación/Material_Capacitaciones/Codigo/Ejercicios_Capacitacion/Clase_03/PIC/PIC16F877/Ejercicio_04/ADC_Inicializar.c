#include <xc.h>
#include <stdint.h>
#include "ADC_Inicializar.h"

/****************************************************************************
 * Nombre de la función: ADC_inicializacion
 * retorna : nada
 * ingresa: nada
 * Descripción : Configura los registros que intervienen en el modulo de ADC
 *****************************************************************************/ 
void ADC_inicializacion(void) {
    /*32 TOSC para trabajar con el cristal de 20MHZ*/
    ADCON0bits.ADCS = 0b010;
    /*Se activa el conversor*/
    ADCON0bits.ADON = 1;
    /*Se justifican los datos hacia la derecha*/
    ADCON1bits.ADFM = 1;
    /*Los pines son todos analógicos*/
    ADCON1bits.PCFG = 0x00;
}

/****************************************************************************
 * Nombre de la función: Leer_ADC
 * retorna : Un dato de tipo "Int16"
 * ingresa: Un dato de tipo "Int8"
 * Descripción : El valor ingresado selecciona el canal por el cual se leerá 
		el voltaje para luego ser transformado a digital por el módulo 
 *****************************************************************************/ 
uint16_t Leer_ADC(char Puerto_Analogico) {
    uint16_t Valor_De_Retorno_ADC;
    ADRESH = 0; /*Limpiamos los registros de la conversión del ADC*/
    ADRESL = 0;
    ADCON0bits.CHS = Puerto_Analogico; /*Seleccionamos el puerto análogo que se desea leer*/
    ADCON0bits.GO_nDONE = 1; /*Iniciamos la conversión del timer0*/
    while (ADCON0bits.GO_nDONE); /*Hacemo un bucle mientras se da la conversión del ADC*/

    Valor_De_Retorno_ADC = (ADRESH << 8) | ADRESL;

    return Valor_De_Retorno_ADC; /*Retornamos el valor de retorno del ADC*/
}


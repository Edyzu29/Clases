#include <xc.h>
#include <stdint.h>
#include "ADC_Inicializar.h"

/****************************************************************************
 * Nombre de la funci�n: ADC_inicializacion
 * retorna : nada
 * ingresa: nada
 * Descripci�n : Configura los registros que intervienen en el modulo de ADC
 *****************************************************************************/ 
void ADC_inicializacion(void) {
    /*32 TOSC para trabajar con el cristal de 20MHZ*/
    ADCON0bits.ADCS = 0b010;
    /*Se activa el conversor*/
    ADCON0bits.ADON = 1;
    /*Se justifican los datos hacia la derecha*/
    ADCON1bits.ADFM = 1;
    /*Los pines son todos anal�gicos*/
    ADCON1bits.PCFG = 0x00;
}

/****************************************************************************
 * Nombre de la funci�n: Leer_ADC
 * retorna : Un dato de tipo "Int16"
 * ingresa: Un dato de tipo "Int8"
 * Descripci�n : El valor ingresado selecciona el canal por el cual se leer� 
		el voltaje para luego ser transformado a digital por el m�dulo 
 *****************************************************************************/ 
uint16_t Leer_ADC(char Puerto_Analogico) {
    uint16_t Valor_De_Retorno_ADC;
    ADRESH = 0; /*Limpiamos los registros de la conversi�n del ADC*/
    ADRESL = 0;
    ADCON0bits.CHS = Puerto_Analogico; /*Seleccionamos el puerto an�logo que se desea leer*/
    ADCON0bits.GO_nDONE = 1; /*Iniciamos la conversi�n del timer0*/
    while (ADCON0bits.GO_nDONE); /*Hacemo un bucle mientras se da la conversi�n del ADC*/

    Valor_De_Retorno_ADC = (ADRESH << 8) | ADRESL;

    return Valor_De_Retorno_ADC; /*Retornamos el valor de retorno del ADC*/
}


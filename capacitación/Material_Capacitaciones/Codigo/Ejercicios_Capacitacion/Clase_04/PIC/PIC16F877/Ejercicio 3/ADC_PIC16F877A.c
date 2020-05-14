#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "ADC_PIC16F877A.h"

/****************************************************************************
 * Nombre de la función: Config_ADC
 * retorna : nada
 * ingresa: nada
 * Descripción : Configura los registros que intervienen en el modulo de ADC
 *****************************************************************************/ 
void Config_ADC(void){
    ADCON1bits.ADCS2=0b1; ADCON0bits.ADCS=0b00;
    /*ADCS2    ADCS1-0     Clock Conversion
            0               00               FOSC/2
            0               01              FOSC/8
            0               10              FOSC/32
            0               11              FRC (clock derived from the internal A/D RC oscillator)
            1               00              FOSC/4
            1               01              FOSC/16
            1               10              FOSC/64
            1               11              FRC (clock derived from the internal A/D RC oscillator)
   */
    ADCON0bits.ADON=0b1;
     /*  1 = Enciende el ADC
         0 = Apaga el ADC 
    */
    ADCON1bits.ADFM=1;
    /*  1 = Right justified. Six (6) Most Significant bits of ADRESH are read as ?0?.
     *  0 = Left justified. Six (6) Least Significant bits of ADRESL are read as ?0?.
     */
    ADCON1bits.PCFG=0b1110;
    /*Configurar los pines analogos
     * PCFG3-0   AN7 AN6 AN5 AN4 AN3 AN2 AN1 AN0 VREF+ VREF- 
        0000        A       A        A      A       A       A   A        A   VDD      VSS 
        0001        A       A       A       A   VREF+ A    A        A    AN3     VSS 
        0010        D       D       D       A       A      A    A        A    VDD     VSS 
        0011        D       D       D       A   VREF+ A    A        A    AN3     VSS 
        0100        D       D       D       D       A      D    A        A    VDD      VSS 
        0101        D       D       D       D   VREF+ D    A        A    AN3     VSS 
        011x        D       D       D       D       D      D    D        D      ?          ? 
        1000        A       A       A       A   VREF+ VREF- A   A    AN3    AN2
        1001        D       D       A       A       A       A    A       A    VDD    VSS 
        1010        D       D       A       A   VREF+ A     A       A   AN3     VSS 
        1011        D       D       A       A   VREF+ VREF- A   A   AN3     AN2
        1100        D       D       D       A   VREF+ VREF- A   A   AN3     AN2
        1101        D       D       D       D   VREF+ VREF- A   A   AN3     AN2
        1110        D       D       D       D       D       D       D    A   VDD     VSS 
        1111        D       D       D       D   VREF+ VREF- D   A    AN3   AN2 
     */
}

/****************************************************************************
 * Nombre de la función: Lectura_ADC 
 * retorna : Un dato de tipo "Int16"
 * ingresa: Un dato de tipo "Int8"
 * Descripción : El valor ingresado selecciona el canal por el cual se leerá 
		el voltaje para luego ser transformado a digital por el módulo 
 *****************************************************************************/ 
uint16_t Lectura_ADC (uint8_t canal){
    ADCON0bits.CHS=canal;
    /*  Selector de canales de lecturas
        000 = Channel 0 (AN0)
        001 = Channel 1 (AN1)
        010 = Channel 2 (AN2)
        011 = Channel 3 (AN3)
        100 = Channel 4 (AN4)
        101 = Channel 5 (AN5)
        110 = Channel 6 (AN6)
        111 = Channel 7 (AN7)
     */
    ADCON0bits.GO_nDONE = 1;            // Inicia conversión
    while (ADCON0bits.GO);                    // Espera hasta que conversión termine
    return (ADRESH<<8) + (ADRESL);          // Devuelve los 10 bits obtenidos
}
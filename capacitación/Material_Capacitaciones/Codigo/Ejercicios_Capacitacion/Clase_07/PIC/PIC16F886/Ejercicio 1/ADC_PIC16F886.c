#include <xc.h>
#include <stdint.h>
#include "ADC_PIC16F886.h"

/****************************************************************************
 * Nombre de la función: Config_ADC
 * retorna : nada
 * ingresa: nada
 * Descripción : Configura los registros que intervienen en el modulo de ADC
 *****************************************************************************/
void Config_ADC(void) {
    ADCON0bits.ADCS = 0b00;// Seleccionas el reloj de conversión
    /* 00 = FOSC/2
        01 = FOSC/8
        10 = FOSC/32
        11 = FRC (clock derived from a dedicated internal oscillator = 500 kHz max)
     */
    
    ADCON0bits.ADON = 1;
    /*  1 = Enciende el ADC
        0 = Apaga el ADC 
     */
    
    ADCON1bits.ADFM = 1;
    /*  1 = Right justified. Six (6) Most Significant bits of ADRESH are read as ?0?.
     *  0 = Left justified. Six (6) Least Significant bits of ADRESL are read as ?0?.
     */
    
    ANSELbits.ANS0=0;
    ANSELbits.ANS1=1;
    ANSELbits.ANS2=1;
    ANSELbits.ANS3=1;
    ANSELbits.ANS4=1;
    ANSELHbits.ANS8=1;
    ANSELHbits.ANS9=1;
    ANSELHbits.ANS10=1;
    ANSELHbits.ANS11=1;
    ANSELHbits.ANS12=1;
    ANSELHbits.ANS13=1;
    /*Segun que Pin usaras para leer un valor analógico, lo colocas como 1(High)
     * El numero que acompaña al ANS coincide con el canal 
     */
  
}

/****************************************************************************
 * Nombre de la función: Lectura_ADC 
 * retorna : Un dato de tipo "Int16"
 * ingresa: Un dato de tipo "Int8"
 * Descripción : El valor ingresado selecciona el canal por el cual se leerá 
        el voltaje para luego ser transformado a digital por el módulo 
 *****************************************************************************/
uint16_t Lectura_ADC(uint8_t canal) {
    ADCON0bits.CHS = canal;
    /*  Selector de canales de lecturas
        0000 = AN0
        0001 = AN1
        0010 = AN2
        0011 = AN3
        0100 = AN4
        0101 = AN5
        0110 = AN6
        0111 = AN7
        1000 = AN8
        1001 = AN9
        1010 = AN10
        1011 = AN11
        1100 = AN12
        1101 = AN13
     */
    while (ADCON0bits.GO); // Espera hasta que conversión termine
    return (ADRESH << 8) + (ADRESL); // Devuelve los 10 bits obtenidos
}

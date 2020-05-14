/* 
 * File:   ADC_PIC16F886.h
 * Author: maxed
 *
 * Created on 13 de mayo de 2020, 01:49 AM
 */
#include <stdint.h>

#ifndef ADC_PIC16F886_H
#define	ADC_PIC16F886_H

/*Constantes*/
#define Inicio_Adc ADCON0bits.GO

/*Declaracion de Funciones*/
void Config_ADC(void);              
    /*Configacion del Modulo ADC*/
uint16_t Lectura_ADC(uint8_t);
    /*Funci?n que lee un pin analogico querido*/

#endif	/* ADC_PIC16F886_H */


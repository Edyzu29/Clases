/* 
 * File:   ADC_PIC16F877A.h
 * Author: maxed
 *
 * Created on 1 de abril de 2020, 07:32 PM
 */

#include <stdint.h>

#ifndef ADC_PIC16F877A_H
#define	ADC_PIC16F877A_H


/*Declaracion de Funciones*/
void Config_ADC(void);              
    /*Configacion del Modulo ADC*/
uint16_t Lectura_ADC(uint8_t);
    /*Función que lee un pin analogico querido*/

#endif	/* ADC_PIC16F877A_H */


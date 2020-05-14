/* 
 * File:   ADC_Inicializar.h
 * Author: HP
 *
 * Created on 12 de abril de 2020, 04:45 PM
 */

#ifndef ADC_INICIALIZAR_H
#define	ADC_INICIALIZAR_H

#include <xc.h>
#include <stdint.h>

/*
 * Macros
 */

#define CANAL_AN0 0x0
#define CANAL_AN1 0x1
#define CANAL_AN2 0x2
#define CANAL_AN3 0x3
#define CANAL_AN4 0x4
#define CANAL_AN5 0x5
#define CANAL_AN6 0x6
#define CANAL_AN7 0x7


/****************************************************************************
 * Nombre de la función: ADC_inicialización
 * retorna : Nada
 * Descripción : Configura los registros del ADC para realizar la conversión 
 * Notas :  Al momento de leer la hoja de datos del PIC16F877A en el apartado del ADC 
 * 
 *ADCON1bits.PCFG = 0xE;  Se considera ese valor de 1110 pues a diferencia de para evitar el voltaje de referencia pues
  si se coloca de este modo  ADCON1bits.PCFG = 0xF; usa voltajes de referencia.
 *****************************************************************************/
void ADC_inicializacion(void);

/****************************************************************************
 * Nombre de la función: Leer_ADC
 * retorna : Valor leído del ADC entero de 16 bits
 * ingresa: El puerto que se desea leer con el prefijo CANAL_ANX 
 * - El numero de bits que se desea leer ya sea de 8 bits del ADRESH o la suma con el ADRESH y ADRESL
 * Descripción : Devuelve el valor del ADC del pin que se quiere leer  
 * Notas :  Al momento de leer la hoja de datos del PIC16F877A en el apartado del ADC 
 * 
 *ADCON1bits.PCFG = 0x0;  Se considera ese valor de 0000 pues así ponemos todos los pines como análogos
 *****************************************************************************/
uint16_t Leer_ADC(char Puerto_Analogico);


/****************************************************************************
 * Nombre de la función: Retorno_De_Voltaje_ADC
 * retorna : Valor convertido con relación al voltaje
 * ingresa: El valor de la funcion Leer_ADC
 * Descripción : Devuelve el valor del ADC del pin que se quiere leer  
 *****************************************************************************/
#endif	/* ADC_INICIALIZAR_H */


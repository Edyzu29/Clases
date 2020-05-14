/* 
 * File:   Interrupciones_PIC16F877A.h
 * Author: maxed
 *
 * Created on 7 de abril de 2020, 06:53 PM
 */

/*Incluiones*/
#include <xc.h>

/*Macros y Constantes*/
#ifndef INTERRUPCIONES_PIC16F877A_H
#define	INTERRUPCIONES_PIC16F877A_H

#define INTERRUPCIONES_SWITCH    INTCONbits.GIE

#define RX_SWITCH           PIE1bits.RCIE
#define TX_SWITCH           PIE1bits.TXIE
#define RX_FLAG               PIR1bits.RCIF
#define TX_FLAG               PIR1bits.TXIF

#define ADC_SWITCH        PIE1bits.ADIE
#define ADC_FLAG             PIR1bits.ADIF

#define TMR0_SWITCH  INTCONbits.T0IE
#define TMR0_FLAG       INTCONbits.T0IF

#define TMR1_SWITCH  PIE1bits.TMR1IE
#define TMR1_FLAG       PIR1bits.TMR1IF

#define INT_SWITCH      INTCONbits.INTE
#define INT_FLAG           INTCONbits.INTF

#define EEPROM_SWITCH  PIE2bits.EEIE
#define EEPROM_FLAG       PIR2bits.EEIF

#define SPI_SWITCH          PIE1bits.SSPIE
#define SPI_FLAG               PIR1bits.SSPIF

/*Definicion de funciones*/
void Inicializar_Interrupciones(void);
void Activar_Uart_Tx_Rx_Interrupciones(void);
void Activar_ADC_Interrupciones(void);
void Activar_TMR0_Interrupciones(void);
void Activar_TMR1_Interrupciones(void);
void Activar_INT_Interrupciones(void);
void Activar_EEPROM_Interrupciones(void);
void Activar_SPI_Interrupciones(void);

#endif	/* INTERRUPCIONES_PIC16F877A_H */


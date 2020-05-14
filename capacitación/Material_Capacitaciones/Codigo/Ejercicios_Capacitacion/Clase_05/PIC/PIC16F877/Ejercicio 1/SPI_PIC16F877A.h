/* 
 * File:   SPI_PIC16F877A.h
 * Author: maxed
 *
 * Created on 9 de abril de 2020, 02:03 AM
 */

#ifndef SPI_PIC16F877A_H
#define	SPI_PIC16F877A_H

#include <xc.h>
#include <stdint.h>

/*
 * Macros
 */

#define CS PORTCbits.RC2
/*Definimos los pines usados por el SPI*/
#define TRIS_CS TRISCbits.TRISC2
#define TRIS_SCK TRISCbits.TRISC3
#define TRIS_SDI TRISCbits.TRISC4
#define TRIS_SDO TRISCbits.TRISC5

#define SPI_FLAG    PIR1bits.SSPIF

/*
 * Funciones 
 */

void Spi_Inicializacion(void);

void Escribir_Spi(char dato);

char Leer_Spi(void);

#endif	/* SPI_PIC16F877A_H */


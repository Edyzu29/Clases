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

/*Definimos los pines usados por el SPI*/
#define CS        2
#define SDK     3
#define SDI       4
#define SDO     5

#define CS_ON()      PORTC &= ~(1<<CS)
#define CS_OFF()     PORTC |= (1<<CS)

/*
 * Funciones 
 */

/****************************************************************************
 * Nombre de la función: Config_SPI
 * retorna : Nada
 * ingresa: Nada
 * Descripción : Permite iniciar con la configuración del SPI
 * Nota: Solo llamarse una vez dentro del codigo
 *****************************************************************************/

void Config_SPI(void);

/****************************************************************************
 * Nombre de la función: Escribir_Spi
 * retorna : Nada
 * ingresa: Dato a enviar "caracter"
 * Descripción : Permite enviar un dato o varios por el SPI
 * Nota: Puede enviar caracteres de 8 bits
 *****************************************************************************/

void Escribir_Spi(char dato);

/****************************************************************************
 * Nombre de la función: Leer_Spi
 * retorna : dato leído por el SPI
 * ingresa: nada
 * Descripción : Permite leer un dato o varios por el SPI
 * Nota: Puede leer variables de 8 bits
 *****************************************************************************/

char Leer_Spi(void);

#endif	/* SPI_PIC16F877A_H */


/* 
 * File:   Usart_PIC16F887A.h
 * Author: maxed
 *
 * Created on 31 de marzo de 2020, 12:29 PM
 */
/* Inclusión de archivos */
#include <xc.h>
#include <stdint.h>

/* Constantes y macros */
#ifndef       USART_PIC16F887A_H
#define	USART_PIC16F887A_H

#define     TX                          6
#define     RX                          7
/* Como se pude apreciar en el pinout del µControlador,
 *  los pines Rx y Tx se encuentran en  el puerto C*/

#define VALOR_BAUDIOS   25
/* En la siguientes formulas, tienes que hallar el valor de "X" segun el
 * Baudiaje requerido. Luego colocas tal valor en el VALOR_BAUDIOS
 * 
 * MODO ASICRONO
 *          - BRGH = 0 (Low Speed)
 *                  Baud Rate = FOSC/(64 (X + 1))
 *          - BRGH = 1 (High Speed)
 *                  Baud Rate = FOSC/(16 (X + 1))
 * 
 * MODO SINCRONO
 *          - BRGH = 0 (Low Speed)
 *                  Baud Rate = FOSC/(4 (X + 1))
 *  
 * Fosc= Frecuencia del oscilador 
 * Considera un error <2%, varia Fosc o BRGH para consiguir un valor ideal
 */

                                                    /* Declaracion de Funciones */
void Config_Usart(void);
// Configuraras si la velociadad y el tipo de comunicación; además de activar el puerto serial

void Tx_char(char data);
//Envia un caracter

void UART0_Tx_String(char *str);
//Envia una cadena

void UART0_Tx_Integer(int32_t num);
//Transforma un entero en una cadena, para luego ser enviado

char Rx(void);
//Recive un caracter 


#endif	/* USART_PIC16F887A_H */


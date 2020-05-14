/* 
 * File:   Usart_PIC16F886.h
 * Author: maxed
 *
 * Created on 13 de mayo de 2020, 01:49 AM
 */

#ifndef USART_PIC16F886_H
#define	USART_PIC16F886_H

#define     TX                          6
#define     RX                          7
/* Como se pude apreciar en el pinout del ?Controlador,
 *  los pines Rx y Tx se encuentran en  el puerto C*/

#define MENSAJE_SIZE    20
/*El tama?o del mensaje que ser? recepcionado*/

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

                                                    /* Varieables */
char rx_buffer[MENSAJE_SIZE];   
//Se guarda en un string los valores recepcionados

                                                    /* Declaracion de Funciones */
void Config_Usart(void);
// Configuraras si la velociadad y el tipo de comunicaci?n; adem?s de activar el puerto serial

void Tx_char(char data);
//Envia un caracter

void UART0_Tx_String(char *str);
//Envia una cadena

void UART0_Tx_Integer(int32_t num);
//Transforma un entero en una cadena, para luego ser enviado

char Rx(void);
//Recive un caracter 

void UART0_Rx_String(void);
//recive una cadena, siempre llamar a esta funcion para actulizar rx_buffer


#endif	/* USART_PIC16F886_H */


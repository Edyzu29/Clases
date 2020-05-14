/*
 * File:   principal.c
 * Author: HP
 *
 * Created on 15 de abril de 2020, 08:10 PM
 */


#include <xc.h>
#include <stdint.h>
#include "Config.h"
#include "Usart.h"

#define _XTAL_FREQ 4000000


void TIMER1_INIT(void);

void INTS_INIT(void);

void TIMER1_GO(void);


unsigned char llave = 0;
unsigned int contador_bits = 0;
unsigned int ir_comando = 0, ir_direccion = 0;
unsigned int resultado [35]; /*Dato por protocolo NEC*/

void __interrupt() isr(void) {
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {
        TMR1 = 0; /*Limpiamos el dato del temporizador*/
        llave = 0;
        contador_bits = 0;
        INTCONbits.TMR0IF = 0; /*Limpiamos la bandera de interrupcion del timer1*/
    }

    if (PIR1bits.CCP1IF) {
        TMR1 = 0;
        CCP1IE = 0;
        CCP1IF = 0;
        if (!llave) {
            resultado[contador_bits] = CCPR1;
            contador_bits++;
            if (contador_bits > 33) {
                ir_comando = 0;
                ir_direccion = 0;
                for (unsigned char i = 8; i > 0; i--) {
                    ir_direccion >>= 1;
                    ir_comando >>= 1;

                    if (resultado[i + 1] > 240) ir_direccion |= 0b10000000;
                    if (resultado[i + 17] > 240) ir_comando |= 0b10000000;
                }
                contador_bits = 0;
                llave = 1;
            }
        }
        CCP1IE = 1;
    }
}

void main(void) {
    ADCON1 = 0x06; /*Desactivamos los pines analogas*/
    Confi_Blue();
    char cadena[21];
    TRISCbits.TRISC2 = 1; /*Ponemos como entrada al pin RC2 (CCP1) */
    PIE1bits.CCP1IE = 1; /*Activamos la interrupcion del módulo de comparación*/
    CCP1CON = 0x04; /*Modo de captura cada flano de bajada*/
    /*
     *CCP1CONbits.CCP1M = 0b0100; Configuración como flanco de bajada en el modo de captura
     */
    PIR1bits.CCP1IF = 0; /*Limpiamos la bandera del CCP1*/

    TIMER1_INIT(); /*Configuramos al TIMER1*/

    INTS_INIT(); /*Habilitamos las interrupciones*/

    TIMER1_GO(); /*Damos inicio al timer1*/

    UART0_Tx_String("HOLA");

    while (1) {
        UART0_Tx_String("comando: ");
        UART0_Tx_Integer(ir_comando);
        UART0_Tx_String(" \r\n");
        UART0_Tx_String("direccion: ");
        UART0_Tx_Integer(ir_direccion);
        UART0_Tx_String(" \r\n");
        __delay_ms(100);
    }

    return;
}

void TIMER1_INIT(void) {
    /*Asignamos prescaler de 8 al timer1*/
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.TMR1CS = 0; /*Modo temporizador*/
}

void TIMER1_GO(void) {
    T1CONbits.TMR1ON = 1; /*Damos inicio al timer1*/
}

void INTS_INIT(void) {
    PIE1bits.TMR1IE = 1; /*Activamos las iterrupciones del Timer1*/
    INTCONbits.GIE = 1; /*Activamos las interrupciones globales*/
    INTCONbits.PEIE = 1; /*Activamos las interrupciones por perifericos*/
}
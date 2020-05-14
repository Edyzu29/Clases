#include <xc.h>
#include <stdint.h>
#include "Timer1_Conf.h"

/****************************************************************************
 * Nombre de la función: Timer1_Inicializar
 * retorna : nada
 * ingresa: nada
 * Descripción : Configura los registros del TMR1
 *****************************************************************************/
void Timer1_Inicializar(void) {
    INTCONbits.GIE = 1; /*Activamos las interrupciones globales*/
    INTCONbits.PEIE = 1; /*Activamos las interrupciones procedentes de los perifericos*/
    PIE1bits.TMR1IE = 1; /*Activamos las interrupciones del TIMER1*/
    PIR1bits.TMR1IF = 0; /*Limpiamos la bandera del TIMER1*/
    
    T1CONbits.T1CKPS = 0b11; /*Preescaler de 8*/
    T1CONbits.T1OSCEN = 0; /*El oscilador está habilitado*/
    T1CONbits.TMR1CS = 0; /*Oscilador interno FOSC/4*/
    T1CONbits.TMR1ON = 1; /*Se activa el TIMER1*/
    
    /*Cargamos el dato del timer1*/
      TMR1 = 64910; /*Cargamos el dato para llegar a 1 ms*/
}
#include <xc.h>
#include <stdint.h>
#include "Interrupciones_Timer1.h"

void Inicializar_Interrupciones_Timer1(void) {
    INTCONbits.GIE = 1; /*Activamos las interrupciones globales*/
    INTCONbits.PEIE = 1; /*Activamos las interrupciones procedentes de los perifericos*/
    PIE1bits.TMR1IE = 1; /*Activamos las interrupciones del TIMER1*/
    PIR1bits.TMR1IF = 0; /*Limpiamos la bandera del TIMER1*/
}

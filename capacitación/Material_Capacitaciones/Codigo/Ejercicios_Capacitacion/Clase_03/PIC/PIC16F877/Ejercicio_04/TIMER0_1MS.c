#include <xc.h>
#include "TIMER0_1MS.h"

void Timer0_Inicializar(void) {

    /* Timer0 en Modo Temporizador*/
    OPTION_REGbits.T0CS = 0;
    /* Incrementar con cada flanco de subida (solo para modo contador)*/
    OPTION_REGbits.T0SE = 0;
    /* Activar el Pre-Scaler para el Timer0 */
    OPTION_REGbits.PSA = 0;
    /* Definir el valor del Prescaler = 64 */
    OPTION_REGbits.PS = 0b101;
    /* Cargar el valor de OFFSET */
    TMR0 = 176;
    /* Habilitar la interrupción por desbordamiento del Timer0 */
    INTCONbits.T0IE = 1;
}

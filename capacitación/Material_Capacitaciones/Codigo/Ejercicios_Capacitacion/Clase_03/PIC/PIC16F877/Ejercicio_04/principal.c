/*
 * File:   principal.c
 * Author: HP
 *
 * Created on 1 de abril de 2020, 10:47 PM
 */

/*
 * CLASE 03 ? EJERCICIO 04
 *Conectar un potenciómetro al CANAL 0 del ADC del microcontrontrolador, y también conectar 04 displays de 7 segmentos multiplexados, cátodo común.
 * Visualizar en los displays el valor ADC (binario de 10 bits) correspondiente a la medición del potenciómetro. 
 * Emplear retardos por software (_delay_ms) para establecer un periodo de muestreo de 100ms. 
 * Emplear interrupciones para establecer la frecuencia con que se gestionan los habilitadores de los displays.
 */

#include <xc.h>
#include <stdint.h>
#include "Config.h"
#include "display_7_segmentos.h"
#include "ADC_Inicializar.h"
#include "TIMER0_1MS.h"

/*
 * Macros
 */

#define _XTAL_FREQ 20000000UL

/*
 * Variables
 */

uint8_t Variable_Captura_Adc = 0; /*Variable usada para guardar los datos procedentes de la conversión*/

/*
 * Principal
 */

void main(void) {
    /*Llamamos a la función de inicio del ADC*/
    ADC_inicializacion();
    /*Iniciamos la configuración del display*/
    Display_Inicializar();
    /*Iniciamos la condiguración del timer0*/
    Timer0_Inicializar();
    
    INTCONbits.GIE = 1; /*Activamos las interrupciones globales*/
    INTCONbits.PEIE = 1; /*Activa las interrupciones procedentes de los perifericos*/

    while (1) {    
        Display_Decodificar_Entero_Decimal(Leer_ADC(CANAL_AN0)); /*Se muestran los datos procedentes de la conversión el ADC en relación al voltaje*/
        __delay_ms(100); /*Tiempo de muestreo por software*/
    }
    return;
}

__interrupt() void ISR(void) {
    /* Limpiar la bandera de desbordamiento del Timer0 */
    if ( INTCONbits.T0IF)
    {
    /* Cargar el valor de OFFSET al Timer 0*/
    TMR0 = 131;
    /* Gestionar los habilitadores y escanear los botones */
    Display_Gestionar_Habilitadores();
    INTCONbits.T0IF = 0;
    }
}



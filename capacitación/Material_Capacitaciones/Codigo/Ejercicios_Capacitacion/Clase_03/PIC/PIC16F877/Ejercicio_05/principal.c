/*
 * File:   principal.c
 * Author: HP
 *
 * Created on 1 de abril de 2020, 10:47 PM
 */

/*
 * Conectar un sensor de temperatura LM35 al CANAL 2 del ADC del microcontrontrolador, y también conectar 04 displays de 7 segmentos multiplexados, cátodo común. 
 * Visualizar en los displays el valor de la temperatura (en grados Celsius) correspondiente a la medición del sensor. 
 * Emplear interrupciones para establecer un periodo de muestreo de 100ms y también para actualizar los datos en los displays.
 */

#include <xc.h>
#include <stdint.h>
#include "Config.h"
#include "display_7_segmentos.h"
#include "ADC_Inicializar.h"
#include "Timer1_Conf.h"
#include "Interrupciones_Timer1.h"

/*
 * Macros
 */

#define _XTAL_FREQ 20000000UL

/*
 * Variables
 */

uint16_t Valor_Adc = 1023; /*Variable usada para guardar los datos procedentes de la conversión*/
uint8_t Bandera_Actualizar_Datos = 0; /*Variable usada para realizar la operacion de relación entre el dato de entrada y el valor de conversión*/

/*
 * Principal
 */

void main(void) {

    uint16_t temperatura_por_4 = 0; /*Creamos una variable que guardará el valor de la temperatura en función al voltaje en una relación de 4 veces más*/
    uint16_t temperatura_dato = 0; /*Creamos la variable que guardará el valor real de la temperatura*/

    ADC_inicializacion(ADC_ON); /*Iniciamos el ADC, pero no lo activamos*/

    Display_Inicializar(); /*Configuramos los displays multiplexados*/

    Timer1_Inicializar(); /*Inicializamos el timer1 para que nos de un tiempo de 1ms*/

    Inicializar_Interrupciones_Timer1(); /*Habilitamos las interrupciones globales y del timer1*/

    PIE1bits.ADIE = 1; /*Activamos la interrupcion cuando se complete la conversión del ADC*/

    while (1) {
        if (Bandera_Actualizar_Datos == 1) {
            temperatura_por_4 = ((Valor_Adc * 100) / 255)*5; /*Convertimos el valor del ADC a una funcion de voltaje de 0 a 5v*/
            temperatura_dato = temperatura_por_4 / 4; /*Dividimos entre 4 la relación anterior*/
            Display_Decodificar_Entero_Decimal(temperatura_dato); /*Mostramos por los displays el valor obtenido por el LM35*/
            Bandera_Actualizar_Datos = 0; /*Limpiamos la bandera de muestra de datos*/
        }
    }
    return;
}

__interrupt() void ISR(void) {

    if (PIR1bits.TMR1IF) { /*Preguntamos por la bandera de desbordamiento del timer1*/
        static uint8_t Variable_Cuenta_100ms = 0; /*Creamos una variable estática que nos ayudará a llegar a los 100ms*/
        Display_Gestionar_Habilitadores(); /*Activamos el transistores que habilitan los displays multiplexados*/
        Variable_Cuenta_100ms++;
        if (Variable_Cuenta_100ms == 100) { /*Repetimos la funcion 100 veces para llegar a los 100ms*/
            /*
             * Nota: El timer1 puede llegar a los 100ms sin la necesidad de una cuenta sucesiva
             * solo que deseamos que en ese proceso de cuenta se vayan activando los transitores de los displays multiplexados
             * para poder apreciar los datos de la temperatura
             */
            ADCON0bits.ADON = 1; /*Damos inicio a la conversión del ADC*/
            Variable_Cuenta_100ms = 0; /*Reseteamos el valor de la cuenta para llegar a los 100ms*/
        }
        PIR1bits.TMR1IF = 0; /*Limpiamos la bandera del timer1*/
    }

    if (!PIR1bits.ADIF) { /*Preguntamos por la bandera de conversión del ADC*/
        Valor_Adc = Leer_ADC(CANAL_AN2); /*Tomamos el valor de conversión del canal Análogo AN2*/
        Bandera_Actualizar_Datos = 1; /*Habilitamos la bandera para la muestra de los datos por los displays*/
        PIR1bits.ADIF = 0; /*Limpiamos la bandera de conversión*/
    }
}

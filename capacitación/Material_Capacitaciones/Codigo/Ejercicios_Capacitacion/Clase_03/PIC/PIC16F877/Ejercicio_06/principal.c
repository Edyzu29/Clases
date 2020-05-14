/*
 * File:   principal.c
 * Author: HP
 *
 * Created on 2 de abril de 2020, 09:23 PM
 */


/*
 *CLASE 03 ? EJERCICIO 06 
 *Conectar dos potenciómetros al ADC del Microcontrolador (CANAL 0 y CANAL 1), y también conectar una pantalla LCD 2x16. 
 *Luego, escribir un programa que permita medir el valor ADC en AMBOS potenciómetros en un periodo de 100ms.
 *Mostrar en la pantalla LCD los valores de voltaje correspondientes a cada uno de los potenciómetros.  
 *Emplear interrupciones para establecer un periodo de muestreo de 50ms (para cada canal) y también para actualizar los datos en la LCD.
 */

#include <xc.h>
#include <stdint.h> 
#include "Config.h"
#include "LCD_2X16.h"
#include "ADC_Inicializar.h"
#include "Interrupciones_Timer1.h"
#include "Timer1_Conf.h"

#define _XTAL_FREQ 20000000UL

/*
 * MACROS
 */

#define Numero_Canales 2

/*
 * VARIABLES
 */

volatile uint16_t Valor_retorno_ADC[Numero_Canales];
volatile uint8_t bandera_habilitar_LCD = 0;
/*
 * FUNCIONES
 */

void Escribir_Valor_En_Lcd(uint16_t valor);

void Mensajes_Lcd_Pots(void);

/*
 * Principal
 */

void main(void) {

    uint16_t valor_voltaje[Numero_Canales];

    /*Llamamos a la función de inicio del ADC*/
    ADC_inicializacion(ADC_ON);
    /*Iniciamos la condiguración del timer0*/
    Timer1_Inicializar();
    /*Iniciamos la configuración del LCD*/
    Iniciar_Lcd();
    /*Iniciamos las interrupciones*/
    Inicializar_Interrupciones_Timer1();
    /*Mostramos los mensajes de POT1 y POT2*/
    Mensajes_Lcd_Pots();

    PIE1bits.ADIE = 1; /*Activamos la interrupción de conversión del ADC*/
    while (1) {
        if (bandera_habilitar_LCD == 1) {
            valor_voltaje[0] = ((Valor_retorno_ADC[0]*100) / 255)*5;
            valor_voltaje[1] = ((Valor_retorno_ADC[1]*100) / 255)*5;
            Posicion_Linea_Superior(6);
            Escribir_Valor_En_Lcd(valor_voltaje[0]);
            Posicion_Linea_Inferiror(6);
            Escribir_Valor_En_Lcd(valor_voltaje[1]);
            bandera_habilitar_LCD = 0;
        }
    }
    return;
}

void Escribir_Valor_En_Lcd(uint16_t valor) {
    int Centenas_Del_Valor_Introducido = 0;
    int Decenas_Del_Valor_Introducido = 0;
    int Unidades_Del_Valor_Introducido = 0;

    /*FRACCIONAMIENTO EN DECENAS, UNIDADES Y CENTENAS*/
    Centenas_Del_Valor_Introducido = valor / 100; /*Dividimos en 100 para quedarnos con solo las centenas como parte entera*/
    Decenas_Del_Valor_Introducido = (valor % 100) / 10; /*Sacámos el módulo 100 al valor introducido y luego lo dividimos entre 10 para quedanos con las decenas como parte entera*/
    Unidades_Del_Valor_Introducido = (valor % 100) % 10; /*Sacámos el módulo 100 al valor introducido y sacamos el módulo 10 para quedanos con las unidades como parte entera*/

    Lcd_Escribir_Caracter(Centenas_Del_Valor_Introducido + 0x30); /*Se muestra el dato correspondiente y se le adiciona 0X30 para convertirlo a ASCII*/
    Lcd_Escribir_Caracter('.');
    Lcd_Escribir_Caracter(Decenas_Del_Valor_Introducido + 0x30); /*Se muestra el dato correspondiente y se le adiciona 0X30 para convertirlo a ASCII*/
    Lcd_Escribir_Caracter(Unidades_Del_Valor_Introducido + 0x30); /*Se muestra el dato correspondiente y se le adiciona 0X30 para convertirlo a ASCII*/
    Lcd_Escribir_Caracter('v');

    return;
}

void Mensajes_Lcd_Pots(void) {
    Posicion_Linea_Superior(0); /*Posicionamos el cursor en el primer caracter de la linea superior del LCD*/
    Lcd_Escribir_Cadena("POT 1:"); /*Escribimos el mensaje "PRIMER POT:"*/
    Posicion_Linea_Inferiror(0); /*Posicionamos el cursor en el primer caracter de la linea superior del LCD*/
    Lcd_Escribir_Cadena("POT 2:"); /*Escribimos el mensaje "SEGUND POT:"*/
}

__interrupt() void ISR(void) {
    if (PIR1bits.TMR1IF) {
        ADCON0bits.ADON = 1; /*Habilitamos el ADC para obtener los datos que deseamos*/
        PIR1bits.TMR1IF = 0; /*Limpiamos la bandera del TIMER1*/
    }
    if (!PIR1bits.ADIF) {
        static uint8_t Selector_canal = 0; /*Variable para elegir el canal de conversión*/
        Valor_retorno_ADC[Selector_canal] = Leer_ADC(Selector_canal);
        Selector_canal++;
        if (Selector_canal == Numero_Canales) {
            bandera_habilitar_LCD = 1; /*Habilitamos la bandera para poder visualizar el valor de la conversión*/
            Selector_canal = 0; /*Ponemos a 0 la variable selectora de canales de conversión*/
        }
        PIR1bits.ADIF = 0; /*Limpiamos la bandera de la conversión*/
    }
}

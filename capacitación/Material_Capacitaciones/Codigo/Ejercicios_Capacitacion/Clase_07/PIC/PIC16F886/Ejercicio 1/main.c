#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "Usart_PIC16F886.h"
#include "TMR_PIC16F886.h"
#include "ADC_PIC16F886.h"
#include "Eplimin_TEMP_CONTROL_MODULE_PIC_Defines.h"
#include "LCD.h"
#include "Interrupciones_PIC16F886.h"
#include "PID_basico.h"

/* Constantes y macros */
#define N_MUESTRAS		32
#define TEMP_SETPOINT	40
#define ADC_SETPOINT	(((TEMP_SETPOINT * 1024.0)/500.0) + 102.0)

/* Variables globales */
volatile uint8_t muestra_filtrada;
volatile uint16_t valor_adc = 0;
uint16_t valor_adc_anterior = 0;
uint8_t temperatura = 0;
char buffer[17];


/* Declaración de funciones (prototipos) */
void Clean_TMR0Flag(void);

/* Función principal */
int main(void) {
    Config_Usart();
    Config_ADC();
    Config_TMR0();
    Config_TMR2_PWM();
    Activar_ADC_Interrupciones();
    Activar_TMR0_Interrupciones();
    /* Establecer el setpoint del PID */
    pid_setpoint = ADC_SETPOINT;
    /* Habilitar las interrupciones */
    Inicializar_Interrupciones();

    INPUT_TRIS(0, TRISA);

    OUTPUT_TRIS(TX, TRISC);
    INPUT_TRIS(RX, TRISC);

    OUTPUT_TRIS(1, TRISC);
    OUTPUT_TRIS(2, TRISC);

    /* Forzar a 0 el pin RW de la LCD */
    TRIS_LCD_RW &= ~(1 << LCD_RW);
    PORT_LCD_RW &= ~(1 << LCD_RW);
    /* Configurar la pantalla LCD */
    LCD_Init_4_Bits();
    /* Impresión del mensaje de bienvenida */
    LCD_Goto_XY(1, 1);
    LCD_Put_String(" Modulo Control ");
    LCD_Goto_XY(1, 2);
    LCD_Put_String(" de Temperatura ");

    __delay_ms(2000);

    /* Impresión de las cabeceras */
    LCD_Tx_Command(LCD_CLEAR);
    LCD_Goto_XY(1, 1);
    LCD_Put_String("Setpoint:      C");
    LCD_Goto_XY(15, 1);
    LCD_Put_Char(0xDF);
    LCD_Goto_XY(1, 2);
    LCD_Put_String("Medicion:      C");
    LCD_Goto_XY(15, 2);
    LCD_Put_Char(0xDF);
    sprintf(buffer, "%d", TEMP_SETPOINT);
    LCD_Goto_XY(11, 1);
    LCD_Put_String(buffer);


    /* Repetir indefinidamente */
    while (1) {
        if (muestra_filtrada) {
            muestra_filtrada = 0;
            pid_feedback = (double) valor_adc;
            PID_Basic();
            Duty_Cycle((uint16_t) pid_output);

            if (valor_adc != valor_adc_anterior) {
                valor_adc_anterior = valor_adc;
                temperatura = ((uint32_t) (valor_adc - 102) * 500) / 1024;
                sprintf(buffer, "%d", temperatura);
                LCD_Goto_XY(11, 2);
                LCD_Put_String("   ");
                LCD_Goto_XY(11, 2);
                LCD_Put_String(buffer);
                sprintf(buffer, "%d,%d\r\n", TEMP_SETPOINT, temperatura);
            }
            UART0_Tx_Integer(buffer);

        }

        return 0;
    }
}

/* Definición de funciones */
void Clean_TMR0Flag(void) {
    TMR0_FLAG = 0;
    TMR0 = 60;
}

void __interrupt(high_priority) ISR(void) {
    static uint32_t acumulador = 0;
    static uint8_t indice_muestra = 0;
    static uint8_t contador_15ms = 0;

    if (TMR0_FLAG == 1) {
        Clean_TMR0Flag();
        contador_15ms++;
        if (contador_15ms == 5) {
            contador_15ms = 0;
            Inicio_Adc = 1;
        }
    }

    if (ADC_FLAG == 1) {
        ADC_FLAG=0;
        acumulador += Lectura_ADC(0);
        indice_muestra++;

        if (indice_muestra == N_MUESTRAS) {
            valor_adc = acumulador / N_MUESTRAS;
            acumulador = 0;
            indice_muestra = 0;
            muestra_filtrada = 1;
        }
    }

}
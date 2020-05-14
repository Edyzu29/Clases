#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "Usart_PIC16F886.h"
#include "SPI_PIC16F886.h"
#include "Max6675.h"
#include "Eplimin_TEMP_CONTROL_MODULE_PIC_Defines.h"
#include "LCD.h"
#include "Interrupciones_PIC16F886.h"
#include "PID_basico.h"

/* Constantes y macros */
#define N_MUESTRAS		32
#define TEMP_SETPOINT	40
#define ADC_SETPOINT	(TEMP_SETPOINT * 4.0)
#define N_PAQUETES		60

/* Variables globales */
volatile uint8_t paquetes_aceptados = 1;
volatile uint8_t medir_y_controlar = 0;
volatile uint16_t valor_adc = 0;
uint16_t valor_adc_anterior = 0;
uint16_t temperatura = 0;
char buffer[17];


/* Función principal */
int main(void) {
    Config_Usart();
    Config_SPI();
    /* Habilitar las interrupciones */
    Inicializar_Interrupciones();
    /* Establecer el setpoint del PID */
    pid_setpoint = ADC_SETPOINT;
    /* Configurar Interrupcion externa*/
    Activar_INT_Interrupciones();


    INPUT_TRIS(0, TRISA);

    OUTPUT_TRIS(TX, TRISC);
    INPUT_TRIS(RX, TRISC);

    OUTPUT_TRIS(1, TRISC);
    OUTPUT_TRIS(2, TRISC);

    /* Configurar como salida el pin de activación del TRIAC */
    TRIS_TRIAC_OUT |= (1 << TRIAC_OUT);
    PORT_TRIAC_OUT &= ~(1 << TRIAC_OUT);
    /* Configurar como salida el pin del CS para el MAX6675 */
    TRIS_CS_MAX6675 |= (1 << CS_MAX6675);
    PORT_CS_MAX6675 |= (1 << CS_MAX6675);
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
        if (medir_y_controlar == 1) {
            /* Limpiar la bandera */
            medir_y_controlar = 0;
            /* Leer el valor de la termocupla */
            PORT_CS_MAX6675 &= ~(1 << CS_MAX6675);
            valor_adc = Max6675_Val();
            PORT_CS_MAX6675 |= (1 << CS_MAX6675);
            /* Ingresar el valor al algoritmo PID y ejecutarlo */
            pid_feedback = (double) valor_adc;
            PID_Basic();
            paquetes_aceptados = pid_output;
            if (valor_adc != valor_adc_anterior) {
                valor_adc_anterior = valor_adc;
                temperatura = valor_adc / 4;
                sprintf(buffer, "%d", temperatura);
                LCD_Goto_XY(11, 2);
                LCD_Put_String("   ");
                LCD_Goto_XY(11, 2);
                LCD_Put_String(buffer);
                sprintf(buffer, "%d,%d\r\n", TEMP_SETPOINT, temperatura);
            }
            UART0_Tx_String(buffer);
        }

    }

    return 0;
}



void __interrupt(high_priority) ISR(void) {
    static uint8_t contador_paquetes = 0;

    if (INT_FLAG == 1) {
        INT_FLAG = 0;
        contador_paquetes++;

        /* Activar el TRIAC según lo que indique el PID */
        if (contador_paquetes <= paquetes_aceptados) {
            PORT_TRIAC_OUT |= (1 << TRIAC_OUT);
        } else {
            PORT_TRIAC_OUT &= ~(1 << TRIAC_OUT);
        }
        /* Reiniciar la cuenta y reajustar cada N_PAQUETES */
        if (contador_paquetes == N_PAQUETES) {
            contador_paquetes = 0;
            medir_y_controlar = 1;
        }
    }


}
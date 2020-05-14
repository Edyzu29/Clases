#include    "Interrupciones_PIC16F877A.h"

/****************************************************************************
 * Nombre de la función: Inicializar_Interrupciones
 * retorna : nada
 * ingresa: nada
 * Descripción : Activa las interrupciones globales
 *****************************************************************************/ 
void Inicializar_Interrupciones(void){
    INTERRUPCIONES_SWITCH=1;
}

/****************************************************************************
 * Nombre de la función: Activar_Uart_Tx_Rx_Interrupciones
 * retorna : nada
 * ingresa: nada
 * Descripción : Activa las interrupciones del Puerto Usart y limpia sus 
		respectivas banderas
 *****************************************************************************/ 
void Activar_Uart_Tx_Rx_Interrupciones(void){
    RX_SWITCH=1;    TX_SWITCH=1;
    RX_FLAG=0;         TX_FLAG=0;
}

/****************************************************************************
 * Nombre de la función: Activar_ADC_Interrupciones
 * retorna : nada
 * ingresa: nada
 * Descripción : Activa las interrupciones del modulo ADC y limpia sus 
		respectivas banderas
 *****************************************************************************/ 
void Activar_ADC_Interrupciones(void){
    ADC_SWITCH=1;
    ADC_FLAG=0;
}

/****************************************************************************
 * Nombre de la función: Activar_TMR0_Interrupciones
 * retorna : nada
 * ingresa: nada
 * Descripción : Activa las interrupciones del modulo TMR0 y limpia sus 
		respectivas banderas
 *****************************************************************************/ 
void Activar_TMR0_Interrupciones(void){
    TMR0_SWITCH=1;
    TMR0_FLAG=0;
}

/****************************************************************************
 * Nombre de la función: Activar_TMR1_Interrupciones
 * retorna : nada
 * ingresa: nada
 * Descripción : Activa las interrupciones del modulo TMR1 y limpia sus 
		respectivas banderas
 *****************************************************************************/ 
void Activar_TMR1_Interrupciones(void){
    TMR1_SWITCH=1;
    TMR1_FLAG=0;
}

/****************************************************************************
 * Nombre de la función: Activar_INT_Interrupciones
 * retorna : nada
 * ingresa: nada
 * Descripción : Activa las interrupciones externas y limpia sus 
		respectivas banderas
 *****************************************************************************/ 
void Activar_INT_Interrupciones(void){
    INT_SWITCH=1;
    INT_FLAG=0;
}

/****************************************************************************
 * Nombre de la función: Activar_EEPROM_Interrupciones
 * retorna : nada
 * ingresa: nada
 * Descripción : Activa las interrupciones del modulo EEPROM y limpia sus 
		respectivas banderas
 *****************************************************************************/ 
void Activar_EEPROM_Interrupciones(void){
    EEPROM_SWITCH=1;
    EEPROM_FLAG=0;
}

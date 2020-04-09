#include    "Interrupciones_PIC16F877A.h"

void Inicializar_Interrupciones(void){
    INTERRUPCIONES_SWITCH=1;
    INTERRUPCIONES_PERIJERICOS_SWITCH=1;
}
void Activar_Uart_Tx_Rx_Interrupciones(void){
    RX_SWITCH=1;    TX_SWITCH=1;
    RX_FLAG=0;         TX_FLAG=0;
}
void Activar_ADC_Interrupciones(void){
    ADC_SWITCH=1;
    ADC_FLAG=0;
}
void Activar_TMR0_Interrupciones(void){
    TMR0_SWITCH=1;
    TMR0_FLAG=0;
}
void Activar_TMR1_Interrupciones(void){
    TMR1_SWITCH=1;
    TMR1_FLAG=0;
}
void Activar_INT_Interrupciones(void){
    INT_SWITCH=1;
    INT_FLAG=0;
}
void Activar_EEPROM_Interrupciones(void){
    EEPROM_SWITCH=1;
    EEPROM_FLAG=0;
}

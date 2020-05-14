#include <xc.h>
#include <stdint.h>
#include "Usart_PIC16F886.h"

/****************************************************************************
 * Nombre de la función: Config_TMR
 * retorna : nada
 * ingresa: nada
 * Descripción : Configura los registros del TMR0
 *****************************************************************************/
void Config_TMR0(void){
    OPTION_REGbits.T0CS=0; // Selecciona la fuente se la señal del clock
    /*1 = Transition on T0CKI pin
     *0 = Internal instruction cycle clock (FOSC/4)
     */
    OPTION_REGbits.T0SE=0; //Selecciona que flanco sera contabilizado
    /*1 = Increment on high-to-low transition on T0CKI pin
     * 0 = Increment on low-to-high transition on T0CKI pin
     */
    OPTION_REGbits.PSA=0; //Assignas a quien sera dirigido el prescaler
    /*1 = Prescaler is assigned to the WDT
     * 0 = Prescaler is assigned to the Timer0 module
     */
    OPTION_REGbits.PS=0b111;//Assignas el valor del prescaler
    /*BIT VALUE         TMR0 RATE           WDT RATE
     * 000                          1:2                         1:1
     * 001                          1:4                         1:2
     * 010                          1:8                         1:4
     * 011                          1:16                       1:8
     * 100                          1:32                       1:16
     * 101                          1:64                       1:32
     * 110                          1:128                     1:64
     * 111                          1:256                     1:128 
     */    
}

/****************************************************************************
 * Nombre de la función: Config_TMR2_PWM
 * retorna : nada
 * ingresa: nada
 * Descripción : Configura los registros del TMR2 para formar una PWM
 *****************************************************************************/
void Config_TMR2_PWM (void){
    T2CONbits.TMR2ON=0;//Desactivar la cuenta
    /*1 = Timer2 is on
       0 = Timer2 is off 
     */
    
    T2CONbits.T2CKPS=0b11;//Asignar el valor del prescaler
    /*00 = Prescaler is 1
       01 = Prescaler is 4
       1x = Prescaler is 16
     */
    
    OPTION_REGbits.nRBPU=0;//Desactivar los pull-ups del puerto B 
    /*1 = PORTB pull-ups are disabled
      0 = PORTB pull-ups are enabled by individual PORT latch values
     */
    
    CCP1CONbits.CCP1M=0b1100;// Modo PWM
    /* 0000 = Capture/Compare/PWM off (resets ECCP module)
        0001 = Unused (reserved)
        0010 = Compare mode, toggle output on match (CCP1IF bit is set)
        0011 = Unused (reserved)
        0100 = Capture mode, every falling edge
        0101 = Capture mode, every rising edge
        0110 = Capture mode, every 4th rising edge
        0111 = Capture mode, every 16th rising edge
        1000 = Compare mode, set output on match (CCP1IF bit is set)
        1001 = Compare mode, clear output on match (CCP1IF bit is set)
        1010 = Compare mode, generate software interrupt on match (CCP1IF bit is set, CCP1 pin is unaffected)
        1011 = Compare mode, trigger special event (CCP1IF bit is set; CCP1 resets TMR1 or TMR2
        1100 = PWM mode; P1A, P1C active-high; P1B, P1D active-high
        1101 = PWM mode; P1A, P1C active-high; P1B, P1D active-low
        1110 = PWM mode; P1A, P1C active-low; P1B, P1D active-high
        1111 = PWM mode; P1A, P1C active-low; P1B, P1D active-low
     */    
}

/****************************************************************************
 * Nombre de la función: PWM_ON
 * retorna : nada
 * ingresa: El periodo de la onda PWM es de tipo "uint8"
 * Descripción : Activa la onda PWM con el periodo requerido 
 * Nota:  PWM Period = [(PR2) + 1] ? 4 ? TOSC ?(TMR2 Prescale Value)
 *****************************************************************************/
void PWM_ON (uint8_t PWM_periodo) {                                   
    PR2=PWM_periodo;
    TMR2=0;
    T2CONbits.TMR2ON=1;   
}                  

/****************************************************************************
 * Nombre de la función: Duty_Cycle
 * retorna : nada
 * ingresa: En duty Cycle es de tipo "uint16" 
 * Descripción : Crea el duty Cycle del PWM
 *****************************************************************************/
void Duty_Cycle (uint16_t duty){             
    CCP1CONbits.DC1B=duty&(0b11);
    CCPR1L=(duty>>2);
}       
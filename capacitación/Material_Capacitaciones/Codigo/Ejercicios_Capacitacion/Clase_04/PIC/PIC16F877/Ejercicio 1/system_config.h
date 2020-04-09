/* 
 * File:   system_config.h
 * Author: mtorres
 *
 * Created on February 24, 2020, 4:19 AM
 */

#ifndef SYSTEM_CONFIG_H
#define	SYSTEM_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif


// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
    
 #define _XTAL_FREQ  16000000UL   

    /* Constantes y macros */
#define     INPUT_TRIS(A, B)        B   |=    (1<<A)   
#define     OUTPUT_TRIS(A, B)     B &= ~(1<<A)
/*En la variable A, debes poner que pin quieres modificar
 * En la varible B, debes poner el puerto al que usaras
 * Ej: 
 *      - Quiero usar el pin 8 del puerto D para prender un Led
 *              OUTPUT_TRIS(7,TRISD)
 *      - Quiero usar el pin 4 del puerto B para leer un bit
 *              INTPUT_TRIS(3,TRISB)
 */
/*Varibles Globales*/
    
#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_CONFIG_H */


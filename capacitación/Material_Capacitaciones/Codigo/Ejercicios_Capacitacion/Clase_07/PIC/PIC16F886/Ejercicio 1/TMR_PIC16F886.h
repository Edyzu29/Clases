/* 
 * File:   TMR_PIC16F886.h
 * Author: maxed
 *
 * Created on 13 de mayo de 2020, 01:58 AM
 */

#include <stdint.h>

#ifndef TMR_PIC16F886_H
#define	TMR_PIC16F886_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*Constates*/

/*Funciones*/
void Config_TMR0(void);
void Config_TMR2_PWM (void);
void PWM_ON (uint8_t PWM_periodo);
void Duty_Cycle (uint16_t duty);

#ifdef	__cplusplus
}
#endif

#endif	/* TMR_PIC16F886_H */


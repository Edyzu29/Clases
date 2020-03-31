
#include "system_config.h"
#include <xc.h>
#include <stdint.h>


#define TRIS_LED    TRISC
#define PORT_LED    PORTC
#define LED         2

#define PERIODO      249

/* Declaración de funciones */
void Timer2_Inicializar(void);
void CCP1_PWM_Inicializar(void);


/* Función principal */
int main(void){

	uint8_t brillo = 0;

	Timer2_Inicializar();
    CCP1_PWM_Inicializar();

	while(1){
		CCPR1L = brillo;
		__delay_ms(20);
		brillo++;
        
        if(brillo > PERIODO){
            brillo = 0;
        }
        
	}


	return 0;
}

void Timer2_Inicializar(void){
    /* PASO 1: Desactivar el Timer2 */
    T2CONbits.TMR2ON = 0;
    /* PASO 2: Configurar el Pre-Scaler = 1 */
    T2CONbits.T2CKPS1 = 0;
    T2CONbits.T2CKPS0 = 0;
    /* PASO 3: Configurar el post-scaler */
    // Podemos obviar este paso al usar el módulo CCP en modo PWM
    /* PASO 4: Cargar el valor TOP a PR2 */
    PR2 = PERIODO;
    /* PASO 5: Activar el Timer2 */
    T2CONbits.TMR2ON = 1;
}   


void CCP1_PWM_Inicializar(void){
    /* PASO 1: Configurar como salida el pin de la señal PWM */
    TRIS_LED &= ~(1 << LED);
    /* PASO 2: COnfigurar el Timer2*/
    // Está hecho en la función TImer2_Init
    /* PASO 3: Cargar los 8 bits más significativos del tiempo en alto */
    CCPR1L = 0;
    /* PASO 4: Cargar los 2 bits menos significativos del tiempo en alto */
    CCP1CONbits.CCP1X = 0;
    CCP1CONbits.CCP1Y = 0;
    /* PASO 5: Configurar el módulo CCP1 para trabajar en modo PWM */
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
}


/* Inclusi�n de archivos */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>


/* Constantes y macros */
#define TRIS_PWM1		TRISC
#define LED1			2
#define TRIS_PWM2		TRISC
#define LED2			1

#define PERIODO         249

/* Variables globales */

/* Declaración de funciones */
void Timer2_Inicializar(void);
void CCP1_PWM_Inicializar(void);
void CCP2_PWM_Inicializar(void);

/* Función principal */
int main(void){

	int16_t ciclo_trabajo;

	/* Configurar el Timer1 en Modo PWM con periodo ajustable (Periodo 10us) */
	Timer2_Inicializar();
    CCP1_PWM_Inicializar();
    CCP2_PWM_Inicializar();

	while(1){
		for(ciclo_trabajo = 0; ciclo_trabajo < 250; ciclo_trabajo++){
			CCPR1L = ciclo_trabajo;
			CCPR2L = 249 - ciclo_trabajo;
			__delay_ms(16);
		}

		for(ciclo_trabajo = 249; ciclo_trabajo >= 0; ciclo_trabajo--){
			CCPR1L = ciclo_trabajo;
			CCPR2L = 79 - ciclo_trabajo;
			__delay_ms(16);
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
    // Podemos obviar este paso al usar el m�dulo CCP en modo PWM
    /* PASO 4: Cargar el valor TOP a PR2 */
    PR2 = PERIODO;
    /* PASO 5: Activar el Timer2 */
    T2CONbits.TMR2ON = 1;
}   


void CCP1_PWM_Inicializar(void){
    /* PASO 1: Configurar como salida el pin de la se�al PWM */
    TRIS_PWM1 &= ~(1 << LED1);
    /* PASO 2: COnfigurar el Timer2*/
    // Est� hecho en la funci�n TImer2_Init
    /* PASO 3: Cargar los 8 bits m�s significativos del tiempo en alto */
    CCPR1L = 0;
    /* PASO 4: Cargar los 2 bits menos significativos del tiempo en alto */
    CCP1CONbits.CCP1X = 0;
    CCP1CONbits.CCP1Y = 0;
    /* PASO 5: Configurar el m�dulo CCP1 para trabajar en modo PWM */
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
}


void CCP2_PWM_Inicializar(void){
    /* PASO 1: Configurar como salida el pin de la se�al PWM */
    TRIS_PWM2 &= ~(1 << LED2);
    /* PASO 2: COnfigurar el Timer2*/
    // Est� hecho en la funci�n TImer2_Init
    /* PASO 3: Cargar los 8 bits m�s significativos del tiempo en alto */
    CCPR2L = 0;
    /* PASO 4: Cargar los 2 bits menos significativos del tiempo en alto */
    CCP2CONbits.CCP2X = 0;
    CCP2CONbits.CCP2Y = 0;
    /* PASO 5: Configurar el m�dulo CCP1 para trabajar en modo PWM */
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;
}




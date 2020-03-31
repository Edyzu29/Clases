

/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "push_buttons.h"
#include "display_7_segmentos.h"

/* Constantes y macros */
#define TIMER0_OFFSET   131


/* Variables globales */
volatile uint8_t bandera_leer_botones = 0;

/* Declaraci贸n de funciones */
void Timer0_Inicializar(void);

/* Funci锟絥 principal */
int main(void){

	uint8_t numero = 0;

	/* Configurar el display */
	Display_Inicializar();
	/* Cargar el valor del n煤mero a los displays */
	Display_Decodificar_Entero_Decimal(numero);
	/* Configurar el Timer0 para generar una interrupci贸n
	 *  por comparaci贸n cada 2ms */
	Timer0_Inicializar();
    /* Habilitar las interrupciones */
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
	/* Repetir permanentemente ... */
	while(1){
		/* Leer los botones y llevar a cabo la acci贸n respectiva */
		if(bandera_leer_botones == 1){
			/* Si BOTON1 es pulsado, incrementar el numero */
			if(Boton_Leer(BOTON1)){
				numero++;
				Display_Decodificar_Entero_Decimal(numero);
			}
			/* Si BOTON2 es pulsado, decrementar el numero */
			if(Boton_Leer(BOTON2)){
				numero--;
				Display_Decodificar_Entero_Decimal(numero);
			}
			/* Limpiar la bandera para indicar que ya se leyeron los botones */
			bandera_leer_botones = 0;
		}
	}

	return 0;
}

/* Rutinas de servicio a interrupci贸n (ISR) */

__interrupt() void ISR(void){
    /* Limpiar la bandera de desbordamiento del Timer0 */
    INTCONbits.T0IF = 0;
    /* Cargar el valor de OFFSET al Timer 0*/
    TMR0 = TIMER0_OFFSET;
	/* Gestionar los habilitadores y escanear los botones */
	Display_Gestionar_Habilitadores();
	Botones_Escanear();
	/* Activar la bandera para indicar que se deben leer los botones */
	bandera_leer_botones = 1;
    
}


/* Definici贸n de funciones */

void Timer0_Inicializar(void){

    /* Timer0 en Modo Temporizador*/
    OPTION_REGbits.T0CS = 0;
    /* Incrementar con cada flanco de subida (solo para modo contador)*/
    OPTION_REGbits.T0SE = 0;
    /* Activar el Pre-Scaler para el Timer0 */
    OPTION_REGbits.PSA = 0;
    /* Definir el valor del Prescaler = 64 */
    OPTION_REGbits.PS = 0b101;
    /* Cargar el valor de OFFSET */
	TMR0 = TIMER0_OFFSET;
    /* Habilitar la interrupci髇 por desbordamiento del Timer0 */
    INTCONbits.T0IE = 1;
}

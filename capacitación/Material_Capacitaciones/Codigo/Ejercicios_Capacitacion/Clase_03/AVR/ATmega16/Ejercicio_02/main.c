

/* Inclusiones */
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "push_buttons.h"
#include "display_7_segmentos.h"

/* Variables globales */
volatile uint8_t bandera_leer_botones = 0;

/* Declaración de funciones */
void Timer0_Inicializar(void);

/* Funci�n principal */
int main(void){

	uint8_t numero = 0;

	/* Configurar el display */
	Display_Inicializar();
	/* Cargar el valor del número a los displays */
	Display_Decodificar_Entero_Decimal(numero);
	/* Configurar el Timer0 para generar una interrupción
	 *  por comparación cada 2ms */
	Timer0_Inicializar();
	/* Habilitar las interrupciones */
	sei();
	/* Repetir permanentemente ... */
	while(1){
		/* Leer los botones y llevar a cabo la acción respectiva */
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

/* Rutinas de servicio a interrupción (ISR) */

ISR(TIMER0_COMP_vect){
	/* Gestionar los habilitadores y escanear los botones */
	Display_Gestionar_Habilitadores();
	Botones_Escanear();
	/* Activar la bandera para indicar que se deben leer los botones */
	bandera_leer_botones = 1;
}


/* Definición de funciones */

void Timer0_Inicializar(void){

	Timer_8b_t timer0;

	/* Establecer los par�metros de configuraci�n del Timer0 */
	timer0.clock = Timer_Clk_PS64;
	timer0.mode = Timer_8b_CTC_Mode;
	timer0.OC = OC_Disabled;
	timer0.OCR = 249;
	/* Configurar el Timer0 */
	Timer0_Configurar(&timer0);
	/* Habilitar la interrupción por comparación (Match) con OCR */
	TIMSK |= (1 << OCIE0);
}

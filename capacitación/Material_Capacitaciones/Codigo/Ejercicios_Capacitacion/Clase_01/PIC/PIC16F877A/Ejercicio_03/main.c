
/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>

/* Constantes y macros */

// Definición de pines
#define	TRIS_LEDS			TRISD
#define PORT_LEDS			PORTD
#define LED1				5
#define LED2				6
#define LED3				7

#define PORT_BOTONES		PORTB
#define BOTON1				0
#define BOTON2				1

#define BOTON1_PRES			(PORT_BOTONES & (1 << BOTON1))
#define BOTON2_PRES			(PORT_BOTONES & (1 << BOTON2))


/* Declaración de funciones (prototipos) */


/* Función principal */
int main(void){

	/* Configurar los pines conectados a los LEDs como salida */
	TRIS_LEDS &= ~((1 << LED1) | (1 << LED2) | (1 << LED3));

	/* Repetir indefinidamente */
	while(1){

		/* Encender LED1 únicamente cuando BOTON1 esté presionado */
		if(BOTON1_PRES){
			PORT_LEDS |= (1 << LED1);
		}else{
			PORT_LEDS &=  ~(1 << LED1);
		}

		/* Encender LED2 cuando BOTON1 y BOTON2 estén presionados */
		if(BOTON1_PRES && BOTON2_PRES){
			PORT_LEDS |= (1 << LED2);
		}else{
			PORT_LEDS &=  ~(1 << LED2);
		}

		/* Encender LED3 cuando únicamente BOTON1 o BOTON2 esté presionado */
		if((BOTON1_PRES && !BOTON2_PRES) || (BOTON2_PRES && !BOTON1_PRES)){
			PORT_LEDS |= (1 << LED3);
		}else{
			PORT_LEDS &= ~(1 << LED3);
		}

	}

	return 0;
}


/* Definición de funciones */



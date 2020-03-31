
/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdio.h>
#include "push_buttons.h"

/* Constantes y macros */

// Definición de pines
#define	TRIS_LEDS			TRISD
#define PORT_LEDS			PORTD


/* Declaración de funciones (prototipos) */


/* Función principal */
int main(void){

	uint8_t contador = 0;

	/* Configurar los pines conectados a los LEDs como salida */
	TRIS_LEDS = 0x00;

	/* Repetir indefinidamente */
	while(1){

		/* Escanear todos los botones */
		Botones_Escanear();

		/* Si BOTON1 es pulsado ... */
		if(Boton_Leer(BOTON1)){
			/* Incrementar el contador */
			contador++;
		}

		/* Si BOTON2 es pulsado ... */
		if(Boton_Leer(BOTON2)){
			/* Decrementar el contador */
			contador--;
		}

		/* Mostrar la cuenta en los LEDs */
		PORT_LEDS = contador;

		/* Esperar 2ms antes de volver a escanear os botones */
		__delay_ms(2);

	}

	return 0;
}


/* Definición de funciones */



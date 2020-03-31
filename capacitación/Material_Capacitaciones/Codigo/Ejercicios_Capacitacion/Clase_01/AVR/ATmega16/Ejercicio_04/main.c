
/* Inclusiones */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "push_buttons.h"

/* Constantes y macros */

// Definición de pines
#define	DDR_LEDS			DDRD
#define PORT_LEDS			PORTD


/* Declaración de funciones (prototipos) */


/* Función principal */
int main(void){

	uint8_t contador = 0;

	/* Configurar los pines conectados a los LEDs como salida */
	DDR_LEDS = 0xFF;

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
		_delay_ms(2);

	}

	return 0;
}


/* Definición de funciones */



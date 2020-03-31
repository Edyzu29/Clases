
/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>

/* Constantes y macros */

// Definición de pines
#define	TRIS_LEDS	TRISD
#define PORT_LEDS	PORTD

/* Declaración de funciones (prototipos) */
void Secuencia_LEDs_01(void);
void Secuencia_LEDs_02(void);
void Secuencia_LEDs_03(void);

/* Función principal */
int main(void){

	/* Configurar todos los pines conectados a los LEDs como salida */
	TRIS_LEDS = 0x00;

	/* Repetir indefinidamente */
	while(1){
		Secuencia_LEDs_01();
		Secuencia_LEDs_01();
		Secuencia_LEDs_02();
		Secuencia_LEDs_03();
	}

	return 0;
}


/* Definición de funciones */

void Secuencia_LEDs_01(void){

	uint8_t posicion;

	for(posicion = 0; posicion <= 7; posicion++){
		PORT_LEDS = (1 << posicion);
		__delay_ms(500);
	}
}

void Secuencia_LEDs_02(void){

	int8_t posicion;

	/* Encendido de ida de los LEDs */
	for(posicion = 0; posicion <= 7; posicion++){
		PORT_LEDS = (1 << posicion);
		__delay_ms(500);
	}

	/* Encendido de regreso de los LEDs */
	for(posicion = 6; posicion >= 0; posicion--){
		PORT_LEDS = (1 << posicion);
		__delay_ms(500);
	}
}

void Secuencia_LEDs_03(void){

	int8_t posicion;

	/* Apaga todos los LEDs antes de iniciar la secuencia */
	PORT_LEDS = 0;

	/* Secuencia de encendido de los LEDs */
	for(posicion = 0; posicion <= 7; posicion++){
		PORT_LEDS |= (1 << posicion);
		__delay_ms(500);
	}

	/* Secuencia de apagado de los LEDs */
	for(posicion = 7; posicion >= 0; posicion--){
		PORT_LEDS &= ~(1 << posicion);
		__delay_ms(500);
	}
}

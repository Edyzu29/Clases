
/* Inclusiones */
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "push_buttons.h"
#include "lcd.h"

/* Constantes y macros */

// Definición de pines
#define TRIS_ALARM			TRISD
#define PORT_ALARM			PORTD
#define ALARM				7


/* Variables globales */
char buffer[20];

/* Declaración de funciones (prototipos) */


/* Función principal */
int main(void){

	uint8_t contador = 0;
	uint8_t actualizar_lcd = 0;

	/* Configurar el pin de la alarma (buzzer) como salida */
	TRIS_ALARM &= ~(1 << ALARM);
	/* Configurar la pantalla LCD 2x16 para trabajar con 4 líneas de datos */
	LCD_Init_4_Bits();
	/* Imprimir las "cabeceras" */
	LCD_Tx_Command(LCD_HOME);
	LCD_Put_String("Cuenta: 0");
	LCD_Goto_XY(1,2);
	LCD_Put_String("Alarma: OFF");


	/* Repetir indefinidamente */
	while(1){

		/* Escanear todos los botones */
		Botones_Escanear();

		/* Si BOTON1 es pulsado ... */
		if(Boton_Leer(BOTON1)){
			/* Incrementar la cuenta (máximo 15) */
			if(contador < 15){
				contador++;
				actualizar_lcd = 1;
			}
		}

		/* Si BOTON2 es pulsado ... */
		if(Boton_Leer(BOTON2)){
			/* Decrementar la cuenta (mínimo 0) */
			if(contador > 0){
				contador--;
				actualizar_lcd = 1;
			}
		}

		/* Si BOTON3 es pulsado ... */
		if(Boton_Leer(BOTON3)){
			/* Encender/Apagar la alarma */
			PORT_ALARM ^= (1 << ALARM);
			actualizar_lcd = 1;
		}

		/* Actualizar el display solamente cuando haya ocurrido un cambio */
		if(actualizar_lcd == 1){
			/* Borrar el valor anterior de la cuenta en la LCD */
			LCD_Goto_XY(9, 1);
			LCD_Put_String("  ");
			/* Mostrar el nuevo valor de la cuenta en la LCD */
			sprintf(buffer, "%d", contador);
			LCD_Goto_XY(9, 1);
			LCD_Put_String(buffer);
			/* Borrar el valor anterior del estado de la alarma */
			LCD_Goto_XY(9, 2);
			LCD_Put_String("   ");
			/* Mostrar el estado de la alarma */
			LCD_Goto_XY(9, 2);
			if(PORT_ALARM & (1 << ALARM)){
				LCD_Put_String("ON");
			}else{
				LCD_Put_String("OFF");
			}
			/* Limpiar la bandera de actualización */
			actualizar_lcd = 0;
		}


		/* Esperar 1ms antes de volver a escanear os botones */
		__delay_ms(1);

	}

	return 0;
}


/* Definición de funciones */



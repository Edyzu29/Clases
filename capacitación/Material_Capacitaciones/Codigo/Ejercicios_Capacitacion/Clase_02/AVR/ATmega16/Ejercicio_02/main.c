
/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "timer.h"
#include "lcd.h"
#include "matrix_keypad_4x4.h"

/* Constantes y macros */

#define DDR_LED			DDRC
#define PORT_LED		PORTC
#define LED1			0

#define N_CARACTERES	4

/* Variables globales */
char clave[N_CARACTERES] = {K_1, K_7, K_5, K_2};


/* Declaración de funciones */
void Timer0_Inicializar(void);

/* Función principal */
int main(void){

	uint8_t clave_correcta = 0;
	uint8_t aux = 0;
	uint8_t indice = 0;
	uint8_t tecla = 0;

	/* Configurar el pin del LED como salida y apagar el LED */
	DDR_LED |= (1 << LED1);
	PORT_LED &= ~(1 << LED1);
	/* Configurar la LCD */
	LCD_Init_4_Bits();
	/* Imprimir el encabezado */
	LCD_Tx_Command(LCD_HOME);
	LCD_Put_String("Ingresar clave:");
	/* Ubicar el cursor en la segunda línea */
	LCD_Goto_XY(1, 2);
	/* Configurar el Timer 0 para contar 10ms (aprox.)*/
	Timer0_Inicializar();
	/* Repetir indefinidamente ... */

	/* Repetir hasta que se haya ingresado la clave correcta */
	while(clave_correcta == 0){
		/* Si ha transcurrido 10 ms */
		if(TIFR & (1 << OCIE0)){
			/* Limpiar la bandera de desbordamiento */
			TIFR |= (1 << OCIE0);
			/* Leer si alguna tecla ha sido pulsada */
			tecla = Keypad_Leer_Tecla_Pulsada();
			/* Comparar la tecla con la secuencia establecida */
			if(tecla != 0){
				LCD_Put_Char('*');
				/* Detectar si la tecla corresponde a la clave */
				if(tecla != clave[indice]){
					aux = 0;
				}
				/* Pasar a ingresar el siguiente caracter */
				indice++;
				/* Si ya se ingresaron todos los caracteres ... */
				if (indice == N_CARACTERES){
					/* Limpiar la segunda línea */
					LCD_Goto_XY(1, 2);
					LCD_Put_String("                ");
					LCD_Goto_XY(1, 2);
					/* Indicar si la clave es correcta ... */
					if(aux == 1){
						clave_correcta = 1;
					}else{
						/* De lo contrario, mostrar un mensaje de error */
						LCD_Put_String("Clave Incorrecta");
						_delay_ms(1000);
						/* Limpiar la segunda línea */
						LCD_Goto_XY(1, 2);
						LCD_Put_String("                ");
						LCD_Goto_XY(1, 2);
					}
					/* Reiniciar el índice y la variable aux*/
					indice = 0;
					aux = 1;
				}
			}
		}
	}

	/* ¡Clave correcta! */
	LCD_Put_String("ACCESO CONCEDIDO");
	PORT_LED |= (1 << LED1);

	while(1){
		// Detener el programa aqui
	}


	return 0;
}

void Timer0_Inicializar(void){

	Timer_8b_t timer0;

	/* Establecer los par�metros de configuraci�n del Timer0 */
	timer0.clock = Timer_Clk_PS1024;
	timer0.mode = Timer_8b_CTC_Mode;
	timer0.OC = OC_Disabled;
	timer0.OCR = 77;
	/* Configurar el Timer0 */
	Timer0_Configurar(&timer0);
}





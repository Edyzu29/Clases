

/* Inclusiones */
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "display_7_segmentos.h"
#include "tsop1738.h"
#include "remote_controller_defines.h"

#define DDR_LED		DDRD
#define PORT_LED	PORTD
#define LED			0


/* Variables globales */
volatile uint8_t bandera_gestionar_habilitadores = 0;
volatile uint8_t bandera_comando_recibido = 0;

/* Declaración de funciones */
void Timer0_Inicializar(void);
void Timer1_Inicializar(void);
void Int_Externa_Inicializar(void);

/* Funci�n principal */
int main(void){
	DDR_LED |= (1 << LED);
	PORT_LED &= ~(1 << LED);

	/* Configurar el display */
	Display_Inicializar();
	/* Configurar el Timer0 para generar una interrupción
	 *  por comparación cada 2ms */
	Timer0_Inicializar();
	/* Configurar el Timer1 */
	Timer1_Inicializar();
	/* Configurar las interrupciones externas */
	Int_Externa_Inicializar();
	/* Habilitar las interrupciones */
	sei();
	/* Visualizar un valor inicial en los displays */
	Display_Decodificar_Entero_Decimal(8888);
	/* Repetir indefinidamente ... */
	while(1){
		/* Gestionar los habilitadores cuando sea requerido */
		if(bandera_gestionar_habilitadores == 1){
			Display_Gestionar_Habilitadores();
			bandera_gestionar_habilitadores = 0;
		}
		/* Leer el comando recibido cuando sea requerido */
		if(bandera_comando_recibido == 1){
			Display_Decodificar_Entero_Decimal(ir_command);
			bandera_comando_recibido = 0;
		}
	}

	return 0;
}

/* Rutinas de servicio a interrupción (ISR) */

ISR(TIMER0_COMP_vect){
	/* Activar la bandera para indicar que se deben gestionar los habilitadores */
	bandera_gestionar_habilitadores = 1;
}


ISR(TIMER1_COMPA_vect){

	PORT_LED ^= (1 << LED);
	/* Configurar el TImer0 para generar una interrupción cada 1700 us */
	OCR1A = 1679;
	/* Si se han terminado de leer los 13 bits ... (se ignora el primer bit START) */
	if(read_ir_state == IR_READING_COMPLETE){
		/* Desactivar la interrupción del Timer1 y activar la interrupción externa */
		TIMSK &= ~(1 << OCIE1A);
		GIFR |= (1 << INTF0);
		GICR |= (1 << INT0);
		/* Indicar que se ha terminado de leer un comando del control infra-rojo */
		bandera_comando_recibido = 1;
	/* Si aún no se ha terminado de leer los 13 bits ... */
	}else{
		/* Leer el siguiente bit y continuar ensamblando el comando */
		IR_Control_Rx();
	}

}

ISR(INT0_vect){
	PORT_LED &= ~(1 << LED);
	/* Desactivar la interrupción externa */
	GICR &= ~(1 << INT0);
	/* Configurar el Timer0 para generar una interrupción luego de 3825 us */
	OCR1A = 3779;
	TCNT1 = 0;
	TIFR |= (1 << OCF1A);
	TIMSK |= (1 << OCIE1A);
	/* Indicar que se iniciará la lectura del control infra-rojo */
	read_ir_state = 0;

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


void Timer1_Inicializar(void){

	Timer_16b_t timer1;

	/* Establecer los par�metros de configuraci�n del Timer0 */
	timer1.clock = Timer_Clk_PS8;
	timer1.mode = Timer_16b_CTC_OCRA_Mode;
	timer1.OCA = OC_Disabled;
	timer1.OCB = OC_Disabled;
	timer1.ic_edge_selector = Timer_IC_Falling_Edge;
	timer1.ic_noise_canceler = Timer_ICNC_Disabled;

	/* Configurar el Timer1 */
	Timer1_Configurar(&timer1);
}

void Int_Externa_Inicializar(void){
	MCUCR &= ~((1 << ISC01) | (1 << ISC00));
	GICR |= (1 << INT0);
}






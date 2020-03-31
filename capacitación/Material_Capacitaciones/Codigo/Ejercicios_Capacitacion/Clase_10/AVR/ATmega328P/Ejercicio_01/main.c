
/* Inclusion de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "eplimin_servo_module_avr_defines.h"
#include "timer.h"


/* Constantes y macros */

/* Variables globales */
uint8_t pos_servo_2 = 0;
uint16_t tabla_angulos[] = {
								124, //0°
								152, //10°
								179, //20°
								207, //30°
								235, //40°
								263, //50°
								291, //60°
								318, //70°
								346, //80°
								374, //90°
								402, //100°
								429, //110°
								457, //120°
								485, //130°
								513, //140°
								541, //150°
								568, //160°
								596, //170°
								624  //180°
};

/* Declaración de funciones */
void Timer0_Inicializar(void);
void Timer1_Inicializar(void);
uint8_t Detecta_Evento_S3_Pulsado(void);
uint8_t Detecta_Evento_S4_Pulsado(void);


/* Función principal */
int main(void){

	/* Configurar el Timer0 para interrumpir cada 1 ms*/
	Timer0_Inicializar();
	/* Configurar el Timer1 para generar una PWM de periodo 20ms */
	Timer1_Inicializar();
	/* Habilitar las interrupciones */
	sei();

	/* Repetir indefinidamente */
	while(1){

	}
	return 0;
}


/* Rutinas de servicio a interrupción */
ISR(TIMER0_COMPA_vect){

	if(Detecta_Evento_S3_Pulsado()){
		if(pos_servo_2 < 18){
			pos_servo_2++;
			OCR1A = tabla_angulos[pos_servo_2];
		}
	}

	if(Detecta_Evento_S4_Pulsado()){
		if(pos_servo_2 > 0){
			pos_servo_2--;
			OCR1A = tabla_angulos[pos_servo_2];
		}
	}

}


/* Definición de funciones */

void Timer0_Inicializar(void){

	Timer_8b_t timer0;

	/* Establecer los par�metros de configuraci�n del Timer1 */
	timer0.clock = Timer_Clk_PS64;
	timer0.mode = Timer_8b_CTC_Mode;
	timer0.OCA = OC_Disabled;
	timer0.OCB = OC_Disabled;
	timer0.interrupt_mask = Timer_CompA_IE;
	timer0.OCRA = 249;

	/* Configurar el Timer1 */
	Timer0_Configurar(&timer0);

}


void Timer1_Inicializar(void){

	Timer_16b_t timer1;

	DDR_SERVO_2 |= (1 << SERVO_2);

	/* Establecer los par�metros de configuraci�n del Timer1 */
	timer1.clock = Timer_Clk_PS64;
	timer1.mode = Timer_16b_Fast_PWM_ICR_Mode;
	timer1.OCA = OC_Clear_Non_Inverting_Mode;
	timer1.OCB = OC_Disabled;
	timer1.ic_edge_selector = Timer_IC_Falling_Edge;
	timer1.ic_noise_canceler = Timer_ICNC_Disabled;
	timer1.interrupt_mask = Timer_Interrupts_Disabled;
	timer1.OCRA = 124;
	timer1.ICR = 4999;

	/* Configurar el Timer1 */
	Timer1_Configurar(&timer1);

}

uint8_t Detecta_Evento_S3_Pulsado(void){

	static uint8_t estado_anterior = 0;
	uint8_t estado_actual;
	uint8_t flanco_subida;

	estado_actual = (PIN_S3 & (1 << S3));
	flanco_subida = estado_actual && !estado_anterior;
	estado_anterior = estado_actual;

	return flanco_subida;
}

uint8_t Detecta_Evento_S4_Pulsado(void){

	static uint8_t estado_anterior = 0;
	uint8_t estado_actual;
	uint8_t flanco_subida;

	estado_actual = (PIN_S4 & (1 << S4));
	flanco_subida = estado_actual && !estado_anterior;
	estado_anterior = estado_actual;

	return flanco_subida;
}



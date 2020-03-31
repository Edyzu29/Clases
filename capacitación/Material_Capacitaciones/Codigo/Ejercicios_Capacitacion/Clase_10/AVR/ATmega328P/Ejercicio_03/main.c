
/* Inclusion de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "eplimin_servo_module_avr_defines.h"
#include "timer.h"


/* Constantes y macros */

/* Variables globales */

uint8_t pos_servo_1 = 0;
uint8_t pos_servo_2 = 0;
uint8_t pos_servo_3 = 0;

volatile uint8_t leer_botones = 0;

volatile uint16_t dc_servo_1 = 45;
volatile uint16_t dc_servo_2 = 45;
volatile uint16_t dc_servo_3 = 45;

uint16_t tabla_angulos[] = {
								45, //0°
								55, //10°
								65, //20°
								75, //30°
								85, //40°
								95, //50°
								105, //60°
								115, //70°
								125, //80°
								135, //90°
								145, //100°
								155, //110°
								165, //120°
								175, //130°
								185, //140°
								195, //150°
								205, //160°
								215, //170°
								225  //180°
};

/* Declaración de funciones */
void Timer0_Inicializar(void);
uint8_t Detecta_Evento_S1_Pulsado(void);
uint8_t Detecta_Evento_S2_Pulsado(void);
uint8_t Detecta_Evento_S3_Pulsado(void);
uint8_t Detecta_Evento_S4_Pulsado(void);
uint8_t Detecta_Evento_S5_Pulsado(void);
uint8_t Detecta_Evento_S6_Pulsado(void);

/* Función principal */
int main(void){

	DDR_SERVO_1 |= (1 << SERVO_1);
	DDR_SERVO_2 |= (1 << SERVO_2);
	DDR_SERVO_3 |= (1 << SERVO_3);

	/* Configurar el Timer0 para interrumpir cada 11.11 us (aproximadamente)*/
	Timer0_Inicializar();
	/* Habilitar las interrupciones */
	sei();

	/* Repetir indefinidamente */
	while(1){
		if(leer_botones == 1){

			if(Detecta_Evento_S1_Pulsado()){
				if(pos_servo_1 < 18){
					pos_servo_1++;
					dc_servo_1 = tabla_angulos[pos_servo_1];
				}
			}

			if(Detecta_Evento_S2_Pulsado()){
				if(pos_servo_1 > 0){
					pos_servo_1--;
					dc_servo_1 = tabla_angulos[pos_servo_1];
				}
			}

			if(Detecta_Evento_S3_Pulsado()){
				if(pos_servo_2 < 18){
					pos_servo_2++;
					dc_servo_2 = tabla_angulos[pos_servo_2];
				}
			}

			if(Detecta_Evento_S4_Pulsado()){
				if(pos_servo_2 > 0){
					pos_servo_2--;
					dc_servo_2 = tabla_angulos[pos_servo_2];
				}
			}


			if(Detecta_Evento_S5_Pulsado()){
				if(pos_servo_3 < 18){
					pos_servo_3++;
					dc_servo_3 = tabla_angulos[pos_servo_3];
				}
			}

			if(Detecta_Evento_S6_Pulsado()){
				if(pos_servo_3 > 0){
					pos_servo_3--;
					dc_servo_3 = tabla_angulos[pos_servo_3];
				}
			}

			leer_botones = 0;
		}
	}
	return 0;
}


/* Rutinas de servicio a interrupción */
ISR(TIMER0_COMPA_vect){

	static uint16_t conta_servos = 0;

	conta_servos++;

	if(conta_servos >= 1806){
		conta_servos = 0;
		leer_botones = 1;
	}

	if(conta_servos < dc_servo_1){
		PORT_SERVO_1 |= (1 << SERVO_1);
	}else{
		PORT_SERVO_1 &= ~(1 << SERVO_1);
	}

	if(conta_servos < dc_servo_2){
		PORT_SERVO_2 |= (1 << SERVO_2);
	}else{
		PORT_SERVO_2 &= ~(1 << SERVO_2);
	}


	if(conta_servos < dc_servo_3){
		PORT_SERVO_3 |= (1 << SERVO_3);
	}else{
		PORT_SERVO_3 &= ~(1 << SERVO_3);
	}
}


/* Definición de funciones */

void Timer0_Inicializar(void){

	Timer_8b_t timer0;

	/* Establecer los par�metros de configuraci�n del Timer1 */
	timer0.clock = Timer_Clk_PS1;
	timer0.mode = Timer_8b_CTC_Mode;
	timer0.OCA = OC_Disabled;
	timer0.OCB = OC_Disabled;
	timer0.interrupt_mask = Timer_CompA_IE;
	timer0.OCRA = 176;

	/* Configurar el Timer1 */
	Timer0_Configurar(&timer0);

}



uint8_t Detecta_Evento_S1_Pulsado(void){

	static uint8_t estado_anterior = 0;
	uint8_t estado_actual;
	uint8_t flanco_subida;

	estado_actual = (PIN_S1 & (1 << S1));
	flanco_subida = estado_actual && !estado_anterior;
	estado_anterior = estado_actual;

	return flanco_subida;
}

uint8_t Detecta_Evento_S2_Pulsado(void){

	static uint8_t estado_anterior = 0;
	uint8_t estado_actual;
	uint8_t flanco_subida;

	estado_actual = (PIN_S2 & (1 << S2));
	flanco_subida = estado_actual && !estado_anterior;
	estado_anterior = estado_actual;

	return flanco_subida;
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

uint8_t Detecta_Evento_S5_Pulsado(void){

	static uint8_t estado_anterior = 0;
	uint8_t estado_actual;
	uint8_t flanco_subida;

	estado_actual = (PIN_S5 & (1 << S5));
	flanco_subida = estado_actual && !estado_anterior;
	estado_anterior = estado_actual;

	return flanco_subida;
}

uint8_t Detecta_Evento_S6_Pulsado(void){

	static uint8_t estado_anterior = 0;
	uint8_t estado_actual;
	uint8_t flanco_subida;

	estado_actual = (PIN_S6 & (1 << S6));
	flanco_subida = estado_actual && !estado_anterior;
	estado_anterior = estado_actual;

	return flanco_subida;
}


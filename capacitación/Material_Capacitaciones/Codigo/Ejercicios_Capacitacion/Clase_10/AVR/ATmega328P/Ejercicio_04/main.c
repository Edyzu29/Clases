
/* Inclusion de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "eplimin_servo_module_avr_defines.h"
#include "timer.h"
#include "adc.h"


/* Constantes y macros */

/* Variables globales */

volatile uint16_t valor_adc[3];

volatile uint8_t conversion_completa = 0;

volatile uint8_t indice_canal = 0;

volatile uint16_t dc_servo_1 = 45;
volatile uint16_t dc_servo_2 = 45;
volatile uint16_t dc_servo_3 = 45;

//uint16_t tabla_angulos[] = {
//								45, //0°
//								55, //10°
//								65, //20°
//								75, //30°
//								85, //40°
//								95, //50°
//								105, //60°
//								115, //70°
//								125, //80°
//								135, //90°
//								145, //100°
//								155, //110°
//								165, //120°
//								175, //130°
//								185, //140°
//								195, //150°
//								205, //160°
//								215, //170°
//								225  //180°
//};

/* Declaración de funciones */
void Timer0_Inicializar(void);
void Timer0_Inicializar(void);

/* Función principal */
int main(void){

	DDR_SERVO_1 |= (1 << SERVO_1);
	DDR_SERVO_2 |= (1 << SERVO_2);
	DDR_SERVO_3 |= (1 << SERVO_3);

	/* Configurar el ADC */
	ADC_Inicializar();
	/* Configurar el Timer0 para interrumpir cada 11.11 us*/
	Timer0_Inicializar();
	/* Habilitar las interrupciones */
	sei();

	/* Repetir indefinidamente */
	while(1){
		if(conversion_completa){

			conversion_completa = 0;

			indice_canal++;
			if(indice_canal >= 3){

				indice_canal = 0;

				dc_servo_1 = 225 -(((uint32_t)valor_adc[0] * 180)/ 1023);
				dc_servo_2 = 225 -(((uint32_t)valor_adc[1] * 180)/ 1023);
				dc_servo_3 = 225 -(((uint32_t)valor_adc[2] * 180)/ 1023);

			}

			if(indice_canal == 0){
				ADC_Seleccionar_Canal(ADC_CANAL_POT1);
			}else if(indice_canal == 1){
				ADC_Seleccionar_Canal(ADC_CANAL_POT2);
			}else{
				ADC_Seleccionar_Canal(ADC_CANAL_POT3);
			}

		}

	}
	return 0;
}


/* Rutinas de servicio a interrupción */
ISR(TIMER0_COMPA_vect){

	static uint16_t conta_servos = 0;

	conta_servos++;

	/* Al completar un periodo de la señal PWM (20 ms) */
	if(conta_servos >= 1806){
		/* Resetear el contador de pasos */
		conta_servos = 0;
		/* Iniciar una conversión */
		ADCSRA |= (1 << ADSC);
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


ISR(ADC_vect){
	valor_adc[indice_canal] = ADC;
	conversion_completa = 1;
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

	/* Configurar el Timer0 */
	Timer0_Configurar(&timer0);

}

void ADC_Inicializar(void){
	ADC_t my_adc;

	my_adc.prescaler = ADC_Prescaler_32;
	my_adc.vref = AVCC;
	my_adc.channel = ADC_Channel_1;
	my_adc.auto_trigger = ADC_Autotrigger_Disabled;
	my_adc.trigger_source = ADC_Free_Running_Mode;
	my_adc.interrupt_mask = ADC_Interrupt_Enabled;

	ADC_Configurar(&my_adc);
}



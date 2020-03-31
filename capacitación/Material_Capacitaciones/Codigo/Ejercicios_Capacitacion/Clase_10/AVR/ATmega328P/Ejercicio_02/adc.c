/*
 * adc.c
 *
 *  Created on: 19 mar. 2018
 *      Author: MiguelAngel
 */

/* Inclusiones */
#include <stdint.h>
#include <avr/io.h>
#include "adc.h"


/****************************************************************************
* Nombre de la funci�n: ADC_Configurar
* retorna : Nada
* arg : adc_param
* Descripci�n : Recibe un puntero hacia una estructura de tipo ADC_t (la
* 				cual contiene los par�metros de configuraci�n) y configura
* 				el m�dulo ADC.
* Notas : Esta funci�n no est� dise�ada para configurar canales diferenciales
* 		  ni ganancias. Tampoco permite deshabilitar las entradas digitales
* 		  que no se van a utilizar.
*****************************************************************************/
void ADC_Configurar(ADC_t *adc_param){

	/* Inicializar los registros de configuraci�n */
	ADMUX = 0;
	ADCSRA = 0;
	ADCSRB = 0;


	/* Habilitar el ADC */
	ADCSRA |= (1 << ADEN);
	/* Configurar el Pre-Scaler del ADC */
	ADCSRA |= (adc_param->prescaler) << ADPS0;
	/* Configurar el voltaje de referencia */
	ADMUX |= (adc_param->vref & 0x03) << REFS0;
	/* Cargar los nuevos valores de los bits selectores de canal */
	ADMUX |= (adc_param->channel & 0x07) << MUX0;
	/* Configurar el trigger autom�tico */
	ADCSRA |= (adc_param->auto_trigger) << ADATE;
	ADCSRB |= (adc_param->trigger_source) << ADTS0;
	/* Configurar el habilitador de interrupci�n */
	ADCSRA |= adc_param->interrupt_mask;
	/* Llevar a cabo una primera conversi�n */
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1 << ADIF))){
		/* No hacer nada hasta que la conversi�n finalice */
	}

	ADCSRA |= (1 << ADIF);
}

/****************************************************************************
* Nombre de la funci�n: ADC_Canal_Seleccionar
* retorna : Nada
* arg : canal
* Descripci�n : Recibe como par�metro el n�mero de canal anal�gico que
* 				se desea muestrear (de 0 a 15) y configura el m�dulo
* 				ADC para leer el canal seleccionado.
* Notas : No llamar a esta funci�n mientras una conversi�n se est� llevando
* 		  a cabo (esperar a que la conversi�n termine). Esta funci�n solo
* 		  permite seleccionar los canales simples de 0 a 15 (no permite
* 		  seleccionar canales diferenciales con ganancia programable). La
* 		  funci�n cuenta con un "filtro" interno en caso que el par�metro
* 		  ingresado se encuentre fuera del rango de 0 a 15.
*****************************************************************************/
void ADC_Seleccionar_Canal(uint8_t canal){

	ADC_Channel_t selector;

	/* Tabla para la configuraci�n de los canales */
	ADC_Channel_t tabla_canales[] ={
										ADC_Channel_0,
										ADC_Channel_1,
										ADC_Channel_2,
										ADC_Channel_3,
										ADC_Channel_4,
										ADC_Channel_5,
										ADC_Channel_6,
										ADC_Channel_7
									};

	/* Filtrar el par�metro canal para que no apunte fuera de la tabla */
	canal &= 0x0F;
	/* Cargar el valor correspondiente al selector */
	selector = tabla_canales[canal];
	/* Limpiar todos los bits selectores de canal del ADC */
	ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
	/* Cargar los nuevos valores de los bits selectores */
	ADMUX |= (selector & 0x1F) << MUX0;

}

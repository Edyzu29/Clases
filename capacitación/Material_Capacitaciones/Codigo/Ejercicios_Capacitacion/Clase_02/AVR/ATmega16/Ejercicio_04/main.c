
/* Inclusión de archivos */
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include "timer.h"
#include "push_buttons.h"
#include "lcd.h"

/* Constantes y macros */
#define DDR_LED			DDRD
#define PORT_LED		PORTD
#define LED1			0

#define DDR_MOTOR		DDRD
#define PORT_MOTOR		PORTD
#define MOTOR_IN1		6
#define MOTOR_IN2		7

#define DDR_PWM			DDRB
#define MOTOR_ENA		3

#define MOTOR_DETENIDO1		0
#define GIRO_HORARIO		1
#define MOTOR_DETENIDO2		2
#define GIRO_ANTIHORARIO	3

/* Variables globales */
uint16_t dc_bin = 0, dc_porc = 0;
uint8_t estado_motor = 0;
char buffer[17];

/* Declaración de funciones */
void Timer0_Inicializar(void);
void Motor_Speed_Inc(void);
void Motor_Speed_Dec(void);
void Motor_State_Toggle(void);


/* Función principal */
int main(void){

	/* Configurar como salida el pin del LED e inicializarlo apagado */
	DDR_LED |= (1 << LED1);
	PORT_LED &= ~(1 << LED1);
	/* Configurar como salida los pines para el control del motor e inicializar
	 * el motor detenido */
	DDR_MOTOR |= (1 << MOTOR_IN1) | (1 << MOTOR_IN2);
	PORT_MOTOR &= ~((1 << MOTOR_IN1) | (1 << MOTOR_IN2));
	/* Configurar la LCD */
	LCD_Init_4_Bits();
	/* Imprimir las cabeceras */
	LCD_Goto_XY(1,1);
	LCD_Put_String("duty cycle: 0%");
	LCD_Goto_XY(1,2);
	LCD_Put_String("estado: Detenido");
	/* Configurar el Timer0 en Modo Fast PWM */
	Timer0_Inicializar();
	/* Repetir indefinidamente */
	while(1){
		/* Escanear todos los botones */
		Botones_Escanear();
		/* Si se pulsa BOTON1, incrementar el ciclo de trabajo */
		if(Boton_Leer(BOTON1)){
			Motor_Speed_Inc();
		}
		/* Si se pulsa BOTON2, decrementar el ciclo de trabajo */
		if(Boton_Leer(BOTON2)){
			Motor_Speed_Dec();
		}
		/* Si se pulsa BOTON3, cambiar el estado del motor */
		if(Boton_Leer(BOTON3)){
			Motor_State_Toggle();
		}
		_delay_ms(10);
	}


	return 0;
}

void Timer0_Inicializar(void){

	Timer_8b_t timer0;

	/* Configurar como salida el pin OC0 (salida PWM) */
	DDR_PWM |= (1 << MOTOR_ENA);

	/* Establecer los par�metros de configuraci�n del Timer0 */
	timer0.clock = Timer_Clk_PS8;
	timer0.mode = Timer_8b_Fast_PWM_FixPeriod_Mode;
	timer0.OC = OC_Clear_Non_Inverting_Mode;
	timer0.OCR = 255;
	/* Configurar el Timer0 */
	Timer0_Configurar(&timer0);
}

void Motor_Speed_Inc(void){

	if(dc_porc < 100){
		dc_porc++;
	}

	/* Traducir de porcentaje a valor binario */
	dc_bin = (dc_porc * 256) / 100;
	if(dc_bin > 0){
		dc_bin--;
	}
	/* Actualizar el ciclo de trabajo */
	OCR0 = dc_bin;

	/* Actualzar los datos en la LCD */
	LCD_Goto_XY(1,1);
	LCD_Put_String("                ");
	sprintf(buffer, "duty cycle: %d", dc_porc);
	LCD_Goto_XY(1,1);
	LCD_Put_String(buffer);
	LCD_Put_Char('%');
}

void Motor_Speed_Dec(void){

	if(dc_porc > 0){
		dc_porc--;
	}

	/* Traducir de porcentaje a valor binario */
	dc_bin = (dc_porc * 256) / 100;
	if(dc_bin > 0){
		dc_bin--;
	}
	/* Actualizar el ciclo de trabajo */
	OCR0 = dc_bin;

	/* Actualzar los datos en la LCD */
	LCD_Goto_XY(1,1);
	LCD_Put_String("                ");
	sprintf(buffer, "duty cycle: %d", dc_porc);
	LCD_Goto_XY(1,1);
	LCD_Put_String(buffer);
	LCD_Put_Char('%');
}

void Motor_State_Toggle(void){

	estado_motor++;
	if(estado_motor > 3){
		estado_motor = 0;
	}

	switch(estado_motor){
		case MOTOR_DETENIDO1:
			PORT_MOTOR &= ~((1 << MOTOR_IN1) | (1 << MOTOR_IN2));
			LCD_Goto_XY(1,2);
			LCD_Put_String("estado: Detenido");
			break;
		case GIRO_HORARIO:
			PORT_MOTOR &= ~(1 << MOTOR_IN2);
			PORT_MOTOR |=  (1 << MOTOR_IN1);
			LCD_Goto_XY(1,2);
			LCD_Put_String("estado: Horario ");
			break;
		case MOTOR_DETENIDO2:
			PORT_MOTOR &= ~((1 << MOTOR_IN1) | (1 << MOTOR_IN2));
			LCD_Goto_XY(1,2);
			LCD_Put_String("estado: Detenido");
			break;
		case GIRO_ANTIHORARIO:
			PORT_MOTOR &= ~(1 << MOTOR_IN1);
			PORT_MOTOR |=  (1 << MOTOR_IN2);
			LCD_Goto_XY(1,2);
			LCD_Put_String("estado: AntiHor ");
			break;
		default:
			break;
	}


}




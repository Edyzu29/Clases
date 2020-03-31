
/* Inclusi√≥n de archivos */
#include "system_config.h"
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "push_buttons.h"
#include "lcd.h"

/* Constantes y macros */
#define TRIS_LED		TRISD
#define PORT_LED		PORTD
#define LED1			0

#define TRIS_MOTOR		TRISD
#define PORT_MOTOR		PORTD
#define MOTOR_IN1		6
#define MOTOR_IN2		7

#define TRIS_PWM		TRISC
#define MOTOR_ENA		2

#define MOTOR_DETENIDO1		0
#define GIRO_HORARIO		1
#define MOTOR_DETENIDO2		2
#define GIRO_ANTIHORARIO	3

#define PERIODO             99

/* Variables globales */
uint16_t dc_bin = 0, dc_porc = 0;
uint8_t estado_motor = 0;
char buffer[17];

/* Declaraci√≥n de funciones */
void Timer2_Inicializar(void);
void CCP1_PWM_Inicializar(void);
void Motor_Speed_Inc(void);
void Motor_Speed_Dec(void);
void Motor_State_Toggle(void);


/* FunciÛn principal */
int main(void){

	/* Configurar como salida el pin del LED e inicializarlo apagado */
	TRIS_LED &= ~(1 << LED1);
	PORT_LED &= ~(1 << LED1);
	/* Configurar como salida los pines para el control del motor e inicializar
	 * el motor detenido */
	TRIS_MOTOR &= ~((1 << MOTOR_IN1) | (1 << MOTOR_IN2));
	PORT_MOTOR &= ~((1 << MOTOR_IN1) | (1 << MOTOR_IN2));
	/* Configurar la LCD */
	LCD_Init_4_Bits();
	/* Imprimir las cabeceras */
	LCD_Goto_XY(1,1);
	LCD_Put_String("duty cycle: 0%");
	LCD_Goto_XY(1,2);
	LCD_Put_String("estado: Detenido");
	/* Configurar el Timer0 en Modo Fast PWM */
	Timer2_Inicializar();
    CCP1_PWM_Inicializar();
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
		__delay_ms(10);
	}


	return 0;
}


void Timer2_Inicializar(void){
    /* PASO 1: Desactivar el Timer2 */
    T2CONbits.TMR2ON = 0;
    /* PASO 2: Configurar el Pre-Scaler = 16 */
    T2CONbits.T2CKPS1 = 1;
    T2CONbits.T2CKPS0 = 0;
    /* PASO 3: Configurar el post-scaler */
    // Podemos obviar este paso al usar el mÛdulo CCP en modo PWM
    /* PASO 4: Cargar el valor TOP a PR2 */
    PR2 = PERIODO;
    /* PASO 5: Activar el Timer2 */
    T2CONbits.TMR2ON = 1;
}   


void CCP1_PWM_Inicializar(void){
    /* PASO 1: Configurar como salida el pin de la seÒal PWM */
    TRIS_PWM &= ~(1 << MOTOR_ENA);
    /* PASO 2: COnfigurar el Timer2*/
    // Est· hecho en la funciÛn TImer2_Init
    /* PASO 3: Cargar los 8 bits m·s significativos del tiempo en alto */
    CCPR1L = 0;
    /* PASO 4: Cargar los 2 bits menos significativos del tiempo en alto */
    CCP1CONbits.CCP1X = 0;
    CCP1CONbits.CCP1Y = 0;
    /* PASO 5: Configurar el mÛdulo CCP1 para trabajar en modo PWM */
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
}



void Motor_Speed_Inc(void){

	if(dc_porc < 100){
		dc_porc++;
	}

	/* Traducir de porcentaje a valor binario */
	dc_bin = (dc_porc * PERIODO) / 100;
	if(dc_bin > 0){
		dc_bin--;
	}
	/* Actualizar el ciclo de trabajo */
	CCPR1L = dc_bin;

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
	dc_bin = (dc_porc * PERIODO) / 100;
	if(dc_bin > 0){
		dc_bin--;
	}
	/* Actualizar el ciclo de trabajo */
	CCPR1L = dc_bin;

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




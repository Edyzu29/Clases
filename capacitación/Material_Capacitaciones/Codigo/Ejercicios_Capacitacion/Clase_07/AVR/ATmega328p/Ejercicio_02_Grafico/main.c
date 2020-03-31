
/* Inclusion de archivos */
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "eplimin_temp_control_module_avr_defines.h"
#include "uart.h"
#include "spi_master.h"
#include "lcd.h"
#include "max6675.h"
#include "pid_basic.h"


/* Constantes y macros */
#define N_MUESTRAS		32
#define TEMP_SETPOINT	40
#define ADC_SETPOINT	(TEMP_SETPOINT * 4.0)
#define N_PAQUETES		60

/* Variables globales */
volatile uint8_t paquetes_aceptados = 1;
volatile uint8_t medir_y_controlar = 0;
volatile uint16_t valor_adc = 0;
uint16_t valor_adc_anterior = 0;
uint16_t temperatura = 0;
char buffer[17];

/* Declaración de funciones */
void UART0_Inicializar(void);
void SPI_Master_Inicializar(void);
void Int_Externa_Inicializar(void);

/* Función principal */
int main(void){
	/* Configurar como salida el pin de activación del TRIAC */
	DDR_TRIAC_OUT |= (1 << TRIAC_OUT);
	PORT_TRIAC_OUT &= ~(1 << TRIAC_OUT);
	/* Configurar como salida el pin del CS para el MAX6675 */
	DDR_CS_MAX6675 |= (1 << CS_MAX6675);
	PORT_CS_MAX6675 |= (1 << CS_MAX6675);
	/* Forzar a 0 el pin RW de la LCD */
	DDR_LCD_RW |= (1 << LCD_RW);
	PORT_LCD_RW &= ~(1 << LCD_RW);
	/* Configurar la pantalla LCD */
	LCD_Init_4_Bits();
	/* Impresión del mensaje de bienvenida */
	LCD_Goto_XY(1,1);
	LCD_Put_String(" Modulo Control ");
	LCD_Goto_XY(1,2);
	LCD_Put_String(" de Temperatura ");

	_delay_ms(2000);

	/* Impresión de las cabeceras */
	LCD_Tx_Command(LCD_CLEAR);
	LCD_Goto_XY(1,1);
	LCD_Put_String("Setpoint:      C");
	LCD_Goto_XY(15,1);
	LCD_Put_Char(0xDF);
	LCD_Goto_XY(1,2);
	LCD_Put_String("Medicion:      C");
	LCD_Goto_XY(15,2);
	LCD_Put_Char(0xDF);
	sprintf(buffer, "%d", TEMP_SETPOINT);
	LCD_Goto_XY(11,1);
	LCD_Put_String(buffer);
	/* Configurar el módulo SPI */
	SPI_Master_Inicializar();
	/* Configurar el UART */
	UART0_Inicializar();
	/* Establecer el setpoint del PID */
	pid_setpoint = ADC_SETPOINT;
	/* Configurar la interrupción externa */
	Int_Externa_Inicializar();
	/* Habilitar las interrupciones */
	sei();

	/* Repetir indefinidamente */
	while(1){

		if(medir_y_controlar == 1){
			/* Limpiar la bandera */
			medir_y_controlar = 0;
			/* Leer el valor de la termocupla */
			PORT_CS_MAX6675 &= ~(1 << CS_MAX6675);
			valor_adc = MAX6675_Get_Bin_Val();
			PORT_CS_MAX6675 |= (1 << CS_MAX6675);
			/* Ingresar el valor al algoritmo PID y ejecutarlo */
			pid_feedback = (double)valor_adc;
			PID_Basic();
			paquetes_aceptados = pid_output;
				if(valor_adc != valor_adc_anterior){
					valor_adc_anterior = valor_adc;
					temperatura = valor_adc / 4;
					sprintf(buffer, "%d", temperatura);
					LCD_Goto_XY(11,2);
					LCD_Put_String("   ");
					LCD_Goto_XY(11,2);
					LCD_Put_String(buffer);
					sprintf(buffer, "%d,%d\r\n", TEMP_SETPOINT, temperatura);
				}
				UARTn_Tx_String(UART0, buffer);
		}

	}

	return 0;
}

/* Rutinas de servicio a interrupción */

ISR(INT0_vect){

	static uint8_t contador_paquetes = 0;

	contador_paquetes++;

	/* Activar el TRIAC según lo que indique el PID */
	if(contador_paquetes <= paquetes_aceptados){
		PORT_TRIAC_OUT |= (1 << TRIAC_OUT);
	}else{
		PORT_TRIAC_OUT &= ~(1 << TRIAC_OUT);
	}
	/* Reiniciar la cuenta y reajustar cada N_PAQUETES */
	if(contador_paquetes == N_PAQUETES){
		contador_paquetes = 0;
		medir_y_controlar = 1;
	}
}


/* Definición de funciones */

void UART0_Inicializar(void){

	UART_t my_uart;

	my_uart.u2x = U2X_Enabled;
	my_uart.baud_rate = Baud_115200_bps;
	my_uart.tx_status = UART_Tx_Enabled;
	my_uart.rx_status = UART_Rx_Disabled;
	my_uart.data_bits = UART_8_Data_Bits;
	my_uart.stop_bits = UART_1_Stop_Bit;
	my_uart.parity_mode = UART_Parity_Disabled;
	my_uart.interrupt_mask = UART_Interrupts_Disabled;

	UART0_Configurar(&my_uart);
}

void SPI_Master_Inicializar(void){

	spi_config_t my_spi;

	my_spi.mode = SPI_Master;
	my_spi.sck_prescaler = SCK_Fosc_16;
	my_spi.cpol = CPOL_0;
	my_spi.cpha = CPHA_1;
	my_spi.data_order = MSB_First;
	my_spi.interrupt = SPI_Int_Disabled;

	SPI_Configurar(&my_spi);
}

void Int_Externa_Inicializar(void){
	EICRA |= (1 << ISC01) | (1 << ISC00);
	EIMSK |= (1 << INT0);
}

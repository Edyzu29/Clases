
/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "spi_master.h"
#include "max6675.h"
#include "uart.h"
#include "timer.h"

/* Constantes y macros */

#define DDR_CS_TC1	 		DDRB
#define PORT_CS_TC1			PORTB
#define CS_TC1				0

#define DDR_CS_TC2	 		DDRB
#define PORT_CS_TC2			PORTB
#define CS_TC2				1

#define	DDR_ALARM			DDRD
#define PORT_ALARM			PORTD
#define ALARM				5

#define LIMITE_SUPERIOR_TC1		40
#define LIMITE_INFERIOR_TC1		10
#define LIMITE_SUPERIOR_TC2		40
#define LIMITE_INFERIOR_TC2		4

/* Declaración de funciones */
void SPI_Master_Inicializar(void);
void UART0_Inicializar(void);

/* Función principal */
int main(void){

	uint16_t temp1, temp2;

	/* Configurar como salida e inicializar en 0 el pin de la alarma */
	DDR_ALARM |= (1 << ALARM);
	PORT_ALARM &= ~(1 << ALARM);
	/* Configurar como salida e inicializar en 1 los pines Chip Select */
	DDR_CS_TC1 |= (1 << CS_TC1);
	PORT_CS_TC1 |= (1 << CS_TC1);
	DDR_CS_TC2 |= (1 << CS_TC2);
	PORT_CS_TC2 |= (1 << CS_TC2);
	/* Inicializar los módulos Timer1, UART y SPI */
	Timer1_Inicializar();
	UART0_Inicializar();
	SPI_Master_Inicializar();

	while(1){
		/* Medir la temperatura de cada termocupla */
		PORT_CS_TC1 &= ~(1 << CS_TC1);
		temp1 = MAX6675_Get_Temp_Celsius();
		PORT_CS_TC1 |= (1 << CS_TC1);
		PORT_CS_TC1 |= (1 << CS_TC1);

		PORT_CS_TC2 &= ~(1 << CS_TC2);
		temp1 = MAX6675_Get_Temp_Celsius();
		PORT_CS_TC2 |= (1 << CS_TC2);

		/* Visualizar la temperatura en la consola serial */
		UARTn_Tx_String(UART0, "Temp1: ");
		UARTn_Tx_Integer(UART0, temp1);
		UARTn_Tx_String(UART0, "\n\r");
		UARTn_Tx_String(UART0, "Temp2: ");
		UARTn_Tx_Integer(UART0, temp2);
		UARTn_Tx_String(UART0, "\n\n\r");
		/* Determinar si hace falta activar la alarma */
		if((temp1 < LIMITE_INFERIOR_TC1) || (temp1 > LIMITE_SUPERIOR_TC1)){
			TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
			PORT_ALARM |= (1 << ALARM);
		}else if ((temp2 < LIMITE_INFERIOR_TC2) || (temp2 > LIMITE_SUPERIOR_TC2)){
			TCCR1A |=  (1 << COM1A0);
		}else{
			TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
			PORT_ALARM &= ~(1 << ALARM);
		}
		/* Esperar un segundo */
		_delay_ms(1000);
	}


	return 0;
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

void UART0_Inicializar(void){

	UART_t my_uart;

	my_uart.u2x = U2X_Enabled;
	my_uart.baud_rate = Baud_9600_bps;
	my_uart.tx_status = UART_Tx_Enabled;
	my_uart.rx_status = UART_Rx_Disabled;
	my_uart.data_bits = UART_8_Data_Bits;
	my_uart.stop_bits = UART_1_Stop_Bit;
	my_uart.parity_mode = UART_Parity_Disabled;
	my_uart.interrupt_mask = UART_Interrupts_Disabled;

	UART0_Configurar(&my_uart);
}

void Timer1_Inicializar(void){

	Timer_16b_t timer1;

	/* Configurar como salida los pines el pin de l alarma (PD5) */
	DDR_ALARM |= (1 << ALARM);
	/* Establecer los par�metros de configuraci�n del Timer0 */
	timer1.clock = Timer_Clk_PS256;
	timer1.mode = Timer_16b_CTC_OCRA_Mode;
	timer1.OCA = OC_Disabled;
	timer1.OCB = OC_Disabled;
	timer1.ic_edge_selector = Timer_IC_Falling_Edge;
	timer1.ic_noise_canceler = Timer_ICNC_Disabled;
	timer1.OCRA = 15624;
	timer1.OCRB = 0;
	timer1.ICR = 0;
	/* Configurar el Timer1 */
	Timer1_Configurar(&timer1);
}

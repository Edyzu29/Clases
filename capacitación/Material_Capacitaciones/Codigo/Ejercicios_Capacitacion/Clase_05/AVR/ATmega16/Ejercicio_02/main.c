
/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "spi_master.h"
#include "max6675.h"
#include "uart.h"

/* Constantes y macros */

#define DDR_CS_TC1	 		DDRB
#define PORT_CS_TC1			PORTB
#define CS_TC1				0

#define	DDR_ALARM			DDRD
#define PORT_ALARM			PORTD
#define ALARM				5

#define LIMITE_SUPERIOR		40
#define LIMITE_INFERIOR		27

/* Declaración de funciones */
void SPI_Master_Inicializar(void);
void UART0_Inicializar(void);

/* Función principal */
int main(void){

	uint16_t temperatura;

	/* Configurar como salida e inicializar en 0 el pin de la alarma */
	DDR_ALARM |= (1 << ALARM);
	PORT_ALARM &= ~(1 << ALARM);
	/* Configurar como salida e inicialziar en 1 el pin de Chip Select */
	DDR_CS_TC1 |= (1 << CS_TC1);
	PORT_CS_TC1 |= (1 << CS_TC1);
	/* Inicializar los módulos UART y SPI */
	UART0_Inicializar();
	SPI_Master_Inicializar();

	while(1){
		/* Medir la temperatura y mostrarla en la terminal serial */
		PORT_CS_TC1 &= ~(1 << CS_TC1);
		temperatura = MAX6675_Get_Temp_Celsius();
		PORT_CS_TC1 |= (1 << CS_TC1);
		UARTn_Tx_String(UART0, "Temp: ");
		UARTn_Tx_Integer(UART0, temperatura);
		UARTn_Tx_String(UART0, "\n\r");
		/* Determinar si hace falta activar la alarma */
		if((temperatura < LIMITE_INFERIOR) || (temperatura > LIMITE_SUPERIOR)){
			PORT_ALARM |= (1 << ALARM);
		}else{
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

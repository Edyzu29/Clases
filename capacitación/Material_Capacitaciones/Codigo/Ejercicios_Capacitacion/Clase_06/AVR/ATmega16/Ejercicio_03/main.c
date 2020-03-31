
/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "uart.h"
#include "spi_master.h"
#include "twi_master.h"
#include "eeprom_i2c.h"
#include "rtc_ds1307.h"
#include "max6675.h"

/* Constantes y macros */
#define DDR_CS_TC	 		DDRB
#define PORT_CS_TC			PORTB
#define CS_TC				0

#define PIN_SW_MODO			PINB
#define SW_MODO				1


#define EEPROM1_SLAVE_ADDRESS		0x50

#define MEM_POS_INICIAL		2
#define TEMPERATURA_MAXIMA	35

/* Variables globales */

uint16_t n_muestras = 0;
uint16_t pos_memoria = 0;

uint8_t temperatura = 0;
char fecha_s[15];
char hora_s[15];


/* Declaración de funciones */
void UART0_Inicializar(void);
void SPI_Master_Inicializar(void);
void Visualizar_Dato(void);
void Registrar_Dato(void);
void Mostrar_Registro(void);

/* Función principal */
int main(void){
	UART0_Inicializar();
	SPI_Master_Inicializar();
	TWI_Master_Inicializar();
	DDR_CS_TC |= (1 << CS_TC);
	PORT_CS_TC |= (1 << CS_TC);

	/* Si el switch se encuentra en '1', se configura la fecha y hora */
	if(PIN_SW_MODO & (1 << SW_MODO)){
		RTC_Date_Set(19, 2, 20);
		RTC_Time_Set_24h(14,26,00);
	}else{
		Mostrar_Registro();
	}

	while(1){
		/* Si el seitch se encuentra en '1' se monitorea la temperatura */
		if(PIN_SW_MODO & (1 << SW_MODO)){
			Visualizar_Dato();
			if(temperatura > TEMPERATURA_MAXIMA){
				Registrar_Dato();
			}

			_delay_ms(1000);
		}

	}

	return 0;
}


/* Definición de funciones */
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

void Visualizar_Dato(void){
	/* Medir y visualizar la fecha y hora */
	RTC_Date_Read();
	RTC_Time_Read_24h();
	sprintf(fecha_s,"%d/%d/20%d\t", rtc_date_buffer[DAY], rtc_date_buffer[MONTH], rtc_date_buffer[YEAR]);
	sprintf(hora_s,"%d:%d:%d \t", rtc_time_buffer[HOUR], rtc_time_buffer[MINUTE], rtc_time_buffer[SECOND]);
	UARTn_Tx_String(UART0, fecha_s);
	UARTn_Tx_String(UART0, hora_s);
	/* Medir y visualizar la temperatura */
	PORT_CS_TC &= ~(1 << CS_TC);
	temperatura = MAX6675_Get_Temp_Celsius();
	PORT_CS_TC |= (1 << CS_TC);
	UARTn_Tx_Integer(UART0, temperatura);
	UARTn_Tx_String(UART0, "°C\n\r");
}


void Registrar_Dato(void){

	uint8_t aux = 0;
	uint8_t buffer_temporal[2];

	/* Almacenar fecha, hora y temperatura en la EEPROM */
	aux = EEPROM_Write(EEPROM1_SLAVE_ADDRESS, MEM_POS_INICIAL + (9*n_muestras), 4, rtc_date_buffer);
	_delay_ms(10);
	aux = EEPROM_Write(EEPROM1_SLAVE_ADDRESS, MEM_POS_INICIAL + (9*n_muestras + 4), 4, rtc_time_buffer);
	_delay_ms(10);
	aux = EEPROM_Write(EEPROM1_SLAVE_ADDRESS, MEM_POS_INICIAL + (9*n_muestras + 8), 1, &temperatura);
	_delay_ms(10);
	/* Incrementar y almacenar el número de muestras registradas */
	n_muestras++;
	buffer_temporal[0] = (n_muestras >> 8) & 0xFF;
	buffer_temporal[1] = n_muestras & 0xFF;
	aux = EEPROM_Write(EEPROM1_SLAVE_ADDRESS, 0x0000, 2, buffer_temporal);
}


void Mostrar_Registro(void){

	uint8_t aux = 0;
	uint8_t buffer_temporal[2];
	uint16_t index;

	aux = EEPROM_Read(EEPROM1_SLAVE_ADDRESS, 0x0000, 2, buffer_temporal);
	n_muestras = (buffer_temporal[0] << 8) + buffer_temporal[1];

	for(index = 0; index < n_muestras; index++){
		/* Medir y visualizar la fecha, hora y temperatura */
		aux = EEPROM_Read(EEPROM1_SLAVE_ADDRESS, MEM_POS_INICIAL + (9*index), 4, rtc_date_buffer);
		aux = EEPROM_Read(EEPROM1_SLAVE_ADDRESS, MEM_POS_INICIAL + (9*index + 4), 4, rtc_time_buffer);
		aux = EEPROM_Read(EEPROM1_SLAVE_ADDRESS, MEM_POS_INICIAL + (9*index + 8), 1, &temperatura);
		sprintf(fecha_s,"%d/%d/20%d\t", rtc_date_buffer[DAY], rtc_date_buffer[MONTH], rtc_date_buffer[YEAR]);
		sprintf(hora_s,"%d:%d:%d \t", rtc_time_buffer[HOUR], rtc_time_buffer[MINUTE], rtc_time_buffer[SECOND]);
		UARTn_Tx_String(UART0, fecha_s);
		UARTn_Tx_String(UART0, hora_s);
		UARTn_Tx_Integer(UART0, temperatura);
		UARTn_Tx_String(UART0, "°C\n\r");
		_delay_ms(10);
	}

}






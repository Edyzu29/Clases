
/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "twi_master.h"
#include "rtc_ds1307.h"
#include "uart.h"

/* Constantes y macros */


/* Variables globales */
char fecha_s[15];
char hora_s[15];


/* Declaración de funciones */
void UART0_Inicializar(void);

/* Función principal */
int main(void){
	UART0_Inicializar();
	TWI_Master_Inicializar();
	RTC_Date_Set(18, 2, 20);
	RTC_Time_Set_24h(18,23,00);

	while(1){
		RTC_Date_Read();
		RTC_Time_Read_24h();
		sprintf(fecha_s,"%d/%d/20%d\t", rtc_date_buffer[DAY], rtc_date_buffer[MONTH], rtc_date_buffer[YEAR]);
		sprintf(hora_s,"%d:%d:%d\n\r", rtc_time_buffer[HOUR], rtc_time_buffer[MINUTE], rtc_time_buffer[SECOND]);
		UARTn_Tx_String(UART0, fecha_s);
		UARTn_Tx_String(UART0, hora_s);

		_delay_ms(1000);

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




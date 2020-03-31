/*
 * uart.h
 *
 *  Created on: 18 mar. 2018
 *      Author: MiguelAngel
 */

#ifndef UART_H_
#define UART_H_

/* Inclusiones */
#include <stdint.h>

/* Constantes y macros */
#define F_OSC			8000000UL

/* Definici�n de estructuras y tipos */

typedef enum{
	UART0 = 0,
	UART1,
	UART2,
	UART3
}UART_Module_t;

typedef enum{
	Baud_300_bps = 300,
	Baud_600_bps = 600,
	Baud_1200_bps = 1200,
	Baud_2400_bps = 2400,
	Baud_4800_bps = 4800,
	Baud_9600_bps = 9600,
	Baud_19200_bps = 19200,
	Baud_38400_bps = 38400,
	Baud_57600_bps = 57600,
	Baud_115200_bps = 115200
}UART_Baud_Rate_t;

typedef enum{
	UART_Tx_Disabled = 0,
	UART_Tx_Enabled
}UART_Tx_Status_t;

typedef enum{
	UART_Rx_Disabled = 0,
	UART_Rx_Enabled
}UART_Rx_Status_t;

typedef enum{
	UART_5_Data_Bits = 0,
	UART_6_Data_Bits = 1,
	UART_7_Data_Bits = 2,
	UART_8_Data_Bits = 3,
	UART_9_Data_Bits = 7
}UART_Data_Bits_t;

typedef enum{
	UART_Parity_Disabled = 0,
	UART_Parity_Even = 2,
	UART_Parity_Odd = 3
}UART_Parity_Mode_t;


typedef enum{
	UART_1_Stop_Bit = 0,
	UART_2_Stop_Bits
}UART_Stop_Bits_t;


typedef enum{
	U2X_Disabled = 0,
	U2X_Enabled
}UART_2X_Mode_t;

typedef enum{
	UART_Interrupts_Disabled = 0,
	UART_Data_Register_Empty_IE = 32,
	UART_Tx_Complete_IE = 64,
	UART_Rx_Complete_IE = 128
}UART_Interrupt_Mask_t;


typedef struct{
	UART_2X_Mode_t u2x;
	UART_Baud_Rate_t baud_rate;
	UART_Tx_Status_t tx_status;
	UART_Rx_Status_t rx_status;
	UART_Data_Bits_t data_bits;
	UART_Stop_Bits_t stop_bits;
	UART_Parity_Mode_t parity_mode;
	UART_Interrupt_Mask_t interrupt_mask;
}UART_t;


/* Declaraci�n de funciones (prototipos) */
void UART0_Configurar(UART_t *uart_param);
void UARTn_Tx_Byte(UART_Module_t uart_n, char byte);
void UARTn_Tx_String(UART_Module_t uart_n, char *str);
void UARTn_Tx_Integer(UART_Module_t uart_n, int32_t num);

#endif /* UART_H_ */

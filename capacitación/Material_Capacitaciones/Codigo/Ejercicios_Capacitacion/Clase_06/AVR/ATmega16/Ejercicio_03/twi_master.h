
#ifndef TWI_MASTER_H_
#define TWI_MASTER_H_

 /* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>

/* Constantes y macros */
#define	I2C_START				0x08
#define I2C_RESTART				0x10
#define I2C_SLAW_ACK			0x18
#define I2C_SLAW_NACK			0x20
#define I2C_DATA_TX_ACK			0x28
#define I2C_DATA_TX_NACK		0x30
#define I2C_ARBIT_LOST			0x38
#define I2C_SLAR_ACK			0x40
#define I2C_SLAR_NACK			0x48
#define I2C_DATA_RX_ACK			0x50
#define I2C_DATA_RX_NACK		0x58

#define I2C_OK					0x00
#define I2C_ERROR				0x01

#define I2C_TX_BUFF_SIZE		20
#define I2C_RX_BUFF_SIZE		20

/* Variables globales */
extern volatile uint8_t i2c_tx_buffer[I2C_TX_BUFF_SIZE];
extern volatile uint8_t i2c_rx_buffer[I2C_RX_BUFF_SIZE];
extern volatile uint8_t i2c_status;

/* Declaración de funciones */
void TWI_Master_Inicializar(void);
uint8_t TWI_Master_Start_Condition(void);
uint8_t TWI_Master_Tx_Address(uint8_t slave_address);
uint8_t TWI_Master_Tx_Byte(uint8_t byte);
uint8_t TWI_Master_Rx_Byte_ACK(uint8_t *byte_p);
uint8_t TWI_Master_Rx_Byte_NACK(uint8_t *byte_p);
void TWI_Master_Stop_Condition(void);
uint8_t TWI_Master_Write_Data(uint8_t slave_addr, uint8_t n_bytes, uint8_t *output_buffer);
uint8_t TWI_Master_Read_Data(uint8_t slave_addr, uint8_t n_bytes, uint8_t *input_buffer);


#endif /* TWI_MASTER_H_ */

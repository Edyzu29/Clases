/* Inclusión de archivos */
#include <stdint.h>
#include "twi_master.h"
#include "eeprom_i2c.h"

/* Definición de funciones */

uint8_t EEPROM_Write(uint8_t slave_addr, uint16_t mem_addr, uint8_t n_bytes, uint8_t *buffer){


	uint8_t slaw_address;
	uint8_t mem_address[2];
	uint8_t salida = I2C_OK;
	uint8_t temp = I2C_OK;
	uint8_t index;

	/* Definir la dirección del esclavo para escritura */
	slaw_address = slave_addr << 1;

	if(mem_addr % 32){
		/* Generar la condición de start */
		salida = TWI_Master_Start_Condition();
		/* Transmitir dirección del esclavo en modo escritura */
		salida = TWI_Master_Tx_Address(slaw_address);
		/* Transmitir la posición de memoria en donde se desea empezar a escribir */
		mem_address[0] = (mem_addr >> 8) & 0xFF;
		mem_address[1] = mem_addr & 0xFF;
		salida = TWI_Master_Tx_Byte(mem_address[0]);
		salida = TWI_Master_Tx_Byte(mem_address[1]);
	}


	/* Escribir los datos en posiciones consecutivas de memoria */
	for(index = 0; index < n_bytes; index++){

		if(!(mem_addr % 32)){
			/* Generar la condición de start */
			salida = TWI_Master_Start_Condition();
			/* Transmitir dirección del esclavo en modo escritura */
			salida = TWI_Master_Tx_Address(slaw_address);
			/* Transmitir la posición de memoria en donde se desea empezar a escribir */
			mem_address[0] = (mem_addr >> 8) & 0xFF;
			mem_address[1] = mem_addr & 0xFF;
			salida = TWI_Master_Tx_Byte(mem_address[0]);
			salida = TWI_Master_Tx_Byte(mem_address[1]);
		}

		salida = TWI_Master_Tx_Byte(*(buffer + index));
		mem_addr++;
	}

	TWI_Master_Stop_Condition();
	return salida;
}




uint8_t EEPROM_Read(uint8_t slave_addr, uint16_t mem_addr, uint8_t n_bytes, uint8_t *buffer){

	uint8_t slar_address;
	uint8_t mem_address[2];
	uint8_t salida = I2C_OK;
	uint8_t index;


	/* Definir la dirección del esclavo para lectura */
	slar_address = (slave_addr << 1) | 1;

	if(mem_addr % 32){
		/* Transsmitir la posición de memoria en donde se desea empezar a leer */
		mem_address[0] = (mem_addr >> 8) & 0xFF;
		mem_address[1] = mem_addr & 0xFF;
		salida = TWI_Master_Write_Data(slave_addr, 2, mem_address);
		/* Generar la condición de start */
		salida = TWI_Master_Start_Condition();
		/* Transmitir dirección del esclavo en modo escritura */
		salida = TWI_Master_Tx_Address(slar_address);
	}

	/* Leer los datos en posiciones consecutivas de memoria */
	for(index = 0; index < n_bytes; index++){

		if(!(mem_addr % 32)){
			/* Transsmitir la posición de memoria en donde se desea empezar a leer */
			mem_address[0] = (mem_addr >> 8) & 0xFF;
			mem_address[1] = mem_addr & 0xFF;
			salida = TWI_Master_Write_Data(slave_addr, 2, mem_address);
			/* Generar la condición de start */
			salida = TWI_Master_Start_Condition();
			/* Transmitir dirección del esclavo en modo escritura */
			salida = TWI_Master_Tx_Address(slar_address);
		}

		if(index == (n_bytes - 1)){
			salida = TWI_Master_Rx_Byte_NACK(buffer + index);
		}else{
			salida = TWI_Master_Rx_Byte_ACK(buffer + index);
		}

		mem_addr++;
	}

	TWI_Master_Stop_Condition();

	return salida;
}


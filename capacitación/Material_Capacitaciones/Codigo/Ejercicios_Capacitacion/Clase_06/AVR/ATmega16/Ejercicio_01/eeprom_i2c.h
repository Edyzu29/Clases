/*
 * eeprom_i2c.h
 *
 *  Created on: Feb 2, 2020
 *      Author: mtorres
 */

#ifndef EEPROM_I2C_H_
#define EEPROM_I2C_H_

/* Inclusión de archivos */
#include <stdint.h>

/* Declaración de funciones (prototipos) */
uint8_t EEPROM_Write(uint8_t slave_address, uint16_t mem_addr, uint8_t n_bytes, uint8_t *buffer);
uint8_t EEPROM_Read(uint8_t slave_address, uint16_t mem_addr, uint8_t n_bytes, uint8_t *buffer);


#endif /* EEPROM_I2C_H_ */

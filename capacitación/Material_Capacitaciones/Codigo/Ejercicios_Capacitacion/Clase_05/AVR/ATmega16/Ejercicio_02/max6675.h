/*
 * max6675.h
 *
 *  Created on: Feb 3, 2020
 *      Author: mtorres
 */

#ifndef MAX6675_H_
#define MAX6675_H_

/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>


/* Declaración de funciones (prototipos) */
uint16_t MAX6675_Get_Bin_Val(void);
uint16_t MAX6675_Get_Temp_Celsius(void);

#endif /* MAX6675_H_ */

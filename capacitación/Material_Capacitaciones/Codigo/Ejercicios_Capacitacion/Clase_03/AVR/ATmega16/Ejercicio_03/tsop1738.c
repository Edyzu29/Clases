/*
 * tsop1738.c
 *
 *  Created on: Feb 6, 2020
 *      Author: mtorres
 */

#include <stdint.h>
#include "tsop1738.h"

/* Variables globales */
volatile uint8_t read_ir_state = 0;
volatile uint16_t ir_raw_value = 0;
volatile uint8_t ir_command = 0;

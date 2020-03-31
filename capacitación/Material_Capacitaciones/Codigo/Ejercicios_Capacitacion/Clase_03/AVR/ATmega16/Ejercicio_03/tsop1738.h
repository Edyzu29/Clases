/*
 * tsop1738.h
 *
 *  Created on: Feb 6, 2020
 *      Author: mtorres
 */

#ifndef TSOP1738_H_
#define TSOP1738_H_

/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>

/* Constantes y macros */
#define PIN_IR_CONTROL			PIND
#define IR_CONTROL				2

#define IR_READING_COMPLETE		12

#define IR_TOGGLE_MASK			(~(1 << 12))

/* Variables globales */
extern volatile uint8_t read_ir_state;
extern volatile uint16_t ir_raw_value;
extern volatile uint8_t ir_command;


/* Llamar a esta función en una ISR que ocurra cada 1700 us */

static inline void IR_Control_Rx(void){

	/* Al comenzar, limpiar el valor ir_raw_value */
	if(read_ir_state == 0){
		ir_raw_value = 0;
	}

	/* Insertar el bit leído a la derecha de ir_raw_value */
	ir_raw_value <<= 1;
	if(!(PIN_IR_CONTROL & (1 << IR_CONTROL))){
		ir_raw_value |= 1;
	}

	/* Avanzar al siguiente estado */
	read_ir_state++;

	/* Si se ha terminado de leer todos los bits ... */
	if(read_ir_state == IR_READING_COMPLETE){
		/* Ignorar el bit toggle (Protocolo RC-5) */
		ir_command = ir_raw_value & IR_TOGGLE_MASK;
	}

}

#endif /* TSOP1738_H_ */

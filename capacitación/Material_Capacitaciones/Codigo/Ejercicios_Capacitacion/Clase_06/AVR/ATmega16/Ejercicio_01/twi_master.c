

/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include "twi_master.h"

/* Variables globales */
volatile uint8_t i2c_tx_buffer[I2C_TX_BUFF_SIZE];
volatile uint8_t i2c_rx_buffer[I2C_RX_BUFF_SIZE];
volatile uint8_t i2c_status = I2C_OK;


/* Definición de funciones */

/****************************************************************************
* Nombre de la función: TWI_Master_Inicializar
* retorna : Nada
* arg : Ninguno
* Descripción : Configura la frecuencia de la señal de reloj SCL, la cual
*               define la tasa de transferencia de bits del bus I2C/TWI.
*****************************************************************************/
void TWI_Master_Inicializar(void){

	/* Configurar la interfaz TWI para tener un SCL de frecuencia 50 KHz */
	// Configurar el registro TWI Baud Rate (TWBR): 152
	TWBR = 152;
	//Configurar el TWI Prescaler (TWPS): 00 (Prescaler = 1)
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
}



/****************************************************************************
* Nombre de la función: TWI_Master_Start_Condition
* retorna : el estado del bus I2C/TWI (si hubo algún error en la transmisión)
* arg : Ninguno
* Descripción : Transmite la condición START, necesaria para iniciar la
*               comunicación con un dispositivo esclavo.
* Notas : Al transmitir exitosamente una condición START, el microcontrolador
* 		  asume el rol de dispositivo maestro e inicia la comunicación.
*****************************************************************************/
uint8_t TWI_Master_Start_Condition(void){

	uint8_t salida = I2C_OK;

	/* Enviar una condición de START */
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	/* Esperar a que la transmisión se complete */
	while (!(TWCR & (1<<TWINT))){
		/* No hacer nada */
	}

	/* Verificar la condición de START */
	if ((TWSR & 0xF8) != I2C_START){
		salida = I2C_ERROR;
	}

	return salida;

}



/****************************************************************************
* Nombre de la función: TWI_Master_Tx_Address
* retorna : el estado del bus I2C/TWI (si hubo algún error en la transmisión)
* arg : slave_address
* Descripción : Recibe como parámetro un número de 8 bits, conformado por la
* 				dirección de 7 bits del dispositivo esclavo y el bit R/W (LSB).
* 				Este número se transmite por el bus I2C/TWI en la forma de una
* 				trama de dirección.
* Notas : Si ningún dispositivo esclavo responde a la transmisión con una
*         condición ACK, entonces la función retornará un valor que indica
*         que ha ocurrido un ERROR.
*****************************************************************************/
uint8_t TWI_Master_Tx_Address(uint8_t slave_address){

	uint8_t salida = I2C_OK;

	/* Cargar la dirección del esclavo e iniciar la transmisión */
	TWDR = slave_address;
	TWCR = (1<<TWINT) | (1<<TWEN);

	/* Esperar a que la transmisión se complete */
	while (!(TWCR & (1<<TWINT))){
		/* No hacer nada */
	}

	/* Verificar el envío de la dirección */
	if (((TWSR & 0xF8) != I2C_SLAW_ACK) && ((TWSR & 0xF8) != I2C_SLAR_ACK)){
		salida = I2C_ERROR;
	}

	return salida;

}


/****************************************************************************
* Nombre de la función: TWI_Master_Tx_Byte
* retorna : el estado del bus I2C/TWI (si hubo algún error en la transmisión)
* arg : byte
* Descripción : Recibe como parámetro un dato de 8 bits y lo transmite por
* 				el bus I2C/TWI en la forma de una trama de datos.
* Notas : Si ningún dispositivo esclavo responde a la transmisión con una
*         condición ACK, entonces la función retornará un valor que indica
*         que ha ocurrido un ERROR.
*****************************************************************************/
uint8_t TWI_Master_Tx_Byte(uint8_t byte){

	uint8_t salida = I2C_OK;

	/* Cargar el dato e iniciar la transmisión */
	TWDR = byte;
	TWCR = (1<<TWINT) | (1<<TWEN);

	/* Esperar a que el dato se termine de recibir */
	while (!(TWCR & (1<<TWINT))){
		/* No hacer nada */
	}

	/* Verificar la recepción del dato */
	if ((TWSR & 0xF8) != I2C_DATA_TX_ACK){
		salida = I2C_ERROR;
	}

	return salida;

}


/****************************************************************************
* Nombre de la función: TWI_Master_Rx_Byte_ACK
* retorna : el estado del bus I2C/TWI (si hubo algún error en la recepción)
* arg : byte
* Descripción : Recibe como parámetro un puntero a un dato de 8 bits. Este
* 				puntero indica la ubicación de memoria donde se almacenará
* 				el dato que es recibido a través de una trama de datos.
* Notas : Al terminar de recibir el dato de 8 bits, esta función retorna una
* 		  condición ACK, lo cual quiere decir que se espera a que el esclavo
* 		  continúe enviando tramas de datos al maestro.
*****************************************************************************/
uint8_t TWI_Master_Rx_Byte_ACK(uint8_t *byte_p){

	uint8_t salida = I2C_OK;

	/* Iniciar la recepción de datos */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWEA);

	/* Esperar a que el dato se termine de recibir */
	while (!(TWCR & (1<<TWINT))){
		/* No hacer nada */
	}

	/* Verificar la recepción del dato */
	if ((TWSR & 0xF8) != I2C_DATA_RX_ACK){
		salida = I2C_ERROR;
	}

	*byte_p = TWDR;
	return salida;

}


/****************************************************************************
* Nombre de la función: TWI_Master_Rx_Byte_ACK
* retorna : el estado del bus I2C/TWI (si hubo algún error en la recepción)
* arg : byte
* Descripción : Recibe como parámetro un puntero a un dato de 8 bits. Este
* 				puntero indica la ubicación de memoria donde se almacenará
* 				el dato que es recibido a través de una trama de datos.
* Notas : Al terminar de recibir el dato de 8 bits, esta función retorna una
* 		  condición NACK, lo cual quiere decir el esclavo ya no transmitirá
* 		  más datos al maestro. Luego de enviar la condición NACK, el maestro
* 		  debe transmitir una condición STOP.
*****************************************************************************/
uint8_t TWI_Master_Rx_Byte_NACK(uint8_t *byte_p){

	uint8_t salida = I2C_OK;

	/* Iniciar la recepción de datos */
	TWCR = (1<<TWINT) | (1<<TWEN);

	/* Esperar a que el dato se termine de recibir */
	while (!(TWCR & (1<<TWINT))){
		/* No hacer nada */
	}

	/* Verificar la recepción del dato */
	if ((TWSR & 0xF8) != I2C_DATA_RX_NACK){
		salida = I2C_ERROR;
	}

	*byte_p = TWDR;

	return salida;

}


/****************************************************************************
* Nombre de la función: TWI_Master_Stop_Condition
* retorna : nada
* arg : Ninguno
* Descripción : Transmite una condición STOP, la cual completa un proceso de
* 		 		comunicación en el bus I2C.
* Notas : Esta función es utilizada para indicar la finalización del proceso
* 		  de comunicación.
*****************************************************************************/
void TWI_Master_Stop_Condition(void){
	/* Transmitir la condición de STOP */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}


/****************************************************************************
* Nombre de la función: TWI_Master_Restart_Condition
* retorna : el estado del bus I2C/TWI (si hubo algún error en la transmisión)
* arg : Ninguno
* Descripción : Transmite una condición START, sin necesidad de haber enviado
* 				antes una condición STOP.
* Notas : Esta función es utilizada cuando el bus TWI trabaja en modo multi-
* 		  maestro. Con esta función es posible iniciar una nueva comunicación
* 		  sin necesidad de haber transmitido una condición STOP.
*****************************************************************************/
uint8_t TWI_Master_Restart_Condition(void){

	uint8_t salida = I2C_OK;

	/* Enviar una condición de START */
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	/* Esperar a que la condición de START se termine de enviar */
	while (!(TWCR & (1<<TWINT))){
		/* No hacer nada */
	}
	/* Verificar la condición de RESTART */
	if ((TWSR & 0xF8) != I2C_RESTART){
		salida = I2C_ERROR;
	}
	return salida;

}


/****************************************************************************
* Nombre de la función: TWI_Master_Write_Data
* retorna : el estado del bus I2C/TWI (si hubo algún error en la transmisión)
* arg : slave_addr
* arg: n_bytes
* arg: output_buffer
* Descripción : Primero,  transmite el valor slave_write_addr (SLA + 0) en una
* 				trama de dirección. Luego, transmite 'n' bytes consecutivos,
* 				en forma de tramas de datos.
* Notas : Los 'n' bytes que serán transmitidos se encuentran almacenados en el
* 		  arreglo output_buffer. Puede aprovechar el arreglo i2c_tx_buffer y
* 		  pasarlo como parámetro output_buffer.
*****************************************************************************/
uint8_t TWI_Master_Write_Data(uint8_t slave_addr, uint8_t n_bytes, uint8_t *output_buffer){

	uint8_t slaw_addr;
	uint8_t salida = I2C_OK;
	uint8_t temp = I2C_OK;
	uint8_t index;

	/* Definir la dirección de esclavo para escritura */
	slaw_addr = slave_addr << 1;

	/* Generar la condición de start */
	salida = TWI_Master_Start_Condition();

	/* Transmitir dirección del esclavo y modo (lectura o escritura) */
	salida = TWI_Master_Tx_Address(slaw_addr);

	/* Transmitir los datos */
	for(index = 0; index < n_bytes; index++){
		temp = TWI_Master_Tx_Byte(*(output_buffer + index));
		if(temp == I2C_ERROR){
			salida = I2C_ERROR;
		}
	}


	TWI_Master_Stop_Condition();

	return salida;

}


/****************************************************************************
* Nombre de la función: TWI_Master_Read_Data
* retorna : el estado del bus I2C/TWI (si hubo algún error en la transmisión)
* arg : slave_addr
* arg: n_bytes
* arg: intput_buffer
* Descripción : Primero,  transmite el valor slave_read_addr (SLA + 1) en una
* 				trama de dirección. Luego, recibe 'n' bytes consecutivos,
* 				en forma de tramas de datos.
* Notas : Los 'n' bytes que serán recibidos se almacenarán en el arreglo
*  		  input_buffer. Puede aprovechar el arreglo i2c_tx_buffer y pasarlo
*  		  como parámetro input_buffer.
*****************************************************************************/
uint8_t TWI_Master_Read_Data(uint8_t slave_addr, uint8_t n_bytes, uint8_t *input_buffer){

	uint8_t slar_addr;
	uint8_t salida = I2C_OK;
	uint8_t temp = I2C_OK;
	uint8_t index;

	/* Definir la dirección de esclavo para escritura */
	slar_addr = (slave_addr << 1) | 1;

	/* Generar la condición de start */
	salida = TWI_Master_Start_Condition();

	/* Transmitir dirección del esclavo y modo (lectura) */
	salida = TWI_Master_Tx_Address(slar_addr);

	/* Transmitir los datos */
	for(index = 0; index < n_bytes; index++){

		if(index < (n_bytes - 1)){
			temp = TWI_Master_Rx_Byte_ACK(input_buffer + index);
		}else{
			temp = TWI_Master_Rx_Byte_NACK(input_buffer + index);
		}

		if(temp == I2C_ERROR){
			salida = I2C_ERROR;
		}
	}

	TWI_Master_Stop_Condition();

	return salida;

}





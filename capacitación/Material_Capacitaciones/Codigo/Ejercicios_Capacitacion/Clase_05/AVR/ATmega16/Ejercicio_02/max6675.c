
#include <avr/io.h>
#include <stdint.h>
#include "spi_master.h"
#include "max6675.h"


/****************************************************************************
* Nombre de la función: MAX6675_Get_Bin_Val
* retorna : valor binario medido (de 12 bits)
* arg : Ninguno
* Descripción : Lleva a cabo la lectura de los dos bytes porporcionados por
* 				el dispositivo, y retorna el valor binario (de 12 bits) que
* 				corresponde al resultado de la medición.
*
* Notas : El valor binario de la medición es empleado cuando se requiere una
* 		 mayor precisión para monitorear y controlar temperatura.
*****************************************************************************/
uint16_t MAX6675_Get_Bin_Val(void){

	uint8_t valor_binario;
	uint8_t rx_data[2];
	uint8_t tx_data[2]; // No importa lo que se transmita, ya que no hay línea MOSI

	/* Recibir dos bytes desde el dispositivo esclavo */
	SPI_Master_Tx_Bitstream(2, tx_data, rx_data);
	/* Concatenar los bits recibidos para reconstruir el valor binario */
	valor_binario = (rx_data[0] << 5) + (rx_data[1] >> 3);

	/* Retornar el valor binario de la medición */
	return (uint8_t)valor_binario;
}


/****************************************************************************
* Nombre de la función: MAX6675_Get_Temp_Celsius
* retorna : temperatura en grados Celsius (de 0 a 1024)
* arg : Ninguno
* Descripción : Lleva a cabo la lectura de los dos bytes porporcionados por
* 				el dispositivo, y retorna el valor de la medición en grados
* 				Celsius (valores enteros).
*
* Notas : El valor en grados Celsius (entero) se emplea cuando no se requiere
* 		  demasiada precisión (monitoreo).
*****************************************************************************/
uint16_t MAX6675_Get_Temp_Celsius(void){

	uint8_t grados_celsius;
	uint8_t rx_data[2];
	uint8_t tx_data[2]; // No importa lo que se transmita, ya que no hay línea MOSI

	/* Recibir dos bytes desde el dispositivo esclavo */
	SPI_Master_Tx_Bitstream(2, tx_data, rx_data);

	/* Concatenar los bits recibidos para reconstruir el valor de la temperatura */
	grados_celsius = (rx_data[0] << 3) + (rx_data[1] >> 5);

	/* Retornar el valor de la temperatura en grados celsius */
	return grados_celsius;

}



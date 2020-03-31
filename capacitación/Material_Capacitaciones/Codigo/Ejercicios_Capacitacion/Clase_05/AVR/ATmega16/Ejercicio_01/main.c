
/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "spi_master.h"


void SPI_Master_Inicializar(void);

/* Función principal */

int main(void){

	uint8_t valor = 0;

	SPI_Master_Inicializar();

	while(1){
		valor = SPI_Master_Tx_Byte(0x31);
		_delay_ms(1);
	}

	return 0;
}

void SPI_Master_Inicializar(void){

	spi_config_t my_spi;

	my_spi.mode = SPI_Master;
	my_spi.sck_prescaler = SCK_Fosc_64;
	my_spi.cpol = CPOL_1;
	my_spi.cpha = CPHA_0;
	my_spi.data_order = MSB_First;
	my_spi.interrupt = SPI_Int_Disabled;

	SPI_Configurar(&my_spi);
}


/* Inclusión de archivos */
#include <avr/io.h>
#include <stdint.h>
#include "twi_master.h"
#include "rtc_ds1307.h"


/* Variables globales */
volatile uint8_t rtc_time_buffer[4];
volatile uint8_t rtc_date_buffer[4];


/* Definición de funciones */

/****************************************************************************
* Nombre de la función: RTC_Time_Set_12h
* retorna : el estado del bus I2C/TWI (si hubo algún error en la transmisión)
* arg : hh(hora)
* arg: mm(minuto)
* arg: ss(segundo)
* arg: meridiem(am o pm)
* Descripción : Establece el tiempo actual en el RTC, en formato 12 horas.
* 				En este formato, el tiempo puede ir desde 01:00:00 hasta
* 				12:59:59.
*
* Notas : 		El último argumento indica si la hora ingresada corresponde
*  				a la madrugada/mañana(am) o a la tarde/noche(pm).
*****************************************************************************/
uint8_t RTC_Time_Set_12h(uint8_t hh, uint8_t mm, uint8_t ss, meridiem_t mer){

	uint8_t hh_d, hh_u, mm_d, mm_u, ss_d, ss_u;
	uint8_t salida = I2C_OK;

	/* Extraer los dígitos de las horas, minutos y segundos */
	hh_u = hh % 10;
	hh_d = (hh / 10) % 10;

	mm_u = mm % 10;
	mm_d = (mm / 10) % 10;

	ss_u = ss % 10;
	ss_d = (ss / 10) % 10;

	/* Cargar dirección del registro de segundos en el buffer de transmisión */
	i2c_tx_buffer[0] = SECONDS_REG;
	/* Cargar los dígitos y meridiano al buffer de transmisión */
	i2c_tx_buffer[1] = (ss_d << 4) | ss_u;
	i2c_tx_buffer[2] = (mm_d << 4) | mm_u;
	i2c_tx_buffer[3] = (1 << BIT_12_24) | (mer << 5) | ((hh_d & 1) << 4) | hh_u;

	/* Transmitir los 4 bytes al RTC */
	salida = TWI_Master_Write_Data(DS1307_SLAVE_ADDRESS, 4, i2c_tx_buffer);

	return salida;
}


/****************************************************************************
* Nombre de la función: RTC_Time_Set_24h
* retorna : el estado del bus I2C/TWI (si hubo algún error en la transmisión)
* arg : hh(hora)
* arg: mm(minuto)
* arg: ss(segundo)
* Descripción : Establece el tiempo actual en el RTC, en formato 24 horas.
* 				En este formato, el tiempo puede ir desde 00:00:00 hasta
* 				23:59:59.
*
* Notas : 		Eln esste caso no hace falta ingresar el valor del meridiano.
*****************************************************************************/
uint8_t RTC_Time_Set_24h(uint8_t hh, uint8_t mm, uint8_t ss){

	uint8_t hh_d, hh_u, mm_d, mm_u, ss_d, ss_u;
	uint8_t salida = I2C_OK;

	/* Extraer los dígitos de las horas, minutos y segundos */
	hh_u = hh % 10;
	hh_d = (hh / 10) % 10;

	mm_u = mm % 10;
	mm_d = (mm / 10) % 10;

	ss_u = ss % 10;
	ss_d = (ss / 10) % 10;

	/* Cargar dirección del registro de segundos en el buffer de transmisión */
	i2c_tx_buffer[0] = SECONDS_REG;
	/* Cargar los dígitos al buffer de transmisión */
	i2c_tx_buffer[1] = (ss_d << 4) | ss_u;
	i2c_tx_buffer[2] = (mm_d << 4) | mm_u;
	i2c_tx_buffer[3] = (hh_d << 4) | hh_u;

	/* Transmitir los 4 bytes al RTC */
	salida = TWI_Master_Write_Data(DS1307_SLAVE_ADDRESS, 4, i2c_tx_buffer);

	return salida;

}


/****************************************************************************
* Nombre de la función: RTC_Time_Read_12h
* retorna : el estado del bus I2C/TWI (si hubo algún error en la transmisión)
* arg : Ninguno
* Descripción : Lee el tiempo actual en el RTC, en formato 12 horas.
* 				En este formato, el tiempo puede ir desde 01:00:00 am hasta
* 				12:59:59 pm.
*
* Notas : 		Los valores leídos son almacenados en el arreglo rtc_time_buffer.
* 				Cada elemento tiene su índice correspondiente:
*
* 				rtc_time_buffer[HOUR] 		--> Almacena la hora
* 				rtc_time_buffer[MINUTE] 	--> Almacena el minuto
* 				rtc_time_buffer[SECOND] 	--> Almacena el segundo
* 				rtc_time_buffer[MERIDIEM] 	--> Almacena el meridiano
*****************************************************************************/
uint8_t RTC_Time_Read_12h(void){

	uint8_t hh_d, hh_u, mm_d, mm_u, ss_d, ss_u;
	uint8_t salida = I2C_OK;

	/* Cargar dirección del registro de segundos en el buffer de transmisión */
	i2c_tx_buffer[0] = SECONDS_REG;
	/* Escribir la dirección del primer registro a leer (SEGUNDOS) */
	salida = TWI_Master_Write_Data(DS1307_SLAVE_ADDRESS, 1, i2c_tx_buffer);
	/* Leer los registros */
	if(salida == I2C_OK){
		salida = TWI_Master_Read_Data(DS1307_SLAVE_ADDRESS, 3, i2c_rx_buffer);

		/* Cargar los valores de hora, minuto, segundo y meridiano */
		ss_u = i2c_rx_buffer[0] & 0x0F;
		ss_d = (i2c_rx_buffer[0] >> 4) & 0x07;
		rtc_time_buffer[SECOND] = (10 * ss_d) + ss_u;

		mm_u = i2c_rx_buffer[1] & 0x0F;
		mm_d = (i2c_rx_buffer[1] >> 4) & 0x0F;
		rtc_time_buffer[MINUTE] = (10 * mm_d) + mm_u;

		hh_u = i2c_rx_buffer[2] & 0x0F;
		hh_d = (i2c_rx_buffer[2] >> 4) & 1;
		rtc_time_buffer[HOUR] = (10 * hh_d) + hh_u;

		rtc_time_buffer[MERIDIEM] = (i2c_rx_buffer[2] >> BIT_A_P) & 1;

	}


	return salida;
}



/****************************************************************************
* Nombre de la función: RTC_Time_Read_24h
* retorna : el estado del bus I2C/TWI (si hubo algún error en la transmisión)
* arg : Ninguno
* Descripción : Lee el tiempo actual en el RTC, en formato 12 horas.
* 				En este formato, el tiempo puede ir desde 00:00:00 hasta
* 				23:59:59 pm.
*
* Notas : 		Los valores leídos son almacenados en el arreglo rtc_time_buffer.
* 				Cada elemento tiene su índice correspondiente:
*
* 				rtc_time_buffer[HOUR] 		--> Almacena la hora
* 				rtc_time_buffer[MINUTE] 	--> Almacena el minuto
* 				rtc_time_buffer[SECOND] 	--> Almacena el segundo
*****************************************************************************/

uint8_t RTC_Time_Read_24h(void){

	uint8_t hh_d, hh_u, mm_d, mm_u, ss_d, ss_u;
	uint8_t salida = I2C_OK;

	/* Cargar dirección del registro de segundos en el buffer de transmisión */
	i2c_tx_buffer[0] = SECONDS_REG;
	/* Escribir la dirección del primer registro a leer (SEGUNDOS) */
	salida = TWI_Master_Write_Data(DS1307_SLAVE_ADDRESS, 1, i2c_tx_buffer);
	/* Leer los registros */
	if(salida == I2C_OK){
		salida = TWI_Master_Read_Data(DS1307_SLAVE_ADDRESS, 3, i2c_rx_buffer);

		/* Cargar los valores de hora, minuto y segundo */
		ss_u = i2c_rx_buffer[0] & 0x0F;
		ss_d = (i2c_rx_buffer[0] >> 4) & 0x0F;
		rtc_time_buffer[SECOND] = (10 * ss_d) + ss_u;

		mm_u = i2c_rx_buffer[1] & 0x0F;
		mm_d = (i2c_rx_buffer[1] >> 4) & 0x0F;
		rtc_time_buffer[MINUTE] = (10 * mm_d) + mm_u;

		hh_u = i2c_rx_buffer[2] & 0x0F;
		hh_d = (i2c_rx_buffer[2] >> 4) & 3;
		rtc_time_buffer[HOUR] = (10 * hh_d) + hh_u;

	}

	return salida;
}



/****************************************************************************
* Nombre de la función: RTC_Date_Set
* retorna : el estado del bus I2C/TWI (si hubo algún error en la transmisión)
* arg : dd(día)
* arg: mm(mes)
* arg: yy(año)
* Descripción : Establece la fecha actual en el RTC, en el formato: dd, mm, yy.
*				El valor dd puede valer desde 0 hasta 31 (a excepción de los
*				meses que traen menos días).
*				El valor mm puede valer desde 1 hasta 12.
*				El valor yy puede valer desde 00 hasta 99.
*
* Notas : 		Recuerde que solamente se ingresan los 2 dígitos menos
* 				significativos del año, por lo cual debe tomar en cuenta
* 				esta limitación al trabajar con fechas cercanas a un cambio
* 				de siglo..
*****************************************************************************/
uint8_t RTC_Date_Set(uint8_t dd, uint8_t mm, uint8_t yy){

	uint8_t yy_d, yy_u, mm_d, mm_u, dd_d, dd_u;
	uint8_t salida = I2C_OK;

	/* Extraer los dígitos del día, mes y año */
	yy_u = yy % 10;
	yy_d = (yy / 10) % 10;

	mm_u = mm % 10;
	mm_d = (mm / 10) % 10;

	dd_u = dd % 10;
	dd_d = (dd / 10) % 10;

	/* Cargar dirección del registro de fecha en el buffer de transmisión */
	i2c_tx_buffer[0] = DATE_REG;
	/* Cargar los dígitos al buffer de transmisión */
	i2c_tx_buffer[1] = (dd_d << 4) | dd_u;
	i2c_tx_buffer[2] = (mm_d << 4) | mm_u;
	i2c_tx_buffer[3] = (yy_d << 4) | yy_u;

	/* Transmitir los 3 bytes al RTC */
	salida = TWI_Master_Write_Data(DS1307_SLAVE_ADDRESS, 4, i2c_tx_buffer);

	return salida;
}


/****************************************************************************
* Nombre de la función: RTC_Date_Read
* retorna : el estado del bus I2C/TWI (si hubo algún error en la transmisión)
* arg : Ninguno
* Descripción : Lee la fecha actual en el RTC, en el formato dd, mm, yy.
				Adicionalmente, determina el día de la semana correspondiente
				a la fecha actual (del 1 al 7).
*
* Notas : 		Los valores leídos son almacenados en el arreglo rtc_date_buffer.
* 				Cada elemento tiene su índice correspondiente:
*
* 				rtc_date_buffer[DAY] 	 --> Almacena el día
* 				rtc_date_buffer[MONTH] 	 --> Almacena el mes
* 				rtc_date_buffer[YEAR] 	 --> Almacena el año
* 				rtc_date_buffer[WEEKDAY] --> Almacena el día de la semana
*****************************************************************************/
uint8_t RTC_Date_Read(void){
	uint8_t yy_d, yy_u, mm_d, mm_u, dd_d, dd_u;
	uint8_t salida = I2C_OK;

	/* Cargar dirección del registro de segundos en el buffer de transmisión */
	i2c_tx_buffer[0] = DAY_REG;
	/* Escribir la dirección del primer registro a leer (SEGUNDOS) */
	salida = TWI_Master_Write_Data(DS1307_SLAVE_ADDRESS, 1, i2c_tx_buffer);
	/* Leer los registros */
	if(salida == I2C_OK){
		salida = TWI_Master_Read_Data(DS1307_SLAVE_ADDRESS, 4, i2c_rx_buffer);

		/* Cargar los valores de hora, minuto, segundo y meridiano */

		rtc_date_buffer[WEEKDAY] = i2c_rx_buffer[0];

		dd_u = i2c_rx_buffer[1] & 0x0F;
		dd_d = (i2c_rx_buffer[1] >> 4) & 0x03;
		rtc_date_buffer[DAY] = (10 * dd_d) + dd_u;

		mm_u = i2c_rx_buffer[2] & 0x0F;
		mm_d = (i2c_rx_buffer[2] >> 4) & 0x01;
		rtc_date_buffer[MONTH] = (10 * mm_d) + mm_u;

		yy_u = i2c_rx_buffer[3] & 0x0F;
		yy_d = (i2c_rx_buffer[3] >> 4) & 0x0F;
		rtc_date_buffer[YEAR] = (10 * yy_d) + yy_u;

	}

	return salida;

}








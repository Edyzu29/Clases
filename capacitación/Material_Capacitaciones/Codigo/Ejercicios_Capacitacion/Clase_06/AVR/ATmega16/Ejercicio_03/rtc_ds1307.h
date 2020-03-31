/*
 * rtc_ds1307.h
 *
 *  Created on: Jan 31, 2020
 *      Author: mtorres
 */

#ifndef RTC_DS1307_H_
#define RTC_DS1307_H_

/* Dirección del dispositivo */
#define DS1307_SLAVE_ADDRESS	0x68
//#define DS1307_SLAW_ADDRESS		0xD0
//#define DS1307_SLAR_ADDRESS		0xD1

/* Direcciones de los registros */
#define SECONDS_REG				0x00
#define MINUTES_REG				0x01
#define HOURS_REG				0x02
#define DAY_REG					0x03
#define DATE_REG				0x04
#define MONTH_REG				0x05
#define YEAR_REG				0x06
#define CONTROL_REG				0x07
#define RAM_BEGIN				0x08

/* Bits importantes */
#define BIT_CH					7

#define BIT_12_24				6
#define BIT_A_P					5

#define BIT_OUT					7
#define BIT_SQWE				4
#define BIT_RS1					1
#define BIT_RS0					0


/* Posiciones en el buffer de tiempo */
#define SECOND					0
#define MINUTE					1
#define HOUR					2
#define MERIDIEM				3

/* Posiciones en el buffer de fecha */
#define WEEKDAY					0
#define DAY						1
#define MONTH					2
#define YEAR					3

/* Definición de tipos */

typedef enum{
	AM = 0,
	PM
}meridiem_t;

/* Variables globales */
extern volatile uint8_t rtc_time_buffer[4];
extern volatile uint8_t rtc_date_buffer[4];


/* Declaración de funciones (Prototipos) */
uint8_t RTC_Time_Set_12h(uint8_t hh, uint8_t mm, uint8_t ss, meridiem_t mer);
uint8_t RTC_Time_Set_24h(uint8_t hh, uint8_t mm, uint8_t ss);
uint8_t RTC_Time_Read_12h(void);
uint8_t RTC_Time_Read_24h(void);
uint8_t RTC_Date_Set(uint8_t dd, uint8_t mm, uint8_t yy);
uint8_t RTC_Date_Read(void);


#endif /* RTC_DS1307_H_ */

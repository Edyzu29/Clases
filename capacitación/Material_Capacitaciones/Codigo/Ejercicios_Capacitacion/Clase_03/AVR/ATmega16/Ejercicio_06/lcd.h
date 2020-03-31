/* 
 * File:   lcd.h
 * Author: mtorres
 *
 * Created on October 19, 2019, 8:04 PM
 */

#ifndef LCD_H
#define	LCD_H

/* Inclusi�n de archivos*/
#include <avr/io.h>
#include <stdint.h>

/* Definici�n de pines */
/* OBSERVACION: El pin RW de la LCD debe ir conectado a GND */
#define DDR_LCD_DATA   	DDRA
#define PORT_LCD_DATA   PORTA
#define DB7             7
#define DB6             6
#define DB5             5
#define DB4             4
#define LCD_DATA_MASK   ((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4))

#define DDR_LCD_E      	DDRC
#define PORT_LCD_E      PORTC
#define LCD_E           6
#define DDR_LCD_RS     	DDRC
#define PORT_LCD_RS     PORTC
#define LCD_RS          7

/* Definici�n de las instrucciones principales de la pantalla LCD*/

/* clear display and return home */
#define LCD_CLEAR                0x01      /* DB0: clear display                  */
#define LCD_HOME              	 0x02      /* DB1: return to home position        */

/* set entry mode: display shift on/off, dec/inc cursor move direction */
#define LCD_ENTRY_DEC            0x04   /* display shift off, dec cursor move dir */
#define LCD_ENTRY_DEC_SHIFT      0x05   /* display shift on,  dec cursor move dir */
#define LCD_ENTRY_INC           0x06   /* display shift off, inc cursor move dir */
#define LCD_ENTRY_INC_SHIFT      0x07   /* display shift on,  inc cursor move dir */

/* display on/off, cursor on/off, blinking char at cursor position */
#define LCD_DISP_OFF             0x08   /* display off                            */
#define LCD_DISP_ON              0x0C   /* display on, cursor off                 */
#define LCD_DISP_ON_BLINK        0x0D   /* display on, cursor off, blink char     */
#define LCD_DISP_ON_CURSOR       0x0E   /* display on, cursor on                  */
#define LCD_DISP_ON_CURSOR_BLINK 0x0F   /* display on, cursor on, blink char      */

/* move cursor/shift display */
#define LCD_MOVE_CURSOR_LEFT     0x10   /* move cursor left  (decrement)          */
#define LCD_MOVE_CURSOR_RIGHT    0x14   /* move cursor right (increment)          */
#define LCD_MOVE_DISP_LEFT       0x18   /* shift display left                     */
#define LCD_MOVE_DISP_RIGHT      0x1C   /* shift display right                    */

/* function set: set interface data length and number of display lines */
#define LCD_FUNCTION_4BIT_1LINE  0x20   /* 4-bit interface, single line, 5x8 dots */
#define LCD_FUNCTION_4BIT_2LINES 0x28   /* 4-bit interface, dual line,   5x8 dots */
#define LCD_FUNCTION_8BIT_1LINE  0x30   /* 8-bit interface, single line, 5x8 dots */
#define LCD_FUNCTION_8BIT_2LINES 0x38   /* 8-bit interface, dual line,   5x8 dots */


#define LCD_LINES           2     /* number of visible lines of the display */
#define LCD_DISP_LENGTH    16     /* visibles characters per line of the display */
#define LCD_LINE_LENGTH    40     /* internal line length of the display    */
#define LCD_START_LINE1  0x00     /* DDRAM address of first char of line 1 */
#define LCD_START_LINE2  0x40     /* DDRAM address of first char of line 2 */
#define LCD_START_LINE3  0x14     /* DDRAM address of first char of line 3 */
#define LCD_START_LINE4  0x54     /* DDRAM address of first char of line 4 */

/* Definici�n de macros*/
#define LCD_E_HIGH()    PORT_LCD_E |= (1 << LCD_E)
#define LCD_E_LOW()     PORT_LCD_E &= ~(1 << LCD_E)
#define LCD_RS_HIGH()   PORT_LCD_RS |= (1 << LCD_RS)
#define LCD_RS_LOW()    PORT_LCD_RS &= ~(1 << LCD_RS)

/* Declaraci�n de funciones*/
void LCD_Init_4_Bits(void);
void LCD_Tx_Command(uint8_t command);
void LCD_Goto_XY(uint8_t pos_x, uint8_t pos_y);
void LCD_Put_Char(char byte);
void LCD_Put_Const_Char(const char byte); 
void LCD_Put_String(char str[]);
void LCD_Put_Const_String(const char *str);

#endif	/* LCD_H */


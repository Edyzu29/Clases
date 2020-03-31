
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "lcd.h"

/* Definici�n de funciones */

/****************************************************************************
* Nombre de la funci�n: LCD_Init_4_Bits
* retorna : Nada
* Descripci�n : Configura como salida los pines conectados a la pantalla
*               LCD (D7-D4, RS y E). Asimismo, configura la pantalla para
*               trabajar en el modo 4 bits. Esta funci�n debe llamarse antes
*               de empezar a trabajar con la pantalla. 
* Notas : Se asume que el pin R/W de la LCD se encuentra conectado a GND.
*****************************************************************************/
void LCD_Init_4_Bits(void){
    
    uint8_t conta; 
    
    /* Configurar como salida e inicializar en 0 los pines de datos */
    TRIS_LCD_DATA &= ~LCD_DATA_MASK;
    PORT_LCD_DATA &= ~LCD_DATA_MASK;
    /* Configurar como salida e inicializar en 0 los pines E y RS */
    TRIS_LCD_E &= ~(1 << LCD_E);
    PORT_LCD_E &= ~(1 << LCD_E);
    TRIS_LCD_RS &= ~(1 << LCD_RS);
    PORT_LCD_RS &= ~(1 << LCD_RS);
    
    /* Inicializaci�n mediante instrucciones */
    
    /* PASO 1 */
    /* Esperar m�s de 40 ms */
    __delay_ms(50);
    
    /* PASOS 2, 3, y 4 */
    /* Transmitir la instrucci�n 0011xxxx en modo 8 bits, tres veces consecutivas */
    for(conta = 0; conta < 3; conta++)
   {
        /* Hacer RS = 0 y EN = 0 */
        LCD_RS_LOW();
        LCD_E_LOW();
        /* Cargar los bits que se desea transmitir y esperar por lo menos 100ns*/
        PORT_LCD_DATA &= ~LCD_DATA_MASK;
        PORT_LCD_DATA |= (1 << DB5) | (1 << DB4);
        __delay_us(1);
        /* Hacer E = 1 y esperar un tiempo m�nimo de 300ns*/
        LCD_E_HIGH();
        __delay_us(1);
        /* Hacer E = 0 y esperar al menos 2ms*/
        LCD_E_LOW();
        __delay_ms(2);
        /* Tiempo de espera adicional en la etapa de inicializaci�n */
        __delay_ms(5);
    }

    /* PASO 5 */
    /* Transmitir la instrucci�n 0010xxxx en modo 8 bits */
        /* Hacer RS = 0 y EN = 0 */
        LCD_RS_LOW();
        LCD_E_LOW();
        /* Cargar los bits que se desea transmitir y esperar por lo menos 100ns*/
        PORT_LCD_DATA &= ~LCD_DATA_MASK;
        PORT_LCD_DATA |= (1 << DB5);
        __delay_us(1);
        /* Hacer E = 1 y esperar un tiempo m�nimo de 300ns*/
        LCD_E_HIGH();
        __delay_us(1);
        /* Hacer E = 0 y esperar al menos 2ms*/
        LCD_E_LOW();
        __delay_ms(2);
    
    /* NOTA: A partir de aqui la LCD se encuentra en modo 4 bits, y se pueden
     * emplear las funciones creadas */    
        
    /* PASO 6 */
    LCD_Tx_Command(LCD_FUNCTION_4BIT_2LINES);
    LCD_Tx_Command(LCD_DISP_OFF);
    LCD_Tx_Command(LCD_CLEAR);
    LCD_Tx_Command(LCD_ENTRY_INC);

    /* Encender la pantalla */
    LCD_Tx_Command(LCD_DISP_ON);
}



/****************************************************************************
* Nombre de la funci�n: LCD_Tx_Command
* retorna : Nada
* arg: command
* Descripci�n : Env�a una instrucci�n (command) a la pantalla LCD
*               Entre las instrucciones mas comunes tenemos: activar cursor,
*               desactivar cursor, limpiar pantalla y ubicar el cursor en 
*               una posici�n espec�fica.
* Notas : Se asume que el pin R/W de la LCD se encuentra conectado a GND.
*****************************************************************************/
void LCD_Tx_Command(uint8_t command){
    
    /* Variables que almacenan los bits del comando */
    uint8_t d7, d6, d5, d4, d3, d2, d1, d0;
    
    /* Extraer los bits del comando */
    d7 = (command >> 7) & 1;
    d6 = (command >> 6) & 1;
    d5 = (command >> 5) & 1;
    d4 = (command >> 4) & 1;
    d3 = (command >> 3) & 1;
    d2 = (command >> 2) & 1;
    d1 = (command >> 1) & 1;
    d0 = (command >> 0) & 1;
    
    /* PASO 1 */
    /* Hacer E = 0 y RS = 0 */
    LCD_E_LOW();
    LCD_RS_LOW();
            
    /* PASO 2 */
    /* Cargar los 4 bits de datos m�s significativos y esperar como m�nimo 100ns */
    
    /* Cargar bits de datos (ESTILO EST�NDAR) */
    PORT_LCD_DATA &= ~LCD_DATA_MASK;
    PORT_LCD_DATA |= (d7 << DB7) | (d6 << DB6) | (d5 << DB5) | (d4 << DB4);
    __delay_us(1);

    
    /* PASO 3 */
    /* Hacer E = 1 y esperar m�nimo 300ns*/
    LCD_E_HIGH();
    __delay_us(1);
    
    /* PASO 4 */ 
    /* Hacer E = 0*/
    /* Cargar los 4 bits de datos menos significativos y esperar como m�nimo 100ns */
    LCD_E_LOW();

    /* Cargar bits de datos (ESTILO EST�NDAR) */
    PORT_LCD_DATA &= ~LCD_DATA_MASK;
    PORT_LCD_DATA |= (d3 << DB7) | (d2 << DB6) | (d1 << DB5) | (d0 << DB4);
    __delay_us(1);
    
    
    /* PASO 5 */
    /* Hacer E = 1 y esperar por lo menos 300ns */
    LCD_E_HIGH();
    __delay_us(1);

    /* PASO 5 */
    /* Hacer E = 0 y esperar por lo menos 2ms */
    LCD_E_LOW();
    __delay_ms(2);
    
}


/****************************************************************************
* Nombre de la funci�n: LCD_Goto_XY
* retorna : Nada
* arg: pos_x, pos_y
* Descripci�n : Ubica el cursor en una posici�n espec�fica.
*               El cursor es ubicado en el caracter pos_x de la l�nea pos_y.
* Notas : Si alg�n par�metro se encuentra fuera del rango permitido, entonces
*         la funci�n asumir� el valor de 1 para dicho par�metro.
*****************************************************************************/
void LCD_Goto_XY(uint8_t pos_x, uint8_t pos_y){
    
    uint8_t comando, offset, col;
    
    /* Ubicar la direcci�n de la l�nea correspondiente */
    switch(pos_y){
        case 1:
            offset = LCD_START_LINE1;
            break;
        case 2:
            offset = LCD_START_LINE2;
            break;        
        case 3:
            offset = LCD_START_LINE3;
            break;
        case 4:
            offset = LCD_START_LINE4;
            break;
        default:
            offset = LCD_START_LINE1;
            break;
    }
    
    /* Ubicar la direcci�n de la columna correspondiente*/
    if((pos_x >= 1) && (pos_x <= LCD_DISP_LENGTH )){
        col = pos_x - 1;
    }else{
        col = 0;
    }
    
    comando = (offset + col) | 0b10000000;
    LCD_Tx_Command(comando);
    
}


/****************************************************************************
* Nombre de la funci�n: LCD_Put_Char
* retorna : Nada
* arg: byte
* Descripci�n : Imprime un caracter de la CGROM en la pantalla LCD
* Notas : Se recomienda que el caracter se encuentre almacenado en una
*         variable de tipo char.
*****************************************************************************/
void LCD_Put_Char(char byte){

    /* Variables que almacenan los bits del comando */
    uint8_t d7, d6, d5, d4, d3, d2, d1, d0;
    
    /* Extraer los bits del comando */
    d7 = (byte >> 7) & 1;
    d6 = (byte >> 6) & 1;
    d5 = (byte >> 5) & 1;
    d4 = (byte >> 4) & 1;
    d3 = (byte >> 3) & 1;
    d2 = (byte >> 2) & 1;
    d1 = (byte >> 1) & 1;
    d0 = (byte >> 0) & 1;
    
    /* PASO 1 */
    /* Hacer E = 0 y RS = 1 */
    LCD_E_LOW();
    LCD_RS_HIGH();
            
    /* PASO 2 */
    /* Cargar los 4 bits de datos m�s significativos y esperar como m�nimo 100ns */
    
    /* Cargar bits de datos (ESTILO EST�NDAR) */
    PORT_LCD_DATA &= ~LCD_DATA_MASK;
    PORT_LCD_DATA |= (d7 << DB7) | (d6 << DB6) | (d5 << DB5) | (d4 << DB4);
    __delay_us(1);
    
    ///* Cargar bits de datos (ESTILO MICROCHIP) */
    //PORTBbits.PORTB7 = d7;
    //PORTBbits.PORTB6 = d6;
    //PORTBbits.PORTB5 = d5;
    //PORTBbits.PORTB4 = d4;

    /* PASO 3 */
    /* Hacer E = 1 y esperar m�nimo 300ns*/
    LCD_E_HIGH();
    __delay_us(1);
    
    /* PASO 4 */ 
    /* Hacer E = 0*/
    /* Cargar los 4 bits de datos menos significativos y esperar como m�nimo 100ns */
    LCD_E_LOW();
    /* Cargar bits de datos (ESTILO EST�NDAR) */
    PORT_LCD_DATA &= ~LCD_DATA_MASK;
    PORT_LCD_DATA |= (d3 << DB7) | (d2 << DB6) | (d1 << DB5) | (d0 << DB4);
    __delay_us(1);
    

    
    /* PASO 5 */
    /* Hacer E = 1 y esperar por lo menos 300ns */
    LCD_E_HIGH();
    __delay_us(1);
    /* PASO 5 */
    /* Hacer E = 0 y esperar por lo menos 2ms */
    LCD_E_LOW();
    __delay_ms(2);
    
}

/****************************************************************************
* Nombre de la funci�n: LCD_Put_Const_Char
* retorna : Nada
* arg: byte
* Descripci�n : Env�a un caracter de la CGROM a la pantalla LCD
* Notas : El caracter debe ser un literal (constante) de tipo char.
*****************************************************************************/
void LCD_Put_Const_Char(const char byte){
    
    /* Variables que almacenan los bits del comando */
    uint8_t d7, d6, d5, d4, d3, d2, d1, d0;
    
    /* Extraer los bits del comando */
    d7 = (byte >> 7) & 1;
    d6 = (byte >> 6) & 1;
    d5 = (byte >> 5) & 1;
    d4 = (byte >> 4) & 1;
    d3 = (byte >> 3) & 1;
    d2 = (byte >> 2) & 1;
    d1 = (byte >> 1) & 1;
    d0 = (byte >> 0) & 1;
    
    /* PASO 1 */
    /* Hacer E = 0 y RS = 1 */
    LCD_E_LOW();
    LCD_RS_HIGH();
            
    /* PASO 2 */
    /* Cargar los 4 bits de datos m�s significativos y esperar como m�nimo 100ns */
    
    /* Cargar bits de datos (ESTILO EST�NDAR) */
    PORT_LCD_DATA &= ~LCD_DATA_MASK;
    PORT_LCD_DATA |= (d7 << DB7) | (d6 << DB6) | (d5 << DB5) | (d4 << DB4);
    __delay_us(1);
    
    ///* Cargar bits de datos (ESTILO MICROCHIP) */
    //PORTBbits.PORTB7 = d7;
    //PORTBbits.PORTB6 = d6;
    //PORTBbits.PORTB5 = d5;
    //PORTBbits.PORTB4 = d4;

    /* PASO 3 */
    /* Hacer E = 1 y esperar m�nimo 300ns*/
    LCD_E_HIGH();
    __delay_us(1);
    
    /* PASO 4 */ 
    /* Hacer E = 0*/
    /* Cargar los 4 bits de datos menos significativos y esperar como m�nimo 100ns */
    LCD_E_LOW();
    /* Cargar bits de datos (ESTILO EST�NDAR) */
    PORT_LCD_DATA &= ~LCD_DATA_MASK;
    PORT_LCD_DATA |= (d3 << DB7) | (d2 << DB6) | (d1 << DB5) | (d0 << DB4);
    __delay_us(1);
    
    ///* Cargar bits de datos (ESTILO MICROCHIP) */
    //PORTBbits.PORTB7 = d3;
    //PORTBbits.PORTB6 = d2;
    //PORTBbits.PORTB5 = d1;
    //PORTBbits.PORTB4 = d0;
    
    /* PASO 5 */
    /* Hacer E = 1 y esperar por lo menos 300ns */
    LCD_E_HIGH();
    __delay_us(1);
    /* PASO 5 */
    /* Hacer E = 0 y esperar por lo menos 2ms */
    LCD_E_LOW();
    __delay_ms(2);
    
}


/****************************************************************************
* Nombre de la funci�n: LCD_Put_String
* retorna : Nada
* arg: str[]
* Descripci�n : Imprime una cadena de caracteres en la pantalla LCD
* Notas : Se recomienda que la cadena se encuentre almacenada en un
*         arreglo de elementos de tipo char.
*****************************************************************************/
void LCD_Put_String(char str[]){
    
    uint8_t indice = 0;
    
    while((indice < LCD_DISP_LENGTH) && (str[indice] != '\0')){
        LCD_Put_Char(str[indice]);
        indice++;
    }  
}


/****************************************************************************
* Nombre de la funci�n: LCD_Put_Const_String
* retorna : Nada
* arg: str[]
* Descripci�n : Imprime una cadena de caracteres en la pantalla LCD
* Notas : La cadena debe ser un literal (constante) de tipo string.
*****************************************************************************/
void LCD_Put_Const_String(const char *str){
    
    uint8_t indice = 0;
    
    while((indice < LCD_DISP_LENGTH) && (*str != '\0')){
        LCD_Put_Const_Char(*str);
        str++;
        indice++;
    }  
}


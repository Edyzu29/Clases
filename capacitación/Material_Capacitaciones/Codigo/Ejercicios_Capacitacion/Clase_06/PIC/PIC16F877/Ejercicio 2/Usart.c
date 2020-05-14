#include <xc.h>
#include "Config.h"
#include <stdint.h>
#include "Usart.h"

#define _XTAL_FREQ 16000000

/****************************************************************************
 * Nombre de la funci�n: Confi_Blue
 * retorna : nada
 * ingresa: nada
 * Descripci�n : Configura los registros que intervienen en la comunicacion
		Usart
 *****************************************************************************/
void Confi_Blue(void) {
    Tris_Rx = 1; //Configurar el pin Rx como entrada
    Tris_Tx = 0; //Configurar el pin Tx como salida
    SPBRG = 25; //Baudios
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    RCSTAbits.SPEN = 1; //Habilitar Tx y Rx
    TXSTAbits.SYNC = 0; //Asincrono
    //    BAUDCONbits.BRG16=0;                       //8 bits de Baudiage
    TXSTAbits.BRGH = 1; //High Speed
    /*64 (8bits) ---> 000
     *8  (8bits) ---> 001
     *8  (16bits)---> 010
     *4  (16bits)---> 011*/
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //Transmisi�n
    TXSTAbits.TX9 = 0; //8 bits
    TXSTAbits.TXEN = 1; //Activamos transmisi�n
    //Recepci�n
    RCSTAbits.RC9 = 0; //8 bits
    RCSTAbits.CREN = 1; //Activamos recepci�n
}

/****************************************************************************
 * Nombre de la funci�n: Tx_str
 * retorna : nada
 * ingresa: un dato a enviar tipo "Char"
 * Descripci�n : Envia un caracter
 *****************************************************************************/
void Tx_str(char data) {
    while (TXSTAbits.TRMT == 0);
    TXREG = data;
} //Esta funci�n envia datos cadena

/****************************************************************************
 * Nombre de la funci�n: Rx
 * retorna : Un dato recibido de tipo "Char"
 * ingresa: nada
 * Descripci�n : Recibe un caracter por el puerto USART
 *****************************************************************************/ 
char Rx(void) {
    return RCREG;
} //Esta funci�n recibe datos

/****************************************************************************
 * Nombre de la funci�n: UART0_Tx_String
 * retorna : nada
 * ingresa: Una cadena de texto a enviar tipo "Char"
 * Descripci�n : La cadena de texto se secciona para enviar cada elemento 
		como un caracter
 *****************************************************************************/             
void UART0_Tx_String(char *str) {

    /* Mientras no se alcance el final de la cadena */
    while (*str != '\0') {
        /* Transmitir el caracter correspondiente */
        Tx_str(*str);
        /* Incrementar el valor del puntero (apuntar al siguiente caracter
        en el arreglo) */
        str++;
    }
}

/****************************************************************************
 * Nombre de la funci�n: UART0_Tx_Integer
 * retorna : nada
 * ingresa: Un dato a enviar tipo "Int32"
 * Descripci�n : El valor entero ingresado se secciona para enviar cada elemento 
		como un caracter
 *****************************************************************************/ 
void UART0_Tx_Integer(int32_t num) {

    char signo_y_digitos[12];
    uint8_t signo = 0;
    int32_t digito;
    int8_t indice = 11;

    /* Determinar el signo del n�mero */
    if (num < 0) {
        signo = 1;
        num = -num;
    }

    /* Indicar el fin de la cadena de caracteres */
    signo_y_digitos[indice--] = '\0';

    /* Extraer los d�gitos uno a uno, empezando por las unidades */
    do {
        digito = (num % 10) + '0';
        signo_y_digitos[indice--] = (char) digito;
        num /= 10;
    } while (num);

    /* Agregar el signo (de ser necesario) */
    if (signo) {
        signo_y_digitos[indice] = '-';
    } else {
        indice++;
    }
    __delay_ms(100);
    /* Transmitir el n�mero */
    UART0_Tx_String(&signo_y_digitos[indice]);
}

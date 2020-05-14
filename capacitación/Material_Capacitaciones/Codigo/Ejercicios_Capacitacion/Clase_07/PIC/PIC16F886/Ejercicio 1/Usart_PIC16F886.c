/* Inclusiones*/
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "Usart_PIC16F886.h"
#include "Interrupciones_PIC16F886.h"

/****************************************************************************
 * Nombre de la funci�n: Config_Usart
 * retorna : nada
 * ingresa: nada
 * Descripci�n : Configura los registros que intervienen en la comunicacion
		Usart
 *****************************************************************************/
void Config_Usart(void){
                                            /*Configuracion de comunicaci�n */
        TXSTAbits.SYNC=0;
        /*1 = Synchronous mode
          0 = Asynchronous mode */
        
        TXSTAbits.BRGH=0;
        /*1 = High speed
          0 = Low speed*/
        
        RCSTAbits.SPEN=1;
        /*1 = Serial port enabled (Activa los pines TX y RX como puerto serial)
           0 = Serial port disabled */
        
        SPBRG= VALOR_BAUDIOS;       
        //En este registro se generan los baudios necesario
        
                                            /*Configuracion de la transmision */
        TXSTAbits.TX9=0;
        /*1 = Selects 9-bit transmission
           0 = Selects 8-bit transmission*/
        TXSTAbits.TXEN=1;
        /*1 = Transmit enabled
           0 = Transmit disabled*/
        
                                           /*Configuracion de la recepci�n*/
        RCSTAbits.RX9=0;
        /*1 = Selects 9-bit transmission
           0 = Selects 8-bit transmission*/
         RCSTAbits.CREN=1;
        /*1 = Enables continuous receive
           0 = Disables continuous receive*/
}

/****************************************************************************
 * Nombre de la funci�n: Tx_char
 * retorna : nada
 * ingresa: un dato a enviar tipo "Char"
 * Descripci�n : Envia un caracter
 *****************************************************************************/
void Tx_char(char data){
    while(TXSTAbits.TRMT == 0);
    TXREG = data;                               
} 

/****************************************************************************
 * Nombre de la funci�n: UART0_Tx_String
 * retorna : nada
 * ingresa: Una cadena de texto a enviar tipo "Char"
 * Descripci�n : La cadena de texto se secciona para enviar cada elemento 
		como un caracter
 *****************************************************************************/             
void UART0_Tx_String(char *str){

	/* Mientras no se alcance el final de la cadena */
	while(*str != '\0'){
		/* Transmitir el caracter correspondiente */
		Tx_char(*str);
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
void UART0_Tx_Integer(int32_t num){

	char signo_y_digitos[12];
	uint8_t signo = 0;
	int32_t digito;
	int8_t indice = 11;

	/* Determinar el signo del n�mero */
	if(num < 0){
		signo = 1;
		num = -num;
	}

	/* Indicar el fin de la cadena de caracteres */
	signo_y_digitos[indice--] = '\0';

	/* Extraer los d�gitos uno a uno, empezando por las unidades */
	do{
		digito = (num % 10) + '0';
		signo_y_digitos[indice--] = (char)digito;
		num /= 10;
	}while(num);

	/* Agregar el signo (de ser necesario) */
	if(signo){
		signo_y_digitos[indice] = '-';
	}else{
		indice++;
	}
    __delay_ms(100);
	/* Transmitir el n�mero */
	UART0_Tx_String(&signo_y_digitos[indice]);
}

/****************************************************************************
 * Nombre de la funci�n: Rx
 * retorna : Un dato recibido de tipo "Char"
 * ingresa: nada
 * Descripci�n : Recibe un caracter por el puerto USART
 *****************************************************************************/ 
char Rx(void){
    return RCREG; 
}  

/****************************************************************************
 * Nombre de la funci�n: UART0_Rx_String
 * retorna : Una cadena de texto recibido de tipo "Char"
 * ingresa: nada
 * Descripci�n : Realiza una iteraci�n para comprobar si se est� 
		recibiendo una cadena de texto por el puerto USART
 *****************************************************************************/                   
void UART0_Rx_String(void) {
    uint8_t index = 0;
    RX_FLAG = 0;

    while (rx_buffer[index - 1] != '\r') { //Mientras no se reciba el enter, no se saldra del bucle
        if (RX_FLAG) {                                //La bandera indicara cada nuevo caracter recibido 
            RX_FLAG = 0;
            rx_buffer[index] = Rx();
            index++;
        }
    }
}

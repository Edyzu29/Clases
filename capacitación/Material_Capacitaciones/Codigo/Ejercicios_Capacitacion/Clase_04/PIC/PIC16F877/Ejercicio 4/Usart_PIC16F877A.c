/* Inclusiones*/
#include "system_config.h"
#include <xc.h>
#include <stdint.h>
#include "Usart_PIC16F877A.h"

/*Funciones*/
void Config_Usart(void){
                                            /*Configuracion de comunicaci?n */
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
        
                                           /*Configuracion de la recepci?n*/
        RCSTAbits.RX9=0;
        /*1 = Selects 9-bit transmission
           0 = Selects 8-bit transmission*/
         RCSTAbits.CREN=1;
        /*1 = Enables continuous receive
           0 = Disables continuous receive*/
}

void Tx_char(char data){
    while(TXSTAbits.TRMT == 0);
    TXREG = data;                               
}               

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

void UART0_Tx_Integer(int32_t num){

	char signo_y_digitos[12];
	uint8_t signo = 0;
	int32_t digito;
	int8_t indice = 11;

	/* Determinar el signo del n?mero */
	if(num < 0){
		signo = 1;
		num = -num;
	}

	/* Indicar el fin de la cadena de caracteres */
	signo_y_digitos[indice--] = '\0';

	/* Extraer los d?gitos uno a uno, empezando por las unidades */
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
	/* Transmitir el n?mero */
	UART0_Tx_String(&signo_y_digitos[indice]);
}

char Rx(void){
    return RCREG; 
}                    
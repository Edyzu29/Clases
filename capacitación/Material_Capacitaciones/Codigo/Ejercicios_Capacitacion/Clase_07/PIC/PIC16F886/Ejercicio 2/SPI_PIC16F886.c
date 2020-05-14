#include <xc.h>
#include "Interrupciones_PIC16F886.h"
#include "SPI_PIC16F886.h"

/****************************************************************************
 * Nombre de la funci�n: Config_SPI
 * retorna : Nada
 * ingresa: Nada
 * Descripci�n : Permite iniciar con la configuraci�n del SPI
 * Nota: Solo llamarse una vez dentro del codigo
 *****************************************************************************/
void Config_SPI(void) {

    SSPCONbits.SSPEN = 1; /*Habilitamos los pines del SPI*/

    SSPCONbits.SSPM = 0x0000;
    /*Este grupo de 4 bits sirve para establecer la frecuencia del reloj 
        0101 = SPI Slave mode, clock = SCK pin. SS pin control disabled. 
        0100 = SPI Slave mode, clock = SCK pin. SS pin control enabled.
        0011 = SPI Master mode, clock = TMR2 output/2
        0010 = SPI Master mode, clock = FOSC/64
        0001 = SPI Master mode, clock = FOSC/16
        0000 = SPI Master mode, clock = FOSC/4     
     */

    SSPCONbits.CKP = 0; /*El estado inactivo es el 0*/
    /*  este bit selecciona la polaridad del reloj 
     1 = El estado inactivo para el reloj es un nivel alto
     0 = El estado inactivo para el reloj es un nivel bajo
     */

    SSPSTATbits.CKE = 0; /*la transmisi�n ocurre en la transmisi�n del estado de reloj inactivo al activo*/
    /*Bit de selecci�n de reloj SPI
       1 = La transmisi�n ocurre en la transici�n del estado de reloj activo al inactivo
      0 = La transmisi�n ocurre en la transici�n del estado inactivo al estado de reloj activo
     */

    SSPCONbits.WCOL = 0; /*No se colisionan los datos*/

    SSPCONbits.SSPOV = 0; /*No hay desborde*/

    SSPSTATbits.SMP = 0;
    /* Este bit sirve para indicar cuando leer datos recividos
     1 = Datos de entrada muestreados al finalizar el tiempo de salida de los datos
    0 = Datos de entrada muestreados a la mitad del tiempo de salida de los datos
     */
}

/****************************************************************************
 * Nombre de la funci�n: Escribir_Spi
 * retorna : Nada
 * ingresa: Dato a enviar "caracter"
 * Descripci�n : Permite enviar un dato o varios por el SPI
 * Nota: Puede enviar caracteres de 8 bits
 *****************************************************************************/
void Escribir_Spi(char dato) {
    SPI_FLAG = 0; /*Limpiamos la bandera de transmisi�n/Recepci�n de datos por el terminal serial*/
    SSPBUF = dato; /*Enviamos el valor de "dato" por el registro SSPBUF para enviarlo por el SPI*/
    while ( !SPI_FLAG ); /*Espere a que la bandera de interrupci�n se eleve para indicar que la transmisi�n est� completa*/
}

/****************************************************************************
 * Nombre de la funci�n: Leer_Spi
 * retorna : dato le�do por el SPI
 * ingresa: nada
 * Descripci�n : Permite leer un dato o varios por el SPI
 * Nota: Puede leer variables de 8 bits
 *****************************************************************************/
char Leer_Spi(void) {
    SPI_FLAG = 0; /*Se limpia la interrupcion del puerto serial SSP*/
    Escribir_Spi(0xFF); /*Enviamos un valor cualquiera "muchas veces es necesario para algunos dispositivos en SPI"*/
    while ( !SPI_FLAG ); /*Esperamos a que sucesa la interrupcion*/
    return (SSPBUF); /*Devuelve el valor le�do*/
}


//
// Created by fabian on 16/09/19.
//

#include <stdio.h>
#include "Socket_Cliente.h"
#include "Socket.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int size(char *ptr)
{
    //variable used to access the subsequent array elements.
    int offset = 0;
    //variable that counts the number of elements in your array
    int count = 0;

    //While loop that tests whether the end of the array has been reached
    while (*(ptr + offset) != '\0')
    {
        //increment the count variable
        ++count;
        //advance to the next element of the array
        ++offset;
    }
    //return the size of the array
    return count;
}

main ()
{
    /*
    * Descriptor del socket y buffer para datos
    */
    int Socket_Con_Servidor;
    char Cadena[1000];

    /*
    * Se abre la conexion con el servidor, pasando el nombre del ordenador
    * y el servicio solicitado.
    * "localhost" corresponde al nombre del mismo ordenador en el que
    * estamos corriendo. Esta dado de alta en /etc/hosts
    * "cpp_java" es un servicio dado de alta en /etc/services
    */

    Socket_Con_Servidor = Abre_Conexion_Inet ("localhost", "dkserver");
    if (Socket_Con_Servidor == 1)
    {
        printf ("No puedo establecer conexion con el servidor\n");
        exit (-1);
    }

    /*
    * Se prepara una cadena con 5 caracteres y se envia, 4 letras mas
    * el \0 que indica fin de cadena en C
    */

    //strcpy (Cadena, "Hola");
    //Escribe_Socket (Socket_Con_Servidor, Cadena, 5);

    /*
    * Se lee la informacion enviada por el servidor, que se supone es
    * una cadena de 6 caracteres.
    */

    int Longitud_Cadena;
    int Aux;

    while (Cadena!="Null"){

        printf("Leyendo...");

        //PARA RECIBIR
        Lee_Socket (Socket_Con_Servidor, (char *)&Aux, sizeof(int)); /* La función nos devuelve en Aux el entero leido en formato red */
        Longitud_Cadena = ntohl (Aux); /* Guardamos el entero en formato propio en Longitud_Cadena */
        Lee_Socket (Socket_Con_Servidor, Cadena, Longitud_Cadena);
        //HASTA AQUI
        /*
        * Se escribe en pantalla la informacion recibida del servidor
        */
        printf ("Soy cliente, He recibido : %s\n", Cadena);


        //PARA ENVIAR
        memset(Cadena, 0, sizeof Cadena); //LIMPIA BUFFER ANTES DE UTILIZARLO
        printf("Que desea contestar: "); //ESTO en caso que el usuario desee digitar un valor para enviar
        gets(Cadena); //cadena es el buffer, o el "mensaje a enviar"
        Longitud_Cadena = size(Cadena)+1;
        Aux = htonl (Longitud_Cadena); /* Se mete en Aux el entero en formato red */
/* Se envía Aux, que ya tiene los bytes en el orden de red */
        Escribe_Socket (Socket_Con_Servidor, (char *)&Aux, sizeof(Longitud_Cadena));
        Escribe_Socket (Socket_Con_Servidor, Cadena, Longitud_Cadena);
        //HASTA AQUI
    }

    /*
    * Se cierra el socket con el servidor
    */
    close (Socket_Con_Servidor);
}
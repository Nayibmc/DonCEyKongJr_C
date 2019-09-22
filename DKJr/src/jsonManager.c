#include "jsonManager.h"

/*
 * jsonManager.c
 *
 *  Created on: Sep 18, 2019
 *      Author: ruben
 */


/**
 * Envia el JSON deseado al servidor y espera su respuesta.
 */
char* sendJSON(json_object *jObj){

	//Test Print
	printf("\n\nSe enviara el JSON necesario al servidor.\n\n");

	char hols[] = "VIENEJSON";

	//Concierte el JSON a un char*
	const char* jsonToServer;
	jsonToServer = json_object_to_json_string(jObj);

	//Imprime el JSON entrante
	printf("%s",jsonToServer);

	//Descriptor del socket y buffer para datos
	int Socket_Con_Servidor;
	char Cadena[9999];

	/*
	 * Se abre la conexion con el servidor, pasando el nombre del ordenador
	 * y el servicio solicitado.
	 * "localhost" corresponde al nombre del mismo ordenador en el que
	 * estamos corriendo. Esta dado de alta en /etc/hosts
	 * "cpp_java" es un ser
			vicio dado de alta en /etc/services
	 */


	Socket_Con_Servidor = Abre_Conexion_Inet ("localhost", "dkserver");
	if (Socket_Con_Servidor == 1)
		{
		    printf ("No puedo establecer conexion con el servidor\n");
		    exit (-1);
		}

	//Variables de manejo de Cadena
	int Longitud_Cadena;
	int Aux;


	//PARA ENVIAR
	memset(Cadena, 0, sizeof(Cadena)); //LIMPIA BUFFER ANTES DE UTILIZARLO
	//Cadena = jsonToServer;//Cadena es el buffer, o el "mensaje a enviar"

	strcpy(Cadena,jsonToServer);

	//strcpy(Cadena,hols);


	Longitud_Cadena = size(Cadena)+1;
	Aux = htonl (Longitud_Cadena); /* Se mete en Aux el entero en formato red */
	/* Se envía Aux, que ya tiene los bytes en el orden de red */
	Escribe_Socket (Socket_Con_Servidor, (char *)&Aux, sizeof(Longitud_Cadena));
	Escribe_Socket (Socket_Con_Servidor, Cadena, Longitud_Cadena);
	//HASTA AQUI


	//memset(Cadena, 0, sizeof(Cadena)); //LIMPIA BUFFER ANTES DE UTILIZARLO
	Longitud_Cadena = size(Cadena)+1;
	Aux = htonl (Longitud_Cadena); // Se mete en Aux el entero en formato red
	// Se envía Aux, que ya tiene los bytes en el orden de red
	Escribe_Socket (Socket_Con_Servidor, (char *)&Aux, sizeof(Longitud_Cadena));
	Escribe_Socket (Socket_Con_Servidor, Cadena, Longitud_Cadena);
	//HASTA AQUI

	//PARA RECIBIR
	Lee_Socket (Socket_Con_Servidor, (char *)&Aux, sizeof(int)); /* La función nos devuelve en Aux el entero leido en formato red */
	Longitud_Cadena = ntohl (Aux); /* Guardamos el entero en formato propio en Longitud_Cadena */
	Lee_Socket (Socket_Con_Servidor, Cadena, Longitud_Cadena);
	//HASTA AQUI


	/*
	 * Se cierra el socket con el servidor
	 */
	close (Socket_Con_Servidor);

	//Manda a verificar el JSON que ha llegado
	char* aux = Cadena;
	char* checked = checkJSON(aux);

	return checked;


}


/**
 * Verifica el JSON entrante del Servidor.
 */
char* checkJSON(char* jsonRecieved) {

	char* sendThis = "Error";

	printf("\n\nSe recibio del servidor: %s\n\n", jsonRecieved);

	//Toma el String proveniente del Servidor y se convierte a un json
	json_object* parsedJson = json_tokener_parse(jsonRecieved);

	/**
	 * "PLAY"
	 *  Dirá al jugador si puede jugar o no.
	 */

	//json_object para tomar lo que viene junto al Key
	struct json_object* dataFromJSONPlay;
	dataFromJSONPlay = json_object_object_get(parsedJson, "PLAY");
	//Obtiene el dato en String
	char* dataFromJSONPlayString = json_object_get_string(dataFromJSONPlay);
	//Verifica si este existe para retornar su contenido
	if (dataFromJSONPlayString != NULL) {
		return dataFromJSONPlayString;
	}

	/**
	 * "OBSERVE"
	 *  Dira al jugador si puede obervar un juego o no.
	 */

	//json_object para tomar lo que viene junto al Key
	struct json_object* dataFromJSONObserve;
	dataFromJSONObserve = json_object_object_get(parsedJson, "OBSERVE");
	//Obtiene el dato en String
	char* dataFromJSONObserveString = json_object_get_string(dataFromJSONObserve);
	//Verifica si este existe para retornar su contenido
	if (dataFromJSONObserveString != NULL) {
		return dataFromJSONObserveString;
	}

	/**
	 * "UPDATE"
	 *  Trae un array de las 5 listas necesarias para graficar
	 */

	//json_object para tomar lo que viene junto al Key
	struct json_object* dataFromJSONUpdate;
	dataFromJSONUpdate = json_object_object_get(parsedJson, "UPDATE");
	//Obtiene el dato en String
	char* dataFromJSONUpdateString = json_object_get_string(dataFromJSONUpdate);
	//Verifica si este existe para retornar su contenido
	if (dataFromJSONUpdateString != NULL) {
		return dataFromJSONUpdateString;
	}


	return sendThis;

}


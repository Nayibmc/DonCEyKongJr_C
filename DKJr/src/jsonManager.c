#include "jsonManager.h"
//#include <json-c/json.h>

/*
 * jsonManager.c
 *
 *  Created on: Sep 18, 2019
 *      Author: ruben
 */



/**************************************LOGICA**************************************/

/**************************************LOGICA**************************************/



/*******************************MANEJO_DE_RESPUESTAS*******************************/


/**
 * Maneja el contenido del JSON con el Key: "PLAY"
 */
void managePlay(json_object* jObj) {

	char* gameState = json_object_to_json_string(jObj);

	char gState;


	strcpy(gState, gameState);

		if (gState == "0") {
			//No inicia el juego
			printf("No se puede jugar aun.");
			//return gState;
		}
		else {
			//gameState es el codigo del juego
			//EMPEZAR EL JUEGO
			printf("Ya se puede jugar.");
			//return gState;
		}

}


/**
 * Maneja el contenido del JSON con el Key: "GAME"
 */
void manageGame(json_object* jObj){


}

/**
 * Maneja el contenido del JSON con el Key: "DKJR"
 */
void manageDKJr(json_object* jObj){

}

/**
 * Maneja el contenido del JSON con el Key: "CROCODILES"
 */
void manageCrocodiles(json_object* jObj){

}

/**
 * Maneja el contenido del JSON con el Key: "FRUITS"
 */
void manageFruits(json_object* jObj){

}

/**
 * Maneja el contenido del JSON con el Key: "FRUITPOINTS"
 */
void manageFruitPoints(json_object* jObj){

}

/*******************************MANEJO_DE_RESPUESTAS*******************************/




/*************************************SENDJSON*************************************/

/**
 * Envia el JSON deseado al servidor y espera su respuesta.
 */
char* sendJSON(json_object *jObj){

	//Test Print
	printf("Se enviara el JSON necesario al servidor.\n\n");

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
			Aux = htonl (Longitud_Cadena); /* Se mete en Aux el entero en formato red */
			/* Se envía Aux, que ya tiene los bytes en el orden de red */
			Escribe_Socket (Socket_Con_Servidor, (char *)&Aux, sizeof(Longitud_Cadena));
			Escribe_Socket (Socket_Con_Servidor, Cadena, Longitud_Cadena);
			//HASTA AQUI

			//printf("Leyendo...");

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
 * Verifica que ingresa en el JSON y maneja su respuesta
 */
char* checkJSON(char* jsonRecieved) {

	char * sendThis = "Error";


	printf("\n\n\nSe recibio del servidor: %s\n\n\n", jsonRecieved);

	//Toma el String proveniente del Servidor
	//Y es convertido a un JSON
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


	/*

		//Key: "GAME"
		//JSONArray que contiene: Estado del Juego, Puntaje Total, Nivel Actual y Vidas Restantes
		struct json_object *dataFromJSONGame;
		json_object_object_get_ex(parsedJsonFromServer, "GAME", &dataFromJSONGame);
		if (json_object_get_string(dataFromJSONGame) != NULL) {
			manageGame(dataFromJSONGame);
		}


		//Key: "DKJR"
		//JSONArray que contiene: Estado de DKJr, su Posicion en x, y su Posicion en y
		struct json_object *dataFromJSONDKJr;
		json_object_object_get_ex(parsedJsonFromServer, "DKJR", &dataFromJSONDKJr);
		if (json_object_get_string(dataFromJSONDKJr) != NULL) {
			manageDKJr(dataFromJSONDKJr);
		}


		//Key: "CROCODILES"
		//JSONArray de JSONArrays que contienen: Color del Cocodrilo, su Posicion en x, y su Posicion en y
		struct json_object *dataFromJSONCrocodiles;
		json_object_object_get_ex(parsedJsonFromServer, "CROCODILES", &dataFromJSONCrocodiles);
		if (json_object_get_string(dataFromJSONCrocodiles) != NULL) {
			manageCrocodiles(dataFromJSONCrocodiles);
		}


		//Key: "FRUITS"
		//JSONArray de JSONArrays que contienen: Tipo de Fruta, su Posicion en x, y su Posicion en y
		struct json_object *dataFromJSONFruits;
		json_object_object_get_ex(parsedJsonFromServer, "FRUITS", &dataFromJSONFruits);
		if (json_object_get_string(dataFromJSONFruits) != NULL) {
			manageFruits(dataFromJSONFruits);
		}

		//Key: "FRUITPOINTS"
		//JSONArray de JSONArrays que contienen: Puntaje, su Posicion en x, y su Posicion en y
		struct json_object *dataFromJSONFruitPoints;
		json_object_object_get_ex(parsedJsonFromServer, "FRUITPOINTS", &dataFromJSONFruitPoints);
		if (json_object_get_string(dataFromJSONFruitPoints) != NULL) {
			manageFruitPoints(dataFromJSONFruitPoints);
		}*/

	return sendThis;

}

/*************************************SENDJSON*************************************/


#include "jsonManager.h"
//#include <json-c/json.h>

/*
 * jsonManager.c
 *
 *  Created on: Sep 18, 2019
 *      Author: ruben
 */



/**************************************LOGICA**************************************/

/**
 * Al presionar el botón de Iniciar:
 * Envia un request al servidor para ver si inicia el juego.
 */
void startGame(){

	printf("\nSe desea iniciar un Game.\n\n");

	///Se crea un objeto JSON
	json_object *jObj = json_object_new_object();

	///Variables para agregar como Key y Data
	char* jsonKEY = "PLAY";
	char* jsonData = "TestsRequests"; //Este dato no es necesario en el Servidor

	///Se agrega la informacion en el JSON
	//Se crea un jString con el dato por ingresar en JSON
	json_object *jString = json_object_new_string(jsonData);
	//Se ingresa el jString al JSON con su propio key ("jsonKEY")
	json_object_object_add(jObj,jsonKEY, jString);

	///Se envia el JSON a través de la función sendJSON
	sendJSON(jObj);

	//return json;

}

/**
 * Al presionar el botón de Observar:
 * Envia un request al servidor con el código de la partida
 * que se desea iniciar a observar.
 * @param code: codigo del juego al que se desea unir
 */
void observeGame(char* code){

	printf("\n\Se desea observar un Game.\n\n");

	///Se crea un objeto JSON
	json_object *jObj = json_object_new_object();

	///Variables para agregar como Key y Data
	char* jsonKEY = "OBSERVE";
	char* jsonData = code; //Este dato es el codigo del juego al que se desea unir

	///Se agrega la informacion en el JSON
	//Se crea un jString con el dato por ingresar en JSON
	json_object *jString = json_object_new_string(jsonData);
	//Se ingresa el jString al JSON con su propio key ("jsonKEY")
	json_object_object_add(jObj,jsonKEY, jString);

	///Se envia el JSON a través de la función sendJSON
	sendJSON(jObj);

	//return json

}


/**
 * Recibe las posiciones actuales de DKJr y las envia al servidor
 * para obtener las colisiones y que este devuelva todo por graficar
 * de vuelta en el cliente.
 */
void updateGame(char* code, int keyInput[]) {

	///Se crea un objeto JSON
	struct json_object *jObj = json_object_new_object();

	//Se guardan por separado los inputs y se convierten a un char
	char* up;
	up =(char*)calloc(255, sizeof(char));
	char* right;
	right = (char*)calloc(255, sizeof(char));
	char* down;
	down = (char*)calloc(255, sizeof(char));
	char* left;
	left = (char*)calloc(255, sizeof(char));

	sprintf(up, "%d", keyInput[0]);
	sprintf(right, "%d", keyInput[1]);
	sprintf(down, "%d", keyInput[2]);
	sprintf(left, "%d", keyInput[3]);

	//Se crea un JSONArray que contendra los inputs
	struct json_object* inputArray = json_object_new_array();

	//Inputs convertidos en JStrings y dentro de un JSON Array
	json_object* jStringUp = json_object_new_string(up);
	json_object_array_add(inputArray, jStringUp);
	json_object* jStringRight = json_object_new_string(right);
	json_object_array_add(inputArray, jStringRight);
	json_object* jStringDown = json_object_new_string(down);
	json_object_array_add(inputArray, jStringDown);
	json_object* jStringLeft = json_object_new_string(left);
	json_object_array_add(inputArray, jStringLeft);

	//Key para el JSONArray
	char* jsonKeyInput = "INPUT";

	//Se agrega el JSONArray al JSON por enviar
	json_object_object_add(jObj,jsonKeyInput, inputArray);

	///Variables para agregar como Key y Data
	char* jsonKeyCode = "CODE";
	char* jsonDataCode = code;  //Codigo del juego que se modificara

	///Se agrega la informacion del codigo en el JSON
	json_object *jstringCode = json_object_new_string(jsonDataCode);
	json_object_object_add(jObj,jsonKeyCode, jstringCode);

	///Se envia el JSON a través de la función sendJSON
	sendJSON(jObj);

}

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
void sendJSON(json_object *jObj){

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

			//Manda a verificar el JSON que ha llegado

			char* aux = Cadena;
			checkJSON(aux);

			/*
			 * Se escribe en pantalla la informacion recibida del servidor
			 */
			//printf ("Soy cliente, He recibido : %s\n", Cadena);



	/*
	 * Se cierra el socket con el servidor
	 */
	close (Socket_Con_Servidor);




}


void checkJSON(char* jsonRecieved) {

	//Torna el Objeto JSON a un char[] para ser enviado como tal
	//al Servidor.
	//char* jsonPtr;
	//strcpy(jsonRecieved, jsonPtr);


	printf("\n\n\nSe recibio del servidor: %s\n\n\n", jsonRecieved);






	/*

	json_object* parsedJson = json_tokener_parse(jsonRecieved);

	//Key: "PLAY"
		//Dice si se puede iniciar el juego
		struct json_object* dataFromJSONPlay;
		dataFromJSONPlay = json_object_object_get(parsedJson, "PLAY");
		char* dataFromJSONPlayString = json_object_get_string(dataFromJSONPlay);
		if (dataFromJSONPlayString != NULL) {
			printf("\nrecvBuff(\"PLAY\"): ");
			printf(dataFromJSONPlayString);
			printf("\n");

			return dataFromJSONPlayString;

		}



		//Key: "OBSERVE"
		//Dice si se puede iniciar el juego
		struct json_object *dataFromJSONObserve;
		json_object_object_get_ex(parsedJsonFromServer, "OBSERVE", &dataFromJSONObserve);
		if (json_object_get_string(dataFromJSONObserve) != NULL) {
			//estadoDelJuego = update();
			printf("\nrecvBuff(\"OBSERVE\"): ");
			printf(json_object_to_json_string(dataFromJSONObserve));
			printf("\n");


		}

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


}

/*************************************SENDJSON*************************************/


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

	///Objeto JSON por enviar
	json_object *jObj = json_object_new_object();

	///Variables para agregar como Key y Data
	char* jsonKEY = "PLAY";
	char* jsonData = "Request"; //Este dato no es necesario en el Servidor

	///Se agrega la informacion en el JSON
	json_object *jstring = json_object_new_string(jsonData);
	json_object_object_add(jObj,jsonKEY, jstring);

	///Se envia el JSON a través de la función sendJSON
	sendJSON(jObj);

}

/**
 * Al presionar el botón de Observar:
 * Envia un request al servidor con el código de la partida
 * que se desea iniciar a observar.
 * @param code: codigo del juego al que se desea unir
 */
void observeGame(char* code){

	///Objeto JSON por enviar
	json_object *jObj = json_object_new_object();

	///Variables para agregar como Key y Data
	char* jsonKEY = "OBSERVE";
	char* jsonData = code; //Este dato es el codigo del juego al que se desea unir

	///Se agrega la informacion en el JSON
	json_object *jstring = json_object_new_string(jsonData);
	json_object_object_add(jObj,jsonKEY, jstring);

	///Se envia el JSON a través de la función sendJSON
	sendJSON(jObj);

}


/**
 * Recibe las posiciones actuales de DKJr y las envia al servidor
 * para obtener las colisiones y que este devuelva todo por graficar
 * de vuelta en el cliente.
 */
void updateGame(int posx, int posy){

	//Paso de int a char[] para ser ingresado en JSON
	char px[10];
	char py[10];
	sprintf(px,"%d", posx);
	sprintf(py,"%d", posy);

	///Objeto JSON por enviar
	json_object *jObj = json_object_new_object();

	///Variables para agregar como Key y Data
	char* jsonKEYx = "POSX";
	char* jsonDatax =  px; //Este dato es el codigo del juego al que se desea unir
	char* jsonKEYy = "POSY";
	char* jsonDatay =  py; //Este dato es el codigo del juego al que se desea unir

	///Se agrega la informacion en el JSON
	json_object *jstringx = json_object_new_string(jsonDatax);
	json_object_object_add(jObj,jsonKEYx, jstringx);
	json_object *jstringy = json_object_new_string(jsonDatay);
	json_object_object_add(jObj,jsonKEYy, jstringy);

	///Se envia el JSON a través de la función sendJSON
	sendJSON(jObj);

}

/**************************************LOGICA**************************************/



/*******************************MANEJO_DE_RESPUESTAS*******************************/

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
int sendJSON(json_object *jObj){

	//Torna el Objeto JSON a un char[] para ser enviado como tal
	//al Servidor.
	char* jsonToServer = json_object_to_json_string(jObj);

	//TEST print
	printf("\nsendJSON: ");
	printf(jsonToServer);

    /********************************************************************/
    /********************************************************************/
	/*Abre conexion a servidor*/
	/*Probar si funciona*/

	char* str;
	int fd, numbytes;
	struct sockaddr_in client;

	//fd = socket(AF_INET, SOCK_STREAM, 0);

	char sendBuff[MAXDATASIZE];
	char recvBuff[MAXDATASIZE];
/*
	    struct hostent *he;

	    if (fd < 0)
	    {
	        printf("Error : Could not create socket\n");
	        return 1;
	    }
	    else
	    {
	        client.sin_family = AF_INET;
	        client.sin_port = htons(PORT);
	        client.sin_addr.s_addr = inet_addr("192.168.100.20"); //192.168.100.
	        memset(client.sin_zero, '\0', sizeof(client.sin_zero));
	    }

	    if (connect(fd, (const struct sockaddr *)&client, sizeof(client)) < 0)
	    {
	        printf("ERROR connecting to server\n");
	        return 1;
	    }

	    if (strcpy(sendBuff, jsonToServer) == NULL) {
	        printf("ERROR strcpy()");
	        exit(-1);
	    }

	    if (write(fd, sendBuff, strlen(sendBuff)) == -1)
	    {
	        printf("ERROR write()");
	        exit(-1);
	    }

	    if ((numbytes=recv(fd,recvBuff,MAXDATASIZE,0)) < 0){

	        printf("Error en recv() \n");
	        exit(-1);
	    }*/
	    /********************************************************************/
	    /********************************************************************/

	/*************/
	/*FOR TESTING*/
	//Pasa de *str a char[]
	//Para probar como llegaria del servidor
	strcpy(recvBuff, jsonToServer);
	/*FOR TESTING*/
	/*************/

	//Obtiene el json proveniente del servidor, lo parsea
    char* parsedJsonFromServer = json_tokener_parse(recvBuff);


	//Key: "PLAY"
	//Dice si se puede iniciar el juego
	struct json_object *dataFromJSONPlay;
	json_object_object_get_ex(parsedJsonFromServer, "PLAY", &dataFromJSONPlay);
	if (json_object_get_string(dataFromJSONPlay) != NULL) {
	    //estadoDelJuego = update();
		printf("\nrecvBuff(\"PLAY\"): ");
		printf(json_object_to_json_string(dataFromJSONPlay));
		printf("\n");
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
	}


	return 0;

}

/*************************************SENDJSON*************************************/


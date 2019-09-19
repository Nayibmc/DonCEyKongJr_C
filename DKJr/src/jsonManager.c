/*
 * jsonManager.c
 *
 *  Created on: Sep 18, 2019
 *      Author: ruben
 */

#include "jsonManager.h"
#include <json-c/json.h>

#define PORT 3550
#define MAXDATASIZE 999999
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

/**
 * Envia el JSON al servidor con datos solicitados.
 * @param jObj
 * @return data, conection
 */


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





int sendJSON(json_object *jObj){

	char* jsonToServer = json_object_to_json_string(jObj);

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
	struct json_object *dataFromJSON;


	//Key: "PLAY"
	//Dice si se puede iniciar el juego
	json_object_object_get_ex(parsedJsonFromServer, "PLAY", &dataFromJSON);
	if (json_object_get_string(dataFromJSON) != NULL) {
	    //estadoDelJuego = update();
		printf("\nrecvBuff(\"PLAY\"): ");
		printf(json_object_to_json_string(dataFromJSON));
		printf("\n");
	}


	//Key: "OBSERVE"
		//Dice si se puede iniciar el juego
		json_object_object_get_ex(parsedJsonFromServer, "OBSERVE", &dataFromJSON);
		if (json_object_get_string(dataFromJSON) != NULL) {
		    //estadoDelJuego = update();
			printf("\nrecvBuff(\"OBSERVE\"): ");
			printf(json_object_to_json_string(dataFromJSON));
			printf("\n");
		}



	return 0;


}


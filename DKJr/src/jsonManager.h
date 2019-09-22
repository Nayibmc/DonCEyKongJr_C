#ifndef JSONMANAGER_H_
#define JSONMANAGER_H_

#include <stdio.h>
#include <json-c/json.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 3550
#define MAXDATASIZE 999999



/*
 * jsonManager.h
 *
 *  Created on: Sep 18, 2019
 *      Author: ruben
 */


/**
 * Envia el JSON deseado al servidor y espera su respuesta.
 */
char* sendJSON(json_object *jObj);

/**
 * Verifica el JSON entrante del Servidor.
 */
char* checkJSON(char* jsonRecieved);



#endif /* JSONMANAGER_H_ */

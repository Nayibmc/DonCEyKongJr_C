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



/**************************************LOGICA**************************************/
/**************************************LOGICA**************************************/



/*******************************MANEJO_DE_RESPUESTAS*******************************/


/**
 * Maneja el contenido del JSON con el Key: "PLAY"
 */
void managePlay(json_object* jObj);

/**
 * Maneja el contenido del JSON con el Key: "GAME"
 */
void manageGame(json_object* jObj);

/**
 * Maneja el contenido del JSON con el Key: "DKJR"
 */
void manageDKJr(json_object* jObj);

/**
 * Maneja el contenido del JSON con el Key: "CROCODILES"
 */
void manageCrocodiles(json_object* jObj);

/**
 * Maneja el contenido del JSON con el Key: "FRUITS"
 */
void manageFruits(json_object* jObj);

/**
 * Maneja el contenido del JSON con el Key: "FRUITPOINTS"
 */
void manageFruitPoints(json_object* jObj);

/*******************************MANEJO_DE_RESPUESTAS*******************************/



/*************************************SENDJSON*************************************/

/**
 * Envia el JSON deseado al servidor y espera su respuesta.
 */
char* sendJSON(json_object *jObj);

char* checkJSON(char* jsonRecieved);



/*************************************SENDJSON*************************************/


#endif /* JSONMANAGER_H_ */

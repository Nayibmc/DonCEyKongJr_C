/*
 * jsonManager.h
 *
 *  Created on: Sep 18, 2019
 *      Author: ruben
 */


#ifndef JSONMANAGER_H_
#define JSONMANAGER_H_

#include <stdio.h>
#include <json-c/json.h>


/**
 * Iniciar juego
 */
void startGame();

/**
 * Observar juego
 */
void observeGame(char* code);

/**
 * Envia JSON deseado al servidor
 */
int sendJSON(json_object *jObj);




#endif /* JSONMANAGER_H_ */

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "jsonManager.h"
#include <json-c/json.h>

#include <stdio.h>
#include "Socket_Cliente.h"
#include "Socket_Cliente.c"
#include "Socket.h"
#include "Socket.c"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>




///Agregado por JSONManager

/**
 * KeyInput array de teclas presionadas o no
 */
int keyInput[4];   // = {0,1,0,1};










/**********************************Interfaz**********************************/

typedef struct{
    int x, y;
} DKJr;

// Se podría hace un sólo struct Sprite en vez de Fruit y Croc, con x, y y type/////////////////////
typedef struct{
	int x, y;
	int type;		//1 = fruit1, 2 = fruit2, 3 = fruit3
} Fruit;

typedef struct{
	int x, y;
	char type;		//'B' = Blue1, 'b' = Blue2, 'R' = Red1, 'r' = Red2
} Croc;

typedef struct{
	//Dimensiones
	int x, y;
	int sizeMult; 	//depende del tamaño de la pantalla
	int state; 	//0 es el menu, 1 es el juego, 2 es el observador

	DKJr dkjr;
	Fruit fruits[20];
	Croc crocs[20];

	SDL_Renderer *renderer;

	//Imágenes
	SDL_Texture *menuImage;
	SDL_Texture *backImage;
	SDL_Texture *dkjrImage;
	SDL_Texture *crocBlue1Image;
	SDL_Texture *crocBlue2Image;
	SDL_Texture *crocRed1Image;
	SDL_Texture *crocRed2Image;
	SDL_Texture *fruit1Image;
	SDL_Texture *fruit2Image;
	SDL_Texture *fruit3Image;
} Juego;

int mouseInPlay();
int mouseInObserver();

//Eventos (se cierra una ventana, se recibe input, etc.)
int processEvents(SDL_Window *window, Juego *juego){
  SDL_Event event;
  int done = 0;

  while(SDL_PollEvent(&event)){
    switch(event.type){
      case SDL_WINDOWEVENT_CLOSE:{
        if(window){
          SDL_DestroyWindow(window);
          window = NULL;
          done = 1;
        }
      }
      break;

      case SDL_KEYDOWN:{
        switch(event.key.keysym.sym){
          case SDLK_ESCAPE:
            done = 1;
          break;
        }
      }
      break;

      case SDL_QUIT:
        //quit out of the game
        done = 1;
      break;
    }

    //Cuando se está en el menú y se presiona click izquierdo
    if (juego->state == 0 && event.type == SDL_MOUSEBUTTONDOWN){
    	if (event.button.button == SDL_BUTTON_LEFT){
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			if (mouseInPlay(juego, mouseX, mouseY)){
			  juego->state = 1;
			}
    	}
    }
  }

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if (juego->state == 1){
	  //Teclas direccionales

	  if(state[SDL_SCANCODE_UP]){
	  		  juego->dkjr.y -= 1*juego->sizeMult;
	  		keyInput[0] = 1;
	  } else {
		  keyInput[0] = 0;
	  }

	  if(state[SDL_SCANCODE_RIGHT]){
	  		  juego->dkjr.x += 1*juego->sizeMult;
	  		keyInput[1] = 1;
	  } else {
		  keyInput[1] = 0;
	  }

	  if(state[SDL_SCANCODE_DOWN]){
	  		  juego->dkjr.y += 1*juego->sizeMult;
	  		keyInput[2] = 1;
	  } else {
		  keyInput[2] = 0;
	  }

	  if(state[SDL_SCANCODE_LEFT]){
		  juego->dkjr.x -= 1*juego->sizeMult;
		  keyInput[3] = 1;
	  } else {
		  keyInput[3] = 0;
	  }











  }

  return done;
}

int mouseInPlay(Juego *juego, int mouseX, int mouseY){
	int playXLeft = 12*8*juego->sizeMult;
	int playXRight = 19*8*juego->sizeMult;
	int playYDown = juego->y-(6*8*juego->sizeMult);
	int playYUp = juego->y-(10*8*juego->sizeMult);

	if ((mouseX > playXLeft && mouseX < playXRight) == 0){
		return 0;
	}
	if ((mouseY > playYUp && mouseY < playYDown) == 0){
		return 0;
	}
	//Envia JSON de inicio
	//startGame();
	return 1;
}

int mouseInObserver(Juego *juego, int mouseX, int mouseY){
	int obsXLeft = 10*8*juego->sizeMult;
	int obsXRight = 21*8*juego->sizeMult;
	int obsYDown = juego->y-(2*8*juego->sizeMult);
	int obsYUp = juego->y-(5*8*juego->sizeMult);

	if ((mouseX > obsXLeft && mouseX < obsXRight) == 0){
		return 0;
	}
	if ((mouseY > obsYUp && mouseY < obsYDown) == 0){
		return 0;
	}
	//Envia JSON para observar
	//Necesita el codigo
	//observeGame("AXY87MJ6P9R8");
	return 1;
}

//Inicializa variables y funciones necesarias
void initializeGame(SDL_Window *window, Juego *juego){
	SDL_Init(SDL_INIT_VIDEO);   								//Se inicializa SDL2

	juego->state = 0;
	juego->sizeMult = 3;
	juego->x = 248*juego->sizeMult;
	juego->y = 216*juego->sizeMult;
	juego->dkjr.x = 0;
	juego->dkjr.y = juego->y-(4*(8*juego->sizeMult));

	//Create an application window with the following settings:
	window = SDL_CreateWindow("Game Window",                     // window title
							SDL_WINDOWPOS_UNDEFINED,             // initial x position
							SDL_WINDOWPOS_UNDEFINED,             // initial y position
							248*juego->sizeMult,                                 // width, in pixels
							216*juego->sizeMult,                                 // height, in pixels
							0);                                  // flags

	juego->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	///////////PRUEBA Cocodrilos
    for (int i = 0; i < 20; i++){
    	if (i < 19){			//----------------Número de crocs
        	juego->crocs[i].x = 15*i*juego->sizeMult;
        	juego->crocs[i].y = 8*i*juego->sizeMult;

        	if (i <= 3){
        		juego->crocs[i].type = 'B';
        	}
        	if (i > 3 && i <= 7){
				juego->crocs[i].type = 'b';
			}
        	if (i > 7 && i <= 12){
				juego->crocs[i].type = 'R';
			}
        	if (i > 12 && i <= 20){
				juego->crocs[i].type = 'r';
			}
    	}
    	else{
    		juego->crocs[i].x = -1;
    	}
    }

    ///////////PRUEBA Frutas
    for (int i = 0; i < 20; i++){
		if (i < 19){			//----------------Número de crocs
			juego->fruits[i].x = 20*i*juego->sizeMult;
			juego->fruits[i].y = 20*i*juego->sizeMult;

			if (i <= 3){
				juego->fruits[i].type = 1;
			}
			if (i > 3 && i <= 7){
				juego->fruits[i].type = 2;
			}
			if (i > 7 && i <= 12){
				juego->fruits[i].type = 3;
			}
		}
		else{
			juego->fruits[i].x = -1;
		}
    }
}

//Dibuja
void doRender(Juego *juego){
	//
	if (juego->state == 0){
		SDL_Rect menuRect = {0, 0, juego->x, juego->y};
		SDL_RenderCopy(juego->renderer, juego->menuImage, NULL, &menuRect);
	}

	if (juego->state == 1){
		//El fondo
		SDL_Rect backRect = {0, 0, juego->x, juego->y};
		SDL_RenderCopy(juego->renderer, juego->backImage, NULL, &backRect);

		//DKJr
		SDL_Rect dkjrRect = {juego->dkjr.x, juego->dkjr.y, 25*juego->sizeMult, 16*juego->sizeMult};
		SDL_RenderCopy(juego->renderer, juego->dkjrImage, NULL, &dkjrRect);

		//Cocodrilos
		for (int i = 0; juego->crocs[i].x != -1; i++){
			SDL_Rect crocRect = {juego->crocs[i].x, juego->crocs[i].y, 15*juego->sizeMult, 8*juego->sizeMult};

			switch (juego->crocs[i].type){
			case 'B':
				SDL_RenderCopy(juego->renderer, juego->crocBlue1Image, NULL, &crocRect);
				break;
			case 'b':
				SDL_RenderCopy(juego->renderer, juego->crocBlue2Image, NULL, &crocRect);
				break;
			case 'R':
				SDL_RenderCopy(juego->renderer, juego->crocRed1Image, NULL, &crocRect);
				break;
			case 'r':
				SDL_RenderCopy(juego->renderer, juego->crocRed2Image, NULL, &crocRect);
				break;
			}
		}

		//Frutas
		for (int i = 0; juego->fruits[i].x != -1; i++){
			if (juego->fruits[i].type == 1){
				SDL_Rect crocRect = {juego->fruits[i].x, juego->fruits[i].y, 12*juego->sizeMult, 11*juego->sizeMult};
				SDL_RenderCopy(juego->renderer, juego->fruit1Image, NULL, &crocRect);
			}
			if (juego->fruits[i].type == 2){
				SDL_Rect crocRect = {juego->fruits[i].x, juego->fruits[i].y, 14*juego->sizeMult, 11*juego->sizeMult};
				SDL_RenderCopy(juego->renderer, juego->fruit2Image, NULL, &crocRect);
			}
			if (juego->fruits[i].type == 3){
				SDL_Rect crocRect = {juego->fruits[i].x, juego->fruits[i].y, 15*juego->sizeMult, 10*juego->sizeMult};
				SDL_RenderCopy(juego->renderer, juego->fruit3Image, NULL, &crocRect);
			}
		}
	}

    //We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(juego->renderer);
}

//Carga y almacena la imágenes en la memoria
void loadImages(Juego *juego){
	//Surfaces de las imágenes. Son como "canvases" que "reservan" los pixeles que va a ocupar una imagen
	SDL_Surface *menuSurface = NULL;
	SDL_Surface *backSurface = NULL;
	SDL_Surface *dkjrSurface = NULL;
	SDL_Surface *crocBlue1Surface = NULL;
	SDL_Surface *crocBlue2Surface = NULL;
	SDL_Surface *crocRed1Surface = NULL;
	SDL_Surface *crocRed2Surface = NULL;
	SDL_Surface *fruit1Surface = NULL;
	SDL_Surface *fruit2Surface = NULL;
	SDL_Surface *fruit3Surface = NULL;

	//Carga de imágenes
	menuSurface = IMG_Load("Resources/menu.png");
	backSurface = IMG_Load("Resources/back.png");
	dkjrSurface = IMG_Load("Resources/dkjr.png");
	crocBlue1Surface = IMG_Load("Resources/crocBlue1.png");
	crocBlue2Surface = IMG_Load("Resources/crocBlue2.png");
	crocRed1Surface = IMG_Load("Resources/crocRed1.png");
	crocRed2Surface = IMG_Load("Resources/crocRed2.png");
	fruit1Surface = IMG_Load("Resources/fruit1.png");
	fruit2Surface = IMG_Load("Resources/fruit2.png");
	fruit3Surface = IMG_Load("Resources/fruit3.png");

	//Si falla la carga de la imagen
	if (menuSurface == NULL){
		printf("Cannot find menu.png\n\n");
		SDL_Quit();
		exit(1);
	}
	if (dkjrSurface == NULL){
		printf("Cannot find dkjr.png\n\n");
		SDL_Quit();
		exit(1);
	}
	if (dkjrSurface == NULL){
		printf("Cannot find back.png\n\n");
		SDL_Quit();
		exit(1);
	}
	if (crocBlue1Surface == NULL){
		printf("Cannot find crocBlue1.png\n\n");
		SDL_Quit();
		exit(1);
	}
	if (crocBlue2Surface == NULL){
		printf("Cannot find crocBlue2.png\n\n");
		SDL_Quit();
		exit(1);
	}
	if (crocRed1Surface == NULL){
		printf("Cannot find crocRed1.png\n\n");
		SDL_Quit();
		exit(1);
	}
	if (crocRed2Surface == NULL){
		printf("Cannot find crocRed2.png\n\n");
		SDL_Quit();
		exit(1);
	}
	if (fruit1Surface == NULL){
		printf("Cannot find fruit1.png\n\n");
		SDL_Quit();
		exit(1);
	}
	if (fruit2Surface == NULL){
		printf("Cannot find fruit2.png\n\n");
		SDL_Quit();
		exit(1);
	}
	if (fruit3Surface == NULL){
		printf("Cannot find fruit3.png\n\n");
		SDL_Quit();
		exit(1);
	}

	juego->menuImage = SDL_CreateTextureFromSurface(juego->renderer, menuSurface);
	juego->dkjrImage = SDL_CreateTextureFromSurface(juego->renderer, dkjrSurface);
	juego->backImage = SDL_CreateTextureFromSurface(juego->renderer, backSurface);
	juego->crocBlue1Image = SDL_CreateTextureFromSurface(juego->renderer, crocBlue1Surface);
	juego->crocBlue2Image = SDL_CreateTextureFromSurface(juego->renderer, crocBlue2Surface);
	juego->crocRed1Image = SDL_CreateTextureFromSurface(juego->renderer, crocRed1Surface);
	juego->crocRed2Image = SDL_CreateTextureFromSurface(juego->renderer, crocRed2Surface);
	juego->fruit1Image = SDL_CreateTextureFromSurface(juego->renderer, fruit1Surface);
	juego->fruit2Image = SDL_CreateTextureFromSurface(juego->renderer, fruit2Surface);
	juego->fruit3Image = SDL_CreateTextureFromSurface(juego->renderer, fruit3Surface);

	//Se libera la memoria que tenía la surface de la imagen, después de que se dibuja
	SDL_FreeSurface(menuSurface);
	SDL_FreeSurface(backSurface);
	SDL_FreeSurface(dkjrSurface);
	SDL_FreeSurface(crocBlue1Surface);
	SDL_FreeSurface(crocBlue2Surface);
	SDL_FreeSurface(crocRed1Surface);
	SDL_FreeSurface(crocRed2Surface);
	SDL_FreeSurface(fruit1Surface);
	SDL_FreeSurface(fruit2Surface);
	SDL_FreeSurface(fruit3Surface);
}

//Elimina las imágenes de la memoria y cierra SDL2
void closeGame(SDL_Window *window, Juego *juego){
	//Limpia la memoria usada y cierra el programa
	SDL_DestroyTexture(juego->menuImage);
	SDL_DestroyTexture(juego->backImage);
	SDL_DestroyTexture(juego->dkjrImage);
	SDL_DestroyTexture(juego->crocBlue1Image);
	SDL_DestroyTexture(juego->crocBlue2Image);
	SDL_DestroyTexture(juego->crocRed1Image);
	SDL_DestroyTexture(juego->crocRed2Image);
	SDL_DestroyTexture(juego->fruit1Image);
	SDL_DestroyTexture(juego->fruit2Image);
	SDL_DestroyTexture(juego->fruit3Image);

	// Close and destroy the window
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(juego->renderer);

	// Clean up
	SDL_Quit();
}

/**********************************Interfaz**********************************/



































/***************************InterfazPorImplementar***************************/

/**
 * Grafica la información del Juego.
 */
void showGameInfo(char* state, char* score, char* level, char* lives) {

	printf("-> Showing Game Info\n");
	printf("   - State: %s\n   - Score: %s\n   - Level: %s\n   - Lives: %s\n",state,score,level,lives);

}

/**
 * Grafica a DKJr.
 */
void showDKJr(char* state, int posX, int posY) {

	printf("-> Showing DKJr\n");
	printf("   - State: %s\n   - PosX: %d\n   - PosY: %d\n",state,posX,posY);

}

/**
 * Grafica un Cocodrilo.
 */
void showCrocodile(char* color, int posX, int posY) {

	printf("\nShowing Crocodile\n");
	printf("   - Color: %s\n   - PosX: %d\n   - PosY: %d\n",color,posX,posY);

}

/**
 * Grafica una Fruta.
 */
void showFruit(char* type, int posX, int posY) {

	printf("\nShowing Fruit\n");
	printf("   - Type: %s\n   - PosX: %d\n   - PosY: %d\n",type,posX,posY);

}

/**
 * Grafica un Puntaje proveniente de una Fruta.
 */
void showFruitPoints(char* points, int posX, int posY) {

	printf("\nShowing FruitPoints\n");
	printf("   - Points: %s\n   - PosX: %d\n   - PosY: %d\n",points,posX,posY);


}

/***************************InterfazPorImplementar***************************/


















/*******************************JSONManagement*******************************/

//Flag para saber si el juego puede iniciar
//int gameStart = 0;

/**
 * Flag para indicar si es jugador o observador
 * -1: No iniciado
 *  1: Jugador
 *  0: Observador
 */
int userType = -1;

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
	char* jsonData = "Let me play!"; //Este dato no es necesario en el Servidor

	///Se agrega la informacion en el JSON
	//Se crea un jString con el dato por ingresar en JSON
	json_object *jString = json_object_new_string(jsonData);
	//Se ingresa el jString al JSON con su propio key ("jsonKEY")
	json_object_object_add(jObj,jsonKEY, jString);

	///Se envia el JSON a través de la función sendJSON
	//Retona el resultado del JSON entrante del servidor
	char* permition = sendJSON(jObj);

	char* stay = "-1";

	if (*permition == *stay) {
		printf("Too many games in Server.");
		/**
		 * MESSAGEBOX
		 */
	} else {
		printf("Start Game!\nCode: %s",permition);
		//Cambia el tipo de usuario a Jugador
		userType = 1;
		/**
		 * FUNCION PARA INICIAR JUEGO
		 */
	}

}

/**
 * Al presionar el botón de Observar:
 * Envia un request al servidor con el código de la partida
 * que se desea iniciar a observar.
 * @param code: codigo del juego al que se desea unir
 */
void observeGame(char* code){

	printf("\nSe desea observar un Game.\n\n");

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
	//Retona el resultado del JSON entrante del servidor
	char* permition = sendJSON(jObj);

	char* observe = "1";
	char* stay = "0";
	char* wrong = "-1";

	if (*permition == *observe) {
		printf("Start Observing Game.");
		//Cambia el tipo de usuario a
		userType = 0;
		/**
		 * FUNCION PARA OBSERVAR JUEGO
		 */
	} else if (*permition == *stay) {
		printf("Too many observers for this game in Server.");
		/**
		 * MESSAGEBOX
		 */
	} else if (*permition == *wrong) {
		printf("The game with code \"%s\" doesn't exist.",code);
		/**
		* MESSAGEBOX
		*/
	} else {
		printf("Error in observeGame(): %s",permition);
	}

}


/**
 * Recibe las posiciones actuales de DKJr y las envia al servidor
 * para obtener las colisiones y que este devuelva todo por graficar
 * de vuelta en el cliente.
 * @param code: codigo del juego al que se desea unir
 * @param keyInput: Array con las 4 diferentes teclas presionadas o no
 */
void updateGame(char* code, int keyInput[]) {

	///Se crea un objeto JSON
	struct json_object *jObj = json_object_new_object();

	//Se guardan por separado los inputs y se convierten a un char
	char* up;
	up = (char*)calloc(255, sizeof(char));
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
	char* jsonKeyInput = "UPDATE";

	//Se agrega el JSONArray al JSON por enviar
	json_object_object_add(jObj,jsonKeyInput, inputArray);

	///Variables para agregar como Key y Data
	char* jsonKeyCode = "CODE";
	char* jsonDataCode = code;  //Codigo del juego que se modificara

	///Se agrega la informacion del codigo en el JSON
	json_object *jstringCode = json_object_new_string(jsonDataCode);
	json_object_object_add(jObj,jsonKeyCode, jstringCode);

	///Se envia el JSON a través de la función sendJSON
	char* updateArray = sendJSON(jObj);

	///PARSEADO DE ARRAY PROVENIENTE DEL SERVIDOR///

	updateGameAux(updateArray);

}


void updateGameAux(char* updateArray) {

	printf("UpdateArray: %s\n\n", updateArray);

	//Toma el String proveniente del Servidor
	//Y es convertido a un JSON
	json_object* parsedJson = json_tokener_parse(updateArray);

	/**
	 * "GAME"
	 *  Array que contiene el estado del juego, el puntaje actual,
	 *  el nivel actual y la cantidad de vidas restantes.
	 */

	//json_object para tomar lo que viene junto al Key
	struct json_object* dataFromJSONArrayGame;
	dataFromJSONArrayGame = json_object_object_get(parsedJson, "GAME");
	//Obtiene el dato en String
	char* dataFromJSONArrayGameString = json_object_get_string(dataFromJSONArrayGame);
	//Verifica si este existe para retornar su contenido
	if (dataFromJSONArrayGameString != NULL) {
		printf("\nGameArray: %s\n", dataFromJSONArrayGameString);
		/**
		 * FUNCION DONDE ENVIAR A GRAFICAR
		 */
		manageGame(dataFromJSONArrayGame);

	}

	/**
	 * "DKJR"
	 *  Array que contiene el estado de DKJr (sprite por usar),
	 *  la posición actual en x y la posición actual en y.
	 */

	//json_object para tomar lo que viene junto al Key
	struct json_object* dataFromJSONArrayDKJr;
	dataFromJSONArrayDKJr = json_object_object_get(parsedJson, "DKJR");
	//Obtiene el dato en String
	char* dataFromJSONArrayDKJrString = json_object_get_string(dataFromJSONArrayDKJr);
	//Verifica si este existe para retornar su contenido
	if (dataFromJSONArrayDKJrString != NULL) {
		printf("\nDKJrArray: %s\n", dataFromJSONArrayDKJrString);
		/**
		 * FUNCION DONDE ENVIAR A GRAFICAR
		 */
		manageDKJr(dataFromJSONArrayDKJr);
	}

	/**
	 * "CROCODILES"
	 *  Array de array que contienen el color del cocodrilo,
	 *  su posición actual en x y su posición actual en y.
	 */

	//json_object para tomar lo que viene junto al Key
	struct json_object* dataFromJSONArrayCrocodiles;
	dataFromJSONArrayCrocodiles = json_object_object_get(parsedJson, "CROCODILES");
	//Obtiene el dato en String
	char* dataFromJSONArrayCrocodilesString = json_object_get_string(dataFromJSONArrayCrocodiles);
	//Verifica si este existe para retornar su contenido
	if (dataFromJSONArrayCrocodilesString != NULL) {

		printf("\nCrocodilesArray: %s\n", dataFromJSONArrayCrocodilesString);
		/**
		 * FUNCION DONDE ENVIAR A GRAFICAR
		 */
		manageCrocodiles(dataFromJSONArrayCrocodiles);
	}

	/**
	 * "FRUITS"
	 *  Array de arrays que contienen el tipo de fruta (sprite por usar),
	 *  su posición actual en x y su posición actual en y.
	 */

	//json_object para tomar lo que viene junto al Key
	struct json_object* dataFromJSONArrayFruits;
	dataFromJSONArrayFruits = json_object_object_get(parsedJson, "FRUITS");
	//Obtiene el dato en String
	char* dataFromJSONArrayFruitsString = json_object_get_string(dataFromJSONArrayFruits);
	//Verifica si este existe para retornar su contenido
	if (dataFromJSONArrayFruitsString != NULL) {

		printf("\nFruitArray: %s\n", dataFromJSONArrayFruitsString);
		/**
		 * FUNCION DONDE ENVIAR A GRAFICAR
		 */
		manageFruits(dataFromJSONArrayFruits);
	}

	/**
	 * "FRUITPOINTS"
	 *  Array de arrays que contienen el puntaje por graficar,
	 *  su posición actual en x y su posición actual en y.
	 */

	//json_object para tomar lo que viene junto al Key
	struct json_object* dataFromJSONArrayFruitPoints;
	dataFromJSONArrayFruitPoints = json_object_object_get(parsedJson, "FRUITPOINTS");
	//Obtiene el dato en String
	char* dataFromJSONArrayFruitPointsString = json_object_get_string(dataFromJSONArrayFruitPoints);
	//Verifica si este existe para retornar su contenido
	if (dataFromJSONArrayFruitPointsString != NULL) {

		printf("\nFruitPointsArray: %s\n", dataFromJSONArrayFruitPointsString);
		/**
		 * FUNCION DONDE ENVIAR A GRAFICAR
		 */
		manageFruitPoints(dataFromJSONArrayFruitPoints);
	}

	/**
	 * "ERROR"
	 *  Error en JSON.
	 */

	//json_object para tomar lo que viene junto al Key
	struct json_object* dataFromJSONArrayError;
	dataFromJSONArrayError = json_object_object_get(parsedJson, "ERROR");
	//Obtiene el dato en String
	char* dataFromJSONArrayErrorString = json_object_get_string(dataFromJSONArrayError);
	//Verifica si este existe para retornar su contenido
	if (dataFromJSONArrayErrorString != NULL) {

		printf("\nErrorArray: %s\n", dataFromJSONArrayErrorString);
	}

}


/**
 * Maneja el contenido del JSON con el Key: "GAME"
 */
void manageGame(json_object* jObj){

	//Informacion que viene en jObj
	const char* state;
	const char* score;
	const char* level;
	const char* lives;

	//Toma el estado del juego
	json_object* first = json_object_array_get_idx(jObj,0);
	state = json_object_get_string(first);

	//Toma el puntaje del juego
	json_object* second = json_object_array_get_idx(jObj,1);
	score = json_object_get_string(second);

	//Toma el nivel en el que se encuentra el juego
	json_object* third = json_object_array_get_idx(jObj,2);
	level = json_object_get_string(third);

	//Toma la cantidad de vidas que le quedan al jugador
	json_object* fourth = json_object_array_get_idx(jObj,3);
	lives = json_object_get_string(fourth);

	//Grafica la informacion del Juego
	showGameInfo(state, score, level, lives);

}

/**
 * Maneja el contenido del JSON con el Key: "DKJR"
 */
void manageDKJr(json_object* jObj){

	//Informacion que viene en jObj
	 const char* state;
	 int posX;
	 int posY;

	 //Toma el estado de DKJr
	json_object* first;
	first = json_object_array_get_idx(jObj,0);
	state = json_object_get_string(first);

	//Toma la posicion en x de DKJr
	json_object* second;
	second = json_object_array_get_idx(jObj,1);
	posX = json_object_get_int(second);

	//Toma la posicion en y de DKJr
	json_object* third;
	third = json_object_array_get_idx(jObj,2);
	posY = json_object_get_int(third);

	//Grafica a DKJr
	showDKJr(state, posX, posY);

}

/**
 * Maneja el contenido del JSON con el Key: "CROCODILES"
 */
void manageCrocodiles(json_object* jObj){

	//Informacion que viene en cada array de jObj
	const char* color;
	int posX;
	int posY;

	//json_object's temporales para iterar
	json_object* tempCrocodile;
	json_object* first;
	json_object* second;
	json_object* third;

	//Largo del array de Cocodrilos
	int arrayLength = json_object_array_length(jObj);

	//Se itera el array para obtener cada Cocodrilo
	for(int i = 0; i < arrayLength; i++) {

		//Toma el cocodrilo en la posicion i del Array
		tempCrocodile = json_object_array_get_idx(jObj,i);

		//Toma el color del Cocodrilo
		first = json_object_array_get_idx(tempCrocodile,0);
		color = json_object_get_string(first);

		//Toma la posicion en x del Cocodrilo
		second = json_object_array_get_idx(tempCrocodile,1);
		posX = json_object_get_int(second);

		//Toma la posicion en y del Cocodrilo
		third = json_object_array_get_idx(tempCrocodile,2);
		posY = json_object_get_int(third);

		//Grafica al Cocodrilo
		showCrocodile(color, posX, posY);

	}

}

/**
 * Maneja el contenido del JSON con el Key: "FRUITS"
 */
void manageFruits(json_object* jObj){

	//Informacion que viene en cada array de jObj
	const char* type;
	int posX;
	int posY;

	//json_object's temporales para iterar
	json_object* tempFruit;
	json_object* first;
	json_object* second;
	json_object* third;

	//Largo del array de Frutas
	int arrayLength = json_object_array_length(jObj);

	//Se itera el array para obtener cada Fruta
	for(int i = 0; i < arrayLength; i++) {

		//Toma la fruta en la posicion i del Array
		tempFruit = json_object_array_get_idx(jObj,i);

		//Toma el tipo de Fruta
		first = json_object_array_get_idx(tempFruit,0);
		type = json_object_get_string(first);

		//Toma la posicion en x de la Fruta
		second = json_object_array_get_idx(tempFruit,1);
		posX = json_object_get_int(second);

		//Toma la posicion en y de la Fruta
		third = json_object_array_get_idx(tempFruit,2);
		posY = json_object_get_int(third);

		//Grafica la Fruta
		showFruit(type, posX, posY);

	}

}

/**
 * Maneja el contenido del JSON con el Key: "FRUITPOINTS"
 */
void manageFruitPoints(json_object* jObj){

	//Informacion que viene en cada array de jObj
	const char* points;
	int posX;
	int posY;

	//json_object's temporales para iterar
	json_object* tempFruitPoints;
	json_object* first;
	json_object* second;
	json_object* third;

	//Largo del array del Puntaje de las Frutas
	int arrayLength = json_object_array_length(jObj);

	//Se itera el array para obtener cada Puntaje de las Frutas
	for(int i = 0; i < arrayLength; i++) {

		//Toma la fruta en la posicion i del Array
		tempFruitPoints = json_object_array_get_idx(jObj,i);

		//Toma el puntaje de la Fruta
		first = json_object_array_get_idx(tempFruitPoints,0);
		points = json_object_get_string(first);

		//Toma la posicion en x del Puntaje de la Fruta
		second = json_object_array_get_idx(tempFruitPoints,1);
		posX = json_object_get_int(second);

		//Toma la posicion en y del Puntaje de la Fruta
		third = json_object_array_get_idx(tempFruitPoints,2);
		posY = json_object_get_int(third);

		//Grafica el del Puntaje de la Fruta
		showFruitPoints(points, posX, posY);

	}

}


/**************************************LOGICA**************************************/







//Agregado por JSONManager


/**
 * Retorna el tamaño del char array
 */
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


/**
 * Main de DonCEy Kong Jr Client.
 */
int main(int argc, char *argv[]){

	//PRUEBAS JSON MANAGEMENT

	//Para iniciar el juego
	startGame();

	//Para observar un juego
	//observeGame("000A");

	//Para actualizar el juego
	updateGame("000A", keyInput);


/*

	//INTERFAZ
	Juego juego;
		SDL_Window *window = NULL;

		initializeGame(window, &juego);
		loadImages(&juego);

	    // The window is open: enter program loop (see SDL_PollEvent)
	    int done = 0;
	    //Event loop
	    while(!done){
			//Check for events
			done = processEvents(window, &juego);

			//printf("KeyInput:\n-> [%d,%d,%d,%d]",
			//		keyInput[0],keyInput[1],keyInput[2],keyInput[3]);


			 ///* LLamar funcion para enviar input o solo pedir la graficacion
			 ///* si es solo un observador
			updateGame("AXY87MJ6P9R8", keyInput);

			//Render display
			doRender(&juego);

			//don't burn up the CPU
			SDL_Delay(10);
	    }

	    closeGame(window, &juego);
	    return 0;
*/

    return 0;

}

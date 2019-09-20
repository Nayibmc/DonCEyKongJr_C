#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
	  if(state[SDL_SCANCODE_LEFT]){
		  juego->dkjr.x -= 1*juego->sizeMult;
	  }
	  if(state[SDL_SCANCODE_RIGHT]){
		  juego->dkjr.x += 1*juego->sizeMult;
	  }
	  if(state[SDL_SCANCODE_UP]){
		  juego->dkjr.y -= 1*juego->sizeMult;
	  }
	  if(state[SDL_SCANCODE_DOWN]){
		  juego->dkjr.y += 1*juego->sizeMult;
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

int main(int argc, char *argv[]){
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

		//Render display
		doRender(&juego);

		//don't burn up the CPU
		SDL_Delay(10);
    }

    closeGame(window, &juego);
    return 0;
}

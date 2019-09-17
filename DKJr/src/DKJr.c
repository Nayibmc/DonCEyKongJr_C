#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct
{
  int x, y;
  short life;
  char *name;
} DKJr;

int processEvents(SDL_Window *window, DKJr *dkjr)
{
  SDL_Event event;
  int done = 0;

  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_WINDOWEVENT_CLOSE:
      {
        if(window)
        {
          SDL_DestroyWindow(window);
          window = NULL;
          done = 1;
        }
      }
      break;
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
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
  }

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_LEFT])
  {
    dkjr->x -= 10;
  }
  if(state[SDL_SCANCODE_RIGHT])
  {
    dkjr->x += 10;
  }
  if(state[SDL_SCANCODE_UP])
  {
    dkjr->y -= 10;
  }
  if(state[SDL_SCANCODE_DOWN])
  {
    dkjr->y += 10;
  }

  return done;
}

void doRender(SDL_Renderer *renderer, DKJr *dkjr)
{
  //set the drawing color to blue
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

  //Clear the screen (to blue)
  SDL_RenderClear(renderer);

  //set the drawing color to white
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_Rect rect = { dkjr->x, dkjr->y, 200, 200 };
  SDL_RenderFillRect(renderer, &rect);

  //We are done drawing, "present" or show to the screen what we've drawn
  SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
  SDL_Window *window;                    // Declare a window
  SDL_Renderer *renderer;                // Declare a renderer

  SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

  DKJr dkjr;
  dkjr.x = 220;
  dkjr.y = 140;

  //Create an application window with the following settings:
  window = SDL_CreateWindow("Game Window",                     // window title
                            SDL_WINDOWPOS_UNDEFINED,           // initial x position
                            SDL_WINDOWPOS_UNDEFINED,           // initial y position
                            640,                               // width, in pixels
                            480,                               // height, in pixels
                            0                                  // flags
                            );
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



  // The window is open: enter program loop (see SDL_PollEvent)
  int done = 0;

  //Event loop
  while(!done)
  {
    //Check for events
    done = processEvents(window, &dkjr);

    //Render display
    doRender(renderer, &dkjr);

    //don't burn up the CPU
    SDL_Delay(10);
  }


  // Close and destroy the window
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  // Clean up
  SDL_Quit();
  return 0;
}

#include <stdio.h>
#include <SDL2/SDL.h>
#include "consts.h"

int init_SDL(SDL_Window *window, SDL_Surface *surface, SDL_Renderer *renderer) {
  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Failed to init video.\n");
    return -1;
  }

  window = SDL_CreateWindow( "SDL Tutorial",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             SCREEN_WIDTH, SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN);
  if(window == NULL) {
    printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  surface = SDL_GetWindowSurface(window);
  renderer = SDL_CreateRenderer(window, -1,
                                SDL_RENDERER_ACCELERATED
                                | SDL_RENDERER_PRESENTVSYNC);
  

  return 0;
}

int main(void) {
  SDL_Window *window = NULL;
  SDL_Surface *screenSurface = NULL;
  SDL_Renderer *renderer = NULL;

  if (init_SDL(window, screenSurface, renderer) < 0) return -1;
  
  printf("Hello world!\n");


  int running = 1;
  const int render_timer = roundf(1000.0f / (float) FPS);

  while (running) {
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const int start_frame_time = SDL_GetTicks();

    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = 0;
      }
    }
  }
  


  
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}

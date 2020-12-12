#include <stdio.h>
#include <SDL2/SDL.h>
#include "./consts.h"

int init_SDL(SDL_Window *window, SDL_Surface *surface) {
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
  
  return 0;
}

int main(void) {
  //The window we'll be rendering to
  SDL_Window* window = NULL;
  
  //The surface contained by the window
  SDL_Surface* screenSurface = NULL;

  if (init_SDL(window, screenSurface) < 0) return -1;
  
  printf("Hello world!\n");

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}

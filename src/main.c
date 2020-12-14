#include <stdio.h>
#include <SDL2/SDL.h>
#include "consts.h"

int init_SDL(SDL_Window **window,
             SDL_Renderer **renderer) {
  //Initialize SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("Failed to init video.\n");
    return -1;
  }

  *window = SDL_CreateWindow( "SDL Tutorial",
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             SCREEN_WIDTH,
                             SCREEN_HEIGHT,
                             SDL_WINDOW_OPENGL);
  if(*window == NULL) {
    printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  *renderer = SDL_CreateRenderer(*window, -1,
                                SDL_RENDERER_ACCELERATED
                                | SDL_RENDERER_PRESENTVSYNC);

  if (*renderer == NULL) {
    printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    return -1;    
  }
  

  return 0;
}

void draw_square(SDL_Renderer *renderer,
                 const int x,
                 const int y,
                 const int square_length) {
  SDL_Rect rect = { x, y, .w = square_length, .h = square_length };
  if (SDL_RenderDrawRect(renderer, &rect) < 0) {
    const char *error = SDL_GetError();
    printf("Failed to draw rect! %s\n", error);
  };
}

void draw_board(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 166, 104, 41, 255);
  SDL_RenderClear(renderer);

  // 19 x 19 board with 15 pixels of padding on both sides
  int board_length = SCREEN_WIDTH - 30;
  int square_width = board_length / 19;
  
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  for (int i = 0; i < 19; i++) {
    const int x = square_width * i + PADDING;
    for (int j = 0; j < 19; j++) {
      const int y = square_width * j + PADDING;
      draw_square(renderer, x, y, square_width);
    }
  }
}

int main(void) {
  SDL_Window *window     = NULL;
  SDL_Renderer *renderer = NULL;

  if (init_SDL(&window, &renderer) < 0) return -1;
  
  int running = 1;

  const int render_timer = roundf(1000.0f / (float) FPS);

  while (running == 1) {
    if (SDL_SetRenderDrawColor(renderer, 166, 104, 41, 255) < 0) {
      printf("Failed to draw the color. SDL Error: %s\n", SDL_GetError());
    }
    if (SDL_RenderClear(renderer) < 0) {
      printf("Failed to clear the render. SDL Error: %s\n", SDL_GetError());
    }

    SDL_Event event;

    const int start_frame_time = SDL_GetTicks();

    draw_board(renderer);

    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      }
    }

    const int end_frame_time = SDL_GetTicks();
    const int time_to_wait = max(10, render_timer - (end_frame_time - start_frame_time));

    SDL_Delay(time_to_wait);
    SDL_RenderPresent(renderer);
  }
  
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}

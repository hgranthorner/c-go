#include <stdio.h>
#include <SDL2/SDL.h>
#include "consts.h"

typedef struct Coordinate {
  int x;
  int y;
} Coordinate;

int init_SDL(SDL_Window **window,
             SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("Failed to init video.\n");
    return -1;
  }

  *window = SDL_CreateWindow("SDL Tutorial",
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
                 const int y) {
  SDL_Rect rect = { x, y, .w = SQUARE_LENGTH, .h = SQUARE_LENGTH };
  if (SDL_RenderDrawRect(renderer, &rect) < 0) {
    const char *error = SDL_GetError();
    printf("Failed to draw rect! %s\n", error);
  }
}

void draw_board(SDL_Renderer *renderer, Coordinate coordinates[]) {
  SDL_SetRenderDrawColor(renderer, 166, 104, 41, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  for (int i = 0; i < NUM_SQUARES; i++) {
    Coordinate coord = coordinates[i];
    draw_square(renderer, coord.x, coord.y);
  }
}

void generate_coordinates(Coordinate coords[]) {
  for (int i = 0; i < BOARD_DIMENSIONS; i++) {
    const int x = SQUARE_LENGTH * i + PADDING;
    for (int j = 0; j < BOARD_DIMENSIONS; j++) {
      const int y = SQUARE_LENGTH * j + PADDING;
      Coordinate c = { x, y };
      coords[(i * BOARD_DIMENSIONS) + j] = c;
    }
  }
}

void handle_inputs(int *running, SDL_Renderer *renderer, Coordinate coordinates[]) {
  if (SDL_SetRenderDrawColor(renderer, 166, 104, 41, 255) < 0) {
    printf("Failed to draw the color. SDL Error: %s\n", SDL_GetError());
  }
  
  if (SDL_RenderClear(renderer) < 0) {
    printf("Failed to clear the render. SDL Error: %s\n", SDL_GetError());
  }

  SDL_Event event;

  draw_board(renderer, coordinates);

  if (SDL_PollEvent(&event)) {
    if (event.type == SDL_MOUSEBUTTONUP) {
      printf("X: %d, Y: %d\n", event.button.x, event.button.y);
    }
    if (event.type == SDL_QUIT) {
      *running = 0;
    }
  }  
}

int main(void) {
  SDL_Window *window     = NULL;
  SDL_Renderer *renderer = NULL;

  if (init_SDL(&window, &renderer) < 0) return -1;
  
  int running = 1;

  const int render_timer = roundf(1000.0f / (float) FPS);

  Coordinate coordinates[NUM_SQUARES];

  generate_coordinates(coordinates);
  printf("Generated coordinates.\n");

  while (running) {
    const int start_frame_time = SDL_GetTicks();
    
    handle_inputs(&running, renderer, coordinates);

    const int end_frame_time = SDL_GetTicks();
    const int delta_time = end_frame_time - start_frame_time;
    const int time_to_wait = max(10, render_timer - delta_time);

    SDL_Delay(time_to_wait);
    SDL_RenderPresent(renderer);
  }
  
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}

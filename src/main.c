#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include "consts.h"

typedef struct {
  int x;
  int y;
} Coordinate;

typedef enum {Black, White} Color;

typedef struct {
  Coordinate coord;
  Color color;
  int intersection_index;
  bool destroyed;
} Stone;

typedef struct {
  Stone stones[1000];
  int next_index;
  Color turn;
} Stones;

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

void draw_board(SDL_Renderer *renderer, const Stones *stones) {
  SDL_SetRenderDrawColor(renderer, 166, 104, 41, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  // Draw all horizontal lines - same x axis
  for (int i = 0; i < 19; i++) {
    SDL_RenderDrawLine(renderer,
                       PADDING,
                       (i * SQUARE_LENGTH) + PADDING,
                       SCREEN_WIDTH - PADDING + 1,
                       (i * SQUARE_LENGTH) + PADDING);
    
    SDL_RenderDrawLine(renderer,
                       (i * SQUARE_LENGTH) + PADDING,
                       PADDING,
                       (i * SQUARE_LENGTH) + PADDING,
                       SCREEN_WIDTH - PADDING + 1);    
  }

  if (stones != NULL) {
    for (int i = 0; i < stones->next_index; i++) {
      const Stone stone = stones->stones[i];
      if (stone.color == Black) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      if (stone.color == White) SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

      const SDL_Rect rect = { stone.coord.x - 3, stone.coord.y - 3, 6, 6 };
      SDL_RenderFillRect(renderer, &rect);
    }
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

// index of array = i * 19 + j
// x = square_len * i + padding -> x = 24 * i + 30
// y = square_len * j + padding -> y = 24 * j + 30
// index of array = ((x - 30) / 24) * 19 + (y - 30) / 24
void place_stone(Coordinate coords[], const Coordinate click, Stones *stones) {
  const int i = round((click.x - PADDING) / (float) SQUARE_LENGTH);
  const int j = round((click.y - PADDING) / (float) SQUARE_LENGTH);
  const int index = i * 19 + j;
  Coordinate clicked = coords[index];

  // TODO: Create or add a "contains" function
  bool found = false;

  for (int i = 0; i < stones->next_index; i++) {
    found = stones->stones[i].intersection_index == index && !stones->stones[i].destroyed;
  }
  if (!found) {
    const Stone stone = { clicked, stones->turn, index, false };
    stones->stones[stones->next_index] = stone;
    if (stones->turn == Black) stones->turn = White;
    else if (stones->turn == White) stones->turn = Black;
    stones->next_index++;  
  }
}

void handle_inputs(bool *running, SDL_Renderer *renderer, Coordinate coordinates[], Stones *stones) {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    if (event.type == SDL_MOUSEBUTTONUP) {
       const Coordinate click = { event.button.x, event.button.y};
       place_stone(coordinates, click, stones);
    }
    if (event.type == SDL_QUIT) {
      *running = false;
    }
  }  
}

int main(void) {
  SDL_Window *window     = NULL;
  SDL_Renderer *renderer = NULL;

  if (init_SDL(&window, &renderer) < 0) return -1;
  
  bool running = true;

  const int render_timer = roundf(1000.0f / (float) FPS);

  Coordinate coordinates[NUM_INTERSECTIONS];
  generate_coordinates(coordinates);
  
  Stones stones = { {}, 0, Black };
  
  while (running) {
    const int start_frame_time = SDL_GetTicks();
    
    if (SDL_SetRenderDrawColor(renderer, 166, 104, 41, 255) < 0) {
      printf("Failed to draw the color. SDL Error: %s\n", SDL_GetError());
    }
  
    if (SDL_RenderClear(renderer) < 0) {
      printf("Failed to clear the render. SDL Error: %s\n", SDL_GetError());
    }

    handle_inputs(&running, renderer, coordinates, &stones);

    draw_board(renderer, &stones);

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

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
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

void draw_board(SDL_Renderer *renderer, Coordinate *clicked) {
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

  if (clicked != NULL) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    const SDL_Rect rect = { clicked->x - 3, clicked->y - 3, 6, 6 };
    SDL_RenderFillRect(renderer, &rect);
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

// Just to confirm that the intersections are right
void draw_intersections(SDL_Renderer *renderer, const Coordinate coords[]) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);  
  for (int i = 0; i < NUM_INTERSECTIONS; i++) {
    const Coordinate c = coords[i];
    const SDL_Rect rect = { .x = c.x - 5, .y = c.y - 5, .w = 10, .h = 10};
    SDL_RenderFillRect(renderer, &rect);
  }
}

// index of array = i * 19 + j
// x = square_len * i + padding -> x = 24 * i + 30
// index of array = ((x - 30) / 24) * 19 + j
// y = square_len * j + padding -> y = 24 * j + 30
// index of array = ((x - 30) / 24) * 19 + (y - 30) / 24
Coordinate *find_nearest_intersection(Coordinate coords[], const Coordinate click) {
  const int i = round((click.x - PADDING) / (float) SQUARE_LENGTH);
  const int j = round((click.y - PADDING) / (float) SQUARE_LENGTH);
  const int index = i * 19 + j;
  Coordinate *clicked = &coords[index];
  printf("%d, %d vs %d, %d\n", clicked->x, clicked->y, click.x, click.y);
  // printf("%d: %d, %d\n", index, coords[index].x, coords[index].y);
  // printf("X: %d, Y: %d\n", click.x, click.y);
  return clicked;
}

void handle_inputs(int *running, SDL_Renderer *renderer, Coordinate coordinates[], Coordinate **clicked_intersection) {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    if (event.type == SDL_MOUSEBUTTONUP) {
       const Coordinate click = { event.button.x, event.button.y};
       *clicked_intersection = find_nearest_intersection(coordinates, click);
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

  Coordinate coordinates[NUM_INTERSECTIONS];

  generate_coordinates(coordinates);
  
  Coordinate *clicked_intersection = NULL;
  while (running) {
    const int start_frame_time = SDL_GetTicks();
    
    if (SDL_SetRenderDrawColor(renderer, 166, 104, 41, 255) < 0) {
      printf("Failed to draw the color. SDL Error: %s\n", SDL_GetError());
    }
  
    if (SDL_RenderClear(renderer) < 0) {
      printf("Failed to clear the render. SDL Error: %s\n", SDL_GetError());
    }

    draw_board(renderer, clicked_intersection);
    //draw_intersections(renderer, coordinates);

    handle_inputs(&running, renderer, coordinates, &clicked_intersection);

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

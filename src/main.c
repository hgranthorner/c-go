#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include "consts.h"
#include "structs.h"
#include "render.h"

int init_SDL(SDL_Window **window,
             SDL_Renderer **renderer,
             TTF_Font **font) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("Failed to init SDL.\n");
    return -1;
  }

  if (TTF_Init() == -1) {
    printf("Failed to init ttf.\n");
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

  *font = TTF_OpenFont("assets/arial.ttf", FONT_SIZE);
  if (*font == NULL) {
    printf( "Font could not be loaded! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }


  return 0;
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
int find_index(const Coordinate *click) {
  // check to make sure click is inside board
  bool x_outside = click->x < PADDING || click->x > SCREEN_WIDTH  - PADDING;
  bool y_outside = click->y < PADDING || click->y > SCREEN_HEIGHT - PADDING;

  if (x_outside || y_outside) return -1;

  const int i = round((click->x - PADDING) / (float) SQUARE_LENGTH);
  const int j = round((click->y - PADDING) / (float) SQUARE_LENGTH);
  const int index = i * 19 + j;
  return index;
}

Coordinate *find_intersection(Coordinate coords[], const Coordinate *event) {
  const int index = find_index(event);
  if (index == -1) return NULL;
  return &coords[index];
}

const Stone *place_stone(Coordinate coords[], const Coordinate click, Stones *stones) {
  const int index = find_index(&click);

  if (index == -1) {
    return NULL;
  }

  Coordinate clicked = coords[index];

  // TODO: Create or add a "contains" function
  bool found = false;

  for (int i = 0; i < stones->next_index; i++) {
    found = stones->stones[i].intersection_index == index;
  }

  const Stone *stone = NULL;

  if (!found) {
    stone = create_stone(clicked, stones->turn, index);
    stones->stones[stones->next_index] = *stone;
    if (stones->turn == Black) stones->turn = White;
    else if (stones->turn == White) stones->turn = Black;
    stones->next_index++;
  }

  return stone;
}

void get_neighboring_indices(const int index, int *arr) {
  arr[0] = index % BOARD_DIMENSIONS == 0                 ? -1 : index - 1;
  arr[1] = index + BOARD_DIMENSIONS >= NUM_INTERSECTIONS ? -1 : index + BOARD_DIMENSIONS;
  arr[2] = index % BOARD_DIMENSIONS == 18                ? -1 : index + 1;
  arr[3] = index - BOARD_DIMENSIONS < 0                  ? -1 : index - BOARD_DIMENSIONS;
}


// Idea: get the neighboring indices of our stone.
void try_kill_stone(const Stone *stone, const Stones *stones) {

}

void kill_stones(const Stone *stone, Stones *stones) {
  int neighbors[4];
  get_neighboring_indices(stone->intersection_index, neighbors);
  Stone *found_stone = NULL;

  // n is a 4 element array
  for (int i = 0; i < 4; i++) {
    int index = neighbors[i];
    // if there is no neighboring index in that directions
    if (index == -1) continue;

    // otherwise see if a stone exists there
    for (int j = 0; j < stones->next_index; j++) {
      found_stone = &stones->stones[j];
      if (found_stone->intersection_index == index) break;
      found_stone = NULL;
    }

    // if there is no stone, continue to the next pass;
    if (found_stone == NULL) continue;

    // otherwise, let's see if it's dead
    try_kill_stone(found_stone, stones);
  }
}


void handle_inputs(bool *running, SDL_Renderer *renderer, Coordinate coords[], Stones *stones, Coordinate **hover) {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    if (event.type == SDL_MOUSEBUTTONUP) {
      const Coordinate click = { event.button.x, event.button.y};
      const Stone *stone = place_stone(coords, click, stones);
      if (stone != NULL)
        kill_stones(stone, stones);
    }
    if (event.type == SDL_MOUSEMOTION) {
      const Coordinate hover_event = { event.motion.x, event.motion.y };
      *hover = find_intersection(coords, &hover_event);
    }
    if (event.type == SDL_QUIT) {
      *running = false;
    }
  }
}

int main(void) {
  SDL_Window *window     = NULL;
  SDL_Renderer *renderer = NULL;
  TTF_Font *font         = NULL;

  if (init_SDL(&window, &renderer, &font) < 0) return -1;

  bool running = true;

  const int render_timer = roundf(1000.0f / (float) FPS);

  Coordinate coordinates[NUM_INTERSECTIONS];
  generate_coordinates(coordinates);

  Stones stones = { {}, 0, Black };
  Coordinate *hovered_coordinate = NULL;
  Score score = { 0, 0, 0, 0 };

  while (running) {
    const int start_frame_time = SDL_GetTicks();

    if (SDL_SetRenderDrawColor(renderer, 166, 104, 41, 255) < 0) {
      printf("Failed to draw the color. SDL Error: %s\n", SDL_GetError());
    }

    if (SDL_RenderClear(renderer) < 0) {
      printf("Failed to clear the render. SDL Error: %s\n", SDL_GetError());
    }

    handle_inputs(&running, renderer, coordinates, &stones, &hovered_coordinate);

    draw_board(renderer, font, &stones, hovered_coordinate, &score);

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

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include "consts.h"
#include "structs.h"
#include "render.h"

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

const Stone *place_stone(Coordinate coords[], const Coordinate click, Game *game) {
  const int index = find_index(&click);

  if (index == -1) {
    return NULL;
  }

  Coordinate clicked = coords[index];

  // TODO: Create or add a "contains" function
  bool empty = is_empty(&game->stones[index]);

  if (empty) {
    game->stones[index] = *create_stone(clicked, game->turn, index);
    if (game->turn == Black) game->turn = White;
    else if (game->turn == White) game->turn = Black;
    return &game->stones[index];
  }

  return NULL;
}

void get_neighboring_indices(const int index, int *arr) {
  arr[0] = index % BOARD_DIMENSIONS == 0                 ? -1 : index - 1;
  arr[1] = index + BOARD_DIMENSIONS >= NUM_INTERSECTIONS ? -1 : index + BOARD_DIMENSIONS;
  arr[2] = index % BOARD_DIMENSIONS == 18                ? -1 : index + 1;
  arr[3] = index - BOARD_DIMENSIONS < 0                  ? -1 : index - BOARD_DIMENSIONS;
}

void populate_group(const Stone *stone, const Game *game, bool group[]) {
  int neighbors[4];
  get_neighboring_indices(stone->intersection_index, neighbors);
  for (int i = 0; i < 4; i++) {
    const int index = neighbors[i];
    // Our conditions: the index must be on the board; we must have not checked that stone already, and that stone must be on the same team as the on we're currently checking
    const Stone *stone_to_check = &game->stones[index];
    if (index != -1 &&
        !is_empty(stone_to_check) &&
        !group[index] &&
        stone_to_check->color == stone->color) {
      group[index] = true;
      populate_group(stone_to_check, game, group);
    }
  }
}

int try_kill_stone(const Stone *stone, Game *game) {
  // first find the group of stones
  bool group[NUM_INTERSECTIONS];
  for (int i = 0; i < NUM_INTERSECTIONS; i++) group[i] = false;
  group[stone->intersection_index] = true;
  populate_group(stone, game, group);

  // then check if any of the stones have any liberties
  int neighbors[4];
  bool alive = false;
  for (int target_stone_index = 0; target_stone_index < NUM_INTERSECTIONS; target_stone_index++) {
    if (group[target_stone_index]) {
      get_neighboring_indices(target_stone_index, neighbors);
      for (int neighbor_index = 0; neighbor_index < 4; neighbor_index++) {
        int neighbor = neighbors[neighbor_index];
        if (neighbor != -1 && is_empty(&game->stones[neighbor])) {
          printf("Neighbor index %d\n", neighbor);
          alive = true;
          break;
        }
      }
    }
  }

  int points_awarded = 0;

  if (!alive) {
    for (int i = 0; i < NUM_INTERSECTIONS; i++) {
      if (group[i]) {
        game->stones[i] = NO_STONE;
        points_awarded++;
      }
    }
  }

  return points_awarded;
}

int kill_stones(const Stone *stone, Game *game) {
  int neighbors[4];
  get_neighboring_indices(stone->intersection_index, neighbors);
  Stone *found_stone = NULL;

  int points_awarded = 0;

  // n is a 4 element array
  for (int i = 0; i < 4; i++) {
    int index = neighbors[i];
    // if there is no neighboring index in that direction, then that index is off the board
    if (index == -1) continue;

    found_stone = &game->stones[index];

    // if there is no stone, or the stone is friendly, continue to the next pass
    if (is_empty(found_stone) || found_stone->color == stone->color) continue;

    // otherwise, let's see if it's dead
    points_awarded = try_kill_stone(found_stone, game);
  }

  return points_awarded;
}


void handle_inputs(bool *running, Coordinate coords[], Game *game, Score *score, Coordinate **hover) {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    if (event.type == SDL_MOUSEBUTTONUP) {
      const Coordinate click = { event.button.x, event.button.y};
      const Stone *stone = place_stone(coords, click, game);
      int points_awarded = 0;
      // Place stone alternates the current turn
      if (stone != NULL) points_awarded = kill_stones(stone, game);
      if (game->turn == White) score->black_takes += points_awarded;
      if (game->turn == Black) score->white_takes += points_awarded;
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

  Game game;
  create_game(&game, Black);

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

    handle_inputs(&running, coordinates, &game, &score, &hovered_coordinate);

    draw_board(renderer, font, &game, hovered_coordinate, &score);

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

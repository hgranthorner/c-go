#include <string.h>
#include <stdlib.h>
#include "structs.h"

static int stone_id = 0;
static int turn = 1;

const Stone *create_stone(const Coordinate coord, const Color color, const int intersection_index) {
  Stone stone = { stone_id, coord, color, intersection_index, 4, turn };
  const Stone *stone_p = &stone;
  stone_id++;
  turn++;
  return stone_p;
}

bool is_empty(const Stone *stone) {
  return stone->id == -1;
}

void create_game(Game *game, Color player_turn) {
  Stone stone_arr[NUM_INTERSECTIONS];

  for (int i = 0; i < NUM_INTERSECTIONS; i++) stone_arr[i] = NO_STONE;

  memcpy(game->stones, stone_arr, sizeof(stone_arr));
  game->turn = player_turn;
  game->len_history = 0;
}

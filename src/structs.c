#include "structs.h"
#include <string.h>

int stone_id = 0;

const Stone *create_stone(const Coordinate coord, const Color color, const int intersection_index) {
  Stone stone = {stone_id, coord, color, intersection_index, 4};
  const Stone *stone_p = &stone;
  stone_id++;
  return stone_p;
}

bool is_empty(const Stone *stone) {
  return stone->id == -1;
}

void create_game(Game *game, Color turn) {
  Stone stone_arr[NUM_INTERSECTIONS];
  for (int i = 0; i < NUM_INTERSECTIONS; i++) stone_arr[i] = NO_STONE;

  memcpy(game->stones, stone_arr, sizeof(stone_arr));
  game->turn = turn;
}

#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include "consts.h"

typedef struct {
  int x;
  int y;
} Coordinate;

typedef enum {Black, White} Color;

typedef struct {
  int id;
  Coordinate coord;
  Color color;
  int intersection_index;
  int liberties;
  int turn;
} Stone;

static Stone NO_STONE = { -1, {-1, -1}, Black, -1, -1, -1 };
bool is_empty(const Stone *stone);

const Stone *create_stone(Coordinate coord, Color color, int intersection_index);

typedef struct {
  Stone stones[NUM_INTERSECTIONS];
  Stone history[LONGEST_GAME];
  int last_played_index;
  int len_history;
  Color turn;
} Game;

void create_game(Game *game, Color turn);

typedef struct {
  int black_territory;
  int black_takes;
  int white_territory;
  int white_takes;
} Score;

#endif

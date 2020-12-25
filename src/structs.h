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
  bool destroyed;
  int liberties;
} Stone;

const Stone *create_stone(Coordinate coord, Color color, int intersection_index);

typedef struct {
  Stone stones[NUM_INTERSECTIONS];
  int next_index;
  Color turn;
} Stones;

typedef struct {
  int black_territory;
  int black_takes;
  int white_territory;
  int white_takes;
} Score;

#endif

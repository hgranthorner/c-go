#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>

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

typedef struct {
  int black_territory;
  int black_takes;
  int white_territory;
  int white_takes;
} Score;

#endif

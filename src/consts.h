#ifndef CONSTS_H
#define CONSTS_H

static inline
int max(int x, int y)
{
  return x > y ? x : y;
}

static const int PADDING = 30;
static const int BOARD_DIMENSIONS = 19;
static const int SCREEN_WIDTH  = (24 * (BOARD_DIMENSIONS - 1)) + (30 * 2);
static const int SCREEN_HEIGHT = (24 * (BOARD_DIMENSIONS - 1)) + (30 * 2);
static const int BOARD_LENGTH = SCREEN_WIDTH - (PADDING * 2);
static const int SQUARE_LENGTH = BOARD_LENGTH / (19 - 1);
static const int FPS = 60;
static const int NUM_INTERSECTIONS = 361;
static const int FONT_SIZE = 16;
static const int STONE_RADIUS = 10;

#endif

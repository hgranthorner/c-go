#ifndef CONSTS_H
#define CONSTS_H

static inline
int max(int x, int y)
{
  return x > y ? x : y;
}

enum {
  PADDING = 30,
  BOARD_DIMENSIONS = 19,
  LONGEST_GAME = 1000, // The longest known game lasted 411 moves. https://senseis.xmp.net/?LongestPossibleGame
  SCREEN_WIDTH  = (24 * (BOARD_DIMENSIONS - 1)) + (30 * 2),
  SCREEN_HEIGHT = (24 * (BOARD_DIMENSIONS - 1)) + (30 * 2),
  BOARD_LENGTH = SCREEN_WIDTH - (PADDING * 2),
  SQUARE_LENGTH = BOARD_LENGTH / (19 - 1),
  FPS = 60,
  NUM_INTERSECTIONS = 361,
  FONT_SIZE = 16,
  STONE_RADIUS = 10,
};

#endif

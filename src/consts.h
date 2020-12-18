#ifndef CONSTS_H
#define CONSTS_H

static inline
int max(int x, int y)
{
  return x > y ? x : y;
}

const int PADDING = 30;
const int BOARD_DIMENSIONS = 19;
const int SCREEN_WIDTH  = (24 * (BOARD_DIMENSIONS - 1)) + (30 * 2);
const int SCREEN_HEIGHT = (24 * (BOARD_DIMENSIONS - 1)) + (30 * 2);
const int BOARD_LENGTH = SCREEN_WIDTH - (PADDING * 2);
const int SQUARE_LENGTH = BOARD_LENGTH / (19 - 1);
const int FPS = 60;
const int NUM_SQUARES = 361;

#endif

#ifndef CONSTS_H
#define CONSTS_H

static inline
int max(int x, int y)
{
  return x > y ? x : y;
}

const int SCREEN_WIDTH  = 486; // (24 pixels * 19 squares) + 30 padding
const int SCREEN_HEIGHT = 486; // (24 pixels * 19 squares) + 30 padding
const int BOARD_LENGTH = SCREEN_WIDTH - 30;
const int SQUARE_LENGTH = BOARD_LENGTH / 19;
const int PADDING = 15;
const int FPS = 60;
const int BOARD_DIMENSIONS = 19;
const int NUM_SQUARES = 361;

#endif

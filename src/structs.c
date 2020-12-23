#include "structs.h"

int stone_id = 0;

Stone create_stone(Coordinate coord, Color color, int intersection_index) {
  Stone stone = {stone_id, coord, color, intersection_index, false, 4};
  stone_id++;
  return stone;
}

#ifndef RENDER_H
#define RENDER_H

void draw_board(SDL_Renderer *renderer, TTF_Font *font, const Stones *stones, const Coordinate *hover, const Score *score);

#endif

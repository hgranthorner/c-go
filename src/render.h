#ifndef RENDER_H
#define RENDER_H

int init_SDL(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font);
void draw_board(SDL_Renderer *renderer, TTF_Font *font, const Game *game, const Coordinate *hover, const Score *score);

#endif

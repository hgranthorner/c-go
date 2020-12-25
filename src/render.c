#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "consts.h"
#include "structs.h"

int init_SDL(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("Failed to init SDL.\n");
    return -1;
  }

  if (TTF_Init() == -1) {
    printf("Failed to init ttf.\n");
    return -1;
  }

  *window = SDL_CreateWindow("SDL Tutorial",
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             SCREEN_WIDTH,
                             SCREEN_HEIGHT,
                             SDL_WINDOW_OPENGL);
  if(*window == NULL) {
    printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  *renderer = SDL_CreateRenderer(*window, -1,
                                 SDL_RENDERER_ACCELERATED
                                 | SDL_RENDERER_PRESENTVSYNC);

  if (*renderer == NULL) {
    printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  *font = TTF_OpenFont("assets/arial.ttf", FONT_SIZE);
  if (*font == NULL) {
    printf( "Font could not be loaded! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }


  return 0;
}


void draw_black_stone(SDL_Renderer *renderer, int x, int y) {
  filledCircleRGBA(renderer, x, y, STONE_RADIUS, 0, 0, 0, 255);
}

void draw_white_stone(SDL_Renderer *renderer, int x, int y) {
  filledCircleRGBA(renderer, x, y, STONE_RADIUS, 255, 255, 255, 255);
}

void draw_board(SDL_Renderer *renderer, TTF_Font *font, const Stones *stones, const Coordinate *hover, const Score *score) {
  SDL_SetRenderDrawColor(renderer, 166, 104, 41, 255);
  SDL_RenderClear(renderer);

  char black_takes[12];
  char white_takes[12];
  sprintf(black_takes, "%d", score->black_takes);
  sprintf(white_takes, "%d", score->white_takes);
  const SDL_Color black_color = {0, 0, 0};
  const SDL_Color white_color = {255, 255, 255};
  SDL_Surface *surface = TTF_RenderText_Blended(font, black_takes, black_color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  int text_w = 0;
  int text_h = 0;
  SDL_QueryTexture(texture, NULL, NULL, &text_w, &text_h);
  const SDL_Rect black_rect = { PADDING, SCREEN_HEIGHT - PADDING, text_w, text_h };
  SDL_RenderCopy(renderer, texture, NULL, &black_rect);
  surface = TTF_RenderText_Blended(font, white_takes, white_color);
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_QueryTexture(texture, NULL, NULL, &text_w, &text_h);
  const SDL_Rect white_rect = { SCREEN_WIDTH - PADDING - text_w, PADDING - text_h, text_w, text_h };
  SDL_RenderCopy(renderer, texture, NULL, &white_rect);
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  // Draw all horizontal lines - same x axis
  for (int i = 0; i < 19; i++) {
    SDL_RenderDrawLine(renderer,
                       PADDING,
                       (i * SQUARE_LENGTH) + PADDING,
                       SCREEN_WIDTH - PADDING + 1,
                       (i * SQUARE_LENGTH) + PADDING);

    SDL_RenderDrawLine(renderer,
                       (i * SQUARE_LENGTH) + PADDING,
                       PADDING,
                       (i * SQUARE_LENGTH) + PADDING,
                       SCREEN_WIDTH - PADDING + 1);
  }

  if (hover != NULL) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    const SDL_Rect rect = { hover->x - 3, hover->y - 3, 6, 6 };
    SDL_RenderFillRect(renderer, &rect);
  }

  if (stones != NULL) {
    for (int i = 0; i < NUM_INTERSECTIONS; i++) {
      const Stone stone = stones->stones[i];
      if (!is_empty(&stone)) {
        if (stone.color == Black) draw_black_stone(renderer, stone.coord.x, stone.coord.y);
        if (stone.color == White) draw_white_stone(renderer, stone.coord.x, stone.coord.y);
      }
    }
  }
}

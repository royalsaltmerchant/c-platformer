#include "engine.h"

void initEngine(Engine *engine, const char *title, int screenWidth, int screenHeight, const char *fontLocation, int fontSize) {
  // SDL
  SDL_Init(SDL_INIT_VIDEO);
  engine->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
  engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 255);
  SDL_RenderClear(engine->renderer);
  SDL_RenderPresent(engine->renderer);
  // Font
  TTF_Init();
  engine->font = TTF_OpenFont(fontLocation, fontSize);
  if (engine->font == NULL) {
    fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
    exit(1);  // Terminate the program with a non-zero exit status
  }
}

void freeEngine(Engine *engine) {
  SDL_DestroyRenderer(engine->renderer);
  SDL_DestroyWindow(engine->window);

  TTF_CloseFont(engine->font);
  TTF_Quit();

  SDL_Quit();
}

void clearRender(Engine *engine) {
  SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 255);
  SDL_RenderClear(engine->renderer);
}

SDL_Texture* createTextTexture(Engine* engine, const char* text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(engine->font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(engine->renderer, surface);
    SDL_FreeSurface(surface); // Free the surface immediately after creating the texture
    return texture;
}

void renderTextCentered(Engine* engine, SDL_Texture* texture, int x, int y) {
    
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect textRect;
    textRect.x = x - width / 2;
    textRect.y = y - height / 2;
    textRect.w = width;
    textRect.h = height;

    SDL_RenderCopy(engine->renderer, texture, NULL, &textRect);
}
#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
  SDL_Window* window;
  SDL_Renderer* renderer;
  TTF_Font* font;
} Engine;

void initEngine(Engine *engine, const char *title, int screenWidth, int screenHeight, const char *fontLocation, int fontSize);

void freeEngine(Engine *engine);

void clearRender(Engine *engine);

SDL_Texture* createTextTexture(Engine* engine, const char* text, SDL_Color color);

void renderTextCentered(Engine* engine, SDL_Texture* texture, int x, int y);

#endif

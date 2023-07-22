#include "engine.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

typedef struct {
  int x;
  int y;
} Position;

Engine engine;
Position velocity;
int gameState;
SDL_Color textColor = {255, 255, 255, 255};
SDL_Texture* startTexture;

void init() {
  char *title = "New Game";
  char *fontLocation = "./assets/OpenSans-Regular.ttf";
  initEngine(&engine, title, SCREEN_WIDTH, SCREEN_HEIGHT, fontLocation, 24);
  // Game
  gameState = 2;  // Set to starting game state
  // graphics
  // start screen text
  startTexture = createTextTexture(&engine, "Press any key to start", textColor);
}

void handleInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      gameState = 0;
      return;
    }

    if (event.type == SDL_KEYDOWN) {
      // Start screen
      if (gameState == 2) {
        // TODO: INIT GAME
        gameState = 1;
        return;
      }
      // directions
      switch (event.key.keysym.sym) {
        case SDLK_UP:
          if (velocity.y != 1) {
            velocity.x = 0;
            velocity.y = -1;
          }
          break;
        case SDLK_DOWN:
          if (velocity.y != -1) {
            velocity.x = 0;
            velocity.y = 1;
          }
          break;
        case SDLK_LEFT:
          if (velocity.x != 1) {
            velocity.x = -1;
            velocity.y = 0;
          }
          break;
        case SDLK_RIGHT:
          if (velocity.x != -1) {
            velocity.x = 1;
            velocity.y = 0;
          }
          break;
      }
    }
  }
}

void update() {

}

void render() {
  // Don't render if quit
  if (gameState == 0) return;

  // Reset
  clearRender(&engine);

  switch (gameState) {
    case 1: {
      update(); 

      SDL_RenderPresent(engine.renderer);
      break;
    }
    case 2: {
      // Render the start screen
      renderTextCentered(&engine, startTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
      // render
      SDL_RenderPresent(engine.renderer);
      break;
    }
  }
}

int main() {
  init();

  while (gameState != 0) {
    handleInput();
    render();
    SDL_Delay(90);
  }

  // cleanup
  SDL_DestroyTexture(startTexture);
  freeEngine(&engine);
  return 0;
}
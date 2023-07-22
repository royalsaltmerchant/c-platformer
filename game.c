#include "engine.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GRAVITY 0.5f
#define JUMP_VELOCITY -10.0f

typedef struct {
  float x, y;
  float vx, vy;
  int width, height;
  int onGround;
} Player;

typedef struct {
  int x, y;
  int width, height;
} Platform;

Engine engine;
Player player;
Platform platform;
int gameState;
SDL_Color textColor = {255, 255, 255, 255};
SDL_Texture* startTexture;

void initGame() {
  char *title = "New Game";
  char *fontLocation = "./assets/OpenSans-Regular.ttf";
  initEngine(&engine, title, SCREEN_WIDTH, SCREEN_HEIGHT, fontLocation, 24);
  // Game
  gameState = 2;  // Set to starting game state
  // graphics
  // start screen text
  startTexture = createTextTexture(&engine, "Press any key to start", textColor);
}

void initPlayer() {
  player.x = 100;
  player.y = 100;
  player.vx = 0;
  player.vy = 0;
  player.width = 50;
  player.height = 50;
  player.onGround = 0;
}

void initPlatform() {
  platform.x = 0;
  platform.y = SCREEN_HEIGHT - 100;
  platform.width = SCREEN_WIDTH;
  platform.height = 100;
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
        initPlayer();
        initPlatform();
        gameState = 1;
        return;
      }
      // directions
      switch (event.key.keysym.sym) {
        case SDLK_UP:
          if (player.onGround) {
            player.vy = JUMP_VELOCITY;
          }
          break;
        case SDLK_DOWN:
          // if (velocity.y != -1) {
          //   velocity.x = 0;
          //   velocity.y = 1;
          // }
          break;
        case SDLK_LEFT:
          // if (velocity.x != 1) {
          //   velocity.x = -1;
          //   velocity.y = 0;
          // }
          break;
        case SDLK_RIGHT:
          // if (velocity.x != -1) {
          //   velocity.x = 1;
          //   velocity.y = 0;
          // }
          break;
      }
    }
  }
}

void update() {
  player.vy += GRAVITY;
  player.x += player.vx;
  player.y += player.vy;

  if (player.y + player.height > platform.y && 
      player.x < platform.x + platform.width && 
      player.x + player.width > platform.x) {
    player.y = platform.y - player.height;
    player.vy = 0;
    player.onGround = 1;
  } else {
    player.onGround = 0;
  }
}

void render() {
  // Don't render if quit
  if (gameState == 0) return;

  // Reset
  clearRender(&engine);

  switch (gameState) {
    case 1: {
      update(); 


      SDL_Rect playerRect = {player.x, player.y, player.width, player.height};
      SDL_SetRenderDrawColor(engine.renderer, 255, 0, 0, 255);
      SDL_RenderFillRect(engine.renderer, &playerRect);
      
      SDL_Rect platformRect = {platform.x, platform.y, platform.width, platform.height};
      SDL_SetRenderDrawColor(engine.renderer, 0, 255, 0, 255);
      SDL_RenderFillRect(engine.renderer, &platformRect);

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
  initGame();

  while (gameState != 0) {
    handleInput();
    render();
    SDL_Delay(30);
  }

  // cleanup
  SDL_DestroyTexture(startTexture);
  freeEngine(&engine);
  return 0;
}
#include "engine.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GRAVITY 1.2f
#define JUMP_VELOCITY -20.0f
#define SPEED 8
#define GROUND_LEVEL 50

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
Platform platforms[3];
int gameState;
int scrollx;
int scrolly;
SDL_Color textColor;
SDL_Texture* startTexture;

void initGame() {
  char *title = "New Game";
  char *fontLocation = "./assets/OpenSans-Regular.ttf";
  initEngine(&engine, title, SCREEN_WIDTH, SCREEN_HEIGHT, fontLocation, 24);
  // Game
  gameState = 2;  // Set to starting game state
  // graphics
  // start screen text
  textColor = (SDL_Color){255, 255, 255, 255};
  startTexture = createTextTexture(&engine, "Press any key to start", textColor);
  // scroll
  scrollx = 0;
  scrolly = 0;
}

void initPlayer() {
  player.x = 100;
  player.y = 100;
  player.vx = 0;
  player.vy = 0;
  player.width = 20;
  player.height = 20;
  player.onGround = 0;
}

void initPlatforms() {
  Platform pf1, pf2, pf3;

  pf1.x = 0;
  pf1.y = SCREEN_HEIGHT - GROUND_LEVEL;
  pf1.width = 300;
  pf1.height = GROUND_LEVEL;

  pf2.x = 400;
  pf2.y = SCREEN_HEIGHT - 200;
  pf2.width = 80;
  pf2.height = 30;

  pf3.x = 600;
  pf3.y = SCREEN_HEIGHT - 300;
  pf3.width = 80;
  pf3.height = 30;

  platforms[0] = pf1;
  platforms[1] = pf2;
  platforms[2] = pf3;
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
        initPlatforms();
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

          break;
        case SDLK_LEFT:
          if (player.vx != -SPEED) {
            player.vx = -SPEED;
          }
          break;
        case SDLK_RIGHT:
          if (player.vx != SPEED) {
            player.vx = SPEED;
          }
          break;
      }
    }
    if (event.type == SDL_KEYUP) {
      // directions
      switch (event.key.keysym.sym) {
        case SDLK_UP:

          break;
        case SDLK_DOWN:

          break;
        case SDLK_LEFT:
          if (player.vx == -SPEED) {
            player.vx = 0;
          }
          break;
        case SDLK_RIGHT:
          if (player.vx == SPEED) {
            player.vx = 0;
          }
          break;
      }
    }
  }
}

void update() {
  // Update player
  player.vy += GRAVITY;
  player.x += player.vx;
  player.y += player.vy;
  
  player.onGround = 0; // Assume player is not on ground until we find a platform
  // End game when fall
  if (player.y > SCREEN_HEIGHT) {
    gameState = 2;
    return;
  }

  // Scroll
  scrollx = -player.x + (SCREEN_WIDTH / 2);
  if (scrollx > 0) {
    scrollx = 0;
  }
  scrolly = player.y - (SCREEN_HEIGHT / 2);
  if (player.y > (SCREEN_HEIGHT / 2)) {
    scrolly = 0;
  }

  // Platform collision
  for (int i = 0; i < 3; i++) {
    if (player.y + player.height > platforms[i].y && 
        player.x < platforms[i].x + platforms[i].width && 
        player.x + player.width > platforms[i].x) {
      player.y = platforms[i].y - player.height;
      player.vy = 0;
      player.onGround = 1;
    }
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


      SDL_Rect playerRect = {player.x + scrollx, player.y - scrolly, player.width, player.height};
      SDL_SetRenderDrawColor(engine.renderer, 255, 0, 0, 255);
      SDL_RenderFillRect(engine.renderer, &playerRect);
      
      for (int i = 0; i < 3; i++) {
        SDL_Rect platformRect = {platforms[i].x + scrollx, platforms[i].y - scrolly, platforms[i].width, platforms[i].height};
        SDL_SetRenderDrawColor(engine.renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(engine.renderer, &platformRect);

      }

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
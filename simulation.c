#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>
#include <stdbool.h>

typedef struct Vec
{
  float tComponent, xComponent;
} Vec;

int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    return -1;
  }

  int windowWidth = 940, windowHeight = 940, windowFlag = SDL_WINDOW_RESIZABLE;

  Vec initialVelocity = {0.0, 0.1};
  Vec initialRelativePosition = {0.0, 0.0};
  
  Vec velocity = initialVelocity;
  Vec relativePosition = initialRelativePosition;
  Vec realPosition = {relativePosition.tComponent + windowHeight/2, relativePosition.xComponent + windowWidth/2};
  float timeIncrement = 0.05;
  
  SDL_Window* window = SDL_CreateWindow("simulation", windowWidth, windowHeight, windowFlag);
  if (window == NULL)
  {
    SDL_Quit();
    return -1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
  
  if (renderer == NULL)
  {
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  SDL_SetRenderVSync(renderer, 1);

  bool continueLoop = true; 
  SDL_Event event;

  while (continueLoop)
  {
    SDL_PollEvent(&event);
    if (event.type == SDL_EVENT_QUIT)
      continueLoop = false;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPoint(renderer, realPosition.xComponent, realPosition.tComponent);
    SDL_RenderPresent(renderer);

    realPosition.xComponent = realPosition.xComponent + velocity.xComponent * timeIncrement * 10;
    realPosition.tComponent = realPosition.tComponent - timeIncrement * 10;
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

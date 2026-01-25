#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>
#include <stdbool.h>

int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    return -1;
  }

  int windowWidth = 1080, windowHeight = 940;

  SDL_Window* window = SDL_CreateWindow("simulation", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
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

  float xVelocity = 1;
  float relativeXCoordinate = 0, relativeTimeCoordinate = 0;
  float timeIncrement = 0.05;
  float realXCoordinate = relativeXCoordinate + windowWidth/2;
  float realTimeCoordinate = relativeTimeCoordinate + windowHeight/2;
  
  bool continueLoop = true; 
  SDL_Event event;

  while (continueLoop)
  {
    SDL_PollEvent(&event);
    if (event.type == SDL_EVENT_QUIT)
      continueLoop = false;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPoint(renderer, realXCoordinate, realTimeCoordinate);
    SDL_RenderPresent(renderer);

    realXCoordinate += xVelocity * timeIncrement * 10;
    realTimeCoordinate -= timeIncrement * 10;
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

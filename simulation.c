#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>

int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    return -1;
  }

  int windowWidth = 680, windowHeight = 420;

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
  
  bool continueLoop = true; 
  SDL_Event event;

  while (continueLoop)
  {
    SDL_PollEvent(&event);
    if (event.type == SDL_EVENT_QUIT)
      continueLoop = false;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

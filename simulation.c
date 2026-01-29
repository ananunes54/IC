#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>
#include <stdbool.h>

typedef struct Vec
{
  float tComponent, xComponent;
} Vec;

typedef struct M2x2
{
  float m00, m01, m10, m11;
} M2x2;


M2x2 AuxM2x2 (Vec vector1, Vec vector2, float timeIncrement);
Vec VecM2x2Multiplication (Vec vector, M2x2 matrix);
Vec VecScale (Vec vector, float factor);
Vec ApplyMetricTensor (Vec vector);


M2x2 AuxM2x2 (Vec vector1, Vec vector2, float timeIncrement)
{
  M2x2 auxMatrix;
  Vec covector1 = ApplyMetricTensor(vector1);
  Vec covector2 = ApplyMetricTensor(vector2);
  auxMatrix.m00 = (vector1.tComponent * covector2.tComponent - vector2.tComponent * covector1.tComponent) + 1;
  auxMatrix.m01 = (vector1.tComponent * covector2.xComponent - vector2.tComponent * covector1.xComponent) * timeIncrement;
  auxMatrix.m10 = (vector1.xComponent * covector2.tComponent - vector2.xComponent * covector1.tComponent) * timeIncrement;
  auxMatrix.m11 = (vector1.xComponent * covector2.xComponent - vector2.xComponent * covector1.xComponent) + 1;
  return auxMatrix;
}

Vec VecM2x2Multiplication (Vec vector, M2x2 matrix)
{
  Vec auxVector;
  auxVector.tComponent = vector.tComponent * matrix.m00 + vector.xComponent * matrix.m01;
  auxVector.xComponent = vector.tComponent * matrix.m10 + vector.xComponent * matrix.m11;
  return auxVector;
}

Vec VecScale (Vec vector, float factor)
{
  Vec auxVector = {vector.tComponent * factor, vector.xComponent * factor};
  return auxVector;
}

Vec ApplyMetricTensor (Vec vector)
{
  Vec auxVector = {vector.tComponent, - (vector.xComponent)};
  return auxVector;
}


int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    return -1;
  }

  int windowWidth = 2080, windowHeight = 940, windowFlag = SDL_WINDOW_RESIZABLE;

  Vec initialVelocity = {1.0, 0.0};
  Vec initialRelativePosition = {0.0, 0.0};
  Vec initialAcceleration = {0.0, 0.05};

  Vec velocity = initialVelocity;
  Vec acceleration = initialAcceleration;
  Vec relativePosition = initialRelativePosition;
  Vec realPosition = {relativePosition.tComponent + windowHeight/2, relativePosition.xComponent + windowWidth/2};
  float timeIncrement = 0.005;
  
  M2x2 lambdaMatrix = AuxM2x2(acceleration, velocity, timeIncrement);
  printf("%f %f %f %f\n", lambdaMatrix.m00, lambdaMatrix.m01, lambdaMatrix.m10, lambdaMatrix.m11);

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

  float referenceLineX = windowWidth / 2;
  float referenceLineT = windowHeight / 2;

  while (continueLoop)
  {
    SDL_PollEvent(&event);
    if (event.type == SDL_EVENT_QUIT)
      continueLoop = false;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPoint(renderer, realPosition.xComponent, realPosition.tComponent);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderPoint(renderer, referenceLineX, referenceLineT);
    referenceLineX += timeIncrement * 5;
    referenceLineT -= timeIncrement * 5;

    SDL_RenderPresent(renderer);
    velocity = VecM2x2Multiplication(velocity, lambdaMatrix);
    acceleration = VecM2x2Multiplication(acceleration, lambdaMatrix);
    lambdaMatrix = AuxM2x2(acceleration, velocity, timeIncrement);
    realPosition.xComponent = realPosition.xComponent + velocity.xComponent * timeIncrement * 15;
    realPosition.tComponent = realPosition.tComponent - velocity.tComponent * timeIncrement * 15;
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

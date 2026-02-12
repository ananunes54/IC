#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>
#include <stdbool.h>
#include <matrix_exponential.h>
#include <math.h>

typedef struct Vec
{
	float tComponent, xComponent, yComponent;
} Vec;



Vec M3x3vecMultiplication (double matrix[], Vec vector);
Vec VecScale (Vec vector, float factor);
Vec ApplyMetricTensor (Vec vector);
Vec VecSum2(Vec vector1, Vec vector2);
Vec VecSum3(Vec vector1, Vec vector2, Vec vector3);


Vec M3x3vecMultiplication (double matrix[], Vec vector)
{
	Vec auxVector1 = {matrix[0], matrix[3], matrix[6]};
	Vec auxVector2 = {matrix[1], matrix[4], matrix[7]};
	Vec auxVector3 = {matrix[2], matrix[5], matrix[8]};

	auxVector1 = VecScale(auxVector1, vector.tComponent);
	auxVector2 = VecScale(auxVector2, vector.xComponent);
	auxVector3 = VecScale(auxVector3, vector.yComponent);

	return VecSum3(auxVector1, auxVector2, auxVector3);
}

Vec VecSum2(Vec vector1, Vec vector2)
{
	Vec auxVector;
	auxVector.tComponent = vector1.tComponent + vector2.tComponent;
	auxVector.xComponent = vector1.xComponent + vector2.xComponent;
	auxVector.yComponent = vector1.yComponent + vector2.yComponent;
	return auxVector;
}

Vec VecSum3(Vec vector1, Vec vector2, Vec vector3)
{
	Vec auxVector;
	auxVector.tComponent = vector1.tComponent + vector2.tComponent + vector3.tComponent;
	auxVector.xComponent = vector1.xComponent + vector2.xComponent + vector3.tComponent;
	auxVector.yComponent = vector1.yComponent + vector2.yComponent + vector3.tComponent;
	return auxVector;
}

Vec VecScale (Vec vector, float factor)
{
	Vec auxVector = {vector.tComponent * factor, vector.xComponent * factor, vector.yComponent * factor};
	return auxVector;
}

Vec ApplyMetricTensor (Vec vector)
{
	Vec auxVector = {vector.tComponent, - (vector.xComponent), - (vector.yComponent)};
	return auxVector;
}

void ArrayScale (const double *sourceArray, double *array, int d, double factor)
{
	for (int i = 0; i < d; i++)
	{
		array[i] = sourceArray[i] * factor;
	}
	return;
}


int main()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		return -1;
	}

	int windowWidth = 720, windowHeight = 720, windowFlag = SDL_WINDOW_RESIZABLE;


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

	//////////////////////////////////////

	float referencePointX = windowWidth / 2;
	float referencePointT = windowHeight / 2;

	double constOmega[9] = {
		0, 0, 0,
		0, 0, -3,
		0, 3, 0
	};

	double timeScaledOmega[9] = {0};

	double *omegaExponential;

	double v = 0.05;
	double gamma = 1/sqrt(1 - pow(v, 2));
	Vec velocity = {gamma, gamma * v, 0};
	Vec realPosition = {0, 0, 0};

	double elapsedTime = 0.005;
	double timeStep = 0.005;
	
	/////////////////////////////////////

	SDL_SetRenderVSync(renderer, 1);

	bool continueLoop = true; 
	SDL_Event event;

	while (continueLoop)
	{

		ArrayScale(constOmega, timeScaledOmega, 9, elapsedTime);

		omegaExponential = r8mat_expm1(3, timeScaledOmega);

		velocity = M3x3vecMultiplication(omegaExponential, velocity);

		realPosition = VecSum2(realPosition, VecScale(velocity, timeStep * 50));
			
		free(omegaExponential);

		elapsedTime += timeStep;

		///////////////////////////////////////////////////

		SDL_PollEvent(&event);
		if (event.type == SDL_EVENT_QUIT)
			continueLoop = false;

		//renderiza o ponto que estaria em movimento
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderPoint(renderer, referencePointX + realPosition.xComponent , referencePointT - realPosition.tComponent);

		//atualiza a tela
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

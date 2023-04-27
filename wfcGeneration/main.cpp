#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#include "main.h"
#include "world.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")		

int main()
{

	srand((unsigned int)time(NULL));

	// Apparantly we can just jump into SDL
	SDL_Event event;
	SDL_Renderer* renderer;
	SDL_Window* window;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	Tile mapArray[50][50];

	initializeMapArray(mapArray);
	linkMapArray(mapArray);
	drawMapArray(renderer, mapArray);


	//for (int i = 0; i < WINDOW_WIDTH; ++i) {	// Fun Mode
	//	for (int j = 0; j < WINDOW_HEIGHT; ++j) {
	//		
	//		int color = 16 * (i / 16 + j / 16) % 256;

	//		SDL_SetRenderDrawColor(renderer, color, 0, color, 255);
	//		SDL_RenderDrawPoint(renderer, i, j);
	//	}
	//}

	SDL_RenderPresent(renderer);
	while (1) {
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}


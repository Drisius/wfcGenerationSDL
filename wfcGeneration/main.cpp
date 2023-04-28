#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <vector>

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
	SDL_CreateWindowAndRenderer(SPRITE_SIZE * X_DIMENSION, SPRITE_SIZE * Y_DIMENSION, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	std::vector<std::vector<Tile>> mapArray;

	mapArray = initializeVector(X_DIMENSION, Y_DIMENSION, SPRITE_SIZE, RANDOM_COLORS);
	initializeCoordinates(mapArray);
	linkMapArray(mapArray);

	Tile* randomTile = &mapArray[rand() % mapArray.size()][rand() % mapArray[1].size()];

	wfc_4pt(randomTile, true);

	drawMapArray(renderer, mapArray);

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
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

	Tile* randomTile = &mapArray[rand() % mapArray.size()][rand() % mapArray[0].size()];

	//wfc_4pt_weighted(randomTile, true);
	//wfc_2snake_weighted(randomTile);
	gol_seedMapOcean(mapArray);

	//gol_updateMap(mapArray, GOL_UPDATE_PASSES);
	gol_updateMapSnake(mapArray, GOL_UPDATE_PASSES);

	drawMapArray(renderer, mapArray);
	SDL_RenderPresent(renderer);
	Sleep(3000);

	wfc_segment(mapArray);
	scorchedEarthFill(mapArray);
	// wfc_2snake_weighted(randomTile);
	// wfc_lock(mapArray);
	
	drawMapArray(renderer, mapArray);
	SDL_RenderPresent(renderer);
	Sleep(3000);

	// commonFill(mapArray);

	drawMapArray(renderer, mapArray);
	SDL_RenderPresent(renderer);
	Sleep(3000);

	// doubleFill(mapArray, 5);
	tripleFill(mapArray, 20, false);
	// tileFill(mapArray, 2, 20, 1);
	// tileFillAllDirections(mapArray, 2, 20, 1);

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
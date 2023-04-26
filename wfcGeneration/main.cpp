#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL2/SDL.h>

#include "main.h"
#include "map.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")		

int main()
{
	// Apparantly we can just jump into SDL
	SDL_Event event;
	SDL_Renderer *renderer;
	SDL_Window *window;
	int i;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (i = 0; i < WINDOW_WIDTH; ++i)
		SDL_RenderDrawPoint(renderer, i, i);
	SDL_RenderPresent(renderer);
	while (1) {
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


#include <SDL2/SDL.h>

#include "world.h"


Tile::Tile()
{
	r = rand() % 256;
	g = rand() % 256;
	b = rand() % 256;
};

Tile::Tile(int x, int y, int red, int green, int blue)
{
	tileCoordinateX = x;
	tileCoordinateY = y;
	r = red;
	g = green;
	b = blue;
}

void Tile::drawTile(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			SDL_RenderDrawPoint(renderer, tileCoordinateX + i, tileCoordinateY + j);
		}
	}
}

void initializeMapArray(Tile arrayMap[50][50])
{
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			(arrayMap[i][j]).tileCoordinateX = i * 16; 
			(arrayMap[i][j]).tileCoordinateY = j * 16;
		}
	}
}

void linkMapArray(Tile arrayMap[50][50])
{
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			
			if (i > 0) {
				(arrayMap[i][j]).neighbors[3] = &arrayMap[i - 1][j];
			}

			if (i < 49) {
				(arrayMap[i][j]).neighbors[1] = &arrayMap[i + 1][j];
			}

			if (j > 0) {
				(arrayMap[i][j]).neighbors[0] = &arrayMap[i][j - 1];
			}

			if (j < 49) {
				(arrayMap[i][j]).neighbors[2] = &arrayMap[i][j + 1];
			}
		}
	}
}


std::set<int> returnSet(int n)
{
	auto search = partners.find(n);
	if (search != partners.end()) {
		return search->second;
	}
	else {
		return {};
	}
}

std::set<int> intersectSets(std::vector<int> altitudes)
{

	if (altitudes.size() == 1) {
		return returnSet(altitudes[0]);
	}

}

void drawMapArray(SDL_Renderer* renderer, Tile arrayMap[50][50])
{
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			(arrayMap[i][j]).drawTile(renderer);
		}
	}
}

void wfc(Tile* tPtr)
{
	if (tPtr == nullptr || tPtr->tileCoordinateZ != 0) {
		return;
	}

	std::vector<int> altitudes;

	for (int i = 0; i < (tPtr->neighbors).size(); ++i) {
		Tile* neighborPtr = tPtr->neighbors[i];
		if (neighborPtr != nullptr && neighborPtr->tileCoordinateZ != 0) {
			altitudes.push_back(neighborPtr->tileCoordinateZ);
		}
	}


	std::set<int> possiblePartners = intersectSets(altitudes);

}
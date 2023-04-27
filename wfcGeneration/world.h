#pragma once

#include <stdlib.h>
#include <map>
#include <set>
#include <algorithm>
#include <vector>

const std::map<int, std::set<int>> partners =	{

													{1, {1, 2}},
													{2, {1, 2, 3}},
													{3, {2, 3, 4}},
													{4, {3, 4}}

												};

struct Tile
{
	// 0 North and clockwise

	std::vector<Tile*> neighbors = {nullptr, nullptr, nullptr, nullptr};

	int tileCoordinateX = 0;
	int tileCoordinateY = 0;
	int tileCoordinateZ = 0;

	int r = 0;
	int g = 0;
	int b = 0;
	int a = 255;

	int size = 16;

	Tile();
	Tile(int x, int y, int red, int green, int blue);
	void drawTile(SDL_Renderer* renderer);
};

void initializeMapArray(Tile arrayMap[50][50]);
void linkMapArray(Tile arrayMap[50][50]);
void drawMapArray(SDL_Renderer* renderer, Tile arrayMap[50][50]);
void wfc(Tile* tPtr);
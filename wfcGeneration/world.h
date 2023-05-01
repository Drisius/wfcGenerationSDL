#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <algorithm>
#include <vector>
#include <iterator>

enum Color
{
	NONE, BLUE, YELLOW, GREEN, RED, BLACK, PURPLE
};

const std::map<int, std::set<int>> partners =	{

													{1, {1, 2}},
													{2, {1, 2, 3}},
													{3, {2, 3, 4}},
													{4, {3, 4}}
													// ,{5, {4, 5}} -- More kinds of terrain makes it harder to place consistently

												};

struct Tile
{
	// 0 North and clockwise

	std::vector<Tile*> neighbors = {nullptr, nullptr, nullptr, nullptr};
	std::vector<Tile*> dNeighbors = {nullptr, nullptr, nullptr, nullptr};

	int tileCoordinateX = 0;
	int tileCoordinateY = 0;
	Color tileCoordinateZ = NONE;

	int r = 0;
	int g = 0;
	int b = 0;
	int a = 255;

	int tileSize = 16;

	Tile() {};
	Tile(int size, bool randomizeColors);
	Tile(int x, int y, int red, int green, int blue);
	void drawTile(SDL_Renderer* renderer);
	void setColor();
};

Color intToColor(int n);

std::vector<std::vector<Tile>> initializeVector(int X, int Y, int size, bool randomizeColors);
void initializeCoordinates(std::vector<std::vector<Tile>>& arrayMap);
void linkMapArray(std::vector<std::vector<Tile>>& arrayMap, bool linkDiagonal = false);
void drawMapArray(SDL_Renderer* renderer, std::vector<std::vector<Tile>>& arrayMap);

void setPossiblePartner_4Pt(Tile* tilePtr);
void setPossiblePartner_8Pt(Tile* tilePtr);
void setCoherentPossiblePartner_4Pt(Tile* tilePtr);

std::set<int> returnSet(int n);
std::set<int> intersectTwoSets(std::set<int> a, std::set<int> b);
std::set<int> intersectSets(std::vector<int> altitudes);

void wfc_4pt(Tile* tPtr, bool init);
void wfc_duplicate4pt(Tile* tPtr, bool init);
void wfc_8pt(Tile* tPtr, bool init);
void wfc_2snake(Tile* tPtr);
void wfc_duplicate2snake(Tile* tPtr);
void verticalFill(std::vector<std::vector<Tile>>& arrayMap);
void horizontalFill(std::vector<std::vector<Tile>>& arrayMap);

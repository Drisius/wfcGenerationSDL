#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <algorithm>
#include <vector>
#include <iterator>
#include <windows.h>
#include <string>

static constexpr int BIRTH_RATE = 34;			// Standard 35ish
static constexpr int BIRTH_DEATH_NUMBER = 4;	// Standard 4ish

enum Color
{
	NONE, BLUE, LIGHT_BLUE, YELLOW, GREEN, RED, BROWN, PURPLE, WHITE
};

const std::map<int, std::set<int>> partners =	{

													{1, {1, 2}},
													{2, {/*1,*/ 2, 3}},
													{3, {2, 3, 4}},
													{4, {3, 4, 5}}
													,{5, {4, 5, 6}} // -- More kinds of terrain makes it harder to place consistently
													,{6, {5, 6, 7}}
													,{7, {6, 7, 8}}
													,{8, {7, 8}}

												};

const std::vector<int> tileChance = {0, 99, 50, 50, 60, 60, 60, 60, 60, 60};

//const std::vector<std::vector<int>> transitionMatrix =	{
//														
//															{50, 10, 10, 10, 10, 10},
//															{10, 50, 10, 10, 10, 10},
//															{10, 10, 50, 10, 10, 10},
//															{10, 10, 10, 50, 10, 10},
//															{10, 10, 10, 10, 50, 10},
//															{10, 10, 10, 10, 10, 50}
//
//														};

struct Tile
{
	// 0 North and clockwise

	std::vector<Tile*> neighbors = {nullptr, nullptr, nullptr, nullptr};
	std::vector<Tile*> dNeighbors = {nullptr, nullptr, nullptr, nullptr};

	int maxHeight = (int) partners.size();
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

	bool operator == (const Tile &t);
};

Color intToColor(int n);

int randRangeInclusive(int min, int max);

std::vector<std::vector<Tile>> initializeVector(int X, int Y, int size, bool randomizeColors);
void initializeCoordinates(std::vector<std::vector<Tile>>& arrayMap);
int convertCoordinateToGrid(int coordinate);
int convertGridToCoordinate(int gridCoordinate);
int countEmptyTiles(std::vector<std::vector<Tile>>& arrayMap);
std::vector<Tile*> findEmptyTiles(std::vector<std::vector<Tile>>& arrayMap);
void linkMapArray(std::vector<std::vector<Tile>>& arrayMap);
void drawMapArray(SDL_Renderer* renderer, std::vector<std::vector<Tile>>& arrayMap);

void setPossiblePartner_4Pt(Tile* tilePtr);
void setPossibleWeightedPartner_4Pt(Tile* tilePtr);
void setPossiblePartner_8Pt(Tile* tilePtr);
void setCoherentPossiblePartner_4Pt(Tile* tilePtr);

std::set<int> returnSet(int n);
std::set<int> intersectTwoSets(std::set<int> a, std::set<int> b);
std::set<int> intersectSets(std::vector<int> altitudes);

void wfc_4pt(Tile* tPtr, bool init);
void wfc_4pt_weighted(Tile* tPtr, bool init);
void wfc_duplicate4pt(Tile* tPtr, bool init);
void wfc_8pt(Tile* tPtr, bool init);
void wfc_2snake(Tile* tPtr);
void wfc_duplicate2snake(Tile* tPtr);
void wfc_2snake_weighted(Tile* tPtr);
void wfc_lock(std::vector<std::vector<Tile>>& arrayMap);
void wfc_segment(std::vector<std::vector<Tile>>& arrayMap);

bool isValidTile(int i, int j);
bool isEmptyAdjacentTile(Tile t);
int find_next_empty_tile(std::vector<Tile> v, int startPosition);
int find_next_full_tile(std::vector<Tile> v, int startPosition);
bool vectorContainsZeroTile(std::vector<Tile> v);
int checkSpecificPartner(std::vector<std::vector<Tile>>& arrayMap, Color tileType, int i, int j);
Color returnThreeOrMoreIdenticalColors(std::vector<Color> v);
int countColors(std::vector<Color> v, Color c);

void gol_seedMapOcean(std::vector<std::vector<Tile>>& arrayMap);
void gol_updateMap(std::vector<std::vector<Tile>>& arrayMap, int amountPasses);
void gol_updateMapSnake(std::vector<std::vector<Tile>>& arrayMap, int amountPasses);

void scorchedEarthFill(std::vector<std::vector<Tile>>& arrayMap, bool diagonal = false);
void verticalFill(std::vector<std::vector<Tile>>& arrayMap, int passes);
void horizontalFill(std::vector<std::vector<Tile>>& arrayMap, int passes);
void doubleFill(std::vector<std::vector<Tile>>& arrayMap, int passes);
bool containedInVector(std::vector<Tile> v, Tile t);
void floodFill(Tile tile, std::vector<Tile>& vectorFoundTiles);
void tileFill(std::vector<std::vector<Tile>>& arrayMap, int tileType, int targetNum, int targetType);
void floodFillAllDirections(Tile tile, std::vector<Tile>& vectorFoundTiles);
void tileFillAllDirections(std::vector<std::vector<Tile>>& arrayMap, int tileType, int targetNum, int targetType);
void commonFill(std::vector<std::vector<Tile>>& arrayMap);
void tripleFill(std::vector<std::vector<Tile>>& arrayMap, int passes, bool diagonal = false);
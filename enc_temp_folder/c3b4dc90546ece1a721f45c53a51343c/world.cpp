#include <SDL2/SDL.h>
#include <string>
#include <Windows.h>

#include "world.h"

Tile::Tile(int size, bool randomizeColors)
{	
	if (randomizeColors) {
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;
	}

	tileSize = size;
}

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

	for (int i = 0; i < tileSize; ++i) {
		for (int j = 0; j < tileSize; ++j) {
			SDL_RenderDrawPoint(renderer, tileCoordinateX + i, tileCoordinateY + j);
		}
	}
}

void Tile::setColor()
{
	switch (tileCoordinateZ)
	{
	case RED:
		r = 255;
		break;
	case BLUE:
		b = 255;
		break;
	case GREEN:
		g = 255;
		break;
	case YELLOW:
		r = 255;
		g = 255;
		break;
	case BLACK:
		r = 0;
		g = 0;
		b = 0;
		break;
	default:
		break;
	}
}

Color intToColor(int n) {
	switch (n)
	{
	case(4):
		return RED;
		break;
	case(3):
		return GREEN;
		break;
	case(2):
		return YELLOW;
		break;
	case(1):
		return BLUE;
		break;
	default:
		return NONE;
	}
}

std::vector<std::vector<Tile>> initializeVector(int X, int Y, int size, bool randomizeColors)
{	
	std::vector<std::vector<Tile>> tempX;

	for (int i = 0; i < X; ++i) {
		std::vector<Tile> tempY;
		for (int j = 0; j < Y; ++j) {
			Tile t(size, randomizeColors);
			tempY.push_back(t);
		}
		tempX.push_back(tempY);
	}

	return tempX;
}

void initializeCoordinates(std::vector<std::vector<Tile>>& arrayMap)
{
	for (int i = 0; i < arrayMap.size(); ++i) {
		for (int j = 0; j < arrayMap[i].size(); ++j) {
			(arrayMap[i][j]).tileCoordinateX = i * (arrayMap[i][j]).tileSize;
			(arrayMap[i][j]).tileCoordinateY = j * (arrayMap[i][j]).tileSize;
		}
	}
}

void linkMapArray(std::vector<std::vector<Tile>>& arrayMap)
{
	for (int i = 0; i < arrayMap.size(); ++i) {
		for (int j = 0; j < arrayMap[i].size(); ++j) {
			
			if (i > 0) {
				(arrayMap[i][j]).neighbors[3] = &arrayMap[i - 1][j];
			}

			if (i < arrayMap.size() - 1) {
				(arrayMap[i][j]).neighbors[1] = &arrayMap[i + 1][j];
			}

			if (j > 0) {
				(arrayMap[i][j]).neighbors[0] = &arrayMap[i][j - 1];
			}

			if (j < arrayMap[1].size() - 1) {
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

std::set<int> intersectTwoSets(std::set<int> a, std::set<int> b)
{
	std::set<int> tempSet = {};
	if (a.size() == 0 or b.size() == 0) {
		return tempSet;
	}

	for (auto& element : a) {
		auto search = b.find(element);
		if (search != b.end()) {
			tempSet.insert(element);
		}
	}

	return tempSet;
}

std::set<int> intersectSets(std::vector<int> altitudes)
{
	int amountSets = (int) altitudes.size();

	switch (amountSets) {

	case(0):
		return {};
		break;

	case(1):
		return returnSet(altitudes[0]);
		break;

	case(2):
		return intersectTwoSets(returnSet(altitudes[0]), returnSet(altitudes[1]));
		break;

	default:
		std::set<int> tempSet = returnSet(altitudes[0]);

		for (int i = 1; i < altitudes.size(); ++i) {
			tempSet = intersectTwoSets(tempSet, returnSet(altitudes[i]));
		}

		return tempSet;

	}

}

void drawMapArray(SDL_Renderer* renderer, std::vector<std::vector<Tile>>& arrayMap)
{
	for (int i = 0; i < arrayMap.size(); ++i) {
		for (int j = 0; j < arrayMap[i].size(); ++j) {
			(arrayMap[i][j]).drawTile(renderer);
		}
	}
}

void wfc_4pt(Tile* tPtr, bool init)		// This algorithm runs recursively and vertically/diagonally; It does -not- guarantee a complete map coverage.
{
	if (tPtr == nullptr || (tPtr->tileCoordinateZ != NONE && !init)) {
		return;
	}

	std::vector<int> altitudes;

	for (int i = 0; i < (tPtr->neighbors).size(); ++i) {
		Tile* neighborPtr = tPtr->neighbors[i];
		if (neighborPtr != nullptr && neighborPtr->tileCoordinateZ != NONE) {
			altitudes.push_back(neighborPtr->tileCoordinateZ);
		}
	}

	if (init) {
		tPtr->r = 128;
		tPtr->g = 0;
		tPtr->b = 255;
		tPtr->setColor();
		tPtr->tileCoordinateZ = intToColor(4);
	} else {

		std::set<int> possiblePartners = intersectSets(altitudes);

		if (possiblePartners.size() != 0) {
			auto begin = possiblePartners.begin();
			std::advance(begin, rand() % possiblePartners.size());
			tPtr->tileCoordinateZ = intToColor(*begin);
			tPtr->setColor();
		}
		else {
			OutputDebugStringA(const_cast<char *>(std::to_string(tPtr->tileCoordinateX).c_str()));	// -- Our old friend
			OutputDebugStringA(const_cast<char *>(std::to_string(tPtr->tileCoordinateY).c_str()));
			OutputDebugStringA("\n");
			tPtr->tileCoordinateZ = intToColor(5);
			tPtr->setColor();
			tPtr->tileCoordinateZ = intToColor(4);
		}
	}

	for (int i = 0; i < (tPtr->neighbors).size(); ++i) {
		wfc_4pt(tPtr->neighbors[i], false);
	}
}
#include "world.h"
#include "main.h"

Tile::Tile(int size, bool randomizeColors)
{	
	if (randomizeColors) {
		r = randRangeInclusive(0, 255);
		g = randRangeInclusive(0, 255);
		b = randRangeInclusive(0, 255);
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

bool Tile::operator == (const Tile &t)
{
	if (tileCoordinateX == t.tileCoordinateX && tileCoordinateY == t.tileCoordinateY) {
		return true;
	}

	return false;
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
		g = 0;
		b = 0;
		break;
	case BLUE:
		r = 0;
		b = 255;
		g = 0;
		break;
	case GREEN:
		r = 0;
		g = 255;
		b = 0;
		break;
	case YELLOW:
		r = 255;
		g = 255;
		b = 0;
		break;
	case BROWN:
		r = 128;
		g = 64;
		b = 0;
		break;
	case PURPLE:
		r = 128;
		g = 0;
		b = 255;
		break;
	case LIGHT_BLUE:
		r = 0;
		g = 255;
		b = 255;
		break;
	case NONE:
		r = 0;
		g = 0;
		b = 0;
		break;
	default:
		break;
	}
}

int randRangeInclusive(int min, int max)
{
	return rand() % (max - min + 1) + min;
}


Color intToColor(int n) {
	switch (n)
	{
	case(7):
		return PURPLE;
		break;
	case(6):
		return BROWN;
		break;
	case(5):
		return RED;
		break;
	case(4):
		return GREEN;
		break;
	case(3):
		return YELLOW;
		break;
	case(2):
		return LIGHT_BLUE;
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

int convertCoordinateToGrid(int coordinate)
{
	return coordinate / SPRITE_SIZE;
}

int convertGridToCoordinate(int gridCoordinate)
{
	return gridCoordinate * SPRITE_SIZE;
}

void linkMapArray(std::vector<std::vector<Tile>>& arrayMap, bool linkDiagonal)
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

	if (linkDiagonal) {
		for (int i = 0; i < arrayMap.size(); ++i) {
			for (int j = 0; j < arrayMap[i].size(); ++j) {

				if (i - 1 >= 0 && j - 1 >= 0) {
					(arrayMap[i][j]).neighbors[3] = &arrayMap[i - 1][j];
				}

				if (i + 1 < arrayMap.size() && j + 1 < arrayMap[i].size()) {
					(arrayMap[i][j]).neighbors[1] = &arrayMap[i + 1][j];
				}

				if (i + 1 < arrayMap.size() && j - 1 >= 0) {
					(arrayMap[i][j]).neighbors[0] = &arrayMap[i][j - 1];
				}

				if (i - 1 >= 0 && j + 1 < arrayMap[1].size()) {
					(arrayMap[i][j]).neighbors[2] = &arrayMap[i][j + 1];
				}
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

int findDuplicateEntry(std::vector<int> v)
{
	if (v.size() == 0 || v.size() == 1) {
		return 0;
	}

	if (v.size() == 2) {
		if (v[0] == v[1]) {
			return v[0];
		}
		else {
			return 0;
		}

	}

	for (int i = 0; i < v.size() - 1; ++i) {
		for (int j = i + 1; j < v.size(); ++j) {
			if (v[i] == v[j]) {
				return v[i];
			}
		}
	}

	return 0;
}

void drawMapArray(SDL_Renderer* renderer, std::vector<std::vector<Tile>>& arrayMap)
{
	for (int i = 0; i < arrayMap.size(); ++i) {
		for (int j = 0; j < arrayMap[i].size(); ++j) {
			(arrayMap[i][j]).drawTile(renderer);
		}
	}
}

// Possible partner calculation for 4 adjacent tiles
void setPossiblePartner_4Pt(Tile* tilePtr)
{
	std::vector<int> altitudes;

	for (int i = 0; i < (tilePtr->neighbors).size(); ++i) {
		Tile* neighborPtr = tilePtr->neighbors[i];
		if (neighborPtr != nullptr && neighborPtr->tileCoordinateZ != NONE) {
			altitudes.push_back(neighborPtr->tileCoordinateZ);
		}
	}

	std::set<int> possiblePartners = intersectSets(altitudes);

	if (possiblePartners.size() == 0) {
		return;
	}
	else {
		auto begin = possiblePartners.begin();
		std::advance(begin, randRangeInclusive(0, (int) possiblePartners.size() - 1));
		tilePtr->tileCoordinateZ = intToColor(*begin);
		tilePtr->setColor();
	}
}

// Gives a weighted chance to place more tiles relative to itself 

void setPossibleWeightedPartner_4Pt(Tile* tilePtr)
{
	std::vector<int> altitudes;

	for (int i = 0; i < (tilePtr->neighbors).size(); ++i) {
		Tile* neighborPtr = tilePtr->neighbors[i];
		if (neighborPtr != nullptr && neighborPtr->tileCoordinateZ != NONE) {
			altitudes.push_back(neighborPtr->tileCoordinateZ);
		}
	}

	std::set<int> possiblePartners = intersectSets(altitudes);

	if (possiblePartners.size() == 0) {
		return;
	}
	else {
		auto begin = possiblePartners.begin();

		while (tilePtr->tileCoordinateZ == NONE) {
			auto offset = begin;
			std::advance(offset, randRangeInclusive(0, (int) possiblePartners.size() - 1));

			if (randRangeInclusive(0, 100) > tileChance[(*offset) - 1]) {
				tilePtr->tileCoordinateZ = intToColor(*offset);
				tilePtr->setColor();
			}
			else {
				offset = begin;
				std::advance(offset, randRangeInclusive(0, (int) possiblePartners.size() - 1));
			}
		}
	}
}

// Possible partner calculation for 8 surrounding tiles
void setPossiblePartner_8Pt(Tile* tilePtr)
{
	std::vector<int> altitudes;

	for (int i = 0; i < (tilePtr->neighbors).size(); ++i) {
		Tile* neighborPtr = tilePtr->neighbors[i];
		if (neighborPtr != nullptr && neighborPtr->tileCoordinateZ != NONE) {
			altitudes.push_back(neighborPtr->tileCoordinateZ);
		}
	}

	for (int i = 0; i < (tilePtr->dNeighbors).size(); ++i) {
		Tile* neighborPtr = tilePtr->dNeighbors[i];
		if (neighborPtr != nullptr && neighborPtr->tileCoordinateZ != NONE) {
			altitudes.push_back(neighborPtr->tileCoordinateZ);
		}
	}

	std::set<int> possiblePartners = intersectSets(altitudes);

	if (possiblePartners.size() == 0) {
		return;
	}
	else {
		auto begin = possiblePartners.begin();
		std::advance(begin, randRangeInclusive(0, (int) possiblePartners.size() - 1));
		tilePtr->tileCoordinateZ = intToColor(*begin);
		tilePtr->setColor();
	}
}

// Possible partner calculation for 4 adjacent tiles, if two are identical, force that one
// To be used with snake algorithm; never has more than two partners
void setCoherentPossiblePartner_4Pt(Tile* tilePtr)
{
	std::vector<int> altitudes;

	for (int i = 0; i < (tilePtr->neighbors).size(); ++i) {
		Tile* neighborPtr = tilePtr->neighbors[i];
		if (neighborPtr != nullptr && neighborPtr->tileCoordinateZ != NONE) {
			altitudes.push_back(neighborPtr->tileCoordinateZ);
		}
	}

	int sameNumberNeighbors = findDuplicateEntry(altitudes);

	if (sameNumberNeighbors != 0) {
		tilePtr->tileCoordinateZ = intToColor(sameNumberNeighbors);
		tilePtr->setColor();
	}
	else {
		std::set<int> possiblePartners = intersectSets(altitudes);

		if (possiblePartners.size() == 0) {
			return;
		}
		else {
			auto begin = possiblePartners.begin();
			std::advance(begin, randRangeInclusive(0, (int) possiblePartners.size() - 1));
			tilePtr->tileCoordinateZ = intToColor(*begin);
			tilePtr->setColor();
		}
	}
}

// This algorithm runs recursively and vertically/diagonally; It does -not- guarantee a complete map coverage.
// It also scales terribly - too many tiles and stack overflow occurs.

void wfc_4pt(Tile* tPtr, bool init)
{
	if (tPtr == nullptr || (tPtr->tileCoordinateZ != NONE && !init)) {
		return;
	}

	if (init) {
		// Turns the first tile purple then changes it to a "accessible" tile i.e. one that participates in the actual tiling; 1, 2, 3, 4
		tPtr->tileCoordinateZ = PURPLE; 
		tPtr->setColor();
		tPtr->tileCoordinateZ = intToColor(randRangeInclusive(1, tPtr->maxHeight));

	} else {

		setPossiblePartner_4Pt(tPtr);
	}

	for (int i = 0; i < (tPtr->neighbors).size(); ++i) {
		wfc_4pt(tPtr->neighbors[i], false);
	}
}

void wfc_4pt_weighted(Tile* tPtr, bool init)
{
	if (tPtr == nullptr || (tPtr->tileCoordinateZ != NONE && !init)) {
		return;
	}

	if (init) {
		// Turns the first tile purple then changes it to a "accessible" tile i.e. one that participates in the actual tiling; 1, 2, 3, 4
		tPtr->tileCoordinateZ = PURPLE;
		tPtr->setColor();
		tPtr->tileCoordinateZ = intToColor(randRangeInclusive(1, tPtr->maxHeight));

	}
	else {

		setPossibleWeightedPartner_4Pt(tPtr);
	}

	for (int i = 0; i < (tPtr->neighbors).size(); ++i) {
		wfc_4pt(tPtr->neighbors[i], false);
	}
}

// This algorithm runs recursively and in all possible directions (vertical/horizontal/diagonal); It does -not- guarantee a complete map coverage.
// It also scales terribly - too many tiles and stack overflow occurs.
void wfc_8pt(Tile* tPtr, bool init)
{
	if (tPtr == nullptr || (tPtr->tileCoordinateZ != NONE && !init)) {
		return;
	}

	if (init) {
		// Turns the first tile purple then changes it to a "accessible" tile i.e. one that participates in the actual tiling; 1, 2, 3, 4
		tPtr->tileCoordinateZ = PURPLE;
		tPtr->setColor();
		tPtr->tileCoordinateZ = intToColor(randRangeInclusive(1, tPtr->maxHeight));

	}
	else {

		setPossiblePartner_8Pt(tPtr);
	}

	for (int i = 0; i < (tPtr->neighbors).size(); ++i) {
		wfc_8pt(tPtr->neighbors[i], false);
	}
	for (int i = 0; i < (tPtr->dNeighbors).size(); ++i) {
		wfc_8pt(tPtr->dNeighbors[i], false);
	}
}

void verticalFill(std::vector <std::vector<Tile>>& arrayMap, int passes)
{
	if (arrayMap[1].size() < 3 || passes < 1) {
		return;
	}

	for (int timesPassed = 0; timesPassed < passes; ++timesPassed) {
		for (int i = 0; i < arrayMap.size(); ++i) {
			for (int j = 0; j < arrayMap[i].size() - 2; ++j) {
				if (arrayMap[i][j].tileCoordinateZ == arrayMap[i][j + 2].tileCoordinateZ) {
					arrayMap[i][j + 1].tileCoordinateZ = arrayMap[i][j].tileCoordinateZ;
					(arrayMap[i][j + 1]).setColor();
				}
			}
		}
	}
}

void horizontalFill(std::vector<std::vector<Tile>>& arrayMap, int passes)
{
	if (arrayMap.size() < 3 || passes < 1) {
		return;
	}

	for (int timesPassed = 0; timesPassed < passes; ++timesPassed) {
		for (int i = 0; i < arrayMap.size() - 2; ++i) {
			for (int j = 0; j < arrayMap[i].size(); ++j) {
				if (arrayMap[i][j].tileCoordinateZ == arrayMap[i + 2][j].tileCoordinateZ) {
					arrayMap[i + 1][j].tileCoordinateZ = arrayMap[i][j].tileCoordinateZ;
					(arrayMap[i + 1][j]).setColor();
				}
			}
		}
	}
}

void doubleFill(std::vector<std::vector<Tile>>& arrayMap, int passes)
{
	if (arrayMap[1].size() < 3 || passes < 1) {
		return;
	}

	for (int timesPassed = 0; timesPassed < passes; ++timesPassed) {
		for (int i = 0; i < arrayMap.size(); ++i) {
			for (int j = 0; j < arrayMap[i].size() - 2; ++j) {
				if (arrayMap[i][j].tileCoordinateZ == arrayMap[i][j + 2].tileCoordinateZ) {
					arrayMap[i][j + 1].tileCoordinateZ = arrayMap[i][j].tileCoordinateZ;
					(arrayMap[i][j + 1]).setColor();
				}
			}
		}
	}

	for (int timesPassed = 0; timesPassed < passes; ++timesPassed) {
		for (int i = 0; i < arrayMap.size() - 2; ++i) {
			for (int j = 0; j < arrayMap[i].size(); ++j) {
				if (arrayMap[i][j].tileCoordinateZ == arrayMap[i + 2][j].tileCoordinateZ) {
					arrayMap[i + 1][j].tileCoordinateZ = arrayMap[i][j].tileCoordinateZ;
					(arrayMap[i + 1][j]).setColor();
				}
			}
		}
	}
}

bool containedInVector(std::vector<Tile> v, Tile t)
{
	if (std::find(v.begin(), v.end(), t) != v.end()) {
		return true;
	}
	else {
		return false;
	}
}

// Uses a tile T's height to search for all tiles next to it with the same height
void floodFill(Tile tile, std::vector<Tile>& vectorFoundTiles)
{
	if (tile.tileCoordinateZ == 0) {
		return;
	}

	if (vectorFoundTiles.size() > 0 && tile.tileCoordinateZ != vectorFoundTiles[0].tileCoordinateZ) {
		return;
	}

	if (vectorFoundTiles.size() == 0) {
		vectorFoundTiles.push_back(tile);
	}
	else {
		vectorFoundTiles.push_back(tile);
	}

	for (int partner = 0; partner < tile.neighbors.size(); ++partner) {
		if (tile.neighbors[partner] != nullptr && !containedInVector(vectorFoundTiles, *(tile.neighbors[partner]))) {
			floodFill(*tile.neighbors[partner], vectorFoundTiles);
		}
	}
}


// Goes across the map and replaces all areas (adjacent partners) with tileType under size targetNum with targetType
void tileFill(std::vector<std::vector<Tile>>& arrayMap, int tileType, int targetNum, int targetType)
{
	for (int i = 0; i < arrayMap.size(); ++i) {
		for (int j = 0; j < arrayMap[i].size(); ++j) {
			if (arrayMap[i][j].tileCoordinateZ != tileType) {
				continue;
			}

			std::vector<Tile> foundTiles = {};

			floodFill(arrayMap[i][j], foundTiles);
			/*OutputDebugStringA(std::to_string(foundTiles.size()).c_str());
			OutputDebugStringA(" ");*/
			if (foundTiles.size() <= targetNum) {
				for (int k = 0; k < foundTiles.size(); ++k) {
					arrayMap[convertCoordinateToGrid(foundTiles[k].tileCoordinateX)][convertCoordinateToGrid(foundTiles[k].tileCoordinateY)].tileCoordinateZ = intToColor(targetType);
					arrayMap[convertCoordinateToGrid(foundTiles[k].tileCoordinateX)][convertCoordinateToGrid(foundTiles[k].tileCoordinateY)].setColor();
				}
			}
		}
	}
}

// Uses a tile T's height to search for all tiles around it with the same height
void floodFillAllDirections(Tile tile, std::vector<Tile>& vectorFoundTiles)
{
	if (tile.tileCoordinateZ == 0) {
		return;
	}

	if (vectorFoundTiles.size() > 0 && tile.tileCoordinateZ != vectorFoundTiles[0].tileCoordinateZ) {
		return;
	}

	if (vectorFoundTiles.size() == 0) {
		vectorFoundTiles.push_back(tile);
	}
	else {
		vectorFoundTiles.push_back(tile);
	}

	for (int partner = 0; partner < tile.neighbors.size(); ++partner) {
		if (tile.neighbors[partner] != nullptr && !containedInVector(vectorFoundTiles, *(tile.neighbors[partner]))) {
			floodFillAllDirections(*tile.neighbors[partner], vectorFoundTiles);
		}
	}

	for (int partner = 0; partner < tile.dNeighbors.size(); ++partner) {
		if (tile.dNeighbors[partner] != nullptr && !containedInVector(vectorFoundTiles, *(tile.dNeighbors[partner]))) {
			floodFillAllDirections(*tile.dNeighbors[partner], vectorFoundTiles);
		}
	}
}


// Goes across the map and replaces all areas with tileType under size targetNum with targetType
void tileFillAllDirections(std::vector<std::vector<Tile>>& arrayMap, int tileType, int targetNum, int targetType)
{
	for (int i = 0; i < arrayMap.size(); ++i) {
		for (int j = 0; j < arrayMap[i].size(); ++j) {
			if (arrayMap[i][j].tileCoordinateZ != tileType) {
				continue;
			}

			std::vector<Tile> foundTiles = {};

			floodFillAllDirections(arrayMap[i][j], foundTiles);
			/*OutputDebugStringA(std::to_string(foundTiles.size()).c_str());
			OutputDebugStringA(" ");*/
			if (foundTiles.size() <= targetNum){
				for (int k = 0; k < foundTiles.size(); ++k) {
					arrayMap[convertCoordinateToGrid(foundTiles[k].tileCoordinateX)][convertCoordinateToGrid(foundTiles[k].tileCoordinateY)].tileCoordinateZ = intToColor(targetType);
					arrayMap[convertCoordinateToGrid(foundTiles[k].tileCoordinateX)][convertCoordinateToGrid(foundTiles[k].tileCoordinateY)].setColor();
				}
			}
		}
	}
}

// Doesn't work particularly well due to indeterminacy of stack resolution

void wfc_duplicate4pt(Tile* tPtr, bool init)
{
	if (tPtr == nullptr || (tPtr->tileCoordinateZ != NONE && !init)) {
		return;
	}

	if (init) {
		// Turns the first tile purple then changes it to a "accessible" tile i.e. one that participates in the actual tiling; 1, 2, 3, 4
		tPtr->tileCoordinateZ = PURPLE;
		tPtr->setColor();
		tPtr->tileCoordinateZ = intToColor(randRangeInclusive(1, tPtr->maxHeight));

	}
	else {

		setCoherentPossiblePartner_4Pt(tPtr);
	}

	for (int i = 0; i < (tPtr->neighbors).size(); ++i) {
		wfc_4pt(tPtr->neighbors[i], false);
	}
}


// This algorithm runs up from the starting position and snakes around until the entire right quadrant has been filled in, then repeats the proces starting under the starting position and going left.
void wfc_2snake(Tile* tPtr)
{
	Tile* currentPtr = tPtr;

	bool init = true;
	bool up = true;

	// Checks if the snake has hit the upper right or lower right corner
	while (!(currentPtr->neighbors[0] == nullptr && currentPtr->neighbors[1] == nullptr && up) && !(currentPtr->neighbors[2] == nullptr && currentPtr->neighbors[1] == nullptr && !up)) {
		if (init) {
			tPtr->tileCoordinateZ = PURPLE;
			tPtr->setColor();
			tPtr->tileCoordinateZ = intToColor(randRangeInclusive(1, tPtr->maxHeight));
			init = false;
		}
		else {
			setPossiblePartner_4Pt(currentPtr);
		}

		if (up && currentPtr->neighbors[0] != nullptr) {
			currentPtr = currentPtr->neighbors[0];
		}
		else if (up && currentPtr->neighbors[0] == nullptr) {
			currentPtr = currentPtr->neighbors[1];
			up = false;
		}
		else if (!up && currentPtr->neighbors[2] != nullptr) {
			currentPtr = currentPtr->neighbors[2];
		}
		else if (!up && currentPtr->neighbors[2] == nullptr) {
			currentPtr = currentPtr->neighbors[1];
			up = true;
		}
	}

	setPossiblePartner_4Pt(currentPtr);
	currentPtr = tPtr;
	up = false;

	while (!(currentPtr->neighbors[0] == nullptr && currentPtr->neighbors[3] == nullptr && up) && !(currentPtr->neighbors[2] == nullptr && currentPtr->neighbors[3] == nullptr && !up)) {
		
		if (currentPtr != tPtr) setPossiblePartner_4Pt(currentPtr);

		if (up && currentPtr->neighbors[0] != nullptr) {
			currentPtr = currentPtr->neighbors[0];
		}
		else if (up && currentPtr->neighbors[0] == nullptr) {
			currentPtr = currentPtr->neighbors[3];
			up = false;
		}
		else if (!up && currentPtr->neighbors[2] != nullptr) {
			currentPtr = currentPtr->neighbors[2];
		}
		else if (!up && currentPtr->neighbors[2] == nullptr) {
			currentPtr = currentPtr->neighbors[3];
			up = true;
		}
	}

	setPossiblePartner_4Pt(currentPtr);

}

void wfc_duplicate2snake(Tile* tPtr)
{
	Tile* currentPtr = tPtr;

	bool init = true;
	bool up = true;

	// Checks if the snake has hit the upper right or lower right corner
	while (!(currentPtr->neighbors[0] == nullptr && currentPtr->neighbors[1] == nullptr && up) && !(currentPtr->neighbors[2] == nullptr && currentPtr->neighbors[1] == nullptr && !up)) {
		if (init) {
			tPtr->tileCoordinateZ = PURPLE;
			tPtr->setColor();
			tPtr->tileCoordinateZ = intToColor(randRangeInclusive(1, tPtr->maxHeight));
			init = false;
		}
		else {
			setCoherentPossiblePartner_4Pt(currentPtr);
		}

		if (up && currentPtr->neighbors[0] != nullptr) {
			currentPtr = currentPtr->neighbors[0];
		}
		else if (up && currentPtr->neighbors[0] == nullptr) {
			currentPtr = currentPtr->neighbors[1];
			up = false;
		}
		else if (!up && currentPtr->neighbors[2] != nullptr) {
			currentPtr = currentPtr->neighbors[2];
		}
		else if (!up && currentPtr->neighbors[2] == nullptr) {
			currentPtr = currentPtr->neighbors[1];
			up = true;
		}
	}

	setCoherentPossiblePartner_4Pt(currentPtr);
	currentPtr = tPtr;
	up = false;

	while (!(currentPtr->neighbors[0] == nullptr && currentPtr->neighbors[3] == nullptr && up) && !(currentPtr->neighbors[2] == nullptr && currentPtr->neighbors[3] == nullptr && !up)) {

		if (currentPtr != tPtr) setCoherentPossiblePartner_4Pt(currentPtr);

		if (up && currentPtr->neighbors[0] != nullptr) {
			currentPtr = currentPtr->neighbors[0];
		}
		else if (up && currentPtr->neighbors[0] == nullptr) {
			currentPtr = currentPtr->neighbors[3];
			up = false;
		}
		else if (!up && currentPtr->neighbors[2] != nullptr) {
			currentPtr = currentPtr->neighbors[2];
		}
		else if (!up && currentPtr->neighbors[2] == nullptr) {
			currentPtr = currentPtr->neighbors[3];
			up = true;
		}
	}

	setCoherentPossiblePartner_4Pt(currentPtr);

}

void wfc_2snake_weighted(Tile* tPtr)
{
	Tile* currentPtr = tPtr;

	bool init = true;
	bool up = true;

	// Checks if the snake has hit the upper right or lower right corner
	while (!(currentPtr->neighbors[0] == nullptr && currentPtr->neighbors[1] == nullptr && up) && !(currentPtr->neighbors[2] == nullptr && currentPtr->neighbors[1] == nullptr && !up)) {
		if (init) {
			tPtr->tileCoordinateZ = PURPLE;
			tPtr->setColor();
			tPtr->tileCoordinateZ = intToColor(randRangeInclusive(1, tPtr->maxHeight));
			init = false;
		}
		else {
			setPossibleWeightedPartner_4Pt(currentPtr);
		}

		if (up && currentPtr->neighbors[0] != nullptr) {
			currentPtr = currentPtr->neighbors[0];
		}
		else if (up && currentPtr->neighbors[0] == nullptr) {
			currentPtr = currentPtr->neighbors[1];
			up = false;
		}
		else if (!up && currentPtr->neighbors[2] != nullptr) {
			currentPtr = currentPtr->neighbors[2];
		}
		else if (!up && currentPtr->neighbors[2] == nullptr) {
			currentPtr = currentPtr->neighbors[1];
			up = true;
		}
	}

	setPossibleWeightedPartner_4Pt(currentPtr);
	currentPtr = tPtr;
	up = false;

	while (!(currentPtr->neighbors[0] == nullptr && currentPtr->neighbors[3] == nullptr && up) && !(currentPtr->neighbors[2] == nullptr && currentPtr->neighbors[3] == nullptr && !up)) {

		if (currentPtr != tPtr) setPossibleWeightedPartner_4Pt(currentPtr);

		if (up && currentPtr->neighbors[0] != nullptr) {
			currentPtr = currentPtr->neighbors[0];
		}
		else if (up && currentPtr->neighbors[0] == nullptr) {
			currentPtr = currentPtr->neighbors[3];
			up = false;
		}
		else if (!up && currentPtr->neighbors[2] != nullptr) {
			currentPtr = currentPtr->neighbors[2];
		}
		else if (!up && currentPtr->neighbors[2] == nullptr) {
			currentPtr = currentPtr->neighbors[3];
			up = true;
		}
	}

	setPossibleWeightedPartner_4Pt(currentPtr);

}

bool isValidTile(int i, int j)
{
	return !(i < 0 || i >= X_DIMENSION || j < 0 || j >= Y_DIMENSION);
}

int gol_checkPartnersSea(std::vector<std::vector<Tile>>& arrayMap, int i, int j)
{
	int count = 0;

	for (int offsetX = -1; offsetX < 2; ++offsetX) {
		for (int offsetY = -1; offsetY < 2; ++offsetY) {
			if (offsetX == 0 && offsetY == 0) continue;
			if (isValidTile(i + offsetX, j + offsetY) && arrayMap[i + offsetX][j + offsetY].tileCoordinateZ == BLUE) {
				++count;
			}
		}
	}
	// OutputDebugStringA(std::to_string(count).c_str());
	return count;
}

void gol_seedMapOcean(std::vector<std::vector<Tile>>& arrayMap)
{
	for (int i = 0; i < arrayMap.size(); ++i) {
		for (int j = 0; j < arrayMap[0].size(); ++j) {
			if (randRangeInclusive(0, 100) <= BIRTH_RATE) {
				(arrayMap[i][j]).tileCoordinateZ = BLUE;
				(arrayMap[i][j]).setColor();
			}
		}
	}
}

void gol_updateMap(std::vector<std::vector<Tile>>& arrayMap, int amountPasses)
{
	for (int i = 0; i < amountPasses; ++i){
		for (int j = 0; j < arrayMap.size(); ++j) {
			for (int k = 0; k < arrayMap[j].size(); ++k) {
				if (gol_checkPartnersSea(arrayMap, j, k) >= BIRTH_DEATH_NUMBER) {
					arrayMap[j][k].tileCoordinateZ = BLUE;
					arrayMap[j][k].setColor();
				}
				else {
					arrayMap[j][k].tileCoordinateZ = NONE;
					arrayMap[j][k].setColor();
				}
			}
		}
	}
}

// Uses snake method to update map with GOL; final pass sets light blue edge around dark blue for wfc snake to build off 
void gol_updateMapSnake(std::vector<std::vector<Tile>>& arrayMap, int amountPasses)
{
	Tile* beginPtr = &arrayMap[rand() % arrayMap.size()][rand() % arrayMap[1].size()];

	for (int i = 0; i < amountPasses; ++i) {
		
		Tile* currentPtr = beginPtr;
		bool up = true;

		// Checks if the snake has hit the upper right or lower right corner
		while (!(currentPtr->neighbors[0] == nullptr && currentPtr->neighbors[1] == nullptr && up) && !(currentPtr->neighbors[2] == nullptr && currentPtr->neighbors[1] == nullptr && !up)) {
			
			if (gol_checkPartnersSea(arrayMap, convertCoordinateToGrid(currentPtr->tileCoordinateX), convertCoordinateToGrid(currentPtr->tileCoordinateY)) >= BIRTH_DEATH_NUMBER) {
				currentPtr->tileCoordinateZ = BLUE;
				currentPtr->setColor();
			}
			else {
				currentPtr->tileCoordinateZ = NONE;
				currentPtr->setColor();
			}

			// snek
			if (up && currentPtr->neighbors[0] != nullptr) {
				currentPtr = currentPtr->neighbors[0];
			}
			else if (up && currentPtr->neighbors[0] == nullptr) {
				currentPtr = currentPtr->neighbors[1];
				up = false;
			}
			else if (!up && currentPtr->neighbors[2] != nullptr) {
				currentPtr = currentPtr->neighbors[2];
			}
			else if (!up && currentPtr->neighbors[2] == nullptr) {
				currentPtr = currentPtr->neighbors[1];
				up = true;
			}
		}

		if (gol_checkPartnersSea(arrayMap, convertCoordinateToGrid(currentPtr->tileCoordinateX), convertCoordinateToGrid(currentPtr->tileCoordinateY)) >= BIRTH_DEATH_NUMBER) {
			currentPtr->tileCoordinateZ = BLUE;
			currentPtr->setColor();
		}
		else {
			currentPtr->tileCoordinateZ = NONE;
			currentPtr->setColor();
		}

		currentPtr = beginPtr;
		up = false;

		while (!(currentPtr->neighbors[0] == nullptr && currentPtr->neighbors[3] == nullptr && up) && !(currentPtr->neighbors[2] == nullptr && currentPtr->neighbors[3] == nullptr && !up)) {

			if (currentPtr != beginPtr) {
				if (gol_checkPartnersSea(arrayMap, convertCoordinateToGrid(currentPtr->tileCoordinateX), convertCoordinateToGrid(currentPtr->tileCoordinateY)) >= BIRTH_DEATH_NUMBER) {
					currentPtr->tileCoordinateZ = BLUE;
					currentPtr->setColor();
				}
				else {
					currentPtr->tileCoordinateZ = NONE;
					currentPtr->setColor();
				}
			}

			if (up && currentPtr->neighbors[0] != nullptr) {
				currentPtr = currentPtr->neighbors[0];
			}
			else if (up && currentPtr->neighbors[0] == nullptr) {
				currentPtr = currentPtr->neighbors[3];
				up = false;
			}
			else if (!up && currentPtr->neighbors[2] != nullptr) {
				currentPtr = currentPtr->neighbors[2];
			}
			else if (!up && currentPtr->neighbors[2] == nullptr) {
				currentPtr = currentPtr->neighbors[3];
				up = true;
			}
		}

		if (gol_checkPartnersSea(arrayMap, convertCoordinateToGrid(currentPtr->tileCoordinateX), convertCoordinateToGrid(currentPtr->tileCoordinateY)) >= BIRTH_DEATH_NUMBER) {
			currentPtr->tileCoordinateZ = BLUE;
			currentPtr->setColor();
		}
		else {
			currentPtr->tileCoordinateZ = NONE;
			currentPtr->setColor();
		}
	}

	// Light blue edge

	Tile* currentPtr = beginPtr;
	bool up = true;

	// Checks if the snake has hit the upper right or lower right corner
	while (!(currentPtr->neighbors[0] == nullptr && currentPtr->neighbors[1] == nullptr && up) && !(currentPtr->neighbors[2] == nullptr && currentPtr->neighbors[1] == nullptr && !up)) {

		if (gol_checkPartnersSea(arrayMap, convertCoordinateToGrid(currentPtr->tileCoordinateX), convertCoordinateToGrid(currentPtr->tileCoordinateY)) >= 1 && currentPtr->tileCoordinateZ == NONE && !(currentPtr->tileCoordinateZ == BLUE)) {
			currentPtr->tileCoordinateZ = LIGHT_BLUE;
			currentPtr->setColor();
		}

		// snek
		if (up && currentPtr->neighbors[0] != nullptr) {
			currentPtr = currentPtr->neighbors[0];
		}
		else if (up && currentPtr->neighbors[0] == nullptr) {
			currentPtr = currentPtr->neighbors[1];
			up = false;
		}
		else if (!up && currentPtr->neighbors[2] != nullptr) {
			currentPtr = currentPtr->neighbors[2];
		}
		else if (!up && currentPtr->neighbors[2] == nullptr) {
			currentPtr = currentPtr->neighbors[1];
			up = true;
		}
	}

	if (gol_checkPartnersSea(arrayMap, convertCoordinateToGrid(currentPtr->tileCoordinateX), convertCoordinateToGrid(currentPtr->tileCoordinateY)) >= 1 && currentPtr->tileCoordinateZ == NONE && !(currentPtr->tileCoordinateZ == BLUE)) {
		currentPtr->tileCoordinateZ = LIGHT_BLUE;
		currentPtr->setColor();
	}

	currentPtr = beginPtr;
	up = false;

	while (!(currentPtr->neighbors[0] == nullptr && currentPtr->neighbors[3] == nullptr && up) && !(currentPtr->neighbors[2] == nullptr && currentPtr->neighbors[3] == nullptr && !up)) {

		if (currentPtr != beginPtr) {
			if (gol_checkPartnersSea(arrayMap, convertCoordinateToGrid(currentPtr->tileCoordinateX), convertCoordinateToGrid(currentPtr->tileCoordinateY)) >= 1 && currentPtr->tileCoordinateZ == NONE && !(currentPtr->tileCoordinateZ == BLUE)) {
				currentPtr->tileCoordinateZ = LIGHT_BLUE;
				currentPtr->setColor();
			}
		}

		if (up && currentPtr->neighbors[0] != nullptr) {
			currentPtr = currentPtr->neighbors[0];
		}
		else if (up && currentPtr->neighbors[0] == nullptr) {
			currentPtr = currentPtr->neighbors[3];
			up = false;
		}
		else if (!up && currentPtr->neighbors[2] != nullptr) {
			currentPtr = currentPtr->neighbors[2];
		}
		else if (!up && currentPtr->neighbors[2] == nullptr) {
			currentPtr = currentPtr->neighbors[3];
			up = true;
		}
	}

	if (gol_checkPartnersSea(arrayMap, convertCoordinateToGrid(currentPtr->tileCoordinateX), convertCoordinateToGrid(currentPtr->tileCoordinateY)) >= 1 && currentPtr->tileCoordinateZ == NONE && !(currentPtr->tileCoordinateZ == BLUE)) {
		currentPtr->tileCoordinateZ = LIGHT_BLUE;
		currentPtr->setColor();
	}
}

// Uses a common partner to fill in remaining black tiles (DOES NOT [NECESSARILY] FOLLOW PARTNER LOGIC)
void commonFill(std::vector<std::vector<Tile>>& arrayMap)
{
	for (int i = 0; i < arrayMap.size(); ++i) {
		for (int j = 0; j < arrayMap[j].size(); ++j) {
			if (arrayMap[i][j].tileCoordinateZ == NONE) {

				std::vector<int> adjacentHeights;

				for (int offsetX = -1; offsetX < 2; offsetX += 2) {
					if (isValidTile(i + offsetX, j) && arrayMap[i + offsetX][j].tileCoordinateZ != NONE) {
						adjacentHeights.push_back(arrayMap[i + offsetX][j].tileCoordinateZ);
					}
				}

				for (int offsetY = -1; offsetY < 2; offsetY += 2) {
					if (isValidTile(i, j + offsetY) && arrayMap[i][j + offsetY].tileCoordinateZ != NONE) {
						adjacentHeights.push_back(arrayMap[i][j + offsetY].tileCoordinateZ);
					}
				}

				if (adjacentHeights.size() > 0) {
					arrayMap[i][j].tileCoordinateZ = intToColor(adjacentHeights[randRangeInclusive(0, (int)adjacentHeights.size() - 1)]);
					arrayMap[i][j].setColor();
				}

			}
		}
	}
}
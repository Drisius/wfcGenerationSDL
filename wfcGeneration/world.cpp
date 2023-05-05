#include "world.h"

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
	case ORANGE:
		r = 255;
		g = 128;
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
		return ORANGE;
		break;
	case(5):
		return BROWN;
		break;
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

// Gives a weighted chance to place more tiles

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
			std::advance(offset, randRangeInclusive(0, (int)possiblePartners.size() - 1));

			if (randRangeInclusive(0, 100) > tileChance[(*offset) + 1]) {
				tilePtr->tileCoordinateZ = intToColor(*offset);
				tilePtr->setColor();
			}
			else {
				offset = begin;
				std::advance(offset, randRangeInclusive(0, (int)possiblePartners.size() - 1));
			}
		}
	}
}

// Possible partner calculation for 8 surronding tiles
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

void verticalFill(std::vector <std::vector<Tile>>& arrayMap)
{
	if (arrayMap[1].size() < 3) {
		return;
	}

	for (int i = 0; i < arrayMap.size(); ++i) {
		for (int j = 0; j < arrayMap[i].size() - 2; ++j) {
			if (arrayMap[i][j].tileCoordinateZ == arrayMap[i][j + 2].tileCoordinateZ) {
				arrayMap[i][j + 1].tileCoordinateZ = arrayMap[i][j].tileCoordinateZ;
				(arrayMap[i][j + 1]).setColor();
			}
		}
	}
}

void horizontalFill(std::vector<std::vector<Tile>>& arrayMap)
{
	if (arrayMap.size() < 3) {
		return;
	}

	for (int i = 0; i < arrayMap.size() - 2; ++i) {
		for (int j = 0; j < arrayMap[i].size(); ++j) {
			if (arrayMap[i][j].tileCoordinateZ == arrayMap[i + 2][j].tileCoordinateZ) {
				arrayMap[i + 1][j].tileCoordinateZ = arrayMap[i][j].tileCoordinateZ;
				(arrayMap[i + 1][j]).setColor();
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
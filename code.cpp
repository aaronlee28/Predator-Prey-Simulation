// al7619_hw13.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
using namespace std;

struct Position {
	int row;
	int column;
};

const int gridSize = 20;
const int initialDoodlebug = 5;
const int initialAnt = 100;
const int antBreedCount = 3;
const int doodlebugBreedCount = 8;
const int doodlebugStarveCount = 3;
const char antAscii = 'o';
const char doodlebugAscii = 'x';
const char emptyAscii = '-';
const char antType = 'A';
const char doodlebugType = 'D';

class Game;

class Organism {
	Position position;
public:
	Organism(const Position& pos);
	Organism();
	virtual ~Organism();
	Position getPosition() const;
	void setPosition(const Position& pos);
	virtual void move(Game& game) = 0;
	virtual void breed(Game& game) = 0;
	virtual void starve(Game& game) = 0;
	void die(Game& game) const;
	virtual char getType() const = 0;
	virtual void print(ostream& cout) const = 0;
	friend ostream& operator <<(ostream& cout, const Organism& organism) {
		organism.print(cout);
		return cout;
	};
};

class Game {
	Organism* coordinate[gridSize][gridSize];

public:
	Game();
	~Game();
	Organism* getOrganism(const Position& pos) const;
	void setOrganism(Position& pos, Organism* organismPtr);
	bool isCellAvailable(const Position& pos) const;
	vector<Position> getAvailableNeighbor(const Position& currPos) const;
	void moveOrganism(const Position& currPos, const Position& newPos);
	void removeOrganism(const Organism* organismPtr);
	void display() const;
	void nextStep();
	bool isArrayEmpty() const;
};

class Doodlebug : public Organism {
	int breedTimer;
	int starveTimer;
	vector<Position> getAvailableAnts(Game& game);
public:
	Doodlebug(const Position& pos);
	Doodlebug();
	virtual ~Doodlebug();
	virtual void move(Game& game) override;
	virtual void breed(Game& game) override;
	virtual void starve(Game& game) override;
	virtual char getType() const override;
	virtual void print(ostream& cout) const;
};

class Ant : public Organism {
	int breedTimer;
public:
	Ant(const Position& pos);
	Ant();
	virtual ~Ant();
	virtual void move(Game& game) override;
	virtual void breed(Game& game)override;
	virtual void starve(Game& game)override;
	virtual char getType() const override;
	virtual void print(ostream& cout) const;
};
ostream& operator <<(ostream& cout, const Position& pos);
int main()
{
	srand(time(NULL));
	Game game;
	int step = 0;

	while (game.isArrayEmpty() == false) {
		cout << "o = Ant and x = Doodlebug" << endl;
		cout << "=======================================" << endl;
		cout << "Step: " << step << endl;
		game.display();
		cout << "Press enter to go next. " << endl;
		cin.get();
		game.nextStep();
		step++;
	}
	return 0;
}

// Game Functions
Game::Game() {
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			coordinate[i][j] = 0;
		}
	}

	for (int i = 0; i < initialDoodlebug; i++) {
		int row = rand() % gridSize;
		int column = rand() % gridSize;

		while (coordinate[row][column] != 0) {
			row = rand() % gridSize;
			column = rand() % gridSize;
		}

		coordinate[row][column] = new Doodlebug({ row, column });
	}

	for (int i = 0; i < initialAnt; i++) {
		int row = rand() % gridSize;
		int column = rand() % gridSize;

		while (coordinate[row][column] != 0) {
			row = rand() % gridSize;
			column = rand() % gridSize;
		}

		coordinate[row][column] = new Ant({ row, column });
	}
}
Game::~Game() {
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			if (coordinate[i][j] != 0) {
				delete coordinate[i][j];
			}
		}
	}
}
bool Game::isCellAvailable(const Position& pos) const {
	return(0 <= pos.row) && (pos.row < gridSize) && (0 <= pos.column) && (pos.column < gridSize) &&
		(coordinate[pos.row][pos.column] == 0);
}
void Game::moveOrganism(const Position& currPos, const Position& newPos) {
	coordinate[newPos.row][newPos.column] = coordinate[currPos.row][currPos.column];
	coordinate[newPos.row][newPos.column]->setPosition(newPos);
	coordinate[currPos.row][currPos.column] = 0;
}
vector<Position> Game::getAvailableNeighbor(const Position& currPos) const {
	vector<Position> availableNeighbor;
	vector<Position> neighborCheck = {
		{currPos.row - 1,currPos.column},
		{currPos.row + 1,currPos.column},
		{currPos.row ,currPos.column - 1},
		{currPos.row,currPos.column + 1}
	};

	for (Position i : neighborCheck) {
		if (isCellAvailable(i)) {
			availableNeighbor.push_back(i);
		}
	}
	return availableNeighbor;
}
void Game::setOrganism(Position& pos, Organism* organismPtr) {
	coordinate[pos.row][pos.column] = organismPtr;
	organismPtr->setPosition(pos);
}
void Game::removeOrganism(const Organism* organismPtr) {
	Position pos = organismPtr->getPosition();
	coordinate[pos.row][pos.column] = 0;
	delete organismPtr;
}
void Game::display() const {
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			if (coordinate[i][j] == 0) {
				cout << emptyAscii;
			}
			else {
				cout << *coordinate[i][j];
			}
			cout << ' ';
		}
		cout << '\n';
	}
}
Organism* Game::getOrganism(const Position& pos) const {
	if ((pos.row >= 0) && (pos.row < gridSize) && (pos.column >= 0) && (pos.column < gridSize)) {
		return coordinate[pos.row][pos.column];
	}
	else {
		return 0;
	}
}
void Game::nextStep() {
	vector<Organism*> Doodlebugs;
	vector<Organism*> Ants;

	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			if ((coordinate[i][j] != 0) && (coordinate[i][j]->getType() == doodlebugType)) {
				Doodlebugs.push_back(coordinate[i][j]);
			}
		}
	}

	for (Organism* doodlebugPtr : Doodlebugs) {
		doodlebugPtr->move(*this);
	}

	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			if ((coordinate[i][j] != 0) && (coordinate[i][j]->getType() == antType)) {
				Ants.push_back(coordinate[i][j]);
			}
		}
	}

	for (Organism* antPtr : Ants) {
		antPtr->move(*this);
	}

	for (Organism* doodlebugPtr : Doodlebugs) {
		doodlebugPtr->breed(*this);
	}

	for (Organism* doodlebugPtr : Doodlebugs) {
		doodlebugPtr->starve(*this);
	}
	for (Organism* antPtr : Ants) {
		antPtr->breed(*this);
	}
}
bool Game::isArrayEmpty() const {
	vector<Organism*> notEmpty;
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			if (coordinate[i][j] != 0) {
				notEmpty.push_back(coordinate[i][j]);
			}
		}
	}
	if (notEmpty.empty()) {
		return true;
	}
	else { return false; }
}

//Organism Functions
Organism::Organism(const Position& pos) : position(pos) {}
Organism::Organism() : position({ 0, 0 }) {}
Organism::~Organism() {}

Position Organism::getPosition() const {
	return position;
}
void Organism::setPosition(const Position& pos) {
	position.row = pos.row;
	position.column = pos.column;
}
void Organism::die(Game& Game) const {
	Game.removeOrganism(this);
}

// Doodlebug Function
Doodlebug::Doodlebug(const Position& pos) : Organism(pos), breedTimer(0), starveTimer(0) {}
Doodlebug::Doodlebug() : Organism(), breedTimer(0), starveTimer(0) {}
Doodlebug::~Doodlebug() {}

void Doodlebug::move(Game& game) {
	Position currPos = getPosition();
	vector<Position> adjacentAnts = getAvailableAnts(game);

	if (!adjacentAnts.empty()) {
		int randomAnts = rand() % adjacentAnts.size();
		Position eatAnts = adjacentAnts[randomAnts];

		game.getOrganism(eatAnts)->die(game);
		game.moveOrganism(currPos, eatAnts);
		breedTimer++;
		starveTimer = 0;
	}
	else {
		Position newPos = currPos;
		int movement = rand() % 4 + 1;

		if (movement == 1) {
			newPos.row--;
		}
		else if (movement == 2) {
			newPos.row++;
		}
		else if (movement == 3) {
			newPos.column--;
		}
		else {
			newPos.column++;
		}

		if (game.isCellAvailable(newPos)) {
			game.moveOrganism(currPos, newPos);
		}
		starveTimer++;
		breedTimer++;
	}
}
void Doodlebug::breed(Game& game) {
	if (breedTimer < doodlebugBreedCount) {
		return;
	}
	Position currPos = getPosition();
	vector<Position> availableNeighbor = game.getAvailableNeighbor(currPos);

	if (!availableNeighbor.empty()) {
		int pickNeighbor = rand() % availableNeighbor.size();
		Position breedDoodlebug = availableNeighbor[pickNeighbor];
		game.setOrganism(breedDoodlebug, new Doodlebug);
	}
	breedTimer = 0;
}

void Doodlebug::starve(Game& game) {
	if (starveTimer < doodlebugStarveCount) {
		return;
	}
	die(game);
}
char Doodlebug::getType() const {
	return doodlebugType;
}
void Doodlebug::print(ostream& cout) const {
	cout << doodlebugAscii;
}
vector<Position> Doodlebug::getAvailableAnts(Game& game) {
	Position currPos = getPosition();
	vector<Position> availableAnts;
	vector<Position> neighborCheck = {
		{currPos.row,currPos.column + 1},
		{currPos.row + 1,currPos.column},
		{currPos.row,currPos.column - 1},
		{currPos.row - 1,currPos.column}
	};
	for (Position i : neighborCheck) {
		Organism* organismPtr = game.getOrganism(i);
		if ((organismPtr != 0) && (organismPtr->getType() == antType)) {
			availableAnts.push_back(i);
		}
	}
	return availableAnts;
}

// Ant Function
Ant::Ant(const Position& pos) : Organism(pos), breedTimer(0) {}
Ant::Ant() : Organism(), breedTimer(0) {}
Ant::~Ant() {}

void Ant::move(Game& game) {
	int movement = rand() % 4 + 1;
	Position currPos = getPosition();
	Position newPos = currPos;

	if (movement == 1) {
		newPos.row--;
	}
	else if (movement == 2) {
		newPos.row++;
	}
	else if (movement == 3) {
		newPos.column--;
	}
	else {
		newPos.column++;
	}

	if (game.isCellAvailable(newPos)) {
		game.moveOrganism(currPos, newPos);
	}
	breedTimer++;
}
void Ant::breed(Game& game) {
	if (breedTimer < antBreedCount) {
		return;
	}
	Position currPos = getPosition();
	vector<Position> availableNeighbor = game.getAvailableNeighbor(currPos);

	if (!availableNeighbor.empty()) {
		int pickNeighbor = rand() % availableNeighbor.size();
		Position breedAnt = availableNeighbor[pickNeighbor];
		game.setOrganism(breedAnt, new Ant);
	}
	breedTimer = 0;
}
void Ant::starve(Game& game) {}
char Ant::getType() const {
	return antType;
}
void Ant::print(ostream& cout) const {
	cout << antAscii;
}
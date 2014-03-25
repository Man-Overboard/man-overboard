#ifndef LEVEL_H
#define LEVEL_H

#include <queue>

using std::string;

class Level {

public :
	// box size in grid
	int boxSize;
	int gridSize;

	// enemies
	int enemyCount;
	int weaponCount;
	int objectsToAvoidCount;
	int maxMoves;

	// bools for available controls
	bool loopControls;
	bool conditionalControls;

	std::queue<string>			story;

public :

	Level(int box, int grid, int enemy, int weapon, int objects, bool loopControls, bool conditionalControls, int maxMoves, std::queue<string> story);
};
#endif
#ifndef LEVEL_H
#define LEVEL_H

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

public :

	Level(int box, int grid, int enemy, int weapon, int objects, bool loopControls, bool conditionalControls, int maxMoves);
};
#endif
#include "Level.h"

Level::Level(int box, int grid, int enemy, int weapon, int objects)
{
	boxSize = box;
	gridSize = grid;
	enemyCount = enemy;
	weaponCount = weapon;
	objectsToAvoidCount = objects;
}
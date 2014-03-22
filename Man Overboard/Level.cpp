#include "Level.h"

Level::Level(int box, int grid, int enemy, int weapon, int objects, bool loop, bool conditional)
{
	boxSize = box;
	gridSize = grid;
	enemyCount = enemy;
	weaponCount = weapon;
	objectsToAvoidCount = objects;
	loopControls = loop;
	conditionalControls = conditional;
}
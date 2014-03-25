#include "Level.h"
#include <queue>

using std::string;

Level::Level(int box, int grid, int enemy, int weapon, int objects, bool loop, bool conditional, int moves, std::queue<string> list)
{
	boxSize = box;
	gridSize = grid;
	enemyCount = enemy;
	weaponCount = weapon;
	objectsToAvoidCount = objects;
	loopControls = loop;
	conditionalControls = conditional;
	maxMoves = moves;
	story = list;
}
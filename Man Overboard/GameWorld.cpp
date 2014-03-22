#include "GameWorld.h"
#include "constants.h"
#include "Cgdi.h"
#include "WindowUtils.h"
#include "Level.h"

#include <list>
#include <queue>
#include <random>

using std::list;
using std::string;


//------------------------------- creator --------------------------------
//------------------------------------------------------------------------
GameWorld::GameWorld(int cx, int cy):

            m_cxClient(cx),
            m_cyClient(cy),
            m_bPaused(false),
			m_vBox(Vector2D(cxClient()-(constWindowWidth-80), cyClient()-(constWindowHeight-80))), // get the values for the overall box the grid will be contained in
			m_playerDirection('N'),
			init(true),
			m_runCommandSequence(false),
			m_unfoldLoop(false),
			m_inLoop(false),
			m_hasWeapon(false),
			m_movesTaken(0)
{
	// set the levels Level(int box, int grid, int enemy, int weapon, int objects);
	Level level0 = Level(200,2,0,0,0, false, false, 3);
	levels.push(level0);

	Level level1 = Level(150,4,0,0,0, false, false, 9);
	levels.push(level1);

	Level level2 = Level(150,4,0,0,1, false, false, 10);
	levels.push(level2);

	Level level3 = Level(100,6,0,0,2, false, false, 15);
	levels.push(level3);

	Level level4 = Level(150,4,1,1,0, false, false, 20);
	levels.push(level4);

	Level level5 = Level(100,6,1,1,1, false, false, 35);
	levels.push(level5);

	Level level6 = Level(180,4,0,0,0, true, false, 6);
	levels.push(level6);

	Level level7 = Level(150,5,1,1,1, true, false, 25);
	levels.push(level7);

	// things to avoid level
	// multiple to avoid
	// weapon pick up / 1 enemy
	// weapon, enemy and avoid
	// multiple enemies
	// loops
	// if statements

	/*Level level1 = Level(100, 8, 3, 1, 2, false, false);
	levels.push(level1);

	Level level2 = Level(100, 8, 3, 1, 4, true, false);
	levels.push(level2);

	Level level3 = Level(50, 12, 4, 1, 3, true, false);
	levels.push(level3);*/
}


//-------------------------------- destructor ----------------------------
//------------------------------------------------------------------------
GameWorld::~GameWorld()
{

}


//----------------------------- Update -----------------------------------
//------------------------------------------------------------------------
void GameWorld::Update(double time_elapsed)
{ 
  if (m_bPaused) return;

}

//------------------------- HandleKeyPresses -----------------------------
void GameWorld::HandleKeyPresses(WPARAM wParam)
{
	// if sequence currently running so not let new commands be added
	if(m_runCommandSequence){
		return;
	}

	// if moves taken is equal to moves allowed do not let commands be added
	if(m_movesTaken == levels.front().maxMoves && wParam != 'G' && wParam != 'C' && wParam != 'Z' && wParam != 'E'){
		return;
	}

  switch(wParam)
  {
    case 'F':
		if(m_commandQueue.empty() || m_commandQueue.back() != "START"){
			AddToQueueList(constMoveForward);
			m_movesTaken ++;
		}
      break;

    case 'R':
		if(m_commandQueue.empty() || m_commandQueue.back() != "START"){
			AddToQueueList(constMoveRight);
			m_movesTaken ++;
		}
      break;

	case 'L':
		if(m_commandQueue.empty() || m_commandQueue.back() != "START"){
			AddToQueueList(constMoveLeft);
			m_movesTaken ++;
		}
      break;

	case 'C':
		if(!m_commandQueue.empty()){
			if (m_commandQueue.back() == "END"){
				m_inLoop = true;
			}
			if (m_commandQueue.back() == "START"){
				m_inLoop = false;
			}
			if (m_commandQueue.back() != "1" && m_commandQueue.back() != "2" && m_commandQueue.back() != "3" && 
				m_commandQueue.back() != "4" && m_commandQueue.back() != "5" && m_commandQueue.back() != "6" && 
				m_commandQueue.back() != "7" && m_commandQueue.back() != "8" && m_commandQueue.back() != "9" && 
				m_commandQueue.back() != "END"){
				m_movesTaken --;
			}
			m_commandQueue.pop_back();
		}
		break;

	case 'Z':
		init = true;
		break;

	case 'G':
		if (!m_inLoop){
			m_runCommandSequence = true;
		}
		break;

	case 'S':
		if (!m_inLoop && levels.front().loopControls){
			AddToQueueList("START");
			m_unfoldLoop = true;
			m_inLoop = true;
			m_movesTaken ++;
		}
		break;

	case 'E':
		if(!m_commandQueue.empty() && m_commandQueue.back() != "START" && levels.front().loopControls && m_inLoop){
			AddToQueueList("END");
			m_inLoop = false;
		}
		break;

	case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		if(!m_commandQueue.empty() && m_commandQueue.back() == "START"){
			string number;
			number = (char) wParam;
			AddToQueueList(number);
		}
		break;

  }//end switch
}

void GameWorld::AddToQueueList(string command)
{
	// logic to ensure queue not already full
	m_commandQueue.push_back(command);
}


//-------------------------- HandleMenuItems -----------------------------
void GameWorld::HandleMenuItems(WPARAM wParam, HWND hwnd)
{
  switch(wParam)
  {

      break;
      
  }//end switch
}

void GameWorld::DrawGrid(){
  // the grid
  gdi->BlueBrush();
  gdi->BlackPen();
  gdi->Rect(m_vBox.x, m_vBox.y, m_vBox.x + ((levels.front().gridSize)*(levels.front().boxSize)), m_vBox.y + ((levels.front().gridSize)*(levels.front().boxSize)) );
  int i;
  for(i=0; i <= (levels.front().gridSize);i++){
	  gdi->Line(Vector2D(m_vBox.x, m_vBox.y+((levels.front().boxSize)*i)), Vector2D(m_vBox.x + (levels.front().boxSize)*(levels.front().gridSize), m_vBox.y+((levels.front().boxSize)*i)));
  }

  for(i=0; i <= (levels.front().gridSize);i++){
	  gdi->Line(Vector2D(m_vBox.x+((levels.front().boxSize)*i), m_vBox.y), Vector2D(m_vBox.x+((levels.front().boxSize)*i), m_vBox.y + (levels.front().boxSize)*(levels.front().gridSize)));
  }
}

void GameWorld::DrawControls(){
	gdi->BlackPen();
	gdi->WhiteBrush();

	// Container Box
	double x,y;
	x = m_vBox.x + ((levels.front().boxSize)*(levels.front().gridSize)+((levels.front().boxSize)/2));
	y = m_vBox.y;
	gdi->Rect(x,y,x+constControlWidth,y+constControlHeight);

	// Title Text
	int lineHeight = 20;
	gdi->TextAtPos(x + constControlWidth/4, y + 10, "Controls");
	gdi->TextColor(Cgdi::black);

	// Movement Controls
	gdi->TextAtPos(x + constControlTabOffset, y + lineHeight*2, "FORWARD (F)");
	gdi->TextAtPos(x + constControlTabOffset, y + lineHeight*3, "TURN RIGHT 90 (R)");
	gdi->TextAtPos(x + constControlTabOffset, y + lineHeight*4, "TURN LEFT 90 (L)");
	gdi->TextAtPos(x + constControlTabOffset, y + lineHeight*5, "CLEAR (C)");
	gdi->TextAtPos(x + constControlTabOffset, y + lineHeight*6, "GO (G)");
	gdi->TextAtPos(x + constControlTabOffset, y + lineHeight*7, "RESET (Z)");
	if (levels.front().loopControls){
		gdi->TextAtPos(x + constControlTabOffset, y + lineHeight*8, "START (S)");
		gdi->TextAtPos(x + constControlTabOffset, y + lineHeight*9, "END (E)");
	}

	// Queued Moves
	gdi->TextAtPos(x + constControlWidth/4, y + lineHeight*12, "Queued Moves");
	std::deque<std::string> tempQueue = m_commandQueue;
	int counter = 0;
	int offset = constControlTabOffset;
	while (!tempQueue.empty()){
		counter++;
		if(tempQueue.front() == "END"){
			offset = constControlTabOffset;
		}
		gdi->TextAtPos(x + offset, y + lineHeight*(12+counter), tempQueue.front());
		if(tempQueue.front() == "START"){
			/*tempQueue.pop_front();
			gdi->TextAtPos(x + offset+20, y + lineHeight*(10+counter), tempQueue.front());*/
			offset = constControlTabOffset + 30;
		}
		tempQueue.pop_front();
	}

	// Weapon Box
	gdi->Rect(x,y+constControlHeight+10,x+constControlWidth, y+constControlHeight+100);
	gdi->TextAtPos(x + constControlWidth/4,y+constControlHeight+40, "Weapon");

	// Moves Box
	gdi->Rect(x,y+constControlHeight+10,x+constControlWidth, y+constControlHeight+120);
	gdi->TextAtPos(x + constControlWidth/10,y+constControlHeight+20, "Moves :");
	gdi->TextAtPos(x + constControlWidth/3,y+constControlHeight+60, std::to_string(m_movesTaken) + "/" + std::to_string(levels.front().maxMoves));

	// info 
	gdi->Rect(m_vBox.x, m_vBox.y+levels.front().gridSize*levels.front().boxSize + 20, m_vBox.x+levels.front().gridSize*levels.front().boxSize, m_vBox.y+levels.front().gridSize*levels.front().boxSize + 80);
}

void GameWorld::DrawPlayer(Vector2D position) {
	gdi->WhiteBrush();
	gdi->Circle(position.x, position.y, 20);

	m_player = position;
}

void GameWorld::RunCommandSequence() {
	// pop the command off the queue
	if (!m_commandQueue.empty()){
		string value = m_commandQueue.front();
		if (value == constMoveRight || value == constMoveLeft){
			TurnPlayer(value);
		}
		if (value == constMoveForward){
			MovePlayer();
		}
		// if level has finished prevent empty queue from attempted pop
		if(!init){
			m_commandQueue.pop_front();
		}
	} else {
		m_runCommandSequence = false;
	}
	
}

void GameWorld::UnFoldLoop() {
	int count = 0;
	std::deque<std::string> tempQueue = m_commandQueue;
	std::deque<std::string> loopQueue;
	std::deque<std::string> tempLoopQueue;
	std::deque<std::string> finalQueue;
	while(!tempQueue.empty()){
		if (tempQueue.front() == "START"){
			// pop "START" off queue
			tempQueue.pop_front();
			count = atoi(tempQueue.front().c_str());
			tempQueue.pop_front();
			while (tempQueue.front() != "END"){
				loopQueue.push_back(tempQueue.front());
				tempQueue.pop_front();
			}
			// pop "END" off queue
			tempQueue.pop_front();
			for (int i = 0; i < count; i++){
				tempLoopQueue = loopQueue;
				while(!tempLoopQueue.empty()){
					finalQueue.push_back(tempLoopQueue.front());
					tempLoopQueue.pop_front();
				}
			}
			loopQueue = std::deque<std::string>();
		} else {
			finalQueue.push_back(tempQueue.front());
			tempQueue.pop_front();
		}
	}
	m_commandQueue = finalQueue;
}

void GameWorld::MovePlayer(){
	Vector2D position;
	switch (m_playerDirection) {
		case 'N' :
			position.x = m_player.x;
			position.y = m_player.y - (levels.front().boxSize);
			break;
		case 'S' :
			position.x = m_player.x;
			position.y = m_player.y + (levels.front().boxSize);
			break;
		case 'E' :
			position.x = m_player.x + (levels.front().boxSize);
			position.y = m_player.y;
			break;
		case 'W' :
			position.x = m_player.x - (levels.front().boxSize);
			position.y = m_player.y;
			break;

	}
	bool valid = ValidateMove(position);
	if(valid){
		DrawPlayer(position);
		CheckForWeapon();
		CheckForManOverBoard();
		CheckForDanger();
	}
}

void GameWorld::TurnPlayer(string direction) {
	switch (m_playerDirection) {
		case 'N' :
			if (direction == constMoveRight){
				m_playerDirection = 'E';
			}
			if (direction == constMoveLeft){
				m_playerDirection = 'W';
			}
			break;
		case 'E' :
			if (direction == constMoveRight){
				m_playerDirection = 'S';
			}
			if (direction == constMoveLeft){
				m_playerDirection = 'N';
			}
			break;
		case 'S' :
			if (direction == constMoveRight){
				m_playerDirection = 'W';
			}
			if (direction == constMoveLeft){
				m_playerDirection = 'E';
			}
			break;
		case 'W' :
			if (direction == constMoveRight){
				m_playerDirection = 'N';
			}
			if (direction == constMoveLeft){
				m_playerDirection = 'S';
			}
			break;
	}
}

bool GameWorld::ValidateMove(Vector2D newPosition)
{
	if(newPosition.x < m_vBox.x || newPosition.x > m_vBox.x + (levels.front().boxSize)*(levels.front().gridSize)) {
		return false;
	}

	if(newPosition.y < m_vBox.y || newPosition.y > m_vBox.y + (levels.front().boxSize)*(levels.front().gridSize)) {
		return false;
	}

	return true;
}

bool GameWorld::CheckVector(Vector2D v)
{
	std::list<Vector2D>::iterator iter = std::find (m_occupiedPositions.begin(), m_occupiedPositions.end(), v);
	if(iter != m_occupiedPositions.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void GameWorld::CheckForWeapon() {
	std::queue<Vector2D> temp;
	while(!m_weaponPositions.empty()){
		if(m_player == m_weaponPositions.front()){
			// set that weapon has been picked up
			double x,y;
			x = m_vBox.x + ((levels.front().boxSize)*(levels.front().gridSize)+((levels.front().boxSize)/2)) + constControlWidth - 40;
			y = m_vBox.y + constControlHeight + 60;
			temp.push(Vector2D(x,y));
			m_hasWeapon = true;
		} else {
			temp.push(m_weaponPositions.front());
		}
		m_weaponPositions.pop();
	}
	m_weaponPositions = temp;
}

void GameWorld::CheckForManOverBoard() {
	if (m_player == m_manOverboard){
		if(m_enemyPositions.empty()){
			// Game Over!!!
			levels.pop();
			// reset variables ready for next level
			init = true;
			
		}
	}
}

void GameWorld::CheckForDanger() {
	// check for enemy or object to avoid
	// level restarts if player lands on one of these
	// may implement lives at some point
	std::queue<Vector2D> temp;
	while(!m_enemyPositions.empty()){
		if(m_player == m_enemyPositions.front()){
			// reset the grid
			if (!m_hasWeapon){
				init = true;
				m_commandQueue = std::deque<string>();
				return;
			}
		} else {
			temp.push(m_enemyPositions.front());
		}
		m_enemyPositions.pop();
	}
	m_enemyPositions = temp;

	std::queue<Vector2D> temp2;
	while(!m_objectsToAvoid.empty()){
		if(m_player == m_objectsToAvoid.front()){
			// reset the grid
			init = true;
			m_commandQueue = std::deque<string>();
			return;
		} else {
			temp2.push(m_objectsToAvoid.front());
			m_objectsToAvoid.pop();
		}
	}
	m_objectsToAvoid = temp2;
	
}

void GameWorld::DrawGameObjects(){
	gdi->GreenBrush();
	gdi->Circle(m_manOverboard.x, m_manOverboard.y, 20);

	std::queue<Vector2D> tempQueue = m_enemyPositions;
	while (!tempQueue.empty()){
		Vector2D position = tempQueue.front();
		gdi->BlackBrush();
		gdi->Circle(position.x, position.y, 20);
		tempQueue.pop();
	}

	tempQueue = m_weaponPositions;
	while (!tempQueue.empty()){
		Vector2D position = tempQueue.front();
		gdi->YellowBrush();
		gdi->Circle(position.x, position.y, 20);
		tempQueue.pop();
	}

	tempQueue = m_objectsToAvoid;
	while (!tempQueue.empty()){
		Vector2D position = tempQueue.front();
		gdi->RedBrush();
		gdi->Circle(position.x, position.y, 20);
		tempQueue.pop();
	}
}

void GameWorld::GenerateEnemyPoints() {
	int i = 0;
	while (i < levels.front().enemyCount)
	{
		// random numbers
		int randomX = rand() % (levels.front().gridSize) + 0;
		int randomY = rand() % (levels.front().gridSize) + 0;
		// check if equal to player
		Vector2D enemy = Vector2D(m_vBox.x + ((levels.front().boxSize)*randomX + ((levels.front().boxSize)/2)), m_vBox.y + ((levels.front().boxSize)*randomY + ((levels.front().boxSize)/2)));
		if (CheckVector(enemy)){
			m_enemyPositions.push(enemy);
			m_occupiedPositions.push_back(enemy);
			i++;
		}
	}
}

void GameWorld::GenerateWeaponPoints() {
	int i = 0;
	while (i < levels.front().weaponCount)
	{
		// random numbers
		int randomX = rand() % (levels.front().gridSize) + 0;
		int randomY = rand() % (levels.front().gridSize) + 0;
		// check if equal to player, check nothing else on square
		Vector2D weapon = Vector2D(m_vBox.x + ((levels.front().boxSize)*randomX + ((levels.front().boxSize)/2)), m_vBox.y + ((levels.front().boxSize)*randomY + ((levels.front().boxSize)/2)));
		if (CheckVector(weapon)){
			m_weaponPositions.push(weapon);
			m_occupiedPositions.push_back(weapon);
			i++;
		}
	}
}

void GameWorld::GenerateAvoidPoints() {
	int i = 0;
	while (i < levels.front().objectsToAvoidCount)
	{
		// random numbers
		int randomX = rand() % (levels.front().gridSize) + 0;
		int randomY = rand() % (levels.front().gridSize) + 0;
		// check if equal to player, check nothing else on square
		Vector2D barrel = Vector2D(m_vBox.x + ((levels.front().boxSize)*randomX + ((levels.front().boxSize)/2)), m_vBox.y + ((levels.front().boxSize)*randomY + ((levels.front().boxSize)/2)));
		if (CheckVector(barrel)){
			m_objectsToAvoid.push(barrel);
			m_occupiedPositions.push_back(barrel);
			i++;
		}
	}
}

//------------------------------ Render ----------------------------------
//------------------------------------------------------------------------
void GameWorld::Render()
{
  // find all the game objects and draw them!
	if (!levels.empty()){
		DrawGrid();
		DrawControls();

		// draw enemy ships
		if(init){
			m_enemyPositions = std::queue<Vector2D>();
			m_objectsToAvoid = std::queue<Vector2D>();
			m_weaponPositions = std::queue<Vector2D>();
			m_commandQueue = std::deque<string>();
			m_movesTaken = 0;
			// set the player and man overboard positions and direction
			m_player = Vector2D(m_vBox.x + ((levels.front().boxSize)/2), m_vBox.y + (((levels.front().gridSize)*(levels.front().boxSize))-(levels.front().boxSize)/2));
			m_manOverboard = Vector2D(m_vBox.x + (((levels.front().gridSize)*(levels.front().boxSize))-(levels.front().boxSize)/2), m_vBox.y + ((levels.front().boxSize)/2));
			m_playerDirection = 'N';
			// save positions for player and goal
			m_occupiedPositions.push_back(m_player);
			m_occupiedPositions.push_back(m_manOverboard);
			// save positions to prevent player being blocked in
			m_occupiedPositions.push_back(Vector2D(m_player.x, m_player.y - (levels.front().boxSize)));
			m_occupiedPositions.push_back(Vector2D(m_player.x + (levels.front().boxSize), m_player.y));
			GenerateEnemyPoints();
			GenerateWeaponPoints();
			GenerateAvoidPoints();
			init = false;
		}

		DrawGameObjects();

		if(m_runCommandSequence){
			// first time round unfold loop here
			if(m_unfoldLoop){
				UnFoldLoop();
				m_unfoldLoop = false;
			}
			RunCommandSequence();
			DrawPlayer(m_player);
			Sleep(500);
		} else {
			DrawPlayer(m_player);
		}
	}

}

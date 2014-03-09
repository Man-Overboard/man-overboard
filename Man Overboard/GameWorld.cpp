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
			init(true)
{
	// set the levels
	Level level1 = Level(100, 8, 3, 1, 2);
	levels.push(level1);

	Level level2 = Level(100, 8, 3, 1, 4);
	levels.push(level2);

	Level level3 = Level(50, 12, 4, 1, 3);
	levels.push(level3);
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

  switch(wParam)
  {


    case 'F':

		AddToQueueList("F");
      break;

    case 'R':
		 AddToQueueList("R");
      break;

	case 'L':
		 AddToQueueList("L");
      break;

	case 'C':
		while (!m_commandQueue.empty()) {
			m_commandQueue.pop();
		}	
		break;

	case 'G':
		RunCommandSequence();
		break;

  }//end switch
}

void GameWorld::AddToQueueList(string command)
{
	// logic to ensure queue not already full
	m_commandQueue.push(command);
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
	gdi->TextAtPos(x + 20, y + lineHeight*2, "FORWARD (F)");
	gdi->TextAtPos(x + 20, y + lineHeight*3, "TURN RIGHT 90 (R)");
	gdi->TextAtPos(x + 20, y + lineHeight*4, "TURN LEFT 90 (L)");
	gdi->TextAtPos(x + 20, y + lineHeight*5, "CLEAR (C)");
	gdi->TextAtPos(x + 20, y + lineHeight*6, "GO (G)");

	// Queued Moves
	gdi->TextAtPos(x + constControlWidth/4, y + lineHeight*10, "Queued Moves");
	std::queue<std::string> tempQueue = m_commandQueue;
	int counter = 0;
	while (!tempQueue.empty()){
		counter++;
		gdi->TextAtPos(x + 20, y + lineHeight*(10+counter), tempQueue.front());
		tempQueue.pop();
	}

	// Weapon Box
	gdi->Rect(x,y+constControlHeight+10,x+constControlWidth, y+constControlHeight+100);
	gdi->TextAtPos(x + constControlWidth/4,y+constControlHeight+40, "Weapon");
}

void GameWorld::DrawPlayer(Vector2D position) {
	gdi->WhiteBrush();
	gdi->Circle(position.x, position.y, 20);

	m_player = position;
}

void GameWorld::RunCommandSequence() {
	// pop the command off the queue
	while (!m_commandQueue.empty()){
		string value = m_commandQueue.front();
		if (value == "R" || value == "L"){
			TurnPlayer(value);
		}
		if (value == "F"){
			MovePlayer();
		}
		m_commandQueue.pop();
	}
	
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
			if (direction == "R"){
				m_playerDirection = 'E';
			}
			if (direction == "L"){
				m_playerDirection = 'W';
			}
			break;
		case 'E' :
			if (direction == "R"){
				m_playerDirection = 'S';
			}
			if (direction == "L"){
				m_playerDirection = 'N';
			}
			break;
		case 'S' :
			if (direction == "R"){
				m_playerDirection = 'W';
			}
			if (direction == "L"){
				m_playerDirection = 'E';
			}
			break;
		case 'W' :
			if (direction == "R"){
				m_playerDirection = 'N';
			}
			if (direction == "L"){
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
		}
		levels.pop();
		init = true;
		m_enemyPositions = std::queue<Vector2D>();
		m_objectsToAvoid = std::queue<Vector2D>();
		m_weaponPositions = std::queue<Vector2D>();
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
			init = true;
			m_enemyPositions = std::queue<Vector2D>();
			m_objectsToAvoid = std::queue<Vector2D>();
			m_weaponPositions = std::queue<Vector2D>();
			return;
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
			m_enemyPositions = std::queue<Vector2D>();
			m_objectsToAvoid = std::queue<Vector2D>();
			m_weaponPositions = std::queue<Vector2D>();
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
	DrawGrid();
	DrawControls();

	// draw enemy ships
	if(init){
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

	DrawPlayer(m_player);

}

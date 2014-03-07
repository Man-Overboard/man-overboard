#include "GameWorld.h"
#include "constants.h"
#include "Cgdi.h"
#include "WindowUtils.h"

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
			m_player(Vector2D(m_vBox.x + (constBoxSize/2), m_vBox.y + ((constLevelOneGridSize*constBoxSize)-constBoxSize/2))),
			m_playerDirection('N'),
			init(true)
{

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
  gdi->Rect(m_vBox.x, m_vBox.y, m_vBox.x + (constLevelOneGridSize*constBoxSize), m_vBox.y + (constLevelOneGridSize*constBoxSize) );
  int i;
  for(i=0; i <= constLevelOneGridSize;i++){
	  gdi->Line(Vector2D(m_vBox.x, m_vBox.y+(constBoxSize*i)), Vector2D(m_vBox.x + constBoxSize*constLevelOneGridSize, m_vBox.y+(constBoxSize*i)));
  }

  for(i=0; i <= constLevelOneGridSize;i++){
	  gdi->Line(Vector2D(m_vBox.x+(constBoxSize*i), m_vBox.y), Vector2D(m_vBox.x+(constBoxSize*i), m_vBox.y + constBoxSize*constLevelOneGridSize));
  }
}

void GameWorld::DrawControls(){
	gdi->BlackPen();
	gdi->WhiteBrush();

	// Container Box
	double x,y;
	x = m_vBox.x + (constBoxSize*constLevelOneGridSize+(constBoxSize/2));
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
			position.y = m_player.y - constBoxSize;
			break;
		case 'S' :
			position.x = m_player.x;
			position.y = m_player.y + constBoxSize;
			break;
		case 'E' :
			position.x = m_player.x + constBoxSize;
			position.y = m_player.y;
			break;
		case 'W' :
			position.x = m_player.x - constBoxSize;
			position.y = m_player.y;
			break;

	}
	bool valid = ValidateMove(position);
	if(valid){
		DrawPlayer(position);
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
	if(newPosition.x < m_vBox.x || newPosition.x > m_vBox.x + constBoxSize*constLevelOneGridSize) {
		return false;
	}

	if(newPosition.y < m_vBox.y || newPosition.y > m_vBox.y + constBoxSize*constLevelOneGridSize) {
		return false;
	}

	return true;
}

void GameWorld::DrawGameObjects(){
	std::queue<Vector2D> tempQueue = m_enemyPositions;
	while (!tempQueue.empty()){
		Vector2D position = tempQueue.front();
		gdi->BlackBrush();
		gdi->Circle(position.x, position.y, 20);
		tempQueue.pop();
	}
}

void GameWorld::GenerateEnemyPoints() {
	int i = 0;
	while (i < constLevelOneEnemyCount)
	{
		// random numbers
		int randomX = rand() % constLevelOneGridSize + 0;
		int randomY = rand() % constLevelOneGridSize + 0;
		// check if equal to player
		Vector2D enemy = Vector2D(m_vBox.x + (constBoxSize*randomX + (constBoxSize/2)), m_vBox.y + (constBoxSize*randomY + (constBoxSize/2)));
		if (enemy != m_player){
			m_enemyPositions.push(enemy);
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
		GenerateEnemyPoints();
		init = false;
	}

	DrawGameObjects();

	DrawPlayer(m_player);

}

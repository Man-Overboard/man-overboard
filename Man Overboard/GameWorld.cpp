#include "GameWorld.h"
#include "constants.h"
#include "Cgdi.h"
#include "WindowUtils.h"

#include <list>
#include <queue>

using std::list;
using std::string;


//------------------------------- creator --------------------------------
//------------------------------------------------------------------------
GameWorld::GameWorld(int cx, int cy):

            m_cxClient(cx),
            m_cyClient(cy),
            m_bPaused(false),
			m_vBox(Vector2D(cxClient()-(constWindowWidth-80), cyClient()-(constWindowHeight-80))), // get the values for the overall box the grid will be contained in
			m_player(Vector2D(m_vBox.x + (constBoxSize/2), m_vBox.y + ((constLevelOneGridSize*constBoxSize)-constBoxSize/2)))
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
		DrawPlayer(m_player.x, m_player.y - constBoxSize);
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

void GameWorld::DrawPlayer(int x, int y) {
	gdi->WhiteBrush();
	gdi->Circle(x, y, 20);

	m_player.x = x;
	m_player.y = y;
}


//------------------------------ Render ----------------------------------
//------------------------------------------------------------------------
void GameWorld::Render()
{
  // find all the game objects and draw them!
	DrawGrid();
	DrawControls();

	DrawPlayer(m_player.x, m_player.y);

  //gdi->TextAtPos(5, cyClient() - 20, "Click left mouse button to move crosshair, and right mouse button to move box");
  //gdi->TextAtPos(cxClient() -120, cyClient() - 20, "Press R to reset");
 // gdi->TextColor(Cgdi::grey);

}

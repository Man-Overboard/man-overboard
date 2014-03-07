#include "GameWorld.h"
#include "constants.h"
#include "Cgdi.h"
#include "WindowUtils.h"

#include <list>
using std::list;


//------------------------------- creator --------------------------------
//------------------------------------------------------------------------
GameWorld::GameWorld(int cx, int cy):

            m_cxClient(cx),
            m_cyClient(cy),
            m_bPaused(false),
			m_vBox(Vector2D(cxClient()-(constWindowWidth-80), cyClient()-(constWindowHeight-80))) // get the values for the overall box the grid will be contained in
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


    case 'P':
      
      TogglePause(); break;

      break;

    case 'Y':
		 
      break;

  }//end switch
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


//------------------------------ Render ----------------------------------
//------------------------------------------------------------------------
void GameWorld::Render()
{
  // find all the game objects and draw them!
	DrawGrid();


  //gdi->TextAtPos(5, cyClient() - 20, "Click left mouse button to move crosshair, and right mouse button to move box");

  //gdi->TextAtPos(cxClient() -120, cyClient() - 20, "Press R to reset");
 // gdi->TextColor(Cgdi::grey);



}

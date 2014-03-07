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
			m_vBox(Vector2D(cxClient()/8, cxClient()/1.15)), // get the values for the overall box the grid will be contained in
			rectangle1(Vector2D(m_vBox.x, m_vBox.y-600)), // top left corner
			rectangle2(Vector2D(m_vBox.x, m_vBox.y-400)), 
			rectangle3(Vector2D(m_vBox.x, m_vBox.y-200)),
			rectangle4(Vector2D(m_vBox.x, m_vBox.y)), // bottom left corner
			rectangle5(Vector2D(m_vBox.x+200, m_vBox.y-600)),
			rectangle6(Vector2D(m_vBox.x+200, m_vBox.y-400)),
			rectangle7(Vector2D(m_vBox.x+200, m_vBox.y-200)),
			rectangle8(Vector2D(m_vBox.x+200, m_vBox.y)),
			rectangle9(Vector2D(m_vBox.x+400, m_vBox.y-600)),
			rectangle10(Vector2D(m_vBox.x+400, m_vBox.y-400)),
			rectangle11(Vector2D(m_vBox.x+400, m_vBox.y-200)),
			rectangle12(Vector2D(m_vBox.x+400, m_vBox.y)),
			rectangle13(Vector2D(m_vBox.x+600, m_vBox.y-600)), // top right corner
			rectangle14(Vector2D(m_vBox.x+600, m_vBox.y-400)),
			rectangle15(Vector2D(m_vBox.x+600, m_vBox.y-200)),
			rectangle16(Vector2D(m_vBox.x+600, m_vBox.y)) // bottom right corner
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


//------------------------------ Render ----------------------------------
//------------------------------------------------------------------------
void GameWorld::Render()
{
  // find all the game objects and draw them!
 
  // the grid
  gdi->BlueBrush();
  gdi->BlackPen();
  gdi->Rect(rectangle1.x, rectangle1.y, rectangle1.x + 200, rectangle1.y - 200 );
  gdi->Rect(rectangle2.x, rectangle2.y, rectangle2.x + 200, rectangle2.y - 200 );
  gdi->Rect(rectangle3.x, rectangle3.y, rectangle3.x + 200, rectangle3.y - 200 );
  gdi->Rect(rectangle4.x, rectangle4.y, rectangle4.x + 200, rectangle4.y - 200 );
  gdi->Rect(rectangle5.x, rectangle5.y, rectangle5.x + 200, rectangle5.y - 200 );
  gdi->Rect(rectangle6.x, rectangle6.y, rectangle6.x + 200, rectangle6.y - 200 );
  gdi->Rect(rectangle7.x, rectangle7.y, rectangle7.x + 200, rectangle7.y - 200 );
  gdi->Rect(rectangle8.x, rectangle8.y, rectangle8.x + 200, rectangle8.y - 200 );
  gdi->Rect(rectangle9.x, rectangle9.y, rectangle9.x + 200, rectangle9.y - 200 );
  gdi->Rect(rectangle10.x, rectangle10.y, rectangle10.x + 200, rectangle10.y - 200 );
  gdi->Rect(rectangle11.x, rectangle11.y, rectangle11.x + 200, rectangle11.y - 200 );
  gdi->Rect(rectangle12.x, rectangle12.y, rectangle12.x + 200, rectangle12.y - 200 );
  gdi->Rect(rectangle13.x, rectangle13.y, rectangle13.x + 200, rectangle13.y - 200 );
  gdi->Rect(rectangle14.x, rectangle14.y, rectangle14.x + 200, rectangle14.y - 200 );
  gdi->Rect(rectangle15.x, rectangle15.y, rectangle15.x + 200, rectangle15.y - 200 );
  gdi->Rect(rectangle16.x, rectangle16.y, rectangle16.x + 200, rectangle16.y - 200 );
  //gdi->TextAtPos(5, cyClient() - 20, "Click left mouse button to move crosshair, and right mouse button to move box");

  //gdi->TextAtPos(cxClient() -120, cyClient() - 20, "Press R to reset");
 // gdi->TextColor(Cgdi::grey);



}

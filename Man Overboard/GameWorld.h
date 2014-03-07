#ifndef GameWorld_H
#define GameWorld_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   GameWorld.h
//
//  Desc:   All the environment data and methods for the Steering
//          Behavior projects. This class is the root of the project's
//          update and render calls (excluding main of course)
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <windows.h>
#include <vector>

#include "Vector2D.h"

class GameWorld
{ 
private:

  //set true to pause the motion
  bool                          m_bPaused;

  //local copy of client window dimensions
  int                           m_cxClient,
                                m_cyClient;

  //the positions for the grid layout
  Vector2D						m_vBox;

  //keeps track of the average FPS
  double                         m_dAvFrameTime;
  

public:
  
  GameWorld(int cx, int cy);

  ~GameWorld();

  void  Update(double time_elapsed);

  void  Render();
  
  //handle WM_COMMAND messages
  void        HandleKeyPresses(WPARAM wParam);
  void        HandleMenuItems(WPARAM wParam, HWND hwnd);
  
  void        TogglePause(){m_bPaused = !m_bPaused;}
  bool        Paused()const{return m_bPaused;}

  int   cxClient()const{return m_cxClient;}
  int   cyClient()const{return m_cyClient;}
 
};

#endif
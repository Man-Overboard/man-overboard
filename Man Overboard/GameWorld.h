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
#include <queue>
#include <list>

#include "Vector2D.h"
#include "Level.h"

class GameWorld
{ 
private:

  //set true to pause the motion
  bool                          m_bPaused;
  bool							init;

  //local copy of client window dimensions
  int                           m_cxClient,
                                m_cyClient;

  //the positions for the grid layout
  Vector2D						m_vBox;

  // player object 
  Vector2D						m_player;
  char							m_playerDirection;

  Vector2D						m_manOverboard;

  //keeps track of the average FPS
  double                         m_dAvFrameTime;

  std::queue<std::string>						m_commandQueue;
  std::queue<Vector2D>							m_enemyPositions;
  std::queue<Vector2D>							m_weaponPositions;
  std::list<Vector2D>							m_occupiedPositions;
  std::queue<Vector2D>							m_objectsToAvoid;

  // Levels
  std::queue<Level>								levels;

public:
  
  GameWorld(int cx, int cy);

  ~GameWorld();

  void  Update(double time_elapsed);

  void Render();
  void DrawGrid();
  void DrawControls();
  void DrawPlayer(Vector2D position);
  void DrawGameObjects();
  void GenerateEnemyPoints();
  void GenerateWeaponPoints();
  void GenerateAvoidPoints();
  
  //handle WM_COMMAND messages
  void        HandleKeyPresses(WPARAM wParam);
  void        HandleMenuItems(WPARAM wParam, HWND hwnd);
  
  void        TogglePause(){m_bPaused = !m_bPaused;}
  bool        Paused()const{return m_bPaused;}

  // adding to queue
  void	AddToQueueList(std::string command);
  bool ValidateMove(Vector2D newPosition);
  void RunCommandSequence();
  void MovePlayer();
  void TurnPlayer(std::string direction);
  bool CheckVector(Vector2D v);
  void CheckForWeapon();
  void CheckForManOverBoard();
  void CheckForDanger();

  int   cxClient()const{return m_cxClient;}
  int   cyClient()const{return m_cyClient;}
 
};

#endif
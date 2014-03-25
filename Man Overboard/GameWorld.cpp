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
	m_movesTaken(0),
	m_playerScore(0.0),
	m_isGameStart(true),
	m_totalMovesAllowed(0)
{
	// set the levels Level(int box, int grid, int enemy, int weapon, int objects);
	std::queue<string> level0Story = std::queue<string>();
	level0Story.push("Arr, me hearties!");
	level0Story.push("Navigate the seas to pick up yer");
	level0Story.push("first shipmate");

	std::queue<string> level1Story = std::queue<string>();
	level1Story.push("Ye seem to be getting the hang of this here vessel!");
	level1Story.push("We like to work in a sequential order here");
	level1Story.push("Let's save our next shipmate");

	std::queue<string> level2Story = std::queue<string>();
	level2Story.push("Ahoy Landlubber!");
	level2Story.push("Now here's one of those cargo I mentioned that ye have to avoid");
	level2Story.push("Good luck!");

	std::queue<string> level3Story = std::queue<string>();
	level3Story.push("Avast! Yet more of those blasted barrels floating the seas!");
	level3Story.push("Let's see if ye can conquer a larger section");

	std::queue<string> level4Story = std::queue<string>();
	level4Story.push("Pirates' ho!");
	level4Story.push("Lets blast them grubby pirates to smithereens!");
	level4Story.push("Pick up the weapon first then lets teach 'em a lesson.");

	std::queue<string> level5Story = std::queue<string>();
	level5Story.push("More o' them stinkin' Pirates!");
	level5Story.push("Lets show them what real men are made of!");
	level5Story.push("Watch out for that there explosive barrel again.");

	std::queue<string> level6Story = std::queue<string>();
	level6Story.push("Sounds like your losing your voice!");
	level6Story.push("Use some of those there commands to get the crew to do the same things again");
	level6Story.push("Shout START to begin the loop and then tell the crew how many times they need to do it.");
	level6Story.push("Let the crew know you've finished by shouting END");

	std::queue<string> level7Story = std::queue<string>();
	level7Story.push("Looks like them Pirates be coming at us again!");
	level7Story.push("Use your new found skills to get the crew to do more movements with less shouting.");
	level7Story.push("Once we sorted out those ruffians we can sail to pick up another of our fine bunch.");

	Level level0 = Level(200,2,0,0,0, false, false, 3, level0Story);
	levels.push(level0);

	Level level1 = Level(150,4,0,0,0, false, false, 9, level1Story);
	levels.push(level1);

	Level level2 = Level(150,4,0,0,1, false, false, 10, level2Story);
	levels.push(level2);

	Level level3 = Level(100,6,0,0,2, false, false, 15, level3Story);
	levels.push(level3);

	Level level4 = Level(150,4,1,1,0, false, false, 20, level4Story);
	levels.push(level4);

	Level level5 = Level(100,6,1,1,1, false, false, 35, level5Story);
	levels.push(level5);

	Level level6 = Level(180,4,0,0,0, true, false, 6, level6Story);
	levels.push(level6);

	Level level7 = Level(150,5,1,1,1, true, false, 25, level7Story);
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
	if((m_movesTaken == levels.front().maxMoves || m_commandQueue.size() == 15) && wParam != 'G' && wParam != 'C' && wParam != 'Z' && wParam != 'E'){
		return;
	}

	if(!m_isGameStart) {
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
	} else {
		switch(wParam){
		case 'G' :
			m_isGameStart = false;
			break;
		}
	}
}

void GameWorld::AddToQueueList(string command)
{
	// logic to ensure queue not already full
	m_commandQueue.push_back(command);
}


//-------------------------- HandleMenuItems -----------------------------
void GameWorld::HandleMenuItems(WPARAM wParam, HWND hwnd)
{
	//switch(wParam)
	//{

	//    break;
	//    
	//}//end switch
}

void GameWorld::DrawGrid(){
	// the grid
	gdi->LightBlueBrush();
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
	std::deque<string> tempQueue = m_commandQueue;
	int counter = 0;
	int offset = constControlTabOffset;
	while (!tempQueue.empty() && counter < 16){
		counter++;
		if(tempQueue.front() == "END"){
			offset = constControlTabOffset;
		}
		gdi->TextAtPos(x + offset, y + lineHeight*(12+counter), tempQueue.front());
		if(tempQueue.front() == "START"){
			offset = constControlTabOffset + 30;
		}
		tempQueue.pop_front();
	}

	// Weapon Box
	gdi->Rect(x+constControlWidth/2,y+constControlHeight+10,x+constControlWidth, y+constControlHeight+120);
	gdi->TextAtPos(x+20 + constControlWidth/2,y+constControlHeight+20, "Weapon");

	// Moves Box
	gdi->Rect(x,y+constControlHeight+10,x+constControlWidth/2, y+constControlHeight+120);
	gdi->TextAtPos(x + constControlWidth/10,y+constControlHeight+20, "Moves :");
	gdi->TextAtPos(x + constControlWidth/4,y+constControlHeight+60, std::to_string(m_movesTaken) + "/" + std::to_string(levels.front().maxMoves));

	// info 
	gdi->Rect(m_vBox.x, m_vBox.y+levels.front().gridSize*levels.front().boxSize + 20, m_vBox.x+levels.front().gridSize*levels.front().boxSize-(constControlWidth/2), m_vBox.y+levels.front().gridSize*levels.front().boxSize + 130);
	// loop over story queue and add to info box
	int storyCounter = 0;
	std::queue<string> tempStoryQueue = levels.front().story;
	while(!tempStoryQueue.empty()){
		gdi->TextAtPos(m_vBox.x+10,m_vBox.y+levels.front().gridSize*levels.front().boxSize + 30 + storyCounter*20, tempStoryQueue.front());
		tempStoryQueue.pop();
		storyCounter++;
	}

	// compass box
	gdi->Rect(m_vBox.x+levels.front().gridSize*levels.front().boxSize-(constControlWidth/2), m_vBox.y+levels.front().gridSize*levels.front().boxSize + 20, m_vBox.x+levels.front().gridSize*levels.front().boxSize, m_vBox.y+levels.front().gridSize*levels.front().boxSize + 130);
	gdi->TextAtPos(m_vBox.x-5+levels.front().gridSize*levels.front().boxSize-(constControlWidth/4),m_vBox.y+levels.front().gridSize*levels.front().boxSize + 35, "N");
	gdi->TextAtPos(m_vBox.x-5+levels.front().gridSize*levels.front().boxSize-(constControlWidth/4),m_vBox.y+levels.front().gridSize*levels.front().boxSize + 100, "S");
	gdi->TextAtPos(m_vBox.x+30+levels.front().gridSize*levels.front().boxSize-(constControlWidth/4),m_vBox.y+levels.front().gridSize*levels.front().boxSize + 65, "E");
	gdi->TextAtPos(m_vBox.x-40+levels.front().gridSize*levels.front().boxSize-(constControlWidth/4),m_vBox.y+levels.front().gridSize*levels.front().boxSize + 65, "W");
	Vector2D from = Vector2D(m_vBox.x+levels.front().gridSize*levels.front().boxSize-(constControlWidth/4),m_vBox.y+levels.front().gridSize*levels.front().boxSize + 70);
	Vector2D to;
	switch(m_playerDirection){
		case 'N' :
			to = Vector2D(m_vBox.x+levels.front().gridSize*levels.front().boxSize-(constControlWidth/4),m_vBox.y+levels.front().gridSize*levels.front().boxSize + 50);
			break;
		case 'E' :
			to = Vector2D(m_vBox.x+20+levels.front().gridSize*levels.front().boxSize-(constControlWidth/4),m_vBox.y+levels.front().gridSize*levels.front().boxSize + 70);
			break;
		case 'S' :
			to = Vector2D(m_vBox.x+levels.front().gridSize*levels.front().boxSize-(constControlWidth/4),m_vBox.y+levels.front().gridSize*levels.front().boxSize + 100);
			break;
		case 'W' :
			to = Vector2D(m_vBox.x-20+levels.front().gridSize*levels.front().boxSize-(constControlWidth/4),m_vBox.y+levels.front().gridSize*levels.front().boxSize + 70);
			break;
	}
	gdi->BlackBrush();
	gdi->LineWithArrow(from, to, 10);
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
			// update player score for level
			m_playerScore += m_movesTaken;
			m_totalMovesAllowed += levels.front().maxMoves;
			levels.pop();
			if(levels.empty()){
				// display player score
				m_playerScore = 100 - ((m_playerScore/m_totalMovesAllowed)*100);
			}
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

void GameWorld::DrawGameCompleteScreen() {
	gdi->WhiteBrush();
	double left = m_vBox.x+(constWindowWidth-900)/2;
	gdi->Rect(left, m_vBox.y, (double)constWindowWidth-((constWindowWidth-900)/2), m_vBox.y + 900);

	gdi->BlackPen();
	gdi->TextAtPos(left+300, m_vBox.y + 100, "Congratulations, Matey!");
	gdi->TextAtPos(left+300, m_vBox.y + 250, "Your Score is " + std::to_string(m_playerScore));
}

void GameWorld::DrawSplashScreen() {
	gdi->WhiteBrush();
	double left = (m_vBox.x+(constWindowWidth-900)/2)-80;
	gdi->Rect(left, m_vBox.y, (double)constWindowWidth-((constWindowWidth-900)/2), m_vBox.y + 900);

	gdi->BlackPen();
	gdi->TextAtPos(left+300, m_vBox.y + 100, "Ahoy there, Matey!");
	gdi->TextAtPos(left+50, m_vBox.y + 150, "We've been sailing the High Seas and we seem to have run into a spot of trouble! During a dark and stormy night");
	gdi->TextAtPos(left+50, m_vBox.y + 170, "some of our salty sea dogs fell overboard! We need your help to rescue the crew in order to plunder the seven seas"); 
	gdi->TextAtPos(left+50, m_vBox.y + 190, "once more! The storm also lost us some of our cargo, dangerous stuff if ye sail over it!"); 
	gdi->TextAtPos(left+50, m_vBox.y + 210, "Oh and one other thing, ye best be wary o' them pesky pirates!");

	// show objects and descriptions
	// player
	gdi->WhiteBrush();
	gdi->Circle(left+200, m_vBox.y + 300, 20);
	gdi->TextAtPos(left+250, m_vBox.y + 290, "You");

	// man overboard
	gdi->GreenBrush();
	gdi->Circle(left+200, m_vBox.y + 400, 20);
	gdi->TextAtPos(left+250, m_vBox.y + 390, "Man Overboard");

	// cargo - to avoid
	gdi->RedBrush();
	gdi->Circle(left+200, m_vBox.y + 500, 20);
	gdi->TextAtPos(left+250, m_vBox.y + 490, "Cargo - AVOID!");

	// pirate ship to defeat
	gdi->BlackBrush();
	gdi->Circle(left+200, m_vBox.y + 600, 20);
	gdi->TextAtPos(left+250, m_vBox.y + 590, "Pirate Ship");

	// weapon to pick up before pirate ship can be destroyed
	gdi->YellowBrush();
	gdi->Circle(left+200, m_vBox.y + 700, 20);
	gdi->TextAtPos(left+250, m_vBox.y + 690, "Weapon - Must be picked up before Pirates can be defeated");

	gdi->TextAtPos(left+700, m_vBox.y + 800, "Press G to Save Our Souls!");
}

//------------------------------ Render ----------------------------------
//------------------------------------------------------------------------
void GameWorld::Render()
{
	gdi->BlueBrush();
	gdi->Rect(m_cxClient, m_cyClient, m_cxClient-constWindowWidth, m_cyClient-constWindowHeight);

	// find all the game objects and draw them!
	if (m_isGameStart){
		// splash screen for start of game
		DrawSplashScreen();
	} else if (!levels.empty()){
		DrawGrid();
		DrawControls();

		// draw enemy ships
		if(init){
			m_enemyPositions = std::queue<Vector2D>();
			m_objectsToAvoid = std::queue<Vector2D>();
			m_weaponPositions = std::queue<Vector2D>();
			m_commandQueue = std::deque<string>();
			m_movesTaken = 0;
			m_hasWeapon = false;
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
	} else {
		DrawGameCompleteScreen();
	}

}

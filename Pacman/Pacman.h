#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

#include "S2D/S2D.h"

#define MUNCHIECOUNT 306
#define GHOSTCOUNT 4
#define WALLCOUNT 482

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;
using namespace std;

// Data to represent Pacman
struct Player
{
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
	int direction;
	int frame;
	int currentFrameTime;
	int move;
	float speedMultiplier;
	bool dead;
};

// Data to represent Munchie
struct Munchie
{
	Vector2* position;
	Rect* sourceRect;
	Texture2D* munchieTex;
	int frame;
	int currentFrameTime;
};

// Data to represent Cherry collectible
struct Cherry
{
	Vector2* posRect;
	Rect* sourceRect;
	Texture2D* texture;
	int frame;
	int currentFrameTime;
	int direction;
	int move;
	int midChange;
	float speed;
};

// Data to represent Ghosts
struct Enemy
{
	Vector2* posRect;
	Rect* sourceRect;
	Texture2D* texture;
	int direction;
	int move;
	int midChange;
	float speed;
};

// Data to represent Maze Walls
struct Wall
{
	Vector2* posRect;
	Rect* sourceRect;
	Texture2D* texture;
};

// Data to represent Title Screen Logo
struct Logo
{
	Vector2* posRect;
	Rect* sourceRect;
	Texture2D* texture;
};

// Data for Pause Menu
struct Menu
{
	Texture2D* background;
	Rect* rectangle;
	Vector2* stringPosition;
	bool paused;
};

// Data to represent Game Over Screen
struct GameOver
{
	Texture2D* background;
	Rect* rectangle;
	Vector2* stringPosition;
	bool gameOver;
};

// Enum Class for Game States
enum class GameState
{
	START = 0,
	PLAY = 1,
	PAUSE = 2,
	GAME_OVER = 3,
	WIN = 4, 
	DEATH = 5
};

// Enum Class for Player's Movement
enum class Movement
{
	NEUTRAL = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3,
	UP = 4
};

// Enum Class for whether or not the Cherry is visible
enum class CherryVisible
{
	INVISIBLE = 0,
	VISIBLE = 1
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	// Sound Effects
	SoundEffect* _chomp;
	SoundEffect* _pop;
	SoundEffect* _titleMusic;
	SoundEffect* _launchGame;
	SoundEffect* _death;
	SoundEffect* _winSound;
	SoundEffect* _mainMusic;

	// Structures
	Player* _pacman;
	Munchie* _munchies[MUNCHIECOUNT];
	Cherry* _cherry;
	Enemy* _ghosts[GHOSTCOUNT];
	Wall* _wall[WALLCOUNT];
	Menu* _menu;
	GameOver* _gameOver;
	Logo* _logo;

	// Enum Classes
	GameState _gameState = GameState::START;
	Movement _movement = Movement::NEUTRAL;
	CherryVisible _cherryVisible = CherryVisible::INVISIBLE;

	// Integers
	int _wallCount;
	int _munchieCount;
	int pacmanSpeed;
	int _score;
	int _winCount;
	int _levelCount;
	int _deathTime;
	int _winTime;
	int _launchWait;

	// Position for String
	Vector2* _stringPosition;
	Vector2* _startPosition;

	// Constant data for Game Variables
	const int _cPacmanFrameTime;
	const int _cMunchieFrameTime;

	// Booleans
	bool _pKeyDown;
	bool _wKeyDown;
	bool _aKeyDown;
	bool _sKeyDown;
	bool _dKeyDown;
	bool _launchTrigger;
	bool _spaceKeyDown;

	// Public Methods
	void Title(Input::KeyboardState* keyboardState);
	void Input(int elapsedTime, Input::KeyboardState* keyboardState);
	void PacmanMove(Input::KeyboardState* state);
	void CheckPausedTrue(Input::KeyboardState* state, Input::MouseState* mouseState, Input::Keys pauseKey);
	void CheckPausedFalse(Input::KeyboardState* state, Input::MouseState* mouseState, Input::Keys pauseKey);
	void CheckDead(Input::KeyboardState* state, Input::Keys spaceKey);
	void CheckViewportCollision(Enemy & Ghost);
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(Munchie& munchie, int elapsedTime);
	void MunchieCollision(Munchie& munchie);
	void UpdateGhosts(Enemy& ghost, int elapsedTime);
	void CheckGhostCollisions(Enemy& ghost);
	void UpdateCherry(int elapsedTime);
	void CherryCollision();
	void PacmanDeath();
	void Win();
	void Reload();

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	// Collision Methods
	bool CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
	bool RightCollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
	bool LeftCollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
	bool TopCollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
	bool BottomCollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

	bool Right();
	bool Left();
	bool Top();
	bool Bottom();

	bool GhostRight(Enemy& ghost);
	bool GhostLeft(Enemy& ghost);
	bool GhostTop(Enemy& ghost);
	bool GhostBottom(Enemy& ghost);

	bool CherryRight();
	bool CherryLeft();
	bool CherryTop();
	bool CherryBottom();

	bool CircleCollision(int radius1, int x1, int y1, int radius2, int x2, int y2);
};
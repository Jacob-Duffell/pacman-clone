#include "Pacman.h"

#include <iostream>
#include <sstream>
#include <time.h>
#include <math.h>

void Pacman::Title(Input::KeyboardState* state)
{
	// Triggers Title Music
	if (_logo->posRect->Y == -100)
	{
		Audio::Play(_titleMusic);
	}

	// Skips Intro Animation
	if (_logo->posRect->Y != 100 && state->IsKeyDown(Input::Keys::SPACE) && !_spaceKeyDown)
	{
		_spaceKeyDown = true;
		_logo->posRect->Y = 100;
		_startPosition->Y = 350;

	}
	// Intro Animation
	else if (_logo->posRect->Y != 100)
	{
		_logo->posRect->Y += 1;
		_startPosition->Y -= 1;
	}
	// Triggers the game to start
	else if (_logo->posRect->Y == 100 && state->IsKeyDown(Input::Keys::SPACE) && !_spaceKeyDown)
	{
		_spaceKeyDown = true;
		_launchTrigger = true;
		Audio::Stop(_titleMusic);

		if (_launchWait == 0)
		{
			Audio::Play(_launchGame);
		}
	}

	// Allows time for _launchGame music to play and prevents game from launching too quickly
	if (_launchWait < 180 && _launchTrigger == true)
	{
		_launchWait++;
	}
	// Launches game
	else if (_launchWait >= 180 && _launchTrigger == true)
	{
		_gameState = GameState::PLAY;
		Audio::Play(_mainMusic);
		_mainMusic->SetLooping(true);
	}

	// Prevents space bar from being held down
	if (state->IsKeyUp(Input::Keys::SPACE))
	{
		_spaceKeyDown = false;
	}
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state)
{
	// Checks if D key is pressed
	if ((state->IsKeyDown(Input::Keys::D)) && (!Right()) && (!_dKeyDown) && (_pacman->move == 20 || _pacman->move == 0))
	{
		_dKeyDown = true;
		_movement = Movement::RIGHT;
	}

	// Checks if A key is pressed
	else if ((state->IsKeyDown(Input::Keys::A)) && (!Left()) && (!_aKeyDown) && (_pacman->move == 20 || _pacman->move == 0))
	{
		_aKeyDown = true;
		_movement = Movement::LEFT;
	}

	// Checks if W key is pressed
	else if ((state->IsKeyDown(Input::Keys::W)) & (!Top()) && (!_wKeyDown) && (_pacman->move == 20 || _pacman->move == 0))
	{
		_wKeyDown = true;
		_movement = Movement::UP;
	}

	// Checks if S key is pressed
	else if ((state->IsKeyDown(Input::Keys::S)) & (!Bottom()) && (!_sKeyDown) && (_pacman->move == 20 || _pacman->move == 0))
	{
		_sKeyDown = true;
		_movement = Movement::DOWN;
	} 

	if (state->IsKeyUp(Input::Keys::W))
	{
		_wKeyDown = false;
	}

	if (state->IsKeyUp(Input::Keys::A))
	{
		_aKeyDown = false;
	}

	if (state->IsKeyUp(Input::Keys::S))
	{
		_sKeyDown = false;
	}

	if (state->IsKeyUp(Input::Keys::D))
	{
		_dKeyDown = false;
	}
}

void Pacman::PacmanMove(Input::KeyboardState* state)
{
	switch (_movement)
	{
		case Movement::RIGHT:
		{
			// Establishes movement speed
			if (_pacman->move < 20)
			{
				_pacman->position->X += 1; //Moves Pacman across X axis
				_pacman->move++;
				_pacman->direction = 0;
			}
			// Resets movement cycle
			else if (state->IsKeyDown(Input::Keys::D) && (!Right()))
			{
				_pacman->move = 0;
			}
			// If player is not moving, do nothing
			else
			{
				_movement = Movement::NEUTRAL;
			}
			break;
		}

		case Movement::LEFT:
		{
			// Establishes movement speed
			if (_pacman->move < 20)
			{
				_pacman->position->X -= 1; //Moves Pacman across -X axis
				_pacman->move++;
				_pacman->direction = 2;
			}
			// Resets movement cycle
			else if (state->IsKeyDown(Input::Keys::A) && (!Left()))
			{
				_pacman->move = 0;
			}
			// If player is not moving, do nothing
			else
			{
				_movement = Movement::NEUTRAL;
			}
			break;
		}

		case Movement::UP:
		{
			// Establishes movement speed
			if (_pacman->move < 20)
			{
				_pacman->position->Y -= 1; //Moves Pacman across -Y axis
				_pacman->move++;
				_pacman->direction = 3;
			}
			// Resets movement cycle
			else if (state->IsKeyDown(Input::Keys::W) && (!Top()))
			{
				_pacman->move = 0;
			}
			// If player is not moving, do nothing
			else
			{
				_movement = Movement::NEUTRAL;
			}
			break;
		}

		case Movement::DOWN:
		{
			// Establishes movement speed
			if (_pacman->move < 20)
			{
				_pacman->position->Y += 1; //Moves Pacman across Y axis
				_pacman->move++;
				_pacman->direction = 1;
			}
			// Resets movement cycle
			else if (state->IsKeyDown(Input::Keys::S) && (!Bottom()))
			{
				_pacman->move = 0;
			}
			// If player is not moving, do nothing
			else
			{
				_movement = Movement::NEUTRAL;
			}
			break;
		}
	}
}

void Pacman::CheckPausedTrue(Input::KeyboardState* state, Input::MouseState* mouseState, Input::Keys pauseKey)
{
	// If P key is pressed
	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_menu->paused = !_menu->paused;
		Audio::Pause(_mainMusic);
		_gameState = GameState::PAUSE;
	}

	if (state->IsKeyUp(Input::Keys::P))
	{
		_pKeyDown = false;
	}
}

void Pacman::CheckPausedFalse(Input::KeyboardState* state, Input::MouseState* mouseState, Input::Keys pauseKey)
{
	// If P key is pressed
	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_menu->paused = !_menu->paused;
		Audio::Resume(_mainMusic);
		_gameState = GameState::PLAY;
	}

	if (state->IsKeyUp(Input::Keys::P))
	{
		_pKeyDown = false;
	}
}

void Pacman::CheckDead(Input::KeyboardState* state, Input::Keys spaceKey)
{
	// If space key is pressed, start a new game
	if (state->IsKeyDown(Input::Keys::SPACE))
	{
		_spaceKeyDown = true;
		Audio::Play(_launchGame);
		_score = 0;
		Reload();
		_gameState = GameState::PLAY;
		Audio::Play(_mainMusic);
	}
	// If Q key is pressed, return to title screen
	else if (state->IsKeyDown(Input::Keys::Q))
	{
		_spaceKeyDown = true;
		_score = 0;
		_launchWait = 0;
		Reload();
		_launchTrigger = false;
		_gameState = GameState::START;
	}

	if (state->IsKeyUp(Input::Keys::SPACE) || state->IsKeyUp(Input::Keys::Q))
	{
		_spaceKeyDown = false;
	}
}

void Pacman::CheckViewportCollision(Enemy & ghost)
{
	// Checks if Pacman is trying to disappear
	if (_pacman->position->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth() + 19)
	{
		// Pacman hit right wall - reset his position
		_pacman->position->X = 20 - _pacman->sourceRect->Width;
	}

	if (_pacman->position->X + _pacman->sourceRect->Width < 1)
	{
		// Pacman hit left wall - reset his position
		_pacman->position->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width;
	}

	// Checks if Ghost is trying to disappear
	if (ghost.posRect->X + ghost.sourceRect->Width > Graphics::GetViewportWidth() + 19)
	{
		// Ghost hit right wall - reset his position
		ghost.posRect->X = 20 - ghost.sourceRect->Width;
	}

	if (ghost.posRect->X + ghost.sourceRect->Width < 1)
	{
		// Ghost hit left wall - reset his position
		ghost.posRect->X = Graphics::GetViewportWidth() - ghost.sourceRect->Width;
	}

	if (_cherryVisible == CherryVisible::VISIBLE)
	{
		// Checks if Cherry is trying to disappear
		if (_cherry->posRect->X + _cherry->sourceRect->Width > Graphics::GetViewportWidth() + 19)
		{
			// Cherry hit right wall - reset it's position
			_cherry->posRect->X = 20 - _cherry->sourceRect->Width;
		}

		if (_cherry->posRect->X + _cherry->sourceRect->Width < 1)
		{
			// Ghost hit left wall - reset his position
			_cherry->posRect->X = Graphics::GetViewportWidth() - _cherry->sourceRect->Width;
		}
	}
	
}

void Pacman::UpdatePacman(int elapsedTime)
{
	_pacman->currentFrameTime += elapsedTime;

	// Keeps frames between 0 and 1
	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;

		if (_pacman->frame >= 2)
		{
			_pacman->frame = 0;
		}

		_pacman->currentFrameTime = 0;
	}

	// Sets Pacman source rectangle for animation
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
}

void Pacman::UpdateMunchie(Munchie& munchie, int elapsedTime)
{
		munchie.currentFrameTime += elapsedTime;

		// Keeps frames between 0 and 1
		if (munchie.currentFrameTime > _cMunchieFrameTime)
		{
			munchie.frame++;

			if (munchie.frame >= 2)
			{
				munchie.frame = 0;
			}

			munchie.currentFrameTime = 0;
		}

		// Sets position of source rect for animation
		munchie.sourceRect->X = (munchie.sourceRect->Width) * (munchie.frame);
}

void Pacman::MunchieCollision(Munchie& munchie)
{
	// If player collides with munchie
	if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, munchie.position->X, munchie.position->Y, munchie.sourceRect->Width, munchie.sourceRect->Height))
	{
		Audio::Play(_chomp);

		// Move munchie off screen
		munchie.position->X = -100;
		munchie.position->Y = -100;

		_score += 10;
		_winCount++;
		
		// If player has collected half the available munchies, Cherry is made visible
		if (_winCount == MUNCHIECOUNT / 2)
		{
			Audio::Play(_winSound);
			_cherryVisible = CherryVisible::VISIBLE;
		}

		// If player has collected all munchies, go to next level
		if (_winCount == MUNCHIECOUNT)
		{
			Audio::Stop(_mainMusic);
			_levelCount++;
			Audio::Play(_winSound);
			_gameState = GameState::WIN;
		}
	}
}

void Pacman::UpdateGhosts(Enemy& ghost, int elapsedTime)
{
	// Randomises whether ghost changes direction at certain points
	ghost.midChange = rand() % 2;

	switch (ghost.direction)
	{
		case (3):
		{
			// Moves ghost up
			if (ghost.move < 20 && !GhostTop(ghost))
			{
				ghost.posRect->Y -= 1; //Moves Pacman across -Y axis
				ghost.move++;
				ghost.direction = 3;
			}
			// Moves ghost right if possible
			else if (!GhostTop(ghost) && !GhostRight(ghost) && (ghost.midChange == 0 || (_pacman->position->X >= ghost.posRect->X && _pacman->position->Y == ghost.posRect->Y)))
			{
				ghost.direction = 0;
			}
			// Moves ghost left if possible
			else if (!GhostTop(ghost) && !GhostLeft(ghost) && (ghost.midChange == 0 || (_pacman->position->X <= ghost.posRect->X && _pacman->position->Y == ghost.posRect->Y)))
			{
				ghost.direction = 2;
			}
			// Resets animation cycle if ghost has not collided with a wall
			else if (!GhostTop(ghost))
			{
				ghost.move = 0;
			}

			// If ghost "sees" pacman, moves towards him
			if (_pacman->position->X == ghost.posRect->X && _pacman->position->Y <= ghost.posRect->Y)
			{
				ghost.direction = 3;
			}
			// Change direction when collision occurs
			else if (GhostTop(ghost) && (ghost.move == 0 || ghost.move == 20))
			{
				int change = rand() % 2;

				if (change == 0)
				{
					ghost.direction = 2;
				}
				else if (change == 1)
				{
					ghost.direction = 0;
				}
			}
			break;
		}

		case (1):
		{
			// Moves ghost down
			if (ghost.move < 20 && !GhostBottom(ghost))
			{
				ghost.posRect->Y += 1; //Moves Pacman across -Y axis
				ghost.move++;
				ghost.direction = 1;
			}
			// Moves ghost right if possible
			else if (!GhostBottom(ghost) && !GhostRight(ghost) && (ghost.midChange == 0 || (_pacman->position->X >= ghost.posRect->X && _pacman->position->Y == ghost.posRect->Y)))
			{
				ghost.direction = 0;
			}
			// Moves ghost left if possible
			else if (!GhostBottom(ghost) && !GhostLeft(ghost) && (ghost.midChange == 0 || (_pacman->position->X <= ghost.posRect->X && _pacman->position->Y == ghost.posRect->Y)))
			{
				ghost.direction = 2;
			}
			// Resets animation cycle
			else if (!GhostBottom(ghost))
			{
				ghost.move = 0;
			}

			// If ghost "sees" pacman, move towards him
			if (_pacman->position->X == ghost.posRect->X && _pacman->position->Y >= ghost.posRect->Y)
			{
				ghost.direction = 1;
			}
			// Change direction if collision occurs
			else if (GhostBottom(ghost) && (ghost.move == 0 || ghost.move == 20))
			{
				int change = rand() % 2;

				if (change == 0)
				{
					ghost.direction = 2;
				}
				else if (change == 1)
				{
					ghost.direction = 0;
				}
			}
			break;
		}
		
		case (2):
		{
			// Moves ghost left
			if (ghost.move < 20 && !GhostLeft(ghost))
			{
				ghost.posRect->X -= 1; //Moves Pacman across -Y axis
				ghost.move++;
				ghost.direction = 2;
			}
			// Moves ghost up, if possible
			else if (!GhostLeft(ghost) && !GhostTop(ghost) && (ghost.midChange == 0 || (_pacman->position->X == ghost.posRect->X && _pacman->position->Y <= ghost.posRect->Y)))
			{
				ghost.direction = 3;
			}
			// Moves ghost down, if possible
			else if (!GhostLeft(ghost) && !GhostBottom(ghost) && (ghost.midChange == 0 || (_pacman->position->X == ghost.posRect->X && _pacman->position->Y >= ghost.posRect->Y)))
			{
				ghost.direction = 1;
			}
			// Resets animation cycle
			else if (!GhostLeft(ghost))
			{
				ghost.move = 0;
			}

			// If ghost "sees" pacman, move towards him
			if (_pacman->position->X <= ghost.posRect->X && _pacman->position->Y == ghost.posRect->Y)
			{
				ghost.direction = 2;
			}
			// Change direction if collision occurs
			else if (GhostLeft(ghost) && (ghost.move == 0 || ghost.move == 20))
			{
				int change = rand() % 2;

				if (change == 0)
				{
					ghost.direction = 3;
				}
				else if (change == 1)
				{
					ghost.direction = 1;
				}
			}
			break;
		}

		case (0):
		{
			// Move ghost right
			if (ghost.move < 20 && !GhostRight(ghost))
			{
				ghost.posRect->X += 1; //Moves Pacman across -Y axis
				ghost.move++;
				ghost.direction = 0;
			}
			// Move ghost up, if possible
			else if (!GhostRight(ghost) && !GhostTop(ghost) && (ghost.midChange == 0 || (_pacman->position->X == ghost.posRect->X && _pacman->position->Y <= ghost.posRect->Y)))
			{
				ghost.direction = 3;
			}
			// Move ghost down, if possible
			else if (!GhostRight(ghost) && !GhostBottom(ghost) && (ghost.midChange == 0 || (_pacman->position->X == ghost.posRect->X && _pacman->position->Y >= ghost.posRect->Y)))
			{
				ghost.direction = 1;
			}
			// Reset animation cycle
			else if (!GhostRight(ghost))
			{
				ghost.move = 0;
			}

			// If ghost "sees" pacman, move towards him
			if (_pacman->position->X >= ghost.posRect->X && _pacman->position->Y == ghost.posRect->Y)
			{
				ghost.direction = 0;
			}
			// Change direction if collision occurs
			else if (GhostRight(ghost) && (ghost.move == 0 || ghost.move == 20))
			{
				int change = rand() % 2;

				if (change == 0)
				{
					ghost.direction = 3;
				}
				else if (change == 1)
				{
					ghost.direction = 1;
				}
			}
			break;
		}
	}

	// Set ghost source rect for animation purposes
	ghost.sourceRect->Y = ghost.sourceRect->Height * ghost.direction;
}

void Pacman::CheckGhostCollisions(Enemy& ghost)
{
	// If pacman collides with ghost, trigger Game Over
	if (CircleCollision(_pacman->sourceRect->Width / 2, _pacman->position->X, _pacman->position->Y, ghost.sourceRect->Width / 2, ghost.posRect->X, ghost.posRect->Y))
	{
		Audio::Stop(_mainMusic);
		_pacman->dead = !_pacman->dead;
		_gameState = GameState::DEATH;
		Audio::Play(_death);
	}
}

void Pacman::UpdateCherry(int elapsedTime)
{
	_cherry->currentFrameTime += elapsedTime;

	// Change Cherry frame from 0 to 1
	if (_cherry->currentFrameTime > _cMunchieFrameTime)
	{
		_cherry->frame++;

		if (_cherry->frame >= 2)
		{
			_cherry->frame = 0;
		}

		_cherry->currentFrameTime = 0;
	}

	// Set position of Cherry source rect for animation purposes
	_cherry->sourceRect->X = (_cherry->sourceRect->Width) * (_cherry->frame);

	// Randomise whether the cherry moves or not
	_cherry->midChange = rand() % 2;

	switch (_cherry->direction)
	{
	case (3) :
	{
		// Move cherry up
		if (_cherry->move < 20 && !CherryTop())
		{
			_cherry->posRect->Y -= 1; //Moves Pacman across -Y axis
			_cherry->move++;
			_cherry->direction = 3;
		}
		// Moves cherry right, if possible
		else if (!CherryTop() && !CherryRight() && _cherry->midChange == 0)
		{
			_cherry->direction = 0;
		}
		// Moves cherry left, if possible
		else if (!CherryTop() && !CherryLeft() && _cherry->midChange == 0)
		{
			_cherry->direction = 2;
		}
		// Resets animation cycle
		else if (!CherryTop())
		{
			_cherry->move = 0;
		}

		// Changes direction if collision occurs
		if (CherryTop() && (_cherry->move == 0 || _cherry->move == 20))
		{
			int change = rand() % 2;

			if (change == 0)
			{
				_cherry->direction = 2;
			}
			else if (change == 1)
			{
				_cherry->direction = 0;
			}
		}
		break;
	}

	case (1) :
	{
		// Moves cherry down
		if (_cherry->move < 20 && !CherryBottom())
		{
			_cherry->posRect->Y += 1; //Moves Pacman across -Y axis
			_cherry->move++;
			_cherry->direction = 1;
		}
		// Moves cherry right, if possible
		else if (!CherryBottom() && !CherryRight() && _cherry->midChange == 0)
		{
			_cherry->direction = 0;
		}
		// Moves cherry left, if possible
		else if (!CherryBottom() && !CherryLeft() && _cherry->midChange == 0)
		{
			_cherry->direction = 2;
		}
		// Resets animation cycle
		else if (!CherryBottom())
		{
			_cherry->move = 0;
		}

		// Chenge direction if collision occurs
		if (CherryBottom() && (_cherry->move == 0 || _cherry->move == 20))
		{
			int change = rand() % 2;

			if (change == 0)
			{
				_cherry->direction = 2;
			}
			else if (change == 1)
			{
				_cherry->direction = 0;
			}
		}
		break;
	}

	case (2) :
	{
		// Moves cherry left
		if (_cherry->move < 20 && !CherryLeft())
		{
			_cherry->posRect->X -= 1; //Moves Pacman across -Y axis
			_cherry->move++;
			_cherry->direction = 2;
		}
		// Moves cherry up, if possible
		else if (!CherryLeft() && !CherryTop() && _cherry->midChange == 0)
		{
			_cherry->direction = 3;
		}
		// Moves cherry down if possible
		else if (!CherryLeft() && !CherryBottom() && _cherry->midChange == 0)
		{
			_cherry->direction = 1;
		}
		// Resets animation cycle
		else if (!CherryLeft())
		{
			_cherry->move = 0;
		}

		// Changes direction if collision occurs
		if (CherryLeft() && (_cherry->move == 0 || _cherry->move == 20))
		{
			int change = rand() % 2;

			if (change == 0)
			{
				_cherry->direction = 3;
			}
			else if (change == 1)
			{
				_cherry->direction = 1;
			}
		}
		break;
	}

	case (0) :
	{
		// Moves cherry right
		if (_cherry->move < 20 && !CherryRight())
		{
			_cherry->posRect->X += 1; //Moves Pacman across -Y axis
			_cherry->move++;
			_cherry->direction = 0;
		}
		// Moves cherry up, if possible
		else if (!CherryRight() && !CherryTop() && _cherry->midChange == 0)
		{
			_cherry->direction = 3;
		}
		// Moves cherry down, if possible
		else if (!CherryRight() && !CherryBottom() && _cherry->midChange == 0)
		{
			_cherry->direction = 1;
		}
		// Resets animation cycle
		else if (!CherryRight())
		{
			_cherry->move = 0;
		}

		// Changes direction if collision occurs
		if (CherryRight() && (_cherry->move == 0 || _cherry->move == 20))
		{
			int change = rand() % 2;

			if (change == 0)
			{
				_cherry->direction = 3;
			}
			else if (change == 1)
			{
				_cherry->direction = 1;
			}
		}
		break;
	}
	}

	// Changes position of cherry source rect for animation purposes
	_cherry->sourceRect->Y = _cherry->sourceRect->Height * _cherry->direction;
}

void Pacman::CherryCollision()
{
	// If pacman collides with cherry, increment score and move cherry offscreen
	if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _cherry->posRect->X, _cherry->posRect->Y, _cherry->sourceRect->Width, _cherry->sourceRect->Height))
	{
		Audio::Play(_pop);

		_cherry->posRect->X = -100;
		_cherry->posRect->Y = -100;

		_score += 1000;
	}
}

void Pacman::PacmanDeath()
{
	// Display death animation
	if (_deathTime < 100)
	{
		_pacman->sourceRect->X = _pacman->sourceRect->Width * 2;

		_deathTime++;
	}
	// Show game over screen
	else if (_deathTime >= 20)
	{
		_gameState = GameState::GAME_OVER;
	}
}

void Pacman::Win()
{
	// Display win screen
	if (_winTime < 100)
	{
		_winTime++;
	}
	// Load new level
	else
	{
		Reload();
		_gameState = GameState::PLAY;
		Audio::Play(_mainMusic);
	}
}

void Pacman::Reload()
{
	_deathTime = 0;
	_winTime = 0;
	_wallCount = -1;
	_munchieCount = -1;

	// Load level as array
	const char _level[31][26] = { { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
								  { '#', 'A', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', 'B', '#' },
								  { '#', '.', '#', '#', '#', '#', '#', '#', '#', '.', '#', '.', '#', '#', '.', '#', '.', '#', '#', '#', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '#', '#', '#', '.', '#', '.', '#', '#', '.', '#', '.', '#', '#', '#', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '.', '#', '#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '#', '.', '#' },
								  { '#', '.', '.', '.', '.', '#', '#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '.', '#', '#', '.', '.', '.', '.', '#' },
								  { '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#' },
								  { '#', '#', '#', '#', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '#', '#', '#', '#' },
								  { '.', '.', '.', '.', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '.', '.', '.', '.' },
								  { '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', 'O', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '-', '-', '-', '-', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '.', '.', '.', '.', '.', '#', '#', '.', '#', '-', '-', '-', '-', '#', '.', '#', '#', '.', '.', '.', '.', '.', '.', '#' },
								  { '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '-', '-', '-', '-', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '1', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '#', '#', '.', '.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '#' },
								  { '#', '#', '#', '.', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '.', '#', '#', '#' },
								  { '#', '#', '#', '.', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '.', '#', '#', '#' },
								  { '#', '.', '.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '#', '#', '.', '.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', 'C', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '.', '.', 'D', '#' },
								  { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };

	for (int i = 0; i < 31; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			switch (char tileType = _level[i][j])
			{
				case '.':
				{
					// Load Munchie
					_munchieCount++;
					_munchies[_munchieCount]->position->X = (j * 20) + 5;
					_munchies[_munchieCount]->position->Y = (i * 20) + 35;
					_munchies[_munchieCount]->currentFrameTime = 0;
					_munchies[_munchieCount]->frame = 0;
					break;
				}

				case 'O':
				{
					// Load Cherry
					_cherry->posRect = new Vector2(j * 20, (i * 20) + 30);
					_cherry->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
					_cherry->currentFrameTime = 0;
					_cherry->frame = 0;
					_cherry->direction = 0;
					_cherry->midChange = 0;
					_cherry->move = 0;
					_cherryVisible = CherryVisible::INVISIBLE;
					break;
				}
				case '1':
				{
					// Load Pacman
					_pacman->position->X = (j * 20);
					_pacman->position->Y = (i * 20) + 30;
					_pacman->direction = 0;
					_pacman->currentFrameTime = 0;
					_pacman->frame = 0;
					_pacman->speedMultiplier = 1.0f / WALLCOUNT;
					_pacman->dead = false;
					_pacman->move = 0;
					_movement = Movement::NEUTRAL;
					break;
				}
				case 'A':
				{
					// Load Blue Ghost
					_ghosts[0]->posRect->X = (j * 20);
					_ghosts[0]->posRect->Y = (i * 20) + 30;
					break;
				}
				case 'B':
				{
					// Load Red ghost					
					_ghosts[1]->posRect->X = (j * 20);
					_ghosts[1]->posRect->Y = (i * 20) + 30;
					break;
				}
				case 'C':
				{
					//Load Orange Ghost
					_ghosts[2]->posRect->X = (j * 20);
					_ghosts[2]->posRect->Y = (i * 20) + 30;
					break;
				}
				case 'D':
				{
					// Load Pink Ghost
					_ghosts[3]->posRect->X = (j * 20);
					_ghosts[3]->posRect->Y = (i * 20) + 30;
					break;
				}
			}
		}
	}

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->direction = 3;
		_ghosts[i]->move = 0;
		_ghosts[i]->midChange = 0;
		_ghosts[i]->speed = 0.2f;
	}

	//Load Logo
	_logo->posRect->X = 60;
	_logo->posRect->Y = -100;

	_startPosition->X = 165.0f;
	_startPosition->Y = 550.0f;

	_pacman->direction = 0;
	_pacman->currentFrameTime = 0;
	_pacman->frame = 0;
	_pacman->speedMultiplier = 1.0f / WALLCOUNT;
	_pacman->dead = false;
	_pacman->move = 0;

	_winCount = 0;

	_pKeyDown = false;
	_wKeyDown = false;
	_aKeyDown = false;
	_sKeyDown = false;
	_dKeyDown = false;
	_spaceKeyDown = false;
}

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanFrameTime(250), _cMunchieFrameTime(500)
{
	srand(time(NULL));

	_pacman = new Player;
	_pacman->direction = 0;
	_pacman->currentFrameTime = 0;
	_pacman->frame = 0;
	_pacman->speedMultiplier = 1.0f / WALLCOUNT;
	_pacman->dead = false;
	_pacman->move = 0;

	for (int i = 0; i < WALLCOUNT; i++)
	{
		_wall[i] = new Wall;
	}
	
	_score = 0;
	_winCount = 0;
	_levelCount = 1;
	_deathTime = 0;
	_winTime = 0;
	
	_menu = new Menu;
	_menu->paused = false;

	_logo = new Logo;

	_pKeyDown = false;
	_wKeyDown = false;
	_aKeyDown = false;
	_sKeyDown = false;
	_dKeyDown = false;
	_spaceKeyDown = false;
	_launchTrigger = false;

	_gameOver = new GameOver;
	
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Munchie;
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frame = 0;
	}

	_cherry = new Cherry;
	_cherry->currentFrameTime = 0;
	_cherry->frame = 0;
	_cherry->direction = 0;
	_cherry->midChange = 0;
	_cherry->move = 0;
	_cherry->speed = 0.2f;

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new Enemy;
		_ghosts[i]->direction = 3;
		_ghosts[i]->move = 0;
		_ghosts[i]->midChange = 0;
		_ghosts[i]->speed = 0.2f;
	}

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 520, 650, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _pacman;

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchies[i]->munchieTex;
		delete _munchies[i]->position;
		delete _munchies[i]->sourceRect;
	}
	
	delete [] _munchies;

	delete _cherry->texture;
	delete _cherry->posRect;
	delete _cherry->sourceRect;
	delete _cherry;

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		delete _ghosts[i]->texture;
		delete _ghosts[i]->posRect;
		delete _ghosts[i]->sourceRect;
	}
	
	delete [] _ghosts;

	for (int i = 0; i < WALLCOUNT; i++)
	{
		delete _wall[i]->texture;
		delete _wall[i]->posRect;
		delete _wall[i]->sourceRect;
	}

	delete [] _wall;

	delete _logo->texture;
	delete _logo->sourceRect;
	delete _logo->posRect;
	delete _logo;

	delete _chomp;
	delete _pop;
	delete _titleMusic;
	delete _launchGame;
	delete _death;
	delete _winSound;
	delete _mainMusic;
}

void Pacman::LoadContent()
{
	_wallCount = -1;
	_munchieCount = -1;

	// Load level as array
	const char _level[31][26] = { { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
								  { '#', 'A', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', 'B', '#' },
								  { '#', '.', '#', '#', '#', '#', '#', '#', '#', '.', '#', '.', '#', '#', '.', '#', '.', '#', '#', '#', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '#', '#', '#', '.', '#', '.', '#', '#', '.', '#', '.', '#', '#', '#', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '.', '#', '#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '#', '.', '#' },
								  { '#', '.', '.', '.', '.', '#', '#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '.', '#', '#', '.', '.', '.', '.', '#' },
								  { '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#' },
								  { '#', '#', '#', '#', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '#', '#', '#', '#' },
								  { '.', '.', '.', '.', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '.', '.', '.', '.' },
								  { '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', 'O', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '-', '-', '-', '-', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '.', '.', '.', '.', '.', '#', '#', '.', '#', '-', '-', '-', '-', '#', '.', '#', '#', '.', '.', '.', '.', '.', '.', '#' },
								  { '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '-', '-', '-', '-', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '1', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '#', '#', '.', '.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '#' },
								  { '#', '#', '#', '.', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '.', '#', '#', '#' },
								  { '#', '#', '#', '.', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '.', '#', '#', '#' },
								  { '#', '.', '.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '#', '#', '.', '.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', '.', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '#' },
								  { '#', 'C', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '.', '.', 'D', '#' },
								  { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };

	for (int i = 0; i < 31; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			switch (char tileType = _level[i][j])
			{
				case '#':
				{
					// Load maze walls
					_wallCount++;
					_wall[_wallCount]->texture = new Texture2D();
					_wall[_wallCount]->texture->Load("Textures/Wall.png", false);
					_wall[_wallCount]->posRect = new Vector2(j * 20, (i * 20) + 30);
					_wall[_wallCount]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
					break;
				}
				case '.':
				{
					// Load Munchie
					_munchieCount++;
					_munchies[_munchieCount]->munchieTex = new Texture2D();
					_munchies[_munchieCount]->munchieTex->Load("Textures/Munchie.png", false);
					_munchies[_munchieCount]->position = new Vector2((j * 20) + 5, (i * 20) + 35);
					_munchies[_munchieCount]->sourceRect = new Rect(0.0f, 0.0f, 10, 10);
					break;
				}
				case 'O':
				{
					// Load Cherry
					_cherry->texture = new Texture2D();
					_cherry->texture->Load("Textures/Cherry.png", false);
					_cherry->posRect = new Vector2(j * 20, (i * 20) + 30);
					_cherry->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
					break;
				}
				case '1':
				{
					// Load Pacman
					_pacman->texture = new Texture2D();
					_pacman->texture->Load("Textures/Pacman.png", false);
					_pacman->position = new Vector2(j * 20, (i * 20) + 30);
					_pacman->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
					break;
				}
				case 'A':
				{
					// Load blue ghost
					_ghosts[0]->texture = new Texture2D();
					_ghosts[0]->texture->Load("Textures/GhostBlue.png", false);
					_ghosts[0]->posRect = new Vector2(j * 20, (i * 20) + 30);
					_ghosts[0]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
					break;
				}
				case 'B':
				{
					// Load red ghost
					_ghosts[1]->texture = new Texture2D();
					_ghosts[1]->texture->Load("Textures/GhostRed.png", false);					
					_ghosts[1]->posRect = new Vector2(j * 20, (i * 20) + 30);
					_ghosts[1]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
					break;
				}
				case 'C':
				{
					//Load Orange Ghost
					_ghosts[2]->texture = new Texture2D();
					_ghosts[2]->texture->Load("Textures/GhostOrange.png", false);
					_ghosts[2]->posRect = new Vector2(j * 20, (i * 20) + 30);
					_ghosts[2]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
					break;
				}
				case 'D':
				{
					// Load Pink Ghost
					_ghosts[3]->texture = new Texture2D();
					_ghosts[3]->texture->Load("Textures/GhostPink.png", false);
					_ghosts[3]->posRect = new Vector2(j * 20, (i * 20) + 30);
					_ghosts[3]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
					break;
				}
			}
		}
	}

	//Load Munchie sound effect
	_chomp = new SoundEffect();
	_chomp->Load("Sounds/Chomp.wav");
	_chomp->SetGain(0.25f);

	// Load Cherry collection sound effect
	_pop = new SoundEffect();
	_pop->Load("Sounds/pop.wav");

	// Load title music
	_titleMusic = new SoundEffect();
	_titleMusic->Load("Sounds/PacmanTitle.wav");

	// Load LaunchGame sound effect
	_launchGame = new SoundEffect();
	_launchGame->Load("Sounds/LaunchGame.wav");

	// Load Death music
	_death = new SoundEffect();
	_death->Load("Sounds/Death.wav");

	// Load Win music
	_winSound = new SoundEffect();
	_winSound->Load("Sounds/Win.wav");

	// Load Main Game music
	_mainMusic = new SoundEffect();
	_mainMusic->Load("Sounds/MainMusic.wav");

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
	_startPosition = new Vector2(165.0f, 550.0f);

	//Load Logo
	_logo->texture = new Texture2D();
	_logo->texture->Load("Textures/Logo.png", false);
	_logo->posRect = new Vector2(60, -100);
	_logo->sourceRect = new Rect(0.0f, 0.0f, 400, 100);

	// Set Menu Parameters
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->stringPosition = new Vector2((Graphics::GetViewportWidth() / 2.0f) - 40, Graphics::GetViewportHeight() / 2.0f);

	_gameOver->background = new Texture2D();
	_gameOver->background->Load("Textures/Transparency.png", false);
	_gameOver->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_gameOver->stringPosition = new Vector2((Graphics::GetViewportWidth() / 2.0f) - 110, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();

	switch (_gameState)
	{
		case GameState::START:
		{
			Title(keyboardState);

			break;
		}
		case GameState::PLAY:
		{
			CheckPausedTrue(keyboardState, mouseState, Input::Keys::P);
			Input(elapsedTime, keyboardState);
			PacmanMove(keyboardState);
			UpdatePacman(elapsedTime);

			for (int i = 0; i < GHOSTCOUNT; i++)
			{
				UpdateGhosts(*_ghosts[i], elapsedTime);
				CheckGhostCollisions(*_ghosts[i]);
				CheckViewportCollision(*_ghosts[i]);
			}

			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				UpdateMunchie(*_munchies[i], elapsedTime);
				MunchieCollision(*_munchies[i]);
			}

			if (_cherryVisible == CherryVisible::VISIBLE)
			{
				UpdateCherry(elapsedTime);
				CherryCollision();
			}

			break;
		}

		case GameState::PAUSE:
		{
			CheckPausedFalse(keyboardState, mouseState, Input::Keys::P);
			break;
		}

		case GameState::DEATH:
		{
			PacmanDeath();
			break;
		}

		case GameState::GAME_OVER:
		{
			CheckDead(keyboardState, Input::Keys::SPACE);
			break;
		}

		case GameState::WIN:
		{
			Win();
			break;
		}
	}
}

void Pacman::Draw(int elapsedTime)
{	
	// Allows us to easily create a string
	std::stringstream stream;
	std::stringstream start;

	//stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;
	stream << "Score: " << _score;

	start << "Press -SPACE- to start";

	// Starts Drawing
	SpriteBatch::BeginDraw(); 

	switch (_gameState)
	{
		case GameState::START:
		{
			// Draw Title screen
			SpriteBatch::Draw(_logo->texture, _logo->posRect, _logo->sourceRect);
			SpriteBatch::DrawString(start.str().c_str(), _startPosition, Color::Yellow);

			break;
		}

		case GameState::PLAY:
		{

			for (int i = 0; i < WALLCOUNT; i++)
			{
				// Draw Maze Walls
				SpriteBatch::Draw(_wall[i]->texture, _wall[i]->posRect, _wall[i]->sourceRect);
			}

			// Draw Munchie
			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				SpriteBatch::Draw(_munchies[i]->munchieTex, _munchies[i]->position, _munchies[i]->sourceRect);

				if (_munchies[i]->frame >= 60)
				{
					_munchies[i]->frame = 0;
				}
			}

			// Draw Cherry
			if (_cherryVisible == CherryVisible::VISIBLE)
			{
				SpriteBatch::Draw(_cherry->texture, _cherry->posRect, _cherry->sourceRect);

				if (_cherry->frame >= 60)
				{
					_cherry->frame = 0;
				}
			}

			// Draw Pacman
			SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect);

			// Draw Ghosts
			for (int i = 0; i < GHOSTCOUNT; i++)
			{
				SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->posRect, _ghosts[i]->sourceRect);
			}

			// Draws String
			SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Yellow);
			
			break;
		}

		case GameState::PAUSE:
		{
			// Draw Pause Menu
			std::stringstream menuStream;
			menuStream << "PAUSED!";

			SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Yellow);

			break;
		}

		case GameState::DEATH:
		{
			// Draw Pacman
			SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect);
			break;
		}

		case GameState::GAME_OVER:
		{
			// Draw Game Over screen
			std::stringstream gameOverStream;
			gameOverStream << "\t\t\t\t\t\t\t\t\t\t\t\tGAME OVER\n";
			gameOverStream << "\t\t\t\t\t\t\t\t\t\t\t\t\t\tScore: " << _score;
			gameOverStream << "\nPress -SPACE- to try again";
			gameOverStream << "\n\t\t\t\tPress -Q- to go to title";


			SpriteBatch::Draw(_gameOver->background, _gameOver->rectangle, nullptr);
			SpriteBatch::DrawString(gameOverStream.str().c_str(), _gameOver->stringPosition, Color::Red);
			break;
		}

		case GameState::WIN:
		{
			// Draw Win Screen
			std::stringstream winStream;
			winStream << "On to Level " << _levelCount << "!";

			SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
			SpriteBatch::DrawString(winStream.str().c_str(), _menu->stringPosition, Color::Green);
		}
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}

bool Pacman::CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;

	if (bottom1 < top2)
	{
		return false;
	}

	if (top1 > bottom2)
	{
		return false;
	}

	if (right1 < left2)
	{
		return false;
	}

	if (left1 > right2)
	{
		return false;
	}

	return true;
}

bool Pacman::RightCollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;

	if (right1 < left2)
	{
		return false;
	}
	else
	{
		//cout << "Right Collision \n";
	}

	return true;
}

bool Pacman::LeftCollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;

	if (left1 > right2)
	{
		return false;
	}
	else
	{
		//cout << "Left Collision \n";
	}

	return true;
}

bool Pacman::TopCollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;

	if (top1 > bottom2)
	{
		return false;
	}
	else
	{
		//cout << "Top Collision \n";
	}

	return true;
}

bool Pacman::BottomCollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;

	if (bottom1 < top2)
	{
		return false;
	}
	else
	{
		//cout << "Bottom Collision \n";
	}

	return true;
}

bool Pacman::CircleCollision(int radius1, int x1, int y1, int radius2, int x2, int y2)
{
	int dx = x1 - x2;
	int dy = y1 - y2;
	int distance = sqrt((dx * dx) + (dy * dy));

	if (distance < (radius1 + radius2)) 
	{
		// collision detected
		return true;
	}

	return false;
}

bool Pacman::Right()
{
	for (int i = 0; i < WALLCOUNT; i++)
	{
		if (_wall[i]->posRect->X >= _pacman->position->X && _wall[i]->posRect->Y == _pacman->position->Y)
		{
			//cout << i << " ";
			if (RightCollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _wall[i]->posRect->X, _wall[i]->posRect->Y, _wall[i]->sourceRect->Width, _wall[i]->sourceRect->Height))
			{
				return true;
			}
		}
	}

	return false;
}

bool Pacman::Left()
{
	for (int i = 0; i < WALLCOUNT; i++)
	{
		if (_wall[i]->posRect->X <= _pacman->position->X && _wall[i]->posRect->Y == _pacman->position->Y)
		{
			//cout << i << " ";
			if (LeftCollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _wall[i]->posRect->X, _wall[i]->posRect->Y, _wall[i]->sourceRect->Width, _wall[i]->sourceRect->Height))
			{
				return true;
			}
		}
	}

	return false;
}

bool Pacman::Top()
{
	for (int i = 0; i < WALLCOUNT; i++)
	{
		if (_wall[i]->posRect->X == _pacman->position->X && _wall[i]->posRect->Y <= _pacman->position->Y)
		{
			//cout << i << " ";
			if (TopCollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _wall[i]->posRect->X, _wall[i]->posRect->Y, _wall[i]->sourceRect->Width, _wall[i]->sourceRect->Height))
			{
				return true;
			}
		}
	}

	return false;
}

bool Pacman::Bottom()
{
	for (int i = 0; i < WALLCOUNT; i++)
	{
		if (_wall[i]->posRect->X == _pacman->position->X && _wall[i]->posRect->Y >= _pacman->position->Y)
		{
			//cout << i << " ";
			if (BottomCollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _wall[i]->posRect->X, _wall[i]->posRect->Y, _wall[i]->sourceRect->Width, _wall[i]->sourceRect->Height))
			{
				return true;
			}
		}
	}

	return false;
}

bool Pacman::GhostRight(Enemy& ghost)
{
	for (int i = 0; i < WALLCOUNT; i++)
	{
		if (_wall[i]->posRect->X >= ghost.posRect->X && _wall[i]->posRect->Y == ghost.posRect->Y)
		{
			//cout << i << " ";
			if (RightCollisionCheck(ghost.posRect->X, ghost.posRect->Y, ghost.sourceRect->Width, ghost.sourceRect->Height, _wall[i]->posRect->X, _wall[i]->posRect->Y, _wall[i]->sourceRect->Width, _wall[i]->sourceRect->Height))
			{
				return true;
			}
		}
	}

	return false;
}

bool Pacman::GhostLeft(Enemy& ghost)
{
	for (int i = 0; i < WALLCOUNT; i++)
	{
		if (_wall[i]->posRect->X <= ghost.posRect->X && _wall[i]->posRect->Y == ghost.posRect->Y)
		{
			//cout << i << " ";
			if (LeftCollisionCheck(ghost.posRect->X, ghost.posRect->Y, ghost.sourceRect->Width, ghost.sourceRect->Height, _wall[i]->posRect->X, _wall[i]->posRect->Y, _wall[i]->sourceRect->Width, _wall[i]->sourceRect->Height))
			{
				return true;
			}
		}
	}

	return false;
}

bool Pacman::GhostTop(Enemy& ghost)
{
	for (int i = 0; i < WALLCOUNT; i++)
	{
		if (_wall[i]->posRect->X == ghost.posRect->X && _wall[i]->posRect->Y <= ghost.posRect->Y)
		{
			//cout << i << " ";
			if (TopCollisionCheck(ghost.posRect->X, ghost.posRect->Y, ghost.sourceRect->Width, ghost.sourceRect->Height, _wall[i]->posRect->X, _wall[i]->posRect->Y, _wall[i]->sourceRect->Width, _wall[i]->sourceRect->Height))
			{
				return true;
			}
		}
	}

	return false;
}

bool Pacman::GhostBottom(Enemy& ghost)
{
	for (int i = 0; i < WALLCOUNT; i++)
	{
		if (_wall[i]->posRect->X == ghost.posRect->X && _wall[i]->posRect->Y >= ghost.posRect->Y)
		{
			//cout << i << " ";
			if (BottomCollisionCheck(ghost.posRect->X, ghost.posRect->Y, ghost.sourceRect->Width, ghost.sourceRect->Height, _wall[i]->posRect->X, _wall[i]->posRect->Y, _wall[i]->sourceRect->Width, _wall[i]->sourceRect->Height))
			{
				return true;
			}
		}
	}

	return false;
}

bool Pacman::CherryRight()
{
	for (int i = 0; i < WALLCOUNT; i++)
	{
		if (_wall[i]->posRect->X >= _cherry->posRect->X && _wall[i]->posRect->Y == _cherry->posRect->Y)
		{
			//cout << i << " ";
			if (RightCollisionCheck(_cherry->posRect->X, _cherry->posRect->Y, _cherry->sourceRect->Width, _cherry->sourceRect->Height, _wall[i]->posRect->X, _wall[i]->posRect->Y, _wall[i]->sourceRect->Width, _wall[i]->sourceRect->Height))
			{
				return true;
			}
		}
	}

	return false;
}

bool Pacman::CherryLeft()
{
	for (int i = 0; i < WALLCOUNT; i++)
	{
		if (_wall[i]->posRect->X <= _cherry->posRect->X && _wall[i]->posRect->Y == _cherry->posRect->Y)
		{
			//cout << i << " ";
			if (LeftCollisionCheck(_cherry->posRect->X, _cherry->posRect->Y, _cherry->sourceRect->Width, _cherry->sourceRect->Height, _wall[i]->posRect->X, _wall[i]->posRect->Y, _wall[i]->sourceRect->Width, _wall[i]->sourceRect->Height))
			{
				return true;
			}
		}
	}

	return false;
}

bool Pacman::CherryTop()
{
	for (int i = 0; i < WALLCOUNT; i++)
	{
		if (_wall[i]->posRect->X == _cherry->posRect->X && _wall[i]->posRect->Y <= _cherry->posRect->Y)
		{
			//cout << i << " ";
			if (TopCollisionCheck(_cherry->posRect->X, _cherry->posRect->Y, _cherry->sourceRect->Width, _cherry->sourceRect->Height, _wall[i]->posRect->X, _wall[i]->posRect->Y, _wall[i]->sourceRect->Width, _wall[i]->sourceRect->Height))
			{
				return true;
			}
		}
	}

	return false;
}

bool Pacman::CherryBottom()
{
	for (int i = 0; i < WALLCOUNT; i++)
	{
		if (_wall[i]->posRect->X == _cherry->posRect->X && _wall[i]->posRect->Y >= _cherry->posRect->Y)
		{
			//cout << i << " ";
			if (BottomCollisionCheck(_cherry->posRect->X, _cherry->posRect->Y, _cherry->sourceRect->Width, _cherry->sourceRect->Height, _wall[i]->posRect->X, _wall[i]->posRect->Y, _wall[i]->sourceRect->Width, _wall[i]->sourceRect->Height))
			{
				return true;
			}
		}
	}

	return false;
}
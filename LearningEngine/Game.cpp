/**********************************************************************************
// Game (Source File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Abstract class to represent the game.
//				HOW TO USE: Create a derived class from Game and override Init(),
//							Update(), Draw() and Finalize() methods. To run the
//							game, pass the Game object to the Engine method Start().
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Includes

#include "Game.h"
#include "Engine.h"

// ---------------------------------------------------------------------------------
// Initialization of static class members

LearningEngine::Canvas    LearningEngine::Game::world		= { 0, 0, 0, 0 };			// Game area
LearningEngine::Window*	& LearningEngine::Game::window		= Engine::window;			// Pointer to window
LearningEngine::Game*	& LearningEngine::Game::game		= Engine::game;				// Pointer to game
double					& LearningEngine::Game::gameTime	= Engine::frameTime;		// Last frame time

// -------------------------------------------------------------------------------
// Constructor
LearningEngine::Game::Game()
{
	// Game size is the same as window size
	SetSize(window->GetWidth(), window->GetHeight());

	// Visible area of the game is the same as its size
	viewport = { 0, 0, window->GetWidth(), window->GetHeight() };
}

// -------------------------------------------------------------------------------
// Destructor
LearningEngine::Game::~Game()
{
}

// -------------------------------------------------------------------------------
// Game pause
void LearningEngine::Game::OnPause()
{
	Sleep(10);
}

// -------------------------------------------------------------------------------
// Set Game Window size
void LearningEngine::Game::SetSize(double width, double height)
{
	world.width = width;
	world.height = height;
	world.centerX = width / 2;
	world.centerY = height / 2;
}
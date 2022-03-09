/**********************************************************************************
// App (Source File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Abstract class to represent the app.
//				HOW TO USE: Create a derived class from App and override Init(),
//							Update(), Draw() and Finalize() methods. To run the
//							app, pass the App object to the Engine method Start().
//
**********************************************************************************/

#include "App.h"
#include "Engine.h"

// ---------------------------------------------------------------------------------
// Initialization of static class members

LearningEngine::Window*		& LearningEngine::App::window		= Engine::window;
LearningEngine::Input*		& LearningEngine::App::input		= Engine::input;
double						& LearningEngine::App::frameTime	= Engine::frameTime;

// -------------------------------------------------------------------------------
// Constructor
LearningEngine::App::App()
{
}

// -------------------------------------------------------------------------------
// Destructor
LearningEngine::App::~App()
{
}
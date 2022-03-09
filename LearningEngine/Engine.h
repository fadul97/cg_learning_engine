/**********************************************************************************
// Engine (Header File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Engine runs applications.
//				HOW TO USE: Create an Engine object and use his Start() method
//							run Game.
**********************************************************************************/

#ifndef LE_ENGINE_H
#define LE_ENGINE_H

// ---------------------------------------------------------------------------------
// Includes

#include "Window.h"													// Window application
#include "Input.h"													// Input device
#include "Timer.h"													// Time counter
#include "App.h"													// App implementation
#include "Game.h"													// Game implementation
#include "DirectX11.h"												// Graphics device

namespace LearningEngine
{
	class Engine
	{
	public:
		Engine();													// Constructor
		~Engine();													// Destructor

		static LearningEngine::Renderer::DirectX11* graphics;		// DirectX 11 device
		
		static Window* window;										// App Window
		static Input* input;										// Input device
		static Game* game;											// Game to be run
		static App* app;											// App to be run
		static double frameTime;									// Current frame time

		int Start(App* application);								// Start to execute app

		static void Pause();										// Pause engine
		static void Resume();										// Resume engine

		// Handle Windows events
		static LRESULT CALLBACK EngineProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		static Timer timer;											// Time counter
		static bool isPaused;										// Game state

		double FrameTime();											// Calculate frame time
		int Loop();													// Main Engine Loop
	};

	// ---------------------------------------------------------------------------------
	// Inline Functions - Setters

	// Pause engine
	inline void Engine::Pause()
	{ isPaused = true; timer.Stop(); }

	// Resume engine
	inline void Engine::Resume()
	{ isPaused = false; timer.Start(); }
}

#endif // !LE_ENGINE_H


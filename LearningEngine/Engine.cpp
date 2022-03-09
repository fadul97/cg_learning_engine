/**********************************************************************************
// Engine (Source File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Engine runs applications.
//				HOW TO USE: Create an Engine object and use his Start() method
//							run Game.
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Includes

#include "Engine.h"
#include <timeapi.h>
#include <sstream>

// ---------------------------------------------------------------------------------
// Initialization of static class members

LearningEngine::Renderer::DirectX11		* LearningEngine::Engine::graphics	= nullptr;		// DirectX 11 device
LearningEngine::Window					* LearningEngine::Engine::window	= nullptr;		// Game Window
LearningEngine::Input					* LearningEngine::Engine::input		= nullptr;		// Input device
LearningEngine::Game					* LearningEngine::Engine::game		= nullptr;		// Game running
LearningEngine::App						* LearningEngine::Engine::app		= nullptr;		// App running
double									  LearningEngine::Engine::frameTime	= 0.0;			// Current frame time
bool									  LearningEngine::Engine::isPaused	= false;		// Game Loop state
LearningEngine::Timer					  LearningEngine::Engine::timer;					// Time counter

// ---------------------------------------------------------------------------------
// Constructor
LearningEngine::Engine::Engine()
{
	window = new Window();
	graphics = new Renderer::DirectX11();
}

// ---------------------------------------------------------------------------------
// Destructor
LearningEngine::Engine::~Engine()
{
	if (game)
		delete game;

	if (app)
		delete app;

	delete graphics;
	delete input;
	delete window;
}

// ---------------------------------------------------------------------------------
// Start to execute app
int LearningEngine::Engine::Start(App* application)
{
	app = application;

	// Create Game Window 
	window->Create();

	// Initialize input (must be done after creating window)
	input = new Input();

	// Initialize graphics device
	graphics->Init(window);

	// Change window procedure from active window to EngineProcedure
	SetWindowLongPtr(window->GetId(), GWLP_WNDPROC, (LONG_PTR)EngineProcedure);

	// Set Sleep resolution to 1 ms
	timeBeginPeriod(1);

	int exitCode = Loop();
	
	// Set Sleep resolution to its default value
	timeEndPeriod(1);

	return exitCode;
}

// ---------------------------------------------------------------------------------
// Calculate frame time
double LearningEngine::Engine::FrameTime()
{
#ifdef _DEBUG
	static double totalTime = 0.0;			// Total time elapsed
	static unsigned int frameCount = 0;		// Elapsed frames counter
#endif

	// Current frame time
	frameTime = timer.Reset();

#ifdef _DEBUG
	// Cumulative frame time
	totalTime += frameTime;

	// Increment frame counter
	frameCount++;

	// Every 1000ms (1s) update FPS indicator in window
	if (totalTime >= 1.0)
	{
		std::stringstream text;
		text << std::fixed;
		text.precision(3);

		text << window->GetTitle().c_str() << "   "
			<< "FPS: " << frameCount << "   "
			<< "Frame Time: " << frameTime * 1000 << " (ms)";

		SetWindowText(window->GetId(), text.str().c_str());

		frameCount = 0;
		totalTime -= 1.0;
	}
#endif

	return frameTime;
}

// ---------------------------------------------------------------------------------
// Main Engine Loop
int LearningEngine::Engine::Loop()
{
	// Init time count
	timer.Start();

	// Windows messages
	MSG msg = { 0 };

	// Init application
	app->Init();

	// Main Loop
	do
	{
		// Handle all events before updating
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Application Pause
			if (input->GetKeyPress(VK_PAUSE))
			{
				if (isPaused)
					Resume();
				else
					Pause();
			}

			if (!isPaused)
			{
				// Calculates frame time
				frameTime = FrameTime();

				// Update application
				app->Update();

				// pplication draw
				app->Draw();
			}
			else
			{
				app->OnPause();
			}
		}
	} while (msg.message != WM_QUIT);

	// Finalize application
	app->Finalize();

	// Closes application
	return int(msg.wParam);
}

// ---------------------------------------------------------------------------------
// Handle Windows events
LRESULT CALLBACK LearningEngine::Engine::EngineProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_PAINT)
		app->Display();
	/*
	else
	{
		switch (msg)
		{
		default:
			break;
		}
	}
	*/


	return CallWindowProc(Input::InputProcedure, hWnd, msg, wParam, lParam);
}
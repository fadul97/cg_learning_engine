/**********************************************************************************
// Main (Source File)
//
// Creation:	03/08/2022
// Update:		03/09/2022
// Compiler:	Visual C++ 2022
//
// Description:	Testing Error Class. Same program from previous commit.
//				->	Test running application in a real-time loop.
//					Change Window color by pressing space.
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Includes

#include "LearningEngine.h"
#include <random>

// Variables to set Window color - public just so we can play with them
// Window will start Purple
int r = 211;		// Red
int g = 3;			// Green
int b = 252;		// Blue

class RandomColorApp : public LearningEngine::App
{
private:
	// Variables to play with random numbers
	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> dist;

	
public:
	void Init();
	void Update();
	void Display();
	void Finalize();
};

// ------------------------------------------------------------------------------

void RandomColorApp::Init()
{
	// Set range for random numbers;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 255);

	r = dist(mt);
	g = dist(mt);
	b = dist(mt);
}

// ------------------------------------------------------------------------------

void RandomColorApp::Update()
{
	// Quit by pressing ESC
	if (input->GetKeyPress(VK_ESCAPE))
		window->Close();

	// Check if Space was pressed
	if (input->GetKeyPress(VK_SPACE))
	{
		r = dist(mt);
		g = dist(mt);
		b = dist(mt);

		window->Clear();
		Display();
	}
}

// ------------------------------------------------------------------------------

void RandomColorApp::Display()
{
	// Set a new color to window
	window->SetColor(r, g, b);
}

// ------------------------------------------------------------------------------

void RandomColorApp::Finalize()
{
}

// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	try
	{
		// Set and Create Engine
		LearningEngine::Engine* engine = new LearningEngine::Engine();

		engine->window->SetMode(LearningEngine::WINDOWED);
		engine->window->SetSize(800, 600);
		//engine->window->SetColor(211, 3, 252);
		engine->window->SetColor(r, g, b);
		engine->window->SetTitle("LearningEngine Window");

		// Create and Execute App
		int exitCode = engine->Start(new RandomColorApp());

		delete engine;
		return exitCode;
	}
	catch (LearningEngine::Error& e)
	{
		// Show message if error occurred
		MessageBox(nullptr, e.ToString().data(), "Main", MB_OK);
		return 0;
	}

}
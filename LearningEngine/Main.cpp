/**********************************************************************************
// Main (Source File)
//
// Creation:	03/08/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Create a window to test Window Class from Engine
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Includes

#include "LearningEngine.h"


// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// Set and Create Window
	LearningEngine::Window* window = new LearningEngine::Window();

	window->SetMode(LearningEngine::FULLSCREEN);
	//window->SetSize(800, 600);
	window->SetColor(100, 100, 255);
	window->SetTitle("LearningEngine Window");
	window->Create();
	
	// Handle window messages
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete window;
	return 0;
}
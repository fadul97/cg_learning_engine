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

// Function prototypes and global variables
LearningEngine::Window* window;
void Welcome();
void Bye();

// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// Set and Create Window
	window = new LearningEngine::Window();

	window->SetMode(LearningEngine::WINDOWED);
	window->SetSize(800, 600);
	window->SetColor(211, 3, 252);
	window->SetTitle("LearningEngine Window");
	window->OnFocus(Welcome);
	window->LostFocus(Bye);
	window->Create();


	// Read Input
	LearningEngine::Input* input = new LearningEngine::Input();
	
	// Handle window messages
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(input->GetKeyPress(VK_ESCAPE))
			window->Close();

		if (input->GetKeyPress(VK_SPACE))
		{
			window->SetColor(252, 3, 207);
			window->Clear();
		}
	}

	delete input;
	delete window;
	return 0;
}

void Welcome()
{
	window->SetColor(255, 0, 0);
	window->Clear();
}

void Bye()
{
	window->SetColor(0, 0, 255);
	window->Clear();
}
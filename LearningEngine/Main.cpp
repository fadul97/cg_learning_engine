/**********************************************************************************
// Main (Source File)
//
// Creation:	03/08/2022
// Update:		03/09/2022
// Compiler:	Visual C++ 2022
//
// Description:	Testing DirectX11 Class by building a 'Hello, Triangle!' that is
//				using Direct3D.
//				-> Changes in Engine.cpp: Start() and Loop() methods.
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Includes

#include "LearningEngine.h"

#include "DirectX11.h"

class Dx11App : public LearningEngine::App
{
private:
	
public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
};

// ------------------------------------------------------------------------------

void Dx11App::Init()
{
}

// ------------------------------------------------------------------------------

void Dx11App::Update()
{
	// Quit by pressing ESC
	if (input->GetKeyPress(VK_ESCAPE))
		window->Close();
}

// ------------------------------------------------------------------------------

void Dx11App::Draw()
{
}

// ------------------------------------------------------------------------------

void Dx11App::Finalize()
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

		engine->window->SetMode(LearningEngine::RESIZABLE_WINDOW);
		engine->window->SetSize(800, 600);
		engine->window->SetColor(211, 3, 252);
		engine->window->SetTitle("LearningEngine Window");

		// Create and Execute App
		int exitCode = engine->Start(new Dx11App());

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
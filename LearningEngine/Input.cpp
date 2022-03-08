/**********************************************************************************
// Input (Source File)
//
// Creation:	03/08/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Input Class handles keyboard and mouse input
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Includes

#include "Input.h"

// ---------------------------------------------------------------------------------
// Initialization of static class members

bool LearningEngine::Input::keys[256] = { 0 };				// Keyboard / Mouse keys states
bool LearningEngine::Input::ctrl[256] = { 0 };				// Keys release control
std::string LearningEngine::Input::text;					// Storage for characters

int LearningEngine::Input::mouseX = 0;						// Mouse position on X-axis
int LearningEngine::Input::mouseY = 0;						// Mouse position on Y-axis
short LearningEngine::Input::mouseWheel = 0;				// Mouse wheel value

// -------------------------------------------------------------------------------
// Constructor
LearningEngine::Input::Input()
{
	/* WARNING: Window object must already be created with method window->Create */
	
	// Change window procedure from active window to InputProcedure
	SetWindowLongPtr(GetActiveWindow(), GWLP_WNDPROC, (LONG_PTR)Input::InputProcedure);
}

// -------------------------------------------------------------------------------
// Destructor
LearningEngine::Input::~Input()
{
	// Returns to use WindowProcedure from Window Class
	SetWindowLongPtr(GetActiveWindow(), GWLP_WNDPROC, (LONG_PTR)Window::WindowProcedure);
}

// -------------------------------------------------------------------------------
// Returns true if key is pressed
bool LearningEngine::Input::GetKeyPress(int vkCode)
{
	if (ctrl[vkCode])
	{
		if (GetKeyDown(vkCode))
		{
			ctrl[vkCode] = false;
			return true;
		}
	}
	else if (GetKeyUp(vkCode))
	{
		ctrl[vkCode] = true;
	}

	return false;
}

// -------------------------------------------------------------------------------
// Returns mouse wheel rotation
short LearningEngine::Input::GetMouseWheel()
{
	short value = mouseWheel;
	mouseWheel = 0;
	return value;
}

// -------------------------------------------------------------------------------
// Handle Windows events
LRESULT CALLBACK LearningEngine::Input::ReadInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CHAR)
	{
	}

	return CallWindowProc(Input::InputProcedure, hWnd, msg, wParam, lParam);
}

// -------------------------------------------------------------------------------
// Handle Windows events
LRESULT CALLBACK LearningEngine::Input::InputProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		keys[wParam] = true;
		return 0;

	case WM_KEYUP:
		keys[wParam] = false;
		return 0;

	case WM_MOUSEMOVE:
		mouseX = GET_X_LPARAM(lParam);
		mouseY = GET_Y_LPARAM(lParam);
		return 0;

	case WM_MOUSEWHEEL:
		mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
		return 0;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		keys[VK_LBUTTON] = true;
		return 0;

	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
		keys[VK_MBUTTON] = true;
		return 0;

	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
		keys[VK_RBUTTON] = true;
		return 0;

	case WM_LBUTTONUP:
		keys[VK_LBUTTON] = false;
		return 0;

	case WM_MBUTTONUP:
		keys[VK_MBUTTON] = false;
		return 0;

	case WM_RBUTTONUP:
		keys[VK_RBUTTON] = false;
		return 0;

	default:
		break;
	}

	return CallWindowProc(Window::WindowProcedure, hWnd, msg, wParam, lParam);
}
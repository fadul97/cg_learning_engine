/**********************************************************************************
// Window (Source File)
//
// Creation:	03/08/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Abstracts window's configuration details
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Includes

#include "Window.h"

// ---------------------------------------------------------------------------------
// Initialization of static class members

void (*LearningEngine::Window::onFocus)() = nullptr;			// No action when gaining focus
void (*LearningEngine::Window::lostFocus)() = nullptr;			// No action when losing focus

// -------------------------------------------------------------------------------
// Constructor
LearningEngine::Window::Window()
{
	windowId		= 0;										// Null ID because Window still doesn't exist
	windowWidth		= GetSystemMetrics(SM_CXSCREEN);			// Window will take up entire screen (X-axis) - FullScreen
	windowHeight	= GetSystemMetrics(SM_CYSCREEN);			// Window will take up entire screen (Y-axis) - FullScreen
	windowIcon		= LoadIcon(NULL, IDI_APPLICATION);			// Default application icon
	windowCursor	= LoadCursor(NULL, IDC_ARROW);				// Default application cursor
	windowColor		= RGB(0, 0, 255);							// Background color is black
	windowTitle		= std::string("Learning Engine");			// Default bar title name
	windowStyle		= WS_POPUP | WS_VISIBLE;					// Style for FullScreen
	windowMode		= FULLSCREEN;								// Default WindowModes value for enum - FULLSCREEN
	windowPosX		= 0;										// Initial position for Window on X-axis
	windowPosY		= 0;										// Initial position for Window on Y-axis
	windowCenterX	= windowWidth / 2;							// Center of Window on X-axis
	windowCenterY	= windowHeight / 2;							// Center of Window on Y-axis
	windowHdc		= { 0 };									// Device context
	windowRect		= { 0, 0, 0, 0 };							// Client area of Window
}

// -------------------------------------------------------------------------------
// Destructor
LearningEngine::Window::~Window()
{
	// Release device context
	if (windowHdc) ReleaseDC(windowId, windowHdc);
}

// -------------------------------------------------------------------------------
// Set enum WindowModes for Window (FULLSCREEN / WINDOWED / RESIZABLE_WINDOW)
void LearningEngine::Window::SetMode(int mode)
{
	windowMode = mode;

	if (mode == WINDOWED)
		windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
	else if (mode == RESIZABLE_WINDOW)
		windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_OVERLAPPEDWINDOW;
	else
		windowStyle = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;
}

// -------------------------------------------------------------------------------
// Set size for Window
void LearningEngine::Window::SetSize(int width, int height)
{
	// Window size
	windowWidth = width;
	windowHeight = height;

	// Calculate center position of Window
	windowCenterX = windowWidth / 2;
	windowCenterY = windowHeight / 2;

	// Adjusts Window position to center of screen
	windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - windowWidth / 2;
	windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - windowHeight / 2;
}

// -------------------------------------------------------------------------------
// Create Window with defined values
bool LearningEngine::Window::Create()
{
	// Application ID
	HINSTANCE appId = GetModuleHandle(NULL);

	// Setting up a Window Class
	WNDCLASSEX wndClass;
	wndClass.cbSize			= sizeof(WNDCLASSEX);
	wndClass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc	= Window::WindowProcedure;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= appId;
	wndClass.hIcon			= windowIcon;
	wndClass.hCursor		= windowCursor;
	wndClass.hbrBackground	= (HBRUSH)CreateSolidBrush(windowColor);
	wndClass.lpszMenuName	= NULL;
	wndClass.lpszClassName	= "EngineWindow";
	wndClass.hIconSm		= windowIcon;

	// Registering Class "EngineWindow"
	if (!RegisterClassEx(&wndClass))
	{
		MessageBox(0, "Failed to Register Window Class 'EngineWindow'.", 0, 0);
		return false;
	}

	//	Create a Window based on "EngineWindow" Class
	windowId = CreateWindowEx(
		NULL,								// Extras styles
		"EngineWindow",						// Window Class name
		windowTitle.c_str(),				// Window title
		windowStyle,						// Window style
		windowPosX, windowPosY,				// Initial (X, Y) position
		windowWidth, windowHeight,			// Width and height of Window
		NULL,								// Window Mother ID
		NULL,								// Menu ID
		appId,								// Application ID
		NULL								// Creation parameters
	);

	// When using WINDOWED mode, we need to take into account that the top bar and borders ocuppie space in Window.
	// The code below adjusts Window size so that the client area is sized (windowWidth x windowHeight)
	if (windowMode == WINDOWED)
	{
		// Rect with desired client area
		RECT rect = { 0, 0, windowWidth, windowHeight };

		// Adjust RECT size
		AdjustWindowRectEx(
			&rect,
			GetWindowStyle(windowId),
			GetMenu(windowId) != NULL,
			GetWindowExStyle(windowId)
		);

		// Update Window position
		windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - (rect.right - rect.left) / 2;
		windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - (rect.bottom - rect.top) / 2;

		// Resize Window with a MoveWindow call
		MoveWindow(
			windowId,
			windowPosX,
			windowPosY,
			rect.right - rect.left,
			rect.bottom - rect.top,
			TRUE
		);
	}

	// Store device context
	windowHdc = GetDC(windowId);

	// Get client area size
	GetClientRect(windowId, &windowRect);

	// Return initialization state (succeeded or not)
	return (windowId ? true : false);
}

// -------------------------------------------------------------------------------
// Handle Windows events
LRESULT CALLBACK LearningEngine::Window::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	// Window lost focus
	case WM_KILLFOCUS:
		if (lostFocus)
			lostFocus();
		return 0;

	// Window regains focus
	case WM_SETFOCUS:
		if (onFocus)
			onFocus();
		return 0;

	// Window was destroyed
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
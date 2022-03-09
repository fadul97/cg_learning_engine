/**********************************************************************************
// Window (Header File)
//
// Creation:	03/08/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Abstracts window's configuration details
//
**********************************************************************************/

#ifndef LE_WINDOW_H
#define LE_WINDOW_H

// ---------------------------------------------------------------------------------
// Includes

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <windowsx.h>
#include <string>

namespace LearningEngine
{
	// ---------------------------------------------------------------------------------
	// Enums

	enum WindowModes { FULLSCREEN, WINDOWED, RESIZABLE_WINDOW };	// Engine window will use RESIZABLE_WINDOW, while the game or app window can use either of them

	// ---------------------------------------------------------------------------------

	class Window
	{
	public:
		Window();													// Constructor
		~Window();													// Destructor

		HWND GetId();												// Returns Window ID (HWND)
		double GetWidth();											// Returns Window width
		double GetHeight();											// Returns Window height
		double GetAspectRatio();									// Returns value of (width / height)
		int GetMode() const;										// Returns enum WindowModes: FULLSCREEN / WINDOWED / RESIZABLE_WINDOW
		int GetCenterX() const;										// Returns center of Window on X-axis 
		int GetCenterY() const;										// Returns center of Window on Y-axis
		std::string GetTitle() const;								// Returns Window title bar name
		COLORREF GetColor();										// Returns Window background color

		void SetIcon(const unsigned int icon);						// Set icon for Window
		void SetCursor(const unsigned int cursor);					// Set cursor for Window
		void SetTitle(const std::string title);						// Set title bar name for Window
		void SetSize(int width, int height);						// Set size for Window
		void SetMode(int mode);										// Set enum WindowModes for Window (FULLSCREEN / WINDOWED / RESIZABLE_WINDOW)
		void SetColor(int r, int g, int b);							// Set color for Window

		void HideCursor(bool hide);									// Enable or disable cursor display
		void Close();												// Close Window
		void Clear();												// Clear client area
		bool Create();												// Create Window with defined values

		void OnFocus(void(*func)());								// Change function performed when gaining focus
		void LostFocus(void(*func)());								// Change function performed when losing focus

		// Handle Windows events
		static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		HDC				windowHdc;									// Device context
		RECT			windowRect;									// Client area of Window
		HWND			windowId;									// Window ID
		int				windowWidth;								// Window width
		int				windowHeight;								// Window height
		HICON			windowIcon;									// Window icon
		HCURSOR			windowCursor;								// Window cursor
		COLORREF		windowColor;								// Window color
		std::string		windowTitle;								// Window title bar name
		DWORD			windowStyle;								// Window style
		int				windowMode;									// FULLSCREEN or WINDOWED or RESIZABLE_WINDOW
		int				windowPosX;									// Window initial position on X-axis
		int				windowPosY;									// Window initial position on Y-axis
		int				windowCenterX;								// Center of Window on X-axis
		int				windowCenterY;								// Center of Window on Y-axis

		static void(*onFocus)();									// Executes when Window regains focus
		static void (*lostFocus)();									// Executes when Window loses focus
	};

	// ---------------------------------------------------------------------------------
	// Inline Functions - Getters

	// Returns Window ID (HWND)
	inline HWND Window::GetId()
	{ return windowId; }

	// Returns Window width
	inline double Window::GetWidth()
	{ return windowWidth; }

	// Returns Window height
	inline double Window::GetHeight()
	{ return windowHeight; }

	// Returns value of (width / height)
	inline double Window::GetAspectRatio()
	{ return (windowWidth / windowHeight); }

	// Returns enum WindowModes: FULLSCREEN / WINDOWED / RESIZABLE_WINDOW
	inline int Window::GetMode() const
	{ return windowMode; }

	// Returns center of Window on X-axis
	inline int Window::GetCenterX() const
	{ return windowCenterX; }

	// Returns center of Window on Y-axis
	inline int Window::GetCenterY() const
	{ return windowCenterY; }

	// Returns Window title bar name
	inline std::string Window::GetTitle() const
	{ return windowTitle; }

	// Returns Window background color
	inline COLORREF Window::GetColor()
	{ return windowColor; }

	// ---------------------------------------------------------------------------------
	// Inline Functions - Setters

	// Set icon for Window
	inline void Window::SetIcon(const unsigned int icon)
	{ windowIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(icon)); }

	// Set cursor for Window
	inline void Window::SetCursor(const unsigned int cursor)
	{ windowCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(cursor)); }

	// Set title bar name for Window
	inline void Window::SetTitle(const std::string title)
	{ windowTitle = title; }

	// Set color for Window
	inline void Window::SetColor(int r, int g, int b)
	{ windowColor = RGB(r, g, b); }

	// Enable or disable cursor display
	inline void Window::HideCursor(bool hide)
	{ ShowCursor(!hide); }

	// Close Window
	inline void Window::Close()
	{ PostMessage(windowId, WM_DESTROY, 0, 0); }

	// Clear client area
	inline void Window::Clear()
	{ FillRect(windowHdc, &windowRect, CreateSolidBrush(GetColor())); }

	// Change function performed when gaining focus
	inline void Window::OnFocus(void(*func)())
	{ onFocus = func; }

	// Change function performed when losing focus
	inline void Window::LostFocus(void(*func)())
	{ lostFocus = func; }
}

#endif // !LE_WINDOW_H


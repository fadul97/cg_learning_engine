/**********************************************************************************
// Input (Header File)
//
// Creation:	03/08/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Input Class handles keyboard and mouse input
//
**********************************************************************************/

#ifndef LE_INPUT_H
#define LE_INPUT_H

// ---------------------------------------------------------------------------------
// Includes

#include "Window.h"

namespace LearningEngine
{
	class Input
	{
	public:
		Input();								// Constructor
		~Input();								// Destructor

		bool GetKeyDown(int vkCode);			// Returns true if key is pressed
		bool GetKeyUp(int vkCode);				// Returns true if key is released 
		bool GetKeyPress(int vkCode);			// Press again only after release

		int GetMouseX();						// Returns mouse position on X-axis
		int GetMouseY();						// Returns mouse position on Y-axis
		short GetMouseWheel();					// Returns mouse wheel rotation

		static const char* GetText();			// Returns address of stored text

		// Handle Windows events
		static LRESULT CALLBACK ReadInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK InputProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		static bool keys[256];					// Keyboard / Mouse keys states
		static bool ctrl[256];					// Keys release control
		static std::string text;				// Storage for characters

		static int mouseX;						// Mouse position on X-axis
		static int mouseY;						// Mouse position on Y-axis
		static short mouseWheel;				// Mouse wheel value
	};
	
	// -------------------------------------------------------------------------------
	// Inline Functions - Getters

	// Returns true if key is pressed
	inline bool Input::GetKeyDown(int vkCode)
	{ return keys[vkCode]; }

	// Returns true if key is released 
	inline bool Input::GetKeyUp(int vkCode)
	{ return !(keys[vkCode]); }

	// Returns mouse position on X-axis
	inline int Input::GetMouseX()
	{ return mouseX; }

	// Returns mouse position on Y-axis
	inline int Input::GetMouseY()
	{ return mouseY; }

	// Returns address of stored text
	inline const char* Input::GetText()
	{ return text.c_str(); }
}

#endif // !LE_INPUT_H


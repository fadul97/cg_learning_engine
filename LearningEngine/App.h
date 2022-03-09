/**********************************************************************************
// App (Header File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Abstract class to represent the app.
//				HOW TO USE: Create a derived class from App and override Init(),
//							Update(), Draw() and Finalize() methods. To run the
//							app, pass the App object to the Engine method Start().
//
**********************************************************************************/

#ifndef LE_APP_H
#define LE_APP_H

// ---------------------------------------------------------------------------------
// Includes

#include "Window.h"
#include "Input.h"

namespace LearningEngine
{
	class App
	{
	public:
		App();														// Constructor
		virtual ~App();												// Destructor

		virtual void Init() = 0;									// App init
		virtual void Update() = 0;									// App update
		virtual void Finalize() = 0;								// App finalize

		virtual void Draw() {}										// App draw
		virtual void Display() {}									// App display
		virtual void OnPause() { Sleep(10); }						// App on pause

	protected:
		static Window*	& window;									// App Window
		static Input*	& input;									// Input device
		static double	& frameTime;								// Last frame time
	};
}

#endif // !LE_APP_H
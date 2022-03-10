/**********************************************************************************
// Graphics (Header File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Abstract Class to represent APIs (DirectX)
//
**********************************************************************************/

#ifndef LE_GRAPHICS_H
#define LE_GRAPHICS_H

// ---------------------------------------------------------------------------------
// Includes

#include "Window.h"

namespace LearninEngine
{
	namespace Renderer
	{
		class Graphics
		{
		public:
			Graphics();														// Constructor
			virtual ~Graphics();											// Destructor

			virtual bool Init(LearningEngine::Window* window) = 0;			// Initialize API3
			virtual void Clear() = 0;										// Clear BackBuffer for next frame
			virtual void Present() = 0;										// Present draw on screen (change FrontBuffer to BackBuffer)

		private:

		};
	}
}

#endif // !LE_GRAPHICS_H
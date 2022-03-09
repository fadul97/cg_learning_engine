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
			Graphics();							// Constructor
			virtual ~Graphics();				// Destructor

			virtual void Init() = 0;			// Initialize API

		private:

		};
	}
}

#endif // !LE_GRAPHICS_H
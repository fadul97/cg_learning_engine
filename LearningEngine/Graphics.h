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
			Graphics();
			virtual ~Graphics();

			virtual void Init() = 0;

		private:

		};
	}
}

#endif // !LE_GRAPHICS_H
/**********************************************************************************
// Graphics (Header File)
//
// Creation:	03/08/2022
// Update:		03/09/2022
// Compiler:	Visual C++ 2022
//
// Description:	Abstract Class to represent APIs (DirectX)
//
**********************************************************************************/

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

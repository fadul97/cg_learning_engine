/**********************************************************************************
// DirectX11 (Header File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Class that uses Direct3D functions from DirectX 11
//
**********************************************************************************/

#ifndef LE_DIRECTX11_H
#define LE_DIRECTX11_H

// ---------------------------------------------------------------------------------
// Includes

#include <dxgi.h>
#include <d3d11.h>
#include "Window.h"

namespace LearningEngine
{
	namespace Renderer
	{
		class DirectX11
		{
		public:
			DirectX11();										// Constructor
			~DirectX11();										// Destructor

			bool Init(LearningEngine::Window* window);			// Initialize API - DirectX 11
			static ID3D11Device* GetDevice();					// Returns Direct3D device
			static ID3D11DeviceContext* GetContext();			// Returns D3D device context

		private:
			static ID3D11Device			* d3d11Device;			// Graphics device
			static ID3D11DeviceContext	* d3d11DevContext;		// Graphics device context

			D3D_FEATURE_LEVEL featureLevel;						// Leevel of D3D features supported by GPU 

			unsigned int antialiasing;							// Number of samples for antialiasing
			unsigned int quality;								// Sample quality level
		};
	
		// ---------------------------------------------------------------------------------
		// Inline Functions - Getters

		// Returns Direct3D device
		inline ID3D11Device* DirectX11::GetDevice()
		{ return d3d11Device; }

		// Returns D3D device context
		inline ID3D11DeviceContext* DirectX11::GetContext()
		{ return d3d11DevContext; }
	}
}

#endif // !LE_DIRECTX11_H
/**********************************************************************************
// DirectX11 (Header File)
//
// Creation:	03/09/2022
// Update:		03/10/2022
// Compiler:	Visual C++ 2022
//
// Description:	Class that uses Direct3D functions from DirectX 11
//
**********************************************************************************/

#ifndef LE_DIRECTX11_H
#define LE_DIRECTX11_H

// ---------------------------------------------------------------------------------
// Includes

#include "Graphics.h"
#include <dxgi.h>
#include <d3d11.h>
#include "Window.h"

namespace LearningEngine
{
	namespace Renderer
	{
		class DirectX11 : LearninEngine::Renderer::Graphics
		{
		public:
			DirectX11();										// Constructor
			~DirectX11();										// Destructor

			bool Init(LearningEngine::Window* window);			// Initialize API - DirectX 11
			void Clear();										// Clear BackBuffer for next frame
			void Present();										// Present buffer on screen (change FrontBuffer to BackBuffer)
			
			static ID3D11Device* GetDevice();					// Returns Direct3D device
			static ID3D11DeviceContext* GetContext();			// Returns D3D device context
			static D3D11_VIEWPORT viewport;						// Viewport

			void InitTriangle();								// Initialize some pipeline stages to setup triangle
			void DrawTriangle();								// Draw triangle

		private:
			static ID3D11Device			* d3d11Device;			// Graphics device
			static ID3D11DeviceContext	* d3d11DevContext;		// Graphics device context

			ID3D11RenderTargetView		* renderTargetView;		// Back buffer Render Target View - RTV
			ID3D11DepthStencilView		* depthStencilView;		// Depth/Stencil View - DSV
			IDXGISwapChain				* swapChain;			// Swap Chain
			ID3D11VertexShader			* vertexShader;			// Manages Vertex Shade Program and control Vertex Shader Stage 
			ID3D11PixelShader			* pixelShader;			// Manages Pixel Shader Program and controls Pixel Shader Stage
			ID3D11Buffer				* vertexBuffer;			// Buffer resource

			D3D_FEATURE_LEVEL			  featureLevel;			// Level of D3D features supported by GPU 

			unsigned int				  antialiasing;			// Number of samples for antialiasing
			unsigned int				  quality;				// Sample quality level
			float						  bgColor[4];			// Background color
			bool						  vSync;				// Vertical sync
		};
	
		// ---------------------------------------------------------------------------------
		// Inline Functions - Getters

		// Returns Direct3D device
		inline ID3D11Device* DirectX11::GetDevice()
		{ return d3d11Device; }

		// Returns D3D device context
		inline ID3D11DeviceContext* DirectX11::GetContext()
		{ return d3d11DevContext; }

		// Clear BackBuffer for next frame
		inline void DirectX11::Clear()
		{ 
			d3d11DevContext->ClearRenderTargetView(renderTargetView, bgColor);
			d3d11DevContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	
		// Present buffer on screen (change FrontBuffer to BackBuffer)
		inline void DirectX11::Present()
		{
			swapChain->Present(vSync, NULL);
			d3d11DevContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
		}
	}
}

#endif // !LE_DIRECTX11_H
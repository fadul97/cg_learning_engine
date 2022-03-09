/**********************************************************************************
// DirectX11 (Source File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Class that uses Direct3D functions from DirectX 11
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Includes

#include "DirectX11.h"
#include "Error.h"

// ---------------------------------------------------------------------------------
// Initialization of static class members

ID3D11Device* LearningEngine::Renderer::DirectX11::d3d11Device = nullptr;
ID3D11DeviceContext* LearningEngine::Renderer::DirectX11::d3d11DevContext = nullptr;

// -------------------------------------------------------------------------------
// Constructor
LearningEngine::Renderer::DirectX11::DirectX11()
{
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	antialiasing = 1;
	quality = 0;

	unsigned int createDeviceFlags = 0;

#ifndef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // !_DEBUG

	HRESULT hr = D3D11CreateDevice(
		NULL,							// Default adapter
		D3D_DRIVER_TYPE_HARDWARE,		// D3D driver type (Hardware, Reference or Software)
		NULL,							// Pointer to software rasterizer - No software device
		createDeviceFlags,				// Debug or Normal Mode
		NULL,							// Default feature level (NULL = max supported)
		0,								// Size of feature level array
		D3D11_SDK_VERSION,				// Direct3D SDK version
		&d3d11Device,					// Stores D3D device created
		&featureLevel,					// Current Direct3D version in use,
		&d3d11DevContext				// D3D context device
	);
}

// -------------------------------------------------------------------------------
// Destructor
LearningEngine::Renderer::DirectX11::~DirectX11()
{
	if (d3d11DevContext)
		d3d11DevContext->Release();

	if (d3d11Device)
		d3d11Device->Release();
}

// -------------------------------------------------------------------------------
// Initialize API - DirectX 11
bool LearningEngine::Renderer::DirectX11::Init(LearningEngine::Window* window)
{
	// -------------------------------
	//	Get DXGI Interfaces
	// -------------------------------

	// Get pointer to Direct3D device 
	IDXGIDevice* dxgiDevice = nullptr;
	if FAILED(d3d11Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice))						// Maybe change to ThrowIfFailed
	{
		MessageBox(nullptr, "Failed to create DXGIDevice.", 0, 0);
		return false;
	}

	// Get Adpapter from Direct3D device (d3d11Device)
	IDXGIAdapter* dxgiAdapter = nullptr;
	if FAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter))
	{
		MessageBox(nullptr, "Failed to create DXGIAdapter.", 0, 0);
		return false;
	}

	// Get pointer to adapter DXGIFactory
	IDXGIFactory* dxgiFactory = nullptr;
	if FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory))
	{
		MessageBox(nullptr, "Failed to create DXGIFactory.", 0, 0);
		return false;
	}

	// -------------------------------
	//	Create Swap Chain
	// -------------------------------

	// Describe Swap Chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc				= {0};
	swapChainDesc.BufferDesc.Width					= UINT(window->GetWidth());								// Back buffer width
	swapChainDesc.BufferDesc.Height					= UINT(window->GetHeight());							// Back buffer height
	swapChainDesc.BufferDesc.RefreshRate.Numerator	= 60;													// Refresh rate in hertz 
	swapChainDesc.BufferDesc.RefreshRate.Numerator	= 1;													// Numerator is an int
	swapChainDesc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;							// Color format - RGBA 8 bits
	swapChainDesc.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;					// Default value for Flags
	swapChainDesc.BufferDesc.Scaling				= DXGI_MODE_SCALING_UNSPECIFIED;						// Default mode for scaling
	swapChainDesc.SampleDesc.Count					= antialiasing;											// Samples per pixel (antialiasing)
	swapChainDesc.SampleDesc.Quality				= quality;												// Level of image quality
	swapChainDesc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;						// Use surface as Render Target
	swapChainDesc.BufferCount						= 1;													// Number of Back buffers
	swapChainDesc.OutputWindow						= window->GetId();										// Window ID
	swapChainDesc.Windowed							= (window->GetMode() == WINDOWED);						// Fullscreen or windowed 
	swapChainDesc.SwapEffect						= DXGI_SWAP_EFFECT_DISCARD;								// Discard surface after presenting
	swapChainDesc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;				// Use Back buffer size for Fullscreen

	// Create Swap Chain
	IDXGISwapChain* swapChain = nullptr;
	if FAILED(dxgiFactory->CreateSwapChain(d3d11Device, &swapChainDesc, &swapChain))
	{
		MessageBox(nullptr, "Failed to create IDXGISwapChain.", 0, 0);
		return false;
	}

	// -------------------------------
	//	Create Render Target View - RTV
	// -------------------------------

	// Create Back buffer surface of a Swap Chain
	ID3D11Texture2D* backBuffer;
	if FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)))
	{
		MessageBox(nullptr, "Failed to create ID3D11Texture2D.", 0, 0);
		return false;
	}

	ID3D11RenderTargetView* renderTargetView;
	if FAILED(d3d11Device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView))
	{
		MessageBox(nullptr, "Failed to create ID3D11RenderTargetView.", 0, 0);
		return false;
	}

	// -------------------------------
	//	Create Depth Stencil Buffer - DSV
	// -------------------------------

	// Describe Depth/Stencil Buffer Desc
	D3D11_TEXTURE2D_DESC depthStencilDesc	= {0};
	depthStencilDesc.Width					= UINT(window->GetWidth());					// Depth/Stencil buffer width
	depthStencilDesc.Height					= UINT(window->GetHeight());				// Depth/Stencil buffer height
	depthStencilDesc.MipLevels				= 1;										// Number of mipmap levels
	depthStencilDesc.ArraySize				= 1;										// Number of textures in array
	depthStencilDesc.Format					= DXGI_FORMAT_D24_UNORM_S8_UINT;			// Color format - Does it need to be the same format of swapChainDesc?
	depthStencilDesc.SampleDesc.Count		= antialiasing;								// Samples per pixel (antialiasing)
	depthStencilDesc.SampleDesc.Quality		= quality;									// Level of image quality
	depthStencilDesc.Usage					= D3D11_USAGE_DEFAULT;						// Default - GPU will both read and write to the resource
	depthStencilDesc.BindFlags				= D3D11_BIND_DEPTH_STENCIL;					// Where resource will be bound to the pipeline
	depthStencilDesc.CPUAccessFlags			= 0;										// CPU will not read not write to the Depth/Stencil buffer
	depthStencilDesc.MiscFlags				= 0;										// Optional flags

	// Create Depth/Stencil Buffer
	ID3D11Texture2D* depthStencilBuffer;
	if FAILED(d3d11Device->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer))
	{
		MessageBox(nullptr, "Failed to create ID3D11Texture2D.", 0, 0);
		return false;
	}

	// Create Depth Stencil View - DSV
	ID3D11DepthStencilView* depthStencilView;
	if FAILED(d3d11Device->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView))
	{
		MessageBox(nullptr, "Failed to create ID3D11DepthStencilView.", 0, 0);
		return false;
	}

	// -------------------------------
	//	Set Output and Viewport
	// -------------------------------

	// Bind Views to the Output Merger Stage
	d3d11DevContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// Describe Viewport
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width	= static_cast<float>(window->GetWidth());
	viewport.Height = static_cast<float>(window->GetHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// Set Viewport
	d3d11DevContext->RSSetViewports(1, &viewport);

	// -------------------------------
	//	Release DXGI Interfaces
	// -------------------------------

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();
	backBuffer->Release();

	// Init succeeded
	return true;
}
/**********************************************************************************
// DirectX11 (Source File)
//
// Creation:	03/09/2022
// Update:		03/11/2022
// Compiler:	Visual C++ 2022
//
// Description:	Class that uses Direct3D functions from DirectX 11
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Includes

#include "DirectX11.h"
#include "Error.h"

// For this build
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

// ---------------------------------------------------------------------------------
// Structs

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

// ---------------------------------------------------------------------------------
// Initialization of static class members

ID3D11Device* LearningEngine::Renderer::DirectX11::d3d11Device = nullptr;				// Graphics device
ID3D11DeviceContext* LearningEngine::Renderer::DirectX11::d3d11DevContext = nullptr;				// Graphics device context
D3D11_VIEWPORT		  LearningEngine::Renderer::DirectX11::viewport = { 0 };				// Viewport

// -------------------------------------------------------------------------------
// Constructor
LearningEngine::Renderer::DirectX11::DirectX11()
{
	renderTargetView = nullptr;						// Back buffer Render Target View - RTV
	depthStencilView = nullptr;						// Back buffer Render Target View - RTV
	swapChain = nullptr;						// Swap Chain
	vertexShader = nullptr;						// Manages Vertex Shade Program and control Vertex Shader Stage 
	pixelShader = nullptr;						// Manages Pixel Shader Program and controls Pixel Shader Stage
	vertexBuffer = nullptr;						// Buffer resource

	featureLevel = D3D_FEATURE_LEVEL_11_0;		// Level of D3D features supported by GPU 

	antialiasing = 1;							// Number of samples for antialiasing
	quality = 0;							// Sample quality level

	bgColor[0] = 1.0f;									// Red
	bgColor[1] = 1.0f;									// Green
	bgColor[2] = 1.0f;									// Blue
	bgColor[3] = 1.0f;									// Alpha (0 = transparent)

	vSync = false;										// Vertical sync off

	unsigned int createDeviceFlags = 0;

#ifndef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // !_DEBUG

	if FAILED(D3D11CreateDevice(
		NULL,											// Default adapter
		D3D_DRIVER_TYPE_HARDWARE,						// D3D driver type (Hardware, Reference or Software)
		NULL,											// Pointer to software rasterizer - No software device
		createDeviceFlags,								// Debug or Normal Mode
		NULL,											// Default feature level (NULL = max supported)
		0,												// Size of feature level array
		D3D11_SDK_VERSION,								// Direct3D SDK version
		&d3d11Device,									// Stores D3D device created
		&featureLevel,									// Current Direct3D version in use,
		&d3d11DevContext								// D3D context device
	))
	{
		if FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP,
			NULL, createDeviceFlags, NULL, 0, D3D11_SDK_VERSION,
			&d3d11Device, &featureLevel, &d3d11DevContext))
		{
			OutputDebugString("--> Failed to create device, using WARP Adapter\n");

		}
	}
}

// -------------------------------------------------------------------------------
// Destructor
LearningEngine::Renderer::DirectX11::~DirectX11()
{
	// Release Vertex Buffer
	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = nullptr;
	}

	// Release Pixel Shader
	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = nullptr;
	}

	// Release Vertex Buffer
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}

	// Release Render Target
	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = nullptr;
	}

	// Release Depth/Stencil Buffer
	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = nullptr;
	}

	// Release Swap Chain
	if (swapChain)
	{
		// Direct3D can't close in Fullscreen
		swapChain->SetFullscreenState(false, NULL);
		swapChain->Release();
		swapChain = nullptr;
	}

	// Release graphics context device
	if (d3d11DevContext)
	{
		// Restores to original state
		d3d11DevContext->ClearState();
		d3d11DevContext->Release();
		d3d11DevContext = nullptr;
	}

	// Release graphics device
	if (d3d11Device)
	{
		d3d11Device->Release();
		d3d11Device = nullptr;
	}
}

// -------------------------------------------------------------------------------
// Initialize API - DirectX 11
bool LearningEngine::Renderer::DirectX11::Init(LearningEngine::Window* window)
{
	// Set background color to be the same of window color
	COLORREF color = window->GetColor();

	bgColor[0] = GetRValue(color) / 255.0f;                       // Red
	bgColor[1] = GetGValue(color) / 255.0f;                       // Green
	bgColor[2] = GetBValue(color) / 255.0f;                       // Blue
	bgColor[3] = 1.0f;											// Alpha (0 = transparent)

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
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferDesc.Width = UINT(window->GetWidth());								// Back buffer width
	swapChainDesc.BufferDesc.Height = UINT(window->GetHeight());							// Back buffer height
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;													// Refresh rate in hertz 
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;													// Numerator is an int
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;							// Color format - RGBA 8 bits
	//swapChainDesc.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;					// Default value for Flags
	//swapChainDesc.BufferDesc.Scaling				= DXGI_MODE_SCALING_UNSPECIFIED;						// Default mode for scaling
	swapChainDesc.SampleDesc.Count = antialiasing;											// Samples per pixel (antialiasing)
	swapChainDesc.SampleDesc.Quality = quality;												// Level of image quality
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						// Use surface as Render Target
	swapChainDesc.BufferCount = 2;													// Number of buffers (Front + Back)
	swapChainDesc.OutputWindow = window->GetId();										// Window ID
	swapChainDesc.Windowed = (window->GetMode() == WINDOWED);						// Fullscreen or windowed 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;								// Discard surface after presenting
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;				// Use Back buffer size for Fullscreen

	// Create Swap Chain
	if FAILED(dxgiFactory->CreateSwapChain(d3d11Device, &swapChainDesc, &swapChain))
	{
		MessageBox(nullptr, "Failed to create IDXGISwapChain.", 0, 0);
		return false;
	}

	// -------------------------------
	//	Create Render Target View - RTV
	// -------------------------------

	// Create Back buffer surface of a Swap Chain
	ID3D11Texture2D* backBuffer = nullptr;
	if FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)))
	{
		MessageBox(nullptr, "Failed to create ID3D11Texture2D.", 0, 0);
		return false;
	}

	if FAILED(d3d11Device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView))
	{
		MessageBox(nullptr, "Failed to create ID3D11RenderTargetView.", 0, 0);
		return false;
	}

	// -------------------------------
	//	Create Depth Stencil Buffer - DSV
	// -------------------------------

	// Describe Depth/Stencil Buffer Desc
	D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };
	depthStencilDesc.Width = UINT(window->GetWidth());					// Depth/Stencil buffer width
	depthStencilDesc.Height = UINT(window->GetHeight());				// Depth/Stencil buffer height
	depthStencilDesc.MipLevels = 0;										// Number of mipmap levels
	depthStencilDesc.ArraySize = 1;										// Number of textures in array
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;			// Color format - Does it need to be the same format of swapChainDesc?
	depthStencilDesc.SampleDesc.Count = antialiasing;								// Samples per pixel (antialiasing)
	depthStencilDesc.SampleDesc.Quality = quality;									// Level of image quality
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;						// Default - GPU will both read and write to the resource
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;					// Where resource will be bound to the pipeline
	depthStencilDesc.CPUAccessFlags = 0;										// CPU will not read not write to the Depth/Stencil buffer
	depthStencilDesc.MiscFlags = 0;										// Optional flags

	// Create Depth/Stencil Buffer
	ID3D11Texture2D* depthStencilBuffer;

	if FAILED(d3d11Device->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer))
	{
		MessageBox(nullptr, "Failed to create ID3D11Texture2D depthStencilDesc.", 0, 0);
		return false;
	}

	// Create Depth Stencil View - DSV
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
	viewport.TopLeftY = 0.0f;
	viewport.TopLeftX = 0.0f;
	viewport.Width = static_cast<float>(window->GetWidth());
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

// -------------------------------------------------------------------------------
// Initialize some pipeline stages to setup triangle -> QUAD for this build
void LearningEngine::Renderer::DirectX11::InitTriangle()
{
	// ------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------
	// Vertex Shader Stage
	// ----------------------

	// Set vertices
	Vertex vertices[6] =
	{
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f),		DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },					// Red
		{ DirectX::XMFLOAT3(-0.5f,  0.5f, 0.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },					// Green
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f),		DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },					// Blue
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f),		DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },					// Purple
		{ DirectX::XMFLOAT3(-0.5f,  0.5f, 0.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },					// Yellow
		{ DirectX::XMFLOAT3(0.5f,  0.5f, 0.0f),		DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },					// Blue
	};

	// Describe Buffer - Resource structure
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// Set data we want to initialize the buffer contents with
	D3D11_SUBRESOURCE_DATA srd = { vertices, 0, 0 };

	// Create Buffer
	if FAILED(d3d11Device->CreateBuffer(&bufferDesc, &srd, &vertexBuffer))
		MessageBoxA(nullptr, "Failed to create Buffer", 0, 0);
	// ------------------------------------------------------------------------------------------------------------------------------------------------


	// ------------------------------------------------------------------------------------------------------------------------------------------------
	DWORD shaderFlags = 0;
#ifndef _DEBUG
	shaderFlags |= D3D10_SHADER_DEBUG;						// Let compiler insert debug information into the output code
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;			// Compiler will not validate the generated code -> Recommended to use only with successfully compiled shaders
#endif // !_DEBUG


	// ------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------
	// Vertex Shader Stage
	// ----------------------

	// Compile and Create Vertex Shader
	ID3DBlob* shaderCompileErrorsBlob;						// To get info about compilation

	ID3DBlob* vsBlob;										// Vertex shader
	if FAILED(D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", shaderFlags, NULL, &vsBlob, &shaderCompileErrorsBlob))
		MessageBoxA(nullptr, "Failed to compile Vertex Shader.", 0, 0);

	if FAILED(d3d11Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader))
		MessageBoxA(nullptr, "Failed to create Vertex Shader.", 0, 0);


	// Compile and Create Pixel Shader
	ID3DBlob* psBlob;										// Pixel shader
	if FAILED(D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0", shaderFlags, NULL, &psBlob, &shaderCompileErrorsBlob))
		MessageBoxA(nullptr, "Failed to compile Pixel Shader.", 0, 0);

	if FAILED(d3d11Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader))
		MessageBoxA(nullptr, "Failed to create Pixel Shader.", 0, 0);
	// ------------------------------------------------------------------------------------------------------------------------------------------------


	// ------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------
	// Input Assembler Stage
	// ----------------------
	// Input layout
	ID3D11InputLayout* inputLayout;

	// Description of Vertex Structure we created
	D3D11_INPUT_ELEMENT_DESC inputDesc[2] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }				// 3 'floats' x 4 bytes = 12 bytes
	};

	// Create input layout
	ThrowIfFailed(d3d11Device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout))

		// Bind input layout to the Imput Assembler Stage
		d3d11DevContext->IASetInputLayout(inputLayout);

	// Tell how Direct3D will form geometric primitives from vertex data
	d3d11DevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ------------------------------------------------------------------------------------------------------------------------------------------------


	// Relase Direct3D resources
	psBlob->Release();
	vsBlob->Release();
}

// -------------------------------------------------------------------------------
// Draw a specific triangle
void LearningEngine::Renderer::DirectX11::DrawTriangle()
{
	UINT stride = sizeof(Vertex);													// Store size os Vertex Structure
	UINT offset = 0;																// Pointer to where the first Vertex Buffer is in array

	// Bind Vertex Buffer to an input slot of the device
	d3d11DevContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Bind Vertex and Pixel Shaders
	d3d11DevContext->VSSetShader(vertexShader, nullptr, 0);
	d3d11DevContext->PSSetShader(pixelShader, nullptr, 0);

	// Draw
	UINT numVerts = 6;
	d3d11DevContext->Draw(numVerts, 0);
}
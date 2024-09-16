#include "Graphics.h"

ID3D11Device* Graphics::device = nullptr;					// graphics device
ID3D11DeviceContext* Graphics::deviceContext = nullptr;		// graphics device context 
D3D11_VIEWPORT Graphics::viewport = { 0 };					// viewport

Graphics::Graphics()
{
	swapChain = nullptr;                    // pointer to swap chain 
	renderTargetView = nullptr;             // render target view
	blendState = nullptr;                   // color blending
	featureLevel = D3D_FEATURE_LEVEL_11_0;  // Direct3D version

	backgroundColor[0] = 0.0f;              // Red
	backgroundColor[1] = 0.0f;              // Green
	backgroundColor[2] = 0.0f;              // Blue
	backgroundColor[3] = 0.0f;              // Alpha (0 = transparent)

	verticalSyncEnabled = false;            // vertical sync disabled
}

// ------------------------------------------------------------------------------

Graphics::~Graphics()
{
	// release blend state
	if (blendState)
	{
		blendState->Release();
		blendState = nullptr;
	}

	// release render target
	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = nullptr;
	}

	// release swap chain
	if (swapChain)
	{
		// Direct3D is unable to close when in full screen
		swapChain->SetFullscreenState(false, NULL);
		swapChain->Release();
		swapChain = nullptr;
	}

	// release graphics device context
	if (deviceContext)
	{
		// restore to original state
		deviceContext->ClearState();
		deviceContext->Release();
		deviceContext = nullptr;
	}

	// release graphics device
	if (device)
	{
		device->Release();
		device = nullptr;
	}
}

// -----------------------------------------------------------------------------

bool Graphics::InitializeDirect3D(Window* window)
{
	// -------------------------------
	// Direct3D Device
	// -------------------------------

	uint createDeviceFlags = 0;

#ifdef _DEBUG
	// display Direct3D error messages in debug mode
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// create object to access Direct3D device
	if FAILED(
		D3D11CreateDevice(
			NULL,                           // video adapter (NULL = default adapter)
			D3D_DRIVER_TYPE_HARDWARE,       // D3D driver type (Hardware, Reference or Software)
			NULL,                           // pointer to software rasterizer
			createDeviceFlags,              // debug mode or normal mode
			NULL,                           // Direct3D feature levels (NULL = highest supported)
			0,                              // size of the feature levels array
			D3D11_SDK_VERSION,              // Direct3D SDK version
			&device,                        // stores the created D3D device
			&featureLevel,                  // Direct3D version used
			&deviceContext))                // D3D device context
	{
		// system does not support D3D11 device
		// creating a WARP Device that 
		// implements a software rasterizer
		if FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP,
			NULL, createDeviceFlags, NULL, 0, D3D11_SDK_VERSION,
			&device, &featureLevel, &deviceContext))
			return false;

		OutputDebugString(L"---> Using WARP Adapter: no support for D3D11\n");
	}

	// -------------------------------
	// Direct3D Background Color
	// -------------------------------

	// set the background color of the backbuffer
	// to the same background color as the window
	COLORREF color = window->GetBackgroundColor();

	backgroundColor[0] = GetRValue(color) / 255.0f;     // Red
	backgroundColor[1] = GetGValue(color) / 255.0f;     // Green
	backgroundColor[2] = GetBValue(color) / 255.0f;     // Blue
	backgroundColor[3] = 1.0f;                          // Alpha (1 = solid color)

	// -------------------------------
	// DXGI Interfaces
	// -------------------------------

	// create object for graphics infrastructure
	IDXGIDevice* dxgiDevice = nullptr;
	if FAILED(device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice))
		return false;

	// create object for video adapter (graphics card)
	IDXGIAdapter* dxgiAdapter = nullptr;
	if FAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter))
		return false;

	// create object for DXGI factory
	IDXGIFactory* dxgiFactory = nullptr;
	if FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory))
		return false;

	// -------------------------------
	// Swap Chain 
	// -------------------------------

	// description of a swap chain
	DXGI_SWAP_CHAIN_DESC swapDesc = { 0 };
	swapDesc.BufferDesc.Width = uint(window->GetWidth());          // backbuffer width
	swapDesc.BufferDesc.Height = uint(window->GetHeight());        // backbuffer height
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;                // refresh rate in hertz 
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;               // numerator is an integer, not a rational number
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;       // RGBA 8-bit color format
	swapDesc.SampleDesc.Count = 1;                                 // samples per pixel (antialiasing)
	swapDesc.SampleDesc.Quality = 0;                               // image quality level
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;        // use surface as RENDER-TARGET
	swapDesc.BufferCount = 2;                                      // number of buffers (front + back)
	swapDesc.OutputWindow = window->GetWindowId();                 // window handle
	swapDesc.Windowed = (window->GetMode() != FULLSCREEN);         // windowed or full screen mode
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;           // discard surface after presentation
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;       // change monitor resolution in full screen

	// create a swap chain
	if FAILED(dxgiFactory->CreateSwapChain(device, &swapDesc, &swapChain))
		return false;

	// prevent DXGI from monitoring ALT-ENTER and switching between windowed/fullscreen
	if FAILED(dxgiFactory->MakeWindowAssociation(window->GetWindowId(), DXGI_MWA_NO_ALT_ENTER))
		return false;

	// -------------------------------
	// Render Target
	// -------------------------------

	// get the backbuffer surface from a swap chain
	ID3D11Texture2D* backBuffer = nullptr;
	if FAILED(swapChain->GetBuffer(0, __uuidof(backBuffer), (void**)(&backBuffer)))
		return false;

	// create a render-target view of the backbuffer
	if FAILED(device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView))
		return false;

	// bind a render-target to the output-merger stage
	deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

	// -------------------------------
	// Viewport / Rasterizer
	// -------------------------------

	// configure a viewport
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = float(window->GetWidth());
	viewport.Height = float(window->GetHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// bind the viewport to the rasterizer stage
	deviceContext->RSSetViewports(1, &viewport);

	// ---------------------------------------------
	// Blend State
	// ---------------------------------------------

	// Color blending equation:
	// finalColor = SrcColor * SrcBlend <OP> DestColor * DestBlend

	// Combining transparent surfaces (Alpha Blending)
	// finalColor = SrcColor * ScrAlpha + DestColor * (1-SrcAlpha)

	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.AlphaToCoverageEnable = false;                                // highlight sprite silhouettes
	blendDesc.IndependentBlendEnable = false;                               // use same blend for all render targets
	blendDesc.RenderTarget[0].BlendEnable = true;                           // enable blending
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;             // source blend factor 
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;        // destination blend RGB is inverted alpha 
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;                 // addition operation in color blending
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;              // source blend Alpha is pixel shader alpha
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;            // destination blend Alpha is inverted alpha
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;            // addition operation in color blending
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;                 // components of each pixel that can be overwritten

	// create the blend state
	if FAILED(device->CreateBlendState(&blendDesc, &blendState))
		return false;

	// bind the blend state to the Output-Merger stage
	deviceContext->OMSetBlendState(blendState, nullptr, 0xffffffff);

	// -------------------------------
	// Release DXGI interfaces
	// -------------------------------

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();
	backBuffer->Release();

	// successful initialization
	return true;
}

// -----------------------------------------------------------------------------

#ifndef _MAGE2D_GRAPHICS_H_
#define _MAGE2D_GRAPHICS_H_

#include <dxgi.h>							   // DirectX Graphics Infrastructure
#include <d3d11.h>							   // Direct3D main functions

#include "Window.h"							   // Creates and configures a Windows window
#include "Types.h"							   // Specific types for the engine

class Graphics
{
private:
	IDXGISwapChain* m_swapChain;                 // Swap chain             
	ID3D11RenderTargetView* m_renderTargetView;  // Render target view of the backbuffer
	ID3D11BlendState* m_blendState;              // Color blending configuration
	D3D_FEATURE_LEVEL m_featureLevel;            // D3D feature level supported by the hardware
	float m_backgroundColor[4];                  // Background color of the backbuffer
	bool m_verticalSyncEnabled;                  // Vertical sync enabled/disabled

public:
	Graphics();
	~Graphics();

	static ID3D11Device* device;               // Graphics device
	static ID3D11DeviceContext* deviceContext; // Graphics device context
	static D3D11_VIEWPORT viewport;            // Viewport

	void SetVerticalSync(bool enabled);        // Enable/disable vertical sync
	void ClearBackBuffer();                    // Clear the backbuffer with the background color
	void PresentFrame();                       // Present the frame on the screen
	bool InitializeDirect3D(Window* window);   // Initialize Direct3D
};

// Enable/disable vertical sync
inline void Graphics::SetVerticalSync(bool enabled)
{
	m_verticalSyncEnabled = enabled;
}

// Clear the backbuffer for the next frame
inline void Graphics::ClearBackBuffer()
{
	deviceContext->ClearRenderTargetView(m_renderTargetView, m_backgroundColor);
}

// Present the frame on the screen (swap backbuffer with frontbuffer)
inline void Graphics::PresentFrame()
{
	m_swapChain->Present(m_verticalSyncEnabled, NULL);
	deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
}

#endif

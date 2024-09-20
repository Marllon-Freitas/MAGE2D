#ifndef _MAGE2D_RENDERER_H_
#define _MAGE2D_RENDERER_H_

#include <vector>

#include "Window.h"
#include "Graphics.h"
#include "Sprite.h"

class Renderer
{
private:
	ID3D11InputLayout* m_inputLayout;                      // input layout
	ID3D11VertexShader* m_vertexShader;                    // vertex shader
	ID3D11PixelShader* m_pixelShader;                      // pixel shader
	ID3D11RasterizerState* m_rasterState;                  // rasterizer state
	ID3D11SamplerState* m_sampler;                         // texture sampler state
	ID3D11Buffer* m_vertexBuffer;                          // vertex buffer
	ID3D11Buffer* m_indexBuffer;                           // index buffer
	ID3D11Buffer* m_constantBuffer;                        // shader constant buffer
	uint m_vertexBufferPosition;                           // current vertex buffer position

	static const uint MinBatchSize = 128;                  // minimum sprite batch size
	static const uint MaxBatchSize = 4096;                 // maximum sprite batch size
	static const uint VerticesPerSprite = 4;               // number of vertices per sprite
	static const uint IndicesPerSprite = 6;                // number of indices per sprite

	std::vector<SpriteData*> m_spriteVector;               // vector of sprites

	// renders a batch of sprites with the same texture
	void RenderBatch(ID3D11ShaderResourceView* texture, SpriteData** sprites, uint count);

public:
	Renderer();
	~Renderer();

	bool Initialize(Window* window, Graphics* graphics);   // initializes the renderer
	void Draw(SpriteData* sprite);                         // adds sprite to the list
	void Render();                                         // sends sprites for drawing
};

#endif

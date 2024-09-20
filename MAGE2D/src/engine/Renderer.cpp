#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <algorithm>

#include "Renderer.h"
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 tex;
};

Renderer::Renderer()
{
	m_inputLayout = nullptr;
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_rasterState = nullptr;
	m_sampler = nullptr;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_constantBuffer = nullptr;

	m_vertexBufferPosition = 0;
}

Renderer::~Renderer()
{
	if (m_constantBuffer)
	{
		m_constantBuffer->Release();
		m_constantBuffer = nullptr;
	}

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	if (m_sampler)
	{
		m_sampler->Release();
		m_sampler = nullptr;
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = nullptr;
	}

	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}

	if (m_inputLayout)
	{
		m_inputLayout->Release();
		m_inputLayout = nullptr;
	}
}

bool Renderer::Initialize(Window* window, Graphics* graphics)
{
	//-------------------------------
	// Vertex Shader
	//-------------------------------

	// load vertex shader bytecode (HLSL)
	ID3DBlob* vShader = nullptr;
	if FAILED(D3DReadFileToBlob(L"shaders/Vertex.cso", &vShader))
		return false;

	// create the vertex shader
	if FAILED(graphics->device->CreateVertexShader(vShader->GetBufferPointer(), vShader->GetBufferSize(), NULL, &m_vertexShader))
		return false;

	//-------------------------------
	// Input Layout
	//-------------------------------

	// describe the vertex input layout
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// create the input layout
	if FAILED(graphics->device->CreateInputLayout(layoutDesc, 3, vShader->GetBufferPointer(), vShader->GetBufferSize(), &m_inputLayout))
		return false;

	// release bytecode
	vShader->Release();

	//-------------------------------
	// Pixel Shader
	//-------------------------------

	// load pixel shader bytecode (HLSL)
	ID3DBlob* pShader = nullptr;
	if FAILED(D3DReadFileToBlob(L"shaders/Pixel.cso", &pShader))
		return false;

	// create the pixel shader
	if FAILED(graphics->device->CreatePixelShader(pShader->GetBufferPointer(), pShader->GetBufferSize(), NULL, &m_pixelShader))
		return false;

	// release bytecode
	pShader->Release();

	//-------------------------------
	// Rasterizer
	//-------------------------------

	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	//rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthClipEnable = true;

	// create rasterizer state
	if FAILED(graphics->device->CreateRasterizerState(&rasterDesc, &m_rasterState))
		return false;

	//-------------------------------
	// Vertex Buffer
	//-------------------------------

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * VerticesPerSprite * MaxBatchSize;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if FAILED(graphics->device->CreateBuffer(&vertexBufferDesc, nullptr, &m_vertexBuffer))
		return false;

	//-------------------------------
	// Index Buffer
	//-------------------------------

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(short) * IndicesPerSprite * MaxBatchSize;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// generate indices for the maximum number of supported sprites
	std::vector<short> indices;
	indices.reserve(MaxBatchSize * IndicesPerSprite);
	for (short i = 0; i < MaxBatchSize * VerticesPerSprite; i += VerticesPerSprite)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);

		indices.push_back(i + 1);
		indices.push_back(i + 3);
		indices.push_back(i + 2);
	}

	D3D11_SUBRESOURCE_DATA indexData = { 0 };
	indexData.pSysMem = &indices.front();

	if FAILED(graphics->device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer))
		return false;

	//-------------------------------
	// Constant Buffer
	//-------------------------------

	D3D11_BUFFER_DESC constBufferDesc = { 0 };
	constBufferDesc.ByteWidth = sizeof(XMMATRIX);
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// calculate the transformation matrix
	float xScale = (graphics->viewport.Width > 0) ? 2.0f / graphics->viewport.Width : 0.0f;
	float yScale = (graphics->viewport.Height > 0) ? 2.0f / graphics->viewport.Height : 0.0f;

	// transform to screen coordinates
	XMMATRIX transformMatrix
	(
		xScale, 0, 0, 0,
		0, -yScale, 0, 0,
		0, 0, 1, 0,
		-1, 1, 0, 1
	);

	D3D11_SUBRESOURCE_DATA constantData = { 0 };
	XMMATRIX worldViewProj = XMMatrixTranspose(transformMatrix);
	constantData.pSysMem = &worldViewProj;

	if FAILED(graphics->device->CreateBuffer(&constBufferDesc, &constantData, &m_constantBuffer))
		return false;

	//-------------------------------
	// Texture Sampler
	//-------------------------------

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = (graphics->device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// create the texture sampler
	if FAILED(graphics->device->CreateSamplerState(&samplerDesc, &m_sampler))
		return false;

	//-------------------------------
	// Configure Direct3D Pipeline
	//-------------------------------

	uint vertexStride = sizeof(Vertex);
	uint vertexOffset = 0;
	graphics->deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexStride, &vertexOffset);
	graphics->deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	graphics->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphics->deviceContext->IASetInputLayout(m_inputLayout);
	graphics->deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	graphics->deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	graphics->deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	graphics->deviceContext->PSSetSamplers(0, 1, &m_sampler);
	graphics->deviceContext->RSSetState(m_rasterState);

	// successful initialization
	return true;
}

void Renderer::RenderBatch(ID3D11ShaderResourceView* texture, SpriteData** sprites, uint count)
{
	// draw using the given texture
	Graphics::deviceContext->PSSetShaderResources(0, 1, &texture);

	while (count > 0)
	{
		// how many sprites to draw
		uint batchSize = count;

		// how many sprites fit in the vertex buffer
		uint remainingSpace = MaxBatchSize - m_vertexBufferPosition;

		// if the number of sprites is greater than the available space
		if (batchSize > remainingSpace)
		{
			// if the available space is too small
			if (remainingSpace < MinBatchSize)
			{
				// reset the buffer position
				m_vertexBufferPosition = 0;
				batchSize = (count < MaxBatchSize) ? count : MaxBatchSize;
			}
			else
			{
				// restrict the number of sprites by the remaining space
				batchSize = remainingSpace;
			}
		}

		// lock the vertex buffer for writing
		D3D11_MAP mapType = (m_vertexBufferPosition == 0) ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		Graphics::deviceContext->Map(m_vertexBuffer, 0, mapType, 0, &mappedBuffer);

		// position within the vertex buffer
		Vertex* vertices = (Vertex*)mappedBuffer.pData + m_vertexBufferPosition * VerticesPerSprite;

		// generate vertex positions for each sprite to be drawn in this batch
		for (uint i = 0; i < batchSize; ++i)
		{
			// get texture size
			XMVECTOR size = XMVectorMergeXY(XMLoadInt(&sprites[i]->width), XMLoadInt(&sprites[i]->height));
			XMVECTOR textureSize = XMConvertVectorUIntToFloat(size, 0);
			XMVECTOR inverseTextureSize = XMVectorReciprocal(textureSize);

			// organize sprite information
			XMFLOAT2 positionxy(sprites[i]->x, sprites[i]->y);
			float scale = sprites[i]->scale;
			XMFLOAT2 center(0.0f, 0.0f);
			float rotation = sprites[i]->rotation;
			float layerDepth = sprites[i]->depth;

			// load sprite information into SIMD registers
			XMVECTOR source = XMVectorSet(0, 0, 1, 1);
			XMVECTOR destination = XMVectorPermute<0, 1, 4, 4>(XMLoadFloat2(&positionxy), XMLoadFloat(&scale));
			XMVECTOR color = XMVectorSet(1, 1, 1, 1);
			XMVECTOR originRotationDepth = XMVectorSet(center.x, center.y, rotation, layerDepth);

			// extract source and destination sizes into separate vectors
			XMVECTOR sourceSize = XMVectorSwizzle<2, 3, 2, 3>(source);
			XMVECTOR destinationSize = XMVectorSwizzle<2, 3, 2, 3>(destination);

			// scale the source offset by the source size, taking care to avoid overflow if the source region is zero
			XMVECTOR isZeroMask = XMVectorEqual(sourceSize, XMVectorZero());
			XMVECTOR nonZeroSourceSize = XMVectorSelect(sourceSize, g_XMEpsilon, isZeroMask);

			XMVECTOR origin = XMVectorDivide(originRotationDepth, nonZeroSourceSize);

			// convert the source region from texels to mod-1 texture coordinates
			origin *= inverseTextureSize;

			// if the destination size is relative to the source region, convert it to pixels
			destinationSize *= textureSize;

			// calculate a 2x2 rotation matrix
			XMVECTOR rotationMatrix1;
			XMVECTOR rotationMatrix2;

			if (rotation != 0)
			{
				float sin, cos;

				XMScalarSinCos(&sin, &cos, rotation);

				XMVECTOR sinV = XMLoadFloat(&sin);
				XMVECTOR cosV = XMLoadFloat(&cos);

				rotationMatrix1 = XMVectorMergeXY(cosV, sinV);
				rotationMatrix2 = XMVectorMergeXY(-sinV, cosV);
			}
			else
			{
				rotationMatrix1 = g_XMIdentityR0;
				rotationMatrix2 = g_XMIdentityR1;
			}

			// the four vertices of the sprite are calculated from transformations of these unit positions
			static XMVECTORF32 cornerOffsets[VerticesPerSprite] =
			{
				{ 0, 0 },
				{ 1, 0 },
				{ 0, 1 },
				{ 1, 1 },
			};

			int mirrorBits = 0;

			// generate the four vertices
			for (int i = 0; i < VerticesPerSprite; ++i)
			{
				// calculate position
				XMVECTOR cornerOffset = (cornerOffsets[i] - origin) * destinationSize;

				// apply 2x2 rotation matrix
				XMVECTOR position1 = XMVectorMultiplyAdd(XMVectorSplatX(cornerOffset), rotationMatrix1, destination);
				XMVECTOR position2 = XMVectorMultiplyAdd(XMVectorSplatY(cornerOffset), rotationMatrix2, position1);

				// insert z component = depth
				XMVECTOR position = XMVectorPermute<0, 1, 7, 6>(position2, originRotationDepth);

				// Write position as a Float4, even though VertexPositionColor::position is an XMFLOAT3.
				// This is faster and harmless because we are only invalidating the first element
				// of the color field, which will be immediately overwritten with its correct value.
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&vertices[i].pos), position);

				// insert color
				XMStoreFloat4(&vertices[i].color, color);

				// compute and write texture coordinates
				XMVECTOR textureCoordinate = XMVectorMultiplyAdd(cornerOffsets[i ^ mirrorBits], sourceSize, source);

				XMStoreFloat2(&vertices[i].tex, textureCoordinate);
			}

			vertices += VerticesPerSprite;
		}

		// unlock the vertex buffer
		Graphics::deviceContext->Unmap(m_vertexBuffer, 0);

		// draw sprites
		uint startIndex = (uint)m_vertexBufferPosition * IndicesPerSprite;
		uint indexCount = (uint)batchSize * IndicesPerSprite;
		Graphics::deviceContext->DrawIndexed(indexCount, startIndex, 0);

		// advance the position in the vertex buffer
		m_vertexBufferPosition += batchSize;

		// advance the position in the sprite array
		sprites += batchSize;

		// batchSize sprites were drawn in this pass
		count -= batchSize;
	}
}

void Renderer::Render()
{
	// sort sprites by depth
	std::sort(m_spriteVector.begin(), m_spriteVector.end(),
		[](SpriteData* a, SpriteData* b) -> bool
		{ return a->depth > b->depth; });

	// number of sprites to be rendered
	uint spriteCount = uint(m_spriteVector.size());

	if (spriteCount == 0)
		return;

	ID3D11ShaderResourceView* currentTexture = nullptr;
	uint batchStartIndex = 0;

	// group adjacent sprites that share the same texture
	for (uint i = 0; i < spriteCount; ++i)
	{
		ID3D11ShaderResourceView* texture = m_spriteVector[i]->texture;

		if (texture != currentTexture)
		{
			if (i > batchStartIndex)
			{
				RenderBatch(currentTexture, &m_spriteVector[batchStartIndex], i - batchStartIndex);
			}

			currentTexture = texture;
			batchStartIndex = i;
		}
	}

	// draw the final group of sprites
	RenderBatch(currentTexture, &m_spriteVector[batchStartIndex], spriteCount - batchStartIndex);

	// clear the drawing list (updated every frame)
	m_spriteVector.clear();
}

void Renderer::Draw(SpriteData* sprite)
{
	m_spriteVector.push_back(sprite);
}

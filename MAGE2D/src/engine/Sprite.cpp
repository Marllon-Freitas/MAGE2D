#include "Sprite.h"
#include "Engine.h"

// predefined depth values
const float Layer::FRONT = 0.00f;
const float Layer::UPPER = 0.25f;
const float Layer::MIDDLE = 0.50f;
const float Layer::LOWER = 0.75f;
const float Layer::BACK = 0.99f;

Sprite::Sprite(const std::string& filename)
{
	// load image
	m_image = new Image(filename);
	m_isLocalImage = true;

	// configure sprite record
	m_spriteData.texture = m_image->GetShaderResourceView();
}

Sprite::Sprite(const Image* img)
{
	// point to external image
	m_image = img;
	m_isLocalImage = false;

	// configure sprite record
	m_spriteData.texture = m_image->GetShaderResourceView();
}

Sprite::~Sprite()
{
	if (m_isLocalImage)
		delete m_image;
}

void Sprite::Draw(float x, float y, float z)
{
	m_spriteData.x = x;
	m_spriteData.y = y;
	m_spriteData.scale = 1.0f;
	m_spriteData.depth = z;
	m_spriteData.rotation = 0.0f;
	m_spriteData.width = m_image->GetWidth();
	m_spriteData.height = m_image->GetHeight();

	// add the sprite to the drawing list
	Engine::renderer->Draw(&m_spriteData);
}

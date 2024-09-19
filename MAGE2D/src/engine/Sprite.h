#ifndef _MAGE2D_SPRITE_H_
#define _MAGE2D_SPRITE_H_

#include "Image.h"

struct SpriteData
{
	float x, y;
	float scale;
	float depth;
	float rotation;
	uint width;
	uint height;
	ID3D11ShaderResourceView* texture;
};

struct Layer
{
	static const float FRONT;
	static const float UPPER;
	static const float MIDDLE;
	static const float LOWER;
	static const float BACK;
};

class Sprite
{
private:
	SpriteData m_spriteData;			  // sprite data
	bool m_isLocalImage;				  // local or external image
	const Image* m_image;				  // pointer to an image

public:
	Sprite(const std::string& filename);  // constructs sprite from a file
	Sprite(const Image* img);             // constructs sprite from an existing image
	~Sprite();                            // sprite destructor

	int GetWidth() const;                 // returns sprite width
	int GetHeight() const;                // returns sprite height

	// draws image at position (x,y) and depth (z)
	void Draw(float x, float y, float z = Layer::MIDDLE);
};


// returns sprite width
inline int Sprite::GetWidth() const
{
	return m_image->GetWidth();
}

// returns sprite height
inline int Sprite::GetHeight() const
{
	return m_image->GetHeight();
}

#endif

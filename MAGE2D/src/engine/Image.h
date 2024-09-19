#ifndef _MAGE2D_IMAGE_H_
#define _MAGE2D_IMAGE_H_

#include <string>

#include "Types.h"
#include "Texture.h"

class Image
{
private:
	ID3D11ShaderResourceView* m_textureView;					// view associated with the texture
	uint m_width;												// image width
	uint m_height;												// image height

public:
	Image(const std::string& filename);							// constructs image from a file
	~Image();													// destructor

	uint GetWidth() const;										// returns image width
	uint GetHeight() const;										// returns image height
	ID3D11ShaderResourceView* GetShaderResourceView() const;	// returns pointer to the image view
};

// returns image width
inline uint Image::GetWidth() const
{
	return m_width;
}

// returns image height
inline uint Image::GetHeight() const
{
	return m_height;
}

// returns pointer to the shader resource view
inline ID3D11ShaderResourceView* Image::GetShaderResourceView() const
{
	return m_textureView;
}


#endif

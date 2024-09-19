#include "Image.h"
#include "Graphics.h"

Image::Image(const std::string& filename) : m_textureView(nullptr), m_width(0), m_height(0)
{
	// create shader resource view from the image file
	D3D11CreateTextureFromFile(
		Graphics::device,					// Direct3D device
		Graphics::deviceContext,            // device context
		filename.c_str(),					// image file name
		nullptr,							// returns texture 
		&m_textureView,						// returns texture view
		m_width,							// returns image width
		m_height);							// returns image height
}

Image::~Image()
{
	// release memory occupied by the texture view
	if (m_textureView)
	{
		// get pointer to resource
		ID3D11Resource* resource = nullptr;
		m_textureView->GetResource(&resource);

		// releasing the view does not automatically release
		// the resource that was created along with the view
		if (resource)
		{
			resource->Release();
			resource = nullptr;
		}

		m_textureView->Release();
		m_textureView = nullptr;
	}
}

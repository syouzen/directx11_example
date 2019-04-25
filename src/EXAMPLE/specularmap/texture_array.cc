#include "texture_array.h"

TextureArray::TextureArray()
{
	m_textures[0] = 0;
	m_textures[1] = 0;
	m_textures[2] = 0;
}

TextureArray::TextureArray(const TextureArray& other)
{
}

TextureArray::~TextureArray()
{
}

bool TextureArray::initialize(ID3D11Device* device, const WCHAR* filename, const WCHAR* filename2, const WCHAR* filename3)
{
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_textures[0], NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CreateShaderResourceViewFromFile(device, filename2, NULL, NULL, &m_textures[1], NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CreateShaderResourceViewFromFile(device, filename3, NULL, NULL, &m_textures[2], NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureArray::shutdown()
{
	if (m_textures[0])
	{
		m_textures[0]->Release();
		m_textures[0] = 0;
	}

	if (m_textures[1])
	{
		m_textures[1]->Release();
		m_textures[1] = 0;
	}

	if (m_textures[2])
	{
		m_textures[2]->Release();
		m_textures[2] = 0;
	}
}

ID3D11ShaderResourceView** TextureArray::getTextureArray()
{
	return m_textures;
}
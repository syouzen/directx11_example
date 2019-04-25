#pragma once

#include <d3d11.h> 
#include <d3dx11tex.h>

class TextureArray
{
public:
	TextureArray();
	TextureArray(const TextureArray&);
	~TextureArray();

	bool initialize(ID3D11Device*, const WCHAR*, const WCHAR*, const WCHAR*);
	void shutdown();

	ID3D11ShaderResourceView** getTextureArray();

private:
	ID3D11ShaderResourceView* m_textures[3];
};
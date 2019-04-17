#pragma once

#include <d3d11.h> 
#include <d3dx11tex.h>

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool initialize(ID3D11Device*, const WCHAR*);
	void shutdown();

	ID3D11ShaderResourceView* getTexture();

private:
	ID3D11ShaderResourceView * m_texture;
};
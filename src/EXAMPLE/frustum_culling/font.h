#pragma once

#include <d3d11.h> 
#include <d3dx10math.h> 
#include <fstream> 

using namespace std; 

#include "texture.h"

class Font
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Font();
	Font(const Font&);
	~Font();

	bool initialize(ID3D11Device*, const char*, const WCHAR*);
	void shutdown();

	ID3D11ShaderResourceView* getTexture();
	void buildVertexArray(void*, const char*, float, float);

private:
	bool loadFontData(const char*);
	void releaseFontData();
	bool loadTexture(ID3D11Device*, const WCHAR*);
	void releaseTexture();

private:
	FontType * m_Font;
	Texture* m_Texture;
};
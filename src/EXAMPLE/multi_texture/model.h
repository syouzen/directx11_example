#pragma once

#include "texture_array.h"

#include <d3d11.h> 
#include <d3dx10math.h>

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position; 
		D3DXVECTOR2 texture;
	};

public:
	Model();
	Model(const Model&);
	~Model();

public:
	bool initialize(ID3D11Device*, const WCHAR*, const WCHAR*);
	void shutdown();
	void render(ID3D11DeviceContext*);

	int getIndexCount();
	ID3D11ShaderResourceView** getTextureArray();

private:
	bool initializeBuffers(ID3D11Device*);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext*);

	bool loadTexture(ID3D11Device*, const WCHAR*, const WCHAR*); 
	void releaseTextures();

private: 
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer; 
	int m_vertexCount, m_indexCount;
	TextureArray* m_TextureArray;
};
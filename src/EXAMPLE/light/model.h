#pragma once

#include "texture.h"

#include <d3d11.h> 
#include <d3dx10math.h>

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position; 
		D3DXVECTOR2 texture; 
		D3DXVECTOR3 normal;
	};

public:
	Model();
	Model(const Model&);
	~Model();

public:
	bool initialize(ID3D11Device*, const WCHAR*);
	void shutdown();
	void render(ID3D11DeviceContext*);

	int getIndexCount();
	ID3D11ShaderResourceView* getTexture();

private:
	bool initializeBuffers(ID3D11Device*);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext*);

	bool loadTexture(ID3D11Device*, const WCHAR*); 
	void releaseTexture();

private: 
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer; 
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
};
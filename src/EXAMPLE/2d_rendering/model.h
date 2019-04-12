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
	};

public:
	Model();
	Model(const Model&);
	~Model();

public:
	bool initialize(ID3D11Device*, int, int, const WCHAR*, int, int);
	void shutdown();
	bool render(ID3D11DeviceContext*, int, int);

	int getIndexCount();
	ID3D11ShaderResourceView* getTexture();

private:
	bool initializeBuffers(ID3D11Device*);
	void shutdownBuffers();
	bool updateBuffers(ID3D11DeviceContext*, int, int);
	void renderBuffers(ID3D11DeviceContext*);

	bool loadTexture(ID3D11Device*, const WCHAR*);
	void releaseTexture();

private:
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;

	int m_screenWidth, m_screenHeight; 
	int m_bitmapWidth, m_bitmapHeight; 
	int m_previousPosX, m_previousPosY;
};
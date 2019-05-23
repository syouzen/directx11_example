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

	struct InstanceType
	{
		D3DXVECTOR3 position;
	};

public:
	Model();
	Model(const Model&);
	~Model();

public:
	bool initialize(ID3D11Device*, const WCHAR*);
	void shutdown();
	void render(ID3D11DeviceContext*);

	int getVertexCount();
	int getInstanceCount();
	ID3D11ShaderResourceView* getTexture();

private:
	bool initializeBuffers(ID3D11Device*);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext*);

	bool loadTexture(ID3D11Device*, const WCHAR*); 
	void releaseTexture();

private:
	ID3D11Buffer * m_vertexBuffer;
	ID3D11Buffer* m_instanceBuffer;
	int m_vertexCount;
	int m_instanceCount;
	Texture* m_Texture;
};
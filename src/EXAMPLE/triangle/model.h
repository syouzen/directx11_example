#pragma once

#include <d3d11.h> 
#include <d3dx10math.h>

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

public:
	Model();
	Model(const Model&);
	~Model();

public:
	bool initialize(ID3D11Device*);
	void shutdown();
	void render(ID3D11DeviceContext*);

	int getIndexCount();

private:
	bool initializeBuffers(ID3D11Device*);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext*);

private: 
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer; 
	int m_vertexCount, m_indexCount;
};
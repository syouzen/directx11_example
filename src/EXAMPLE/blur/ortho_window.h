#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

class OrthoWindow
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	OrthoWindow();
	OrthoWindow(const OrthoWindow&);
	~OrthoWindow();

	bool initialize(ID3D11Device*, int, int);
	void shutdown();
	void render(ID3D11DeviceContext*);

	int getIndexCount();

private:
	bool initializeBuffers(ID3D11Device*, int, int);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};
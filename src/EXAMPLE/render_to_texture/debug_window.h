#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

class DebugWindow
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	DebugWindow();
	DebugWindow(const DebugWindow&);
	~DebugWindow();

	bool initialize(ID3D11Device*, int, int, int, int);
	void shutdown();
	bool render(ID3D11DeviceContext*, int, int);

	int getIndexCount();

private:
	bool initializeBuffers(ID3D11Device*);
	void shutdownBuffers();
	bool updateBuffers(ID3D11DeviceContext*, int, int);
	void renderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};
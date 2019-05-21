#pragma once

#include <d3d11.h> 
#include <d3dx10math.h>
#include <fstream>

using namespace std;

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	};

	struct ModelType 
	{
		float x, y, z;
		float tu, tv; 
		float nx, ny, nz; 
	};

public:
	Model();
	Model(const Model&);
	~Model();

public:
	bool initialize(ID3D11Device*, const char*);
	void shutdown();
	void render(ID3D11DeviceContext*);

	int getIndexCount();

private:
	bool initializeBuffers(ID3D11Device*);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext*);

	bool loadModel(const char*); 
	void releaseModel();

private: 
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer; 
	int m_vertexCount, m_indexCount;
	ModelType* m_model;
};
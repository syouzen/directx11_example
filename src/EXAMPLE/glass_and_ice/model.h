#pragma once

#include "texture.h"

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
		D3DXVECTOR2 texture; 
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
	bool initialize(ID3D11Device*, const char*, const WCHAR*, const WCHAR*);
	void shutdown();
	void render(ID3D11DeviceContext*);

	int getIndexCount();
	ID3D11ShaderResourceView* getTexture();
	ID3D11ShaderResourceView* getNormalMap();

private:
	bool initializeBuffers(ID3D11Device*);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext*);

	bool loadTexture(ID3D11Device*, const WCHAR*, const WCHAR*);
	void releaseTexture();

	bool loadModel(const char*); 
	void releaseModel();

private: 
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer; 
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
	Texture* m_NormalMap;
	ModelType* m_model;
};
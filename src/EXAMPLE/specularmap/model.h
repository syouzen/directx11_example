#pragma once

#include "texture_array.h"

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
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};
	
	struct ModelType
	{
		float x, y, z;
		float tu, tv; 
		float nx, ny, nz; 
		float tx, ty, tz; 
		float bx, by, bz;
	};

	struct TempVertexType
	{
		float x, y, z; 
		float tu, tv;
		float nx, ny, nz;
	};
	
	struct VectorType
	{
		float x, y, z;
	};

public:
	Model();
	Model(const Model&);
	~Model();

public:
	bool initialize(ID3D11Device*, const char*, const WCHAR*, const WCHAR*, const WCHAR*);
	void shutdown();
	void render(ID3D11DeviceContext*);

	int getIndexCount();
	ID3D11ShaderResourceView** getTextureArray();

private:
	bool initializeBuffers(ID3D11Device*);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext*);

	bool loadTexture(ID3D11Device*, const WCHAR*, const WCHAR*, const WCHAR*);
	void releaseTextures();

	bool loadModel(const char*);
	void releaseModel();

	void calculateModelVectors();
	void calculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&); 
	void calculateNormal(VectorType, VectorType, VectorType&);
	
private: 
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer; 
	int m_vertexCount, m_indexCount;
	TextureArray* m_TextureArray;
	ModelType* m_model;
};
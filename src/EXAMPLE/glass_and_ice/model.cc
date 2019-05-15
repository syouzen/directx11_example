#include "model.h"

Model::Model()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_NormalMap = 0;
	m_model = 0;
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}

bool Model::initialize(ID3D11Device* device, const char* modelFilename, const WCHAR* textureFilename, const WCHAR* normalTextureFilename)
{
	bool result;

	result = loadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	result = initializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = loadTexture(device, textureFilename, normalTextureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::shutdown()
{
	releaseTexture();
	shutdownBuffers();
	releaseModel();
}

void Model::render(ID3D11DeviceContext* deviceContext)
{
	renderBuffers(deviceContext);
}

int Model::getIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Model::getTexture()
{
	return m_Texture->getTexture();
}

ID3D11ShaderResourceView* Model::getNormalMap()
{
	return m_NormalMap->getTexture();
}

bool Model::initializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	for (int i = 0; i<m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z); 
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv); 		
		indices[i] = i;
	}
		
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Model::shutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void Model::renderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::loadTexture(ID3D11Device* device, const WCHAR* filename, const WCHAR* filename2)
{
	bool result;

	m_Texture = new Texture;
	if (!m_Texture)
	{
		return false;
	}

	result = m_Texture->initialize(device, filename);
	if (!result)
	{
		return false;
	}

	m_NormalMap = new Texture;
	if (!m_NormalMap)
	{
		return false;
	}

	result = m_NormalMap->initialize(device, filename2);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::releaseTexture()
{	
	if (m_Texture)
	{
		m_Texture->shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	if (m_NormalMap)
	{
		m_NormalMap->shutdown();
		delete m_NormalMap;
		m_NormalMap = 0;
	}
}

bool Model::loadModel(const char* filename)
{
	ifstream fin;
	char input;
	int i;

	fin.open(filename);

	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_vertexCount;

	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for (i = 0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	fin.close();

	return true;
}

void Model::releaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
}
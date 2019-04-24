#include "model.h"

Model::Model()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_TextureArray = 0;
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}

bool Model::initialize(ID3D11Device* device, const WCHAR* textureFilename, const WCHAR* textureFilename2)
{
	bool result;

	result = initializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = loadTexture(device, textureFilename, textureFilename2);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::shutdown()
{
	releaseTextures();
	shutdownBuffers();
}

void Model::render(ID3D11DeviceContext* deviceContext)
{
	renderBuffers(deviceContext);
}

int Model::getIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView** Model::getTextureArray()
{
	return m_TextureArray->getTextureArray();
}

bool Model::initializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = 3;
	m_indexCount = 3;

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

	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);

	indices[0] = 0;  
	indices[1] = 1;  
	indices[2] = 2;

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

	m_TextureArray = new TextureArray;
	if (!m_TextureArray)
	{
		return false;
	}

	result = m_TextureArray->initialize(device, filename, filename2);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::releaseTextures()
{	
	if (m_TextureArray)
	{
		m_TextureArray->shutdown();
		delete m_TextureArray;
		m_TextureArray = 0;
	}
}
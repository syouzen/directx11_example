#include "model.h"

Model::Model()
{
	m_vertexBuffer = 0;
	m_instanceBuffer = 0;
	m_Texture = 0;
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}

bool Model::initialize(ID3D11Device* device, const WCHAR* textureFilename)
{
	bool result;

	result = initializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = loadTexture(device, textureFilename);
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
}

void Model::render(ID3D11DeviceContext* deviceContext)
{
	renderBuffers(deviceContext);
}

int Model::getVertexCount()
{
	return m_vertexCount;
}

int Model::getInstanceCount()
{
	return m_instanceCount;
}

ID3D11ShaderResourceView* Model::getTexture()
{
	return m_Texture->getTexture();
}

bool Model::initializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	InstanceType* instances;
	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	HRESULT result;

	m_vertexCount = 3;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);

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

	delete[] vertices;
	vertices = 0;

	m_instanceCount = 4;

	instances = new InstanceType[m_instanceCount];
	if (!instances)
	{
		return false;
	}

	instances[0].position = D3DXVECTOR3(-1.5f, -1.5f, 5.0f);
	instances[1].position = D3DXVECTOR3(-1.5f, 1.5f, 5.0f);
	instances[2].position = D3DXVECTOR3(1.5f, -1.5f, 5.0f);
	instances[3].position = D3DXVECTOR3(1.5f, 1.5f, 5.0f);

	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] instances;
	instances = 0;

	return true;
}

void Model::shutdownBuffers()
{
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void Model::renderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);

	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::loadTexture(ID3D11Device* device, const WCHAR* filename)
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
}
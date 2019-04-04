#include "model.h"

Model::Model()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


Model::Model(const Model& other)
{
}


Model::~Model()
{
}

bool Model::initialize(ID3D11Device* device)
{
	bool result;

	result = initializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::shutdown()
{
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

	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // 왼쪽 아래
	vertices[0].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // 상단 가운데
	vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // 오른쪽 아래
	vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

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
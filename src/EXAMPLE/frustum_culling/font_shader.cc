#pragma once
#include "font_shader.h"

FontShader::FontShader()
{
	m_vertexFontShader = 0;
	m_pixelFontShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
	m_pixelBuffer = 0;
}

FontShader::FontShader(const FontShader& other)
{
}

FontShader::~FontShader()
{
}

bool FontShader::initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	
	result = initializeShader(device, hwnd, L"./hlsl/FontVertexShader.hlsl", L"./hlsl/FontPixelShader.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

void FontShader::shutdown()
{
	shutdownShader();
}

bool FontShader::render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4 pixelColor)
{
	bool result;

	result = setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, pixelColor);
	if (!result)
	{
		return false;
	}

	renderShader(deviceContext, indexCount);

	return true;
}

bool FontShader::initializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexFontShaderBuffer;
	ID3D10Blob* pixelFontShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC pixelBufferDesc;
	
	errorMessage = 0;
	vertexFontShaderBuffer = 0;
	pixelFontShaderBuffer = 0;

	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexFontShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing FontShader File", MB_OK);
		}

		return false;
	}

	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelFontShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing FontShader File", MB_OK);
		}

		return false;
	}
	result = device->CreateVertexShader(vertexFontShaderBuffer->GetBufferPointer(), vertexFontShaderBuffer->GetBufferSize(), NULL, &m_vertexFontShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pixelFontShaderBuffer->GetBufferPointer(), pixelFontShaderBuffer->GetBufferSize(), NULL, &m_pixelFontShader);
	if (FAILED(result))
	{
		return false;
	}

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, vertexFontShaderBuffer->GetBufferPointer(),
									   vertexFontShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	vertexFontShaderBuffer->Release();
	vertexFontShaderBuffer = 0;

	pixelFontShaderBuffer->Release();
	pixelFontShaderBuffer = 0;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; 
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; 
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; 
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; 
	samplerDesc.MipLODBias = 0.0f; 
	samplerDesc.MaxAnisotropy = 1; 
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; 
	samplerDesc.BorderColor[0] = 0; 
	samplerDesc.BorderColor[1] = 0; 
	samplerDesc.BorderColor[2] = 0; 
	samplerDesc.BorderColor[3] = 0; 
	samplerDesc.MinLOD = 0; 
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC; 
	pixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
	pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; 
	pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
	pixelBufferDesc.MiscFlags = 0; 
	pixelBufferDesc.StructureByteStride = 0; 
	
	result = device->CreateBuffer(&pixelBufferDesc, NULL, &m_pixelBuffer); 
	if (FAILED(result)) 
	{ 
		return false; 
	} 

	return true;
}

void FontShader::shutdownShader()
{
	if (m_pixelBuffer)
	{
		m_pixelBuffer->Release(); 
		m_pixelBuffer = 0;
	}

	if (m_sampleState) 
	{ 
		m_sampleState->Release(); 
		m_sampleState = 0; 
	}

	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_pixelFontShader)
	{
		m_pixelFontShader->Release();
		m_pixelFontShader = 0;
	}

	if (m_vertexFontShader)
	{
		m_vertexFontShader->Release();
		m_vertexFontShader = 0;
	}

	return;
}


void FontShader::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* FontShaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();

	fout.open("FontShader-error.txt");

	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = 0;

	MessageBox(hwnd, L"Error compiling FontShader.  Check FontShader-error.txt for message.", FontShaderFilename, MB_OK);

	return;
}

bool FontShader::setShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4 pixelColor)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	PixelBufferType* dataPtr2;
	unsigned int bufferNumber;
	
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(m_matrixBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	deviceContext->PSSetShaderResources(0, 1, &texture); 
	
	result = deviceContext->Map(m_pixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource); 
	if (FAILED(result))
	{
		return false;
	}
	
	dataPtr2 = (PixelBufferType*)mappedResource.pData; 
		
	dataPtr2->pixelColor = pixelColor; 
	
	deviceContext->Unmap(m_pixelBuffer, 0); 

	bufferNumber = 0; 

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pixelBuffer); 
	
	return true; 
}


void FontShader::renderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(m_layout);

	deviceContext->VSSetShader(m_vertexFontShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelFontShader, NULL, 0);

	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}
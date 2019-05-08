#pragma once

#include <d3d11.h> 
#include <d3dx10math.h> 
#include <d3dx11async.h> 
#include <fstream> 

using namespace std; 

class ReflectionShader
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct ReflectionBufferType
	{
		D3DXMATRIX reflectionMatrix;
	};

public:
	ReflectionShader();
	ReflectionShader(const ReflectionShader&);
	~ReflectionShader();

	bool initialize(ID3D11Device*, HWND);
	void shutdown();
	bool render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, 
				ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, D3DXMATRIX);

private:
	bool initializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool setShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, 
							 ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, D3DXMATRIX);
	void renderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader * m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;

	ID3D11Buffer* m_reflectionBuffer;
};
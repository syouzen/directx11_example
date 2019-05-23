#pragma once

#include <d3d11.h> 
#include <d3dx10math.h> 
#include <d3dx11async.h> 
#include <fstream> 

using namespace std; 

class Shader
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct TessellationBufferType
	{
		float tessellationAmount;
		D3DXVECTOR3 padding;
	};
public:
	Shader();
	Shader(const Shader&);
	~Shader();

	bool initialize(ID3D11Device*, HWND);
	void shutdown();
	bool render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, float);

private:
	bool initializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*, const WCHAR*, const WCHAR*);
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool setShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, float);
	void renderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader * m_vertexShader;
	ID3D11HullShader* m_hullShader;
	ID3D11DomainShader* m_domainShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_tessellationBuffer;
};
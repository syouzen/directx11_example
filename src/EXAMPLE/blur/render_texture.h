#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

class RenderTexture
{
public:
	RenderTexture();
	RenderTexture(const RenderTexture&);
	~RenderTexture();

	bool initialize(ID3D11Device*, int, int, float, float);
	void shutdown();

	void setRenderTarget(ID3D11DeviceContext*);
	void clearRenderTarget(ID3D11DeviceContext*, float, float, float, float);
	ID3D11ShaderResourceView* getShaderResourceView();

	void getProjectionMatrix(D3DXMATRIX&);
	void getOrthoMatrix(D3DXMATRIX&);

	int getTextureWidth();
	int getTextureHeight();

private:
	int m_textureWidth, m_textureHeight;
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_orthoMatrix;
};
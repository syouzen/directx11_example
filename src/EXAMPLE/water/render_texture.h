#pragma once

#include <d3d11.h>

class RenderTexture
{
public:
	RenderTexture();
	RenderTexture(const RenderTexture&);
	~RenderTexture();

	bool initialize(ID3D11Device*, int, int);
	void shutdown();

	void setRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void clearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*, float, float, float, float);
	ID3D11ShaderResourceView* getShaderResourceView();

private:
	ID3D11Texture2D * m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
};
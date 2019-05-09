#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0; 
	m_Camera = 0; 
	m_Model = 0; 
	m_Bitmap = 0;
	m_FadeShader = 0; 
	m_RenderTexture = 0; 
	m_TextureShader = 0;
}

Graphics::Graphics(const Graphics& other)
{
}

Graphics::~Graphics()
{
}

bool Graphics::initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_D3D = new D3D; 
	
	if(!m_D3D) 
	{ 
		return false; 
	} 
	
	result = m_D3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR); 
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK); 
		return false; 
	} 
	
	m_Camera = new Camera; 
	if (!m_Camera) 
	{ 
		return false; 
	}

	m_Camera->setPosition(0.0f, 0.0f, -10.0f); 

	m_Model = new Model; 
	if(!m_Model) 
	{ 
		return false; 
	} 
	
	result = m_Model->initialize(m_D3D->getDevice(), "./resource/cube.txt", L"./resource/seafloor.dds"); 
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK); 
		return false; 
	} 	
	
	m_Bitmap = new Bitmap;
	if (!m_Bitmap)
	{
		return false;
	}

	result = m_Bitmap->initialize(m_D3D->getDevice(), screenWidth, screenHeight, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_RenderTexture = new RenderTexture;
	if (!m_RenderTexture)
	{
		return false;
	}

	result = m_RenderTexture->initialize(m_D3D->getDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}
	
	m_TextureShader = new TextureShader;
	if (!m_TextureShader)
	{
		return false;
	}
	
	result = m_TextureShader->initialize(m_D3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}
	
	m_FadeShader = new FadeShader;
	if (!m_FadeShader)
	{
		return false;
	}

	result = m_FadeShader->initialize(m_D3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fade shader object.", L"Error", MB_OK);
		return false;
	}

	m_fadeInTime = 3000.0f;
	m_accumulatedTime = 0;
	m_fadePercentage = 0;
	m_fadeDone = false;

	return true;
}
void Graphics::shutdown()
{
	if (m_TextureShader)
	{
		m_TextureShader->shutdown();
		delete m_TextureShader;
	}
	
	if (m_RenderTexture)
	{
		m_RenderTexture->shutdown();
		delete m_RenderTexture;
	}
	
	if(m_FadeShader) 
	{ 
		m_FadeShader->shutdown(); 
		delete m_FadeShader; 
	} 	

	if (m_Bitmap)
	{
		m_Bitmap->shutdown();
		delete m_Bitmap;
	}

	if(m_Model) 
	{ 
		m_Model->shutdown(); 
		delete m_Model; 
	}
	
	if (m_Camera) {
		delete m_Camera;
	}

	if (m_D3D)
	{
		m_D3D->shutdown();
		delete m_D3D;
	}
}

bool Graphics::frame(float frameTime)
{
	if (!m_fadeDone)
	{
		m_accumulatedTime += frameTime;

		if (m_accumulatedTime < m_fadeInTime)
		{
			m_fadePercentage = m_accumulatedTime / m_fadeInTime;
		}
		else
		{
			m_fadeDone = true;
			m_fadePercentage = 1.0f;
		}
	}

	m_Camera->setPosition(0.0f, 0.0f, -10.0f);

	return true;
}

bool Graphics::render()
{
	bool result;
	static float rotation = 0.0f;

	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	if (m_fadeDone)
	{
		renderNormalScene(rotation);
	}
	else
	{
		result = renderToTexture(rotation);
		if (!result)
		{
			return false;
		}

		result = renderFadingScene();
		if (!result)
		{
			return false;
		}
	}

	return true;
}

bool Graphics::renderToTexture(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_RenderTexture->setRenderTarget(m_D3D->getDeviceContext(), m_D3D->getDepthStencilView());

	m_RenderTexture->clearRenderTarget(m_D3D->getDeviceContext(), m_D3D->getDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();
	
	m_D3D->getWorldMatrix(worldMatrix);
	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	D3DXMatrixRotationY(&worldMatrix, rotation);

	m_Model->render(m_D3D->getDeviceContext());

	result = m_TextureShader->render(m_D3D->getDeviceContext(), m_Model->getIndexCount(), worldMatrix, viewMatrix,
									 projectionMatrix, m_Model->getTexture());
	if (!result)
	{
		return false;
	}

	m_D3D->setBackBufferRenderTarget();

	return true;
}

bool Graphics::renderFadingScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	m_D3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_D3D->getWorldMatrix(worldMatrix);
	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getOrthoMatrix(orthoMatrix);

	m_D3D->turnZBufferOff();

	result = m_Bitmap->render(m_D3D->getDeviceContext(), 0, 0);
	if (!result)
	{
		return false;
	}

	result = m_FadeShader->render(m_D3D->getDeviceContext(), m_Bitmap->getIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
								  m_RenderTexture->getShaderResourceView(), m_fadePercentage);
	if (!result)
	{
		return false;
	}

	m_D3D->turnZBufferOn();

	m_D3D->endScene();

	return true;
}

bool Graphics::renderNormalScene(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_D3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_D3D->getWorldMatrix(worldMatrix);
	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	D3DXMatrixRotationY(&worldMatrix, rotation);
	
	m_Model->render(m_D3D->getDeviceContext());

	result = m_TextureShader->render(m_D3D->getDeviceContext(), m_Model->getIndexCount(), worldMatrix, viewMatrix,
									 projectionMatrix, m_Model->getTexture());
	if (!result)
	{
		return false;
	}

	m_D3D->endScene();

	return true;
}
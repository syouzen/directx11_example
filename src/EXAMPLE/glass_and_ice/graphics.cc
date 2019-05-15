#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_WindowModel = 0;
	m_RenderTexture = 0;
	m_TextureShader = 0;
	m_GlassShader = 0;
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

	m_Model = new Model;
	if (!m_Model)
	{
		return false;
	}

	result = m_Model->initialize(m_D3D->getDevice(), "./resource/cube.txt", L"./resource/seafloor.dds", L"./resource/bump03.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	
	m_WindowModel = new Model;
	if (!m_WindowModel)
	{
		return false;
	}

	result = m_WindowModel->initialize(m_D3D->getDevice(), "./resource/square.txt", L"./resource/glass01.dds", L"./resource/bump03.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the window model object.", L"Error", MB_OK);
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

	m_GlassShader = new GlassShader;
	if (!m_GlassShader)
	{
		return false;
	}

	result = m_GlassShader->initialize(m_D3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the glass shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}
void Graphics::shutdown()
{
	if (m_GlassShader)
	{
		m_GlassShader->shutdown();
		delete m_GlassShader;
		m_GlassShader = 0;
	}

	if (m_TextureShader)
	{
		m_TextureShader->shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_RenderTexture)
	{
		m_RenderTexture->shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}
	
	if (m_WindowModel)
	{
		m_WindowModel->shutdown();
		delete m_WindowModel;
		m_WindowModel = 0;
	}

	if (m_Model)
	{
		m_Model->shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_D3D)
	{
		m_D3D->shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
}

bool Graphics::frame()
{
	static float rotation = 0.0f;
	bool result;

	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	m_Camera->setPosition(0.0f, 0.0f, -10.0f);

	result = renderToTexture(rotation);
	if (!result)
	{
		return false;
	}

	result = render(rotation);
	if (!result)
	{
		return false;
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

	result = m_TextureShader->render(m_D3D->getDeviceContext(), m_Model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->getTexture());
	if (!result)
	{
		return false;
	}
	
	m_D3D->setBackBufferRenderTarget();

	return true;
}

bool Graphics::render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	float refractionScale;
	bool result;

	refractionScale = 0.01f;

	m_D3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_D3D->getWorldMatrix(worldMatrix);
	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	D3DXMatrixRotationY(&worldMatrix, rotation);

	m_Model->render(m_D3D->getDeviceContext());

	result = m_TextureShader->render(m_D3D->getDeviceContext(), m_Model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
									 m_Model->getTexture());
	if (!result)
	{
		return false;
	}

	m_D3D->getWorldMatrix(worldMatrix);

	D3DXMatrixTranslation(&worldMatrix, 0.0f, 0.0f, -1.5f);

	m_WindowModel->render(m_D3D->getDeviceContext());

	result = m_GlassShader->render(m_D3D->getDeviceContext(), m_WindowModel->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
								   m_WindowModel->getTexture(), m_WindowModel->getNormalMap(), m_RenderTexture->getShaderResourceView(),
								   refractionScale);
	if (!result)
	{
		return false;
	}

	m_D3D->endScene();

	return true;
}
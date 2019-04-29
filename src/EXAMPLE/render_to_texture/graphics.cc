#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0; 
	m_Camera = 0; 
	m_Model = 0; 
	m_LightShader = 0; 
	m_Light = 0; 
	m_RenderTexture = 0; 
	m_DebugWindow = 0; 
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
	
	m_LightShader = new LightShader; 
	if(!m_LightShader) 
	{ 
		return false; 	
	} 	
	
	result = m_LightShader->initialize(m_D3D->getDevice(), hwnd); 
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK); 
		return false; 
	} 

	m_Light = new Light;
	if (!m_Light)
	{
		return false;
	}

	m_Light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->setDirection(0.0f, 0.0f, 1.0f);
	
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

	m_DebugWindow = new DebugWindow;
	if (!m_DebugWindow)
	{
		return false;
	}

	result = m_DebugWindow->initialize(m_D3D->getDevice(), screenWidth, screenHeight, 100, 100);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
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

	return true;
}
void Graphics::shutdown()
{
	if (m_TextureShader)
	{
		m_TextureShader->shutdown();
		delete m_TextureShader;
	}

	if (m_DebugWindow)
	{
		m_DebugWindow->shutdown();
		delete m_DebugWindow;
	}

	if (m_RenderTexture)
	{
		m_RenderTexture->shutdown();
		delete m_RenderTexture;
	}

	if (m_Light)
	{
		delete m_Light;
	}

	if(m_LightShader) 
	{ 
		m_LightShader->shutdown(); 
		delete m_LightShader; 
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

bool Graphics::frame()
{
	m_Camera->setPosition(0.0f, 0.0f, -5.0f);

	return true;
}

bool Graphics::render()
{
	bool result;
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;

	result = renderToTexture();
	if (!result)
	{
		return false;
	}

	m_D3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	result = renderScene();
	if (!result)
	{
		return false;
	}

	m_D3D->turnZBufferOff();

	m_D3D->getWorldMatrix(worldMatrix);
	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getOrthoMatrix(orthoMatrix);

	result = m_DebugWindow->render(m_D3D->getDeviceContext(), 50, 50);
	if (!result)
	{
		return false;
	}

	result = m_TextureShader->render(m_D3D->getDeviceContext(), m_DebugWindow->getIndexCount(), worldMatrix, viewMatrix,
									 orthoMatrix, m_RenderTexture->getShaderResourceView());
	if (!result)
	{
		return false;
	}

	m_D3D->turnZBufferOn();

	m_D3D->endScene();

	return true;
}

bool Graphics::renderToTexture()
{
	bool result;

	m_RenderTexture->setRenderTarget(m_D3D->getDeviceContext(), m_D3D->getDepthStencilView());

	m_RenderTexture->clearRenderTarget(m_D3D->getDeviceContext(), m_D3D->getDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	result = renderScene();
	if (!result)
	{
		return false;
	}

	m_D3D->setBackBufferRenderTarget();

	return true;
}

bool Graphics::renderScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	static float rotation = 0.0f;

	m_Camera->render();

	m_D3D->getWorldMatrix(worldMatrix);
	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	D3DXMatrixRotationY(&worldMatrix, rotation);

	m_Model->render(m_D3D->getDeviceContext());

	result = m_LightShader->render(m_D3D->getDeviceContext(), m_Model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
								   m_Model->getTexture(), m_Light->getDirection(), m_Light->getDiffuseColor());
	if (!result)
	{
		return false;
	}

	return true;
}
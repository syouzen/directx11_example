#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0;
	m_Camera = 0;
	m_GroundModel = 0;
	m_WallModel = 0;
	m_BathModel = 0;
	m_WaterModel = 0;
	m_Light = 0;
	m_RefractionTexture = 0;
	m_ReflectionTexture = 0;
	m_LightShader = 0;
	m_RefractionShader = 0;
	m_WaterShader = 0;
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

	m_GroundModel = new Model;
	if (!m_GroundModel)
	{
		return false;
	}

	result = m_GroundModel->initialize(m_D3D->getDevice(), "./resource/ground.txt", L"./resource/ground01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the ground model object.", L"Error", MB_OK);
		return false;
	}

	m_WallModel = new Model;
	if (!m_WallModel)
	{
		return false;
	}

	result = m_WallModel->initialize(m_D3D->getDevice(), "./resource/wall.txt", L"./resource/wall01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the wall model object.", L"Error", MB_OK);
		return false;
	}

	m_BathModel = new Model;
	if (!m_BathModel)
	{
		return false;
	}

	result = m_BathModel->initialize(m_D3D->getDevice(), "./resource/bath.txt", L"./resource/marble01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bath model object.", L"Error", MB_OK);
		return false;
	}

	m_WaterModel = new Model;
	if (!m_WaterModel)
	{
		return false;
	}

	result = m_WaterModel->initialize(m_D3D->getDevice(), "./resource/water.txt", L"./resource/water01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water model object.", L"Error", MB_OK);
		return false;
	}

	m_Light = new Light;
	if (!m_Light)
	{
		return false;
	}

	m_Light->setAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->setDirection(0.0f, -1.0f, 0.5f);

	
	m_RefractionTexture = new RenderTexture;
	if (!m_RefractionTexture)
	{
		return false;
	}

	result = m_RefractionTexture->initialize(m_D3D->getDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	m_ReflectionTexture = new RenderTexture;
	if (!m_ReflectionTexture)
	{
		return false;
	}

	result = m_ReflectionTexture->initialize(m_D3D->getDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}
	
	m_LightShader = new LightShader;
	if (!m_LightShader)
	{
		return false;
	}

	result = m_LightShader->initialize(m_D3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	m_RefractionShader = new RefractionShader;
	if (!m_RefractionShader)
	{
		return false;
	}

	result = m_RefractionShader->initialize(m_D3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
		return false;
	}

	m_WaterShader = new WaterShader;
	if (!m_WaterShader)
	{
		return false;
	}

	result = m_WaterShader->initialize(m_D3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}

	m_waterHeight = 2.75f;
	m_waterTranslation = 0.0f;

	return true;
}
void Graphics::shutdown()
{
	if (m_WaterShader)
	{
		m_WaterShader->shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	if (m_RefractionShader)
	{
		m_RefractionShader->shutdown();
		delete m_RefractionShader;
		m_RefractionShader = 0;
	}

	if (m_LightShader)
	{
		m_LightShader->shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_ReflectionTexture)
	{
		m_ReflectionTexture->shutdown();
		delete m_ReflectionTexture;
		m_ReflectionTexture = 0;
	}

	if (m_RefractionTexture)
	{
		m_RefractionTexture->shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (m_WaterModel)
	{
		m_WaterModel->shutdown();
		delete m_WaterModel;
		m_WaterModel = 0;
	}

	if (m_BathModel)
	{
		m_BathModel->shutdown();
		delete m_BathModel;
		m_BathModel = 0;
	}

	if (m_WallModel)
	{
		m_WallModel->shutdown();
		delete m_WallModel;
		m_WallModel = 0;
	}

	if (m_GroundModel)
	{
		m_GroundModel->shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
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
	m_waterTranslation += 0.001f;
	if (m_waterTranslation > 1.0f)
	{
		m_waterTranslation -= 1.0f;
	}

	m_Camera->setPosition(-10.0f, 6.0f, -10.0f);
	m_Camera->setRotation(0.0f, 45.0f, 0.0f);
	
	return true;
}

bool Graphics::render()
{
	bool result;

	result = renderRefractionToTexture();
	if (!result)
	{
		return false;
	}
	
	result = renderReflectionToTexture();
	if (!result)
	{
		return false;
	}

	result = renderScene();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::renderRefractionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	clipPlane = D3DXVECTOR4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);

	m_RefractionTexture->setRenderTarget(m_D3D->getDeviceContext(), m_D3D->getDepthStencilView());
	
	m_RefractionTexture->clearRenderTarget(m_D3D->getDeviceContext(), m_D3D->getDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_D3D->getWorldMatrix(worldMatrix);
	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	D3DXMatrixTranslation(&worldMatrix, 0.0f, 2.0f, 0.0f);

	m_BathModel->render(m_D3D->getDeviceContext());

	result = m_RefractionShader->render(m_D3D->getDeviceContext(), m_BathModel->getIndexCount(), worldMatrix, viewMatrix,
										projectionMatrix, m_BathModel->getTexture(), m_Light->getDirection(),
										m_Light->getAmbientColor(), m_Light->getDiffuseColor(), clipPlane);
	if (!result)
	{
		return false;
	}

	m_D3D->setBackBufferRenderTarget();

	return true;
}

bool Graphics::renderReflectionToTexture()
{
	D3DXMATRIX reflectionViewMatrix, worldMatrix, projectionMatrix;
	bool result;

	m_ReflectionTexture->setRenderTarget(m_D3D->getDeviceContext(), m_D3D->getDepthStencilView());

	m_ReflectionTexture->clearRenderTarget(m_D3D->getDeviceContext(), m_D3D->getDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->renderReflection(m_waterHeight);

	reflectionViewMatrix = m_Camera->getReflectionViewMatrix();

	m_D3D->getWorldMatrix(worldMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	D3DXMatrixTranslation(&worldMatrix, 0.0f, 6.0f, 8.0f);

	m_WallModel->render(m_D3D->getDeviceContext());

	result = m_LightShader->render(m_D3D->getDeviceContext(), m_WallModel->getIndexCount(), worldMatrix, reflectionViewMatrix,
								   projectionMatrix, m_WallModel->getTexture(), m_Light->getDirection(),
								   m_Light->getAmbientColor(), m_Light->getDiffuseColor());
	if (!result)
	{
		return false;
	}

	m_D3D->setBackBufferRenderTarget();

	return true;
}

bool Graphics::renderScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix;
	bool result;

	m_D3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_D3D->getWorldMatrix(worldMatrix);
	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	D3DXMatrixTranslation(&worldMatrix, 0.0f, 1.0f, 0.0f);

	m_GroundModel->render(m_D3D->getDeviceContext());

	result = m_LightShader->render(m_D3D->getDeviceContext(), m_GroundModel->getIndexCount(), worldMatrix, viewMatrix,
								   projectionMatrix, m_GroundModel->getTexture(), m_Light->getDirection(),
								   m_Light->getAmbientColor(), m_Light->getDiffuseColor());
	if (!result)
	{
		return false;
	}
	
	m_D3D->getWorldMatrix(worldMatrix);

	D3DXMatrixTranslation(&worldMatrix, 0.0f, 6.0f, 8.0f);

	m_WallModel->render(m_D3D->getDeviceContext());
	
	result = m_LightShader->render(m_D3D->getDeviceContext(), m_WallModel->getIndexCount(), worldMatrix, viewMatrix,
								   projectionMatrix, m_WallModel->getTexture(), m_Light->getDirection(),
								   m_Light->getAmbientColor(), m_Light->getDiffuseColor());
	if (!result)
	{
		return false;
	}

	m_D3D->getWorldMatrix(worldMatrix);

	D3DXMatrixTranslation(&worldMatrix, 0.0f, 2.0f, 0.0f);

	m_BathModel->render(m_D3D->getDeviceContext());

	result = m_LightShader->render(m_D3D->getDeviceContext(), m_BathModel->getIndexCount(), worldMatrix, viewMatrix,
								   projectionMatrix, m_BathModel->getTexture(), m_Light->getDirection(),
								   m_Light->getAmbientColor(), m_Light->getDiffuseColor());
	if (!result)
	{
		return false;
	}

	m_D3D->getWorldMatrix(worldMatrix);

	reflectionMatrix = m_Camera->getReflectionViewMatrix();

	D3DXMatrixTranslation(&worldMatrix, 0.0f, m_waterHeight, 0.0f);

	m_WaterModel->render(m_D3D->getDeviceContext());

	result = m_WaterShader->render(m_D3D->getDeviceContext(), m_WaterModel->getIndexCount(), worldMatrix, viewMatrix,
								   projectionMatrix, reflectionMatrix, m_ReflectionTexture->getShaderResourceView(),
								   m_RefractionTexture->getShaderResourceView(), m_WaterModel->getTexture(),
								   m_waterTranslation, 0.01f);
	if (!result)
	{
		return false;
	}

	m_D3D->endScene();

	return true;
}
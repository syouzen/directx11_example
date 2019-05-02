#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0; 
	m_Camera = 0; 
	m_Model = 0; 
	m_FogShader = 0; 
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
	D3DXMATRIX baseViewMatrix;

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
	if(!m_Model) 
	{ 
		return false; 
	} 
	
	result = m_Model->initialize(m_D3D->getDevice(), L"./resource/seafloor.dds", "./resource/cube.txt"); 
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK); 
		return false; 
	} 	
	
	m_FogShader = new FogShader; 
	if(!m_FogShader) 
	{ 
		return false; 	
	} 	
	
	result = m_FogShader->initialize(m_D3D->getDevice(), hwnd); 
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK); 
		return false; 
	} 

	return true; 
}
void Graphics::shutdown()
{
	if(m_FogShader) 
	{ 
		m_FogShader->shutdown(); 
		delete m_FogShader; 
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
	m_Camera->setPosition(0.0f, 0.0f, -10.0f);

	return true;
}

bool Graphics::render()
{
	float fogColor, fogStart, fogEnd;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	static float rotation = 0.0f;

	fogColor = 0.5f;
	fogStart = 0.0f;
	fogEnd = 10.0f;

	m_D3D->beginScene(fogColor, fogColor, fogColor, 1.0f);

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

	result = m_FogShader->render(m_D3D->getDeviceContext(), m_Model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
								 m_Model->getTexture(), fogStart, fogEnd);
	if (!result)
	{
		return false;
	}

	m_D3D->endScene();

	return true;
}
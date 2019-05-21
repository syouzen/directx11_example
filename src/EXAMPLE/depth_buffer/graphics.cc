#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0;
	m_Camera = 0;
	m_DepthShader = 0;
	m_Model = 0;
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

	m_Camera->setPosition(0.0f, 2.0f, -10.0f);
	
	m_Model = new Model;
	if (!m_Model)
	{
		return false;
	}

	result = m_Model->initialize(m_D3D->getDevice(), "./resource/floor.txt");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the floor model object.", L"Error", MB_OK);
		return false;
	}
	
	m_DepthShader = new DepthShader;
	if (!m_DepthShader)
	{
		return false;
	}

	result = m_DepthShader->initialize(m_D3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the depth shader object.", L"Error", MB_OK);
		return false;
	}

	return true; 
}
void Graphics::shutdown()
{
	if (m_Model)
	{
		m_Model->shutdown();
		delete m_Model;
	}
	
	if (m_DepthShader)
	{
		m_DepthShader->shutdown();
		delete m_DepthShader;
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
	bool result;

	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_D3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getWorldMatrix(worldMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	m_Model->render(m_D3D->getDeviceContext());

	result = m_DepthShader->render(m_D3D->getDeviceContext(), m_Model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	m_D3D->endScene();

	return true;
}
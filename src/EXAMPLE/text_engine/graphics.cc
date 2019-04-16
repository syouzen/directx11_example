#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0; 
	m_Camera = 0; 
	m_Text = 0;
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

	m_Camera->setPosition(0.0f, 0.0f, -10.0f); 
	m_Camera->render();
	m_Camera->getViewMatrix(baseViewMatrix);

	m_Text = new Text;
	if (!m_Text)
	{
		return false;
	}
	
	result = m_Text->initialize(m_D3D->getDevice(), m_D3D->getDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::shutdown()
{
	if(m_Text) 
	{ 
		m_Text->shutdown(); 
		delete m_Text; 
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

void Graphics::frame()
{
	m_Camera->setPosition(0.0f, 0.0f, -10.0f);
}

bool Graphics::render()
{
	bool result;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_D3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render(); 

	m_Camera->getViewMatrix(viewMatrix); 

	m_D3D->getWorldMatrix(worldMatrix); 
	m_D3D->getProjectionMatrix(projectionMatrix); 	
	
	/* 2D Rendering */
	m_D3D->getOrthoMatrix(orthoMatrix);
	m_D3D->turnZBufferOff();

	/* Alpha Blending */
	m_D3D->turnOnAlphaBlending();
	
	result = m_Text->render(m_D3D->getDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	m_D3D->turnOffAlphaBlending();

	/* 3D Rendering */
	m_D3D->turnZBufferOn();

	m_D3D->endScene();
	
	return true;
}
#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0; 
	m_Camera = 0; 
	m_Model = 0; 
	m_Shader = 0;
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
	
	result = m_Model->initialize(m_D3D->getDevice(), screenWidth, screenHeight, L"./resource/seafloor.dds", 256, 256);
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK); 
		return false; 
	} 	
	
	m_Shader = new Shader; 
	if(!m_Shader) 
	{ 
		return false; 	
	} 	
	
	result = m_Shader->initialize(m_D3D->getDevice(), hwnd); 
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK); 
		return false; 
	} 

	return true; 
}
void Graphics::shutdown()
{
	if(m_Shader) 
	{ 
		m_Shader->shutdown(); 
		delete m_Shader; 
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
	bool result;
	static float rotation = 0.0f; 
	
	rotation += (float)D3DX_PI * 0.005f; 
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	result = this->render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::render(float rotation)
{
	bool result;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_D3D->beginScene(0.0f, 0.0f, 0.4f, 1.0f);

	m_Camera->render(); 

	m_Camera->getViewMatrix(viewMatrix); 

	m_D3D->getWorldMatrix(worldMatrix); 
	m_D3D->getProjectionMatrix(projectionMatrix); 	
	
	/* 2D Rendering */
	m_D3D->getOrthoMatrix(orthoMatrix);
	m_D3D->turnZBufferOff();
	
	m_Model->render(m_D3D->getDeviceContext(), 100, 100); 
	
	result = m_Shader->render(m_D3D->getDeviceContext(), m_Model->getIndexCount(), 
							  worldMatrix, viewMatrix, orthoMatrix,
							  m_Model->getTexture());
	
	if(!result)
	{ 
		return false;
	}

	/* 3D Rendering */
	m_D3D->turnZBufferOn();

	m_D3D->endScene();
	
	return true;
}
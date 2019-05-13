#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0; 
	m_Camera = 0; 
	m_Model = 0; 
	m_Shader = 0;
	m_Light1 = 0;
	m_Light2 = 0;
	m_Light3 = 0;
	m_Light4 = 0;
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
	
	result = m_Model->initialize(m_D3D->getDevice(), "plane01.txt", L"stone01.dds");
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

	m_Light1 = new Light;
	if (!m_Light1)
	{
		return false;
	}
	
	m_Light1->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light1->setPosition(-3.0f, 1.0f, 3.0f);

	m_Light2 = new Light;
	if (!m_Light2)
	{
		return false;
	}

	m_Light2->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Light2->setPosition(3.0f, 1.0f, 3.0f);

	m_Light3 = new Light;
	if (!m_Light3)
	{
		return false;
	}
	
	m_Light3->setDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Light3->setPosition(-3.0f, 1.0f, -3.0f);

	m_Light4 = new Light;
	if (!m_Light4)
	{
		return false;
	}
	
	m_Light4->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light4->setPosition(3.0f, 1.0f, -3.0f);
	
	return true; 
}
void Graphics::shutdown()
{
	if (m_Light1)
	{
		delete m_Light1;
		m_Light1 = 0;
	}

	if (m_Light2)
	{
		delete m_Light2;
		m_Light2 = 0;
	}

	if (m_Light3)
	{
		delete m_Light3;
		m_Light3 = 0;
	}

	if (m_Light4)
	{
		delete m_Light4;
		m_Light4 = 0;
	}

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

	m_Camera->setPosition(0.0f, 2.0f, -12.0f);
	
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
	D3DXVECTOR4 diffuseColor[4];
	D3DXVECTOR4 lightPosition[4];
	bool result;

	diffuseColor[0] = m_Light1->getDiffuseColor();
	diffuseColor[1] = m_Light2->getDiffuseColor();
	diffuseColor[2] = m_Light3->getDiffuseColor();
	diffuseColor[3] = m_Light4->getDiffuseColor();

	lightPosition[0] = m_Light1->getPosition();
	lightPosition[1] = m_Light2->getPosition();
	lightPosition[2] = m_Light3->getPosition();
	lightPosition[3] = m_Light4->getPosition();

	m_D3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_D3D->getWorldMatrix(worldMatrix);
	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	m_Model->render(m_D3D->getDeviceContext());

	result = m_Shader->render(m_D3D->getDeviceContext(), m_Model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
								   m_Model->getTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	m_D3D->endScene();

	return true;
}
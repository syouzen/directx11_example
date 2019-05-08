#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0; 
	m_Camera = 0; 
	m_Model1 = 0;
	m_Model2 = 0;
	m_TextureShader = 0;
	m_TransparentShader = 0; 
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

	m_Model1 = new Model; 
	if(!m_Model1) 
	{ 
		return false; 
	} 
	
	result = m_Model1->initialize(m_D3D->getDevice(), L"./resource/dirt01.dds", "./resource/square.txt"); 
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK); 
		return false; 
	} 	

	m_Model2 = new Model;
	if (!m_Model2)
	{
		return false;
	}

	result = m_Model2->initialize(m_D3D->getDevice(), L"./resource/stone01.dds", "./resource/square.txt");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_TextureShader = new TextureShader;
	if(!m_TextureShader)
	{ 
		return false; 	
	} 	
	
	result = m_TextureShader->initialize(m_D3D->getDevice(), hwnd);
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK); 
		return false; 
	} 


	m_TransparentShader = new TransparentShader;
	if (!m_TransparentShader)
	{
		return false;
	}

	result = m_TransparentShader->initialize(m_D3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the transparent shader object.", L"Error", MB_OK);
		return false;
	}


	return true; 
}
void Graphics::shutdown()
{
	if (m_TransparentShader)
	{
		m_TransparentShader->shutdown();
		delete m_TransparentShader;
	}

	if (m_TextureShader)
	{
		m_TextureShader->shutdown();
		delete m_TextureShader;
	}

	if (m_Model2)
	{
		m_Model2->shutdown();
		delete m_Model2;
	}

	if (m_Model1)
	{
		m_Model1->shutdown();
		delete m_Model1;
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
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	float blendAmount;

	blendAmount = 0.5f;

	m_D3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_D3D->getWorldMatrix(worldMatrix);
	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	m_Model1->render(m_D3D->getDeviceContext());

	result = m_TransparentShader->render(m_D3D->getDeviceContext(), m_Model1->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
								 m_Model1->getTexture(), blendAmount);
	if (!result)
	{
		return false;
	}

	D3DXMatrixTranslation(&worldMatrix, 1.0f, 0.0f, -1.0f);

	m_D3D->turnOnAlphaBlending();

	m_Model2->render(m_D3D->getDeviceContext());

	result = m_TransparentShader->render(m_D3D->getDeviceContext(), m_Model2->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
										 m_Model2->getTexture(), blendAmount);
	if (!result)
	{
		return false;
	}

	m_D3D->turnOffAlphaBlending();

	m_D3D->endScene();

	return true;
}
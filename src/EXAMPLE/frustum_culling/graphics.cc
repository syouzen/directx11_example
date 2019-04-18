#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0; 
	m_Camera = 0; 
	m_Text = 0;
	m_Model = 0; 
	m_Shader = 0; 
	m_Light = 0; 
	m_ModelList = 0; 
	m_Frustum = 0;
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

	m_Camera->setPosition(0.0f, 0.0f, -1.0f);
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

	m_Model = new Model; 
	if(!m_Model) 
	{ 
		return false; 
	} 
	
	result = m_Model->initialize(m_D3D->getDevice(), L"./resource/seafloor.dds", "./resource/sphere.txt"); 
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK); 
		return false; 
	} 	
	
	m_Shader = new LightShader; 
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

	m_Light = new Light;
	if (!m_Light)
	{
		return false;
	}

	m_Light->setDirection(0.0f, 0.0f, 1.0f);

	m_ModelList = new ModelList;
	if (!m_ModelList)
	{
		return false;
	}

	result = m_ModelList->initialize(25);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}
	
	m_Frustum = new Frustum; 
	if (!m_Frustum) 
	{ 
		return false; 
	}

	return true; 
}
void Graphics::shutdown()
{
	if (m_Frustum)
	{
		delete m_Frustum; 
	} 

	if (m_ModelList)
	{
		m_ModelList->shutdown();
		delete m_ModelList;
	}
	
	if (m_Light)
	{
		delete m_Light;
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

	if (m_Text)
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

bool Graphics::frame(float rotationY)
{
	m_Camera->setPosition(0.0f, 0.0f, -10.0f); 
	m_Camera->setRotation(0.0f, rotationY, 0.0f);

	return true;
}

bool Graphics::render()
{
	bool renderModel, result;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	int modelCount, renderCount, index; 
	float positionX, positionY, positionZ, radius; 
	D3DXVECTOR4 color;
	
	m_D3D->beginScene(0.0f, 0.0f, 0.4f, 1.0f);

	m_Camera->render(); 

	m_Camera->getViewMatrix(viewMatrix); 

	m_D3D->getWorldMatrix(worldMatrix); 
	m_D3D->getProjectionMatrix(projectionMatrix);
	m_D3D->getOrthoMatrix(orthoMatrix);

	m_Frustum->constructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);
	modelCount = m_ModelList->getModelCount();
	renderCount = 0;
	
	for (index = 0; index<modelCount; index++)
	{
		m_ModelList->getData(index, positionX, positionY, positionZ, color);

		radius = 1.0f;

		renderModel = m_Frustum->checkSphere(positionX, positionY, positionZ, radius);

		if (renderModel)
		{
			D3DXMatrixTranslation(&worldMatrix, positionX, positionY, positionZ);

			m_Model->render(m_D3D->getDeviceContext());
			m_Shader->render(m_D3D->getDeviceContext(), m_Model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
								  m_Model->getTexture(), m_Light->getDirection(), color);

			m_D3D->getWorldMatrix(worldMatrix);
			
			renderCount++;
		}
	}
		
	result = m_Text->setRenderCount(renderCount, m_D3D->getDeviceContext());
	if (!result)
	{
		return false;
	}

	m_D3D->turnZBufferOff();
	m_D3D->turnOnAlphaBlending();

	m_Text->render(m_D3D->getDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	m_D3D->turnOffAlphaBlending();

	m_D3D->turnZBufferOn();

	m_D3D->endScene();

	return true;
}
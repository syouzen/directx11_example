#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0;
	m_Camera = 0;
	m_TextureShader = 0;
	m_FloorModel = 0;
	m_BillboardModel = 0;
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

	m_FloorModel = new Model;
	if (!m_FloorModel)
	{
		return false;
	}

	result = m_FloorModel->initialize(m_D3D->getDevice(), L"./resource/grid01.dds", "./resource/floor.txt");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the floor model object.", L"Error", MB_OK);
		return false;
	}

	m_BillboardModel = new Model;
	if (!m_BillboardModel)
	{
		return false;
	}

	result = m_BillboardModel->initialize(m_D3D->getDevice(), L"./resource/seafloor.dds", "./resource/square.txt");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the billboard model object.", L"Error", MB_OK);
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
	if (m_BillboardModel)
	{
		m_BillboardModel->shutdown();
		delete m_BillboardModel;
	}

	if (m_FloorModel)
	{
		m_FloorModel->shutdown();
		delete m_FloorModel;
	}
	
	if (m_TextureShader)
	{
		m_TextureShader->shutdown();
		delete m_TextureShader;
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

bool Graphics::frame(float positionX, float positionY, float positionZ)
{
	bool result;

	m_Camera->setPosition(positionX, positionY, positionZ);

	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix;
	bool result;
	D3DXVECTOR3 cameraPosition, modelPosition;
	double angle;
	float rotation;

	m_D3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getWorldMatrix(worldMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	m_FloorModel->render(m_D3D->getDeviceContext());

	result = m_TextureShader->render(m_D3D->getDeviceContext(), m_FloorModel->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
									 m_FloorModel->getTexture());
	if (!result)
	{
		return false;
	}

	cameraPosition = m_Camera->getPosition();

	modelPosition.x = 0.0f;
	modelPosition.y = 1.5f;
	modelPosition.z = 0.0f;

	angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / D3DX_PI);

	rotation = (float)angle * 0.0174532925f;

	D3DXMatrixRotationY(&worldMatrix, rotation);

	D3DXMatrixTranslation(&translateMatrix, modelPosition.x, modelPosition.y, modelPosition.z);

	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	m_BillboardModel->render(m_D3D->getDeviceContext());

	result = m_TextureShader->render(m_D3D->getDeviceContext(), m_BillboardModel->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
									 m_BillboardModel->getTexture());
	if (!result)
	{
		return false;
	}

	m_D3D->endScene();

	return true;
}
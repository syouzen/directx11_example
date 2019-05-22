#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
	m_HorizontalBlurShader = 0;
	m_VerticalBlurShader = 0;
	m_RenderTexture = 0;
	m_DownSampleTexure = 0;
	m_HorizontalBlurTexture = 0;
	m_VerticalBlurTexture = 0;
	m_UpSampleTexure = 0;
	m_SmallWindow = 0;
	m_FullScreenWindow = 0;
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
	int downSampleWidth, downSampleHeight;

	downSampleWidth = screenWidth / 2;
	downSampleHeight = screenHeight / 2;

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
	
	m_HorizontalBlurShader = new HorizontalBlurShader;
	if (!m_HorizontalBlurShader)
	{
		return false;
	}

	result = m_HorizontalBlurShader->initialize(m_D3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur shader object.", L"Error", MB_OK);
		return false;
	}

	m_VerticalBlurShader = new VerticalBlurShader;
	if (!m_VerticalBlurShader)
	{
		return false;
	}

	result = m_VerticalBlurShader->initialize(m_D3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur shader object.", L"Error", MB_OK);
		return false;
	}

	m_RenderTexture = new RenderTexture;
	if (!m_RenderTexture)
	{
		return false;
	}

	result = m_RenderTexture->initialize(m_D3D->getDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		return false;
	}

	m_DownSampleTexure = new RenderTexture;
	if (!m_DownSampleTexure)
	{
		return false;
	}

	result = m_DownSampleTexure->initialize(m_D3D->getDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the down sample render to texture object.", L"Error", MB_OK);
		return false;
	}

	m_HorizontalBlurTexture = new RenderTexture;
	if (!m_HorizontalBlurTexture)
	{
		return false;
	}

	result = m_HorizontalBlurTexture->initialize(m_D3D->getDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	m_VerticalBlurTexture = new RenderTexture;
	if (!m_VerticalBlurTexture)
	{
		return false;
	}

	result = m_VerticalBlurTexture->initialize(m_D3D->getDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	m_UpSampleTexure = new RenderTexture;
	if (!m_UpSampleTexure)
	{
		return false;
	}

	result = m_UpSampleTexure->initialize(m_D3D->getDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the up sample render to texture object.", L"Error", MB_OK);
		return false;
	}

	m_SmallWindow = new OrthoWindow;
	if (!m_SmallWindow)
	{
		return false;
	}

	result = m_SmallWindow->initialize(m_D3D->getDevice(), downSampleWidth, downSampleHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the small ortho window object.", L"Error", MB_OK);
		return false;
	}

	m_FullScreenWindow = new OrthoWindow;
	if (!m_FullScreenWindow)
	{
		return false;
	}

	result = m_FullScreenWindow->initialize(m_D3D->getDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
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
	if (m_FullScreenWindow)
	{
		m_FullScreenWindow->shutdown();
		delete m_FullScreenWindow;
	}

	if (m_SmallWindow)
	{
		m_SmallWindow->shutdown();
		delete m_SmallWindow;
	}

	if (m_UpSampleTexure)
	{
		m_UpSampleTexure->shutdown();
		delete m_UpSampleTexure;
	}

	if (m_VerticalBlurTexture)
	{
		m_VerticalBlurTexture->shutdown();
		delete m_VerticalBlurTexture;
	}

	if (m_HorizontalBlurTexture)
	{
		m_HorizontalBlurTexture->shutdown();
		delete m_HorizontalBlurTexture;
	}

	if (m_DownSampleTexure)
	{
		m_DownSampleTexure->shutdown();
		delete m_DownSampleTexure;
	}

	if (m_RenderTexture)
	{
		m_RenderTexture->shutdown();
		delete m_RenderTexture;
	}

	if (m_VerticalBlurShader)
	{
		m_VerticalBlurShader->shutdown();
		delete m_VerticalBlurShader;
	}

	if (m_HorizontalBlurShader)
	{
		m_HorizontalBlurShader->shutdown();
		delete m_HorizontalBlurShader;
	}

	if (m_TextureShader)
	{
		m_TextureShader->shutdown();
		delete m_TextureShader;
	}

	if (m_Model)
	{
		m_Model->shutdown();
		delete m_Model;
	}

	if (m_Camera)
	{
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

	result = render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::render(float rotation)
{
	bool result;

	result = renderSceneToTexture(rotation);
	if (!result)
	{
		return false;
	}
	
	result = downSampleTexture();
	if (!result)
	{
		return false;
	}

	result = renderHorizontalBlurToTexture();
	if (!result)
	{
		return false;
	}

	result = renderVerticalBlurToTexture();
	if (!result)
	{
		return false;
	}

	result = upSampleTexture();
	if (!result)
	{
		return false;
	}

	result = render2DTextureScene();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::renderSceneToTexture(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_RenderTexture->setRenderTarget(m_D3D->getDeviceContext());

	m_RenderTexture->clearRenderTarget(m_D3D->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getWorldMatrix(worldMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	D3DXMatrixRotationY(&worldMatrix, rotation);

	m_Model->render(m_D3D->getDeviceContext());

	result = m_TextureShader->render(m_D3D->getDeviceContext(), m_Model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
									 m_Model->getTexture());
	if (!result)
	{
		return false;
	}

	m_D3D->setBackBufferRenderTarget();

	m_D3D->resetViewport();

	return true;
}

bool Graphics::downSampleTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	m_DownSampleTexure->setRenderTarget(m_D3D->getDeviceContext());

	m_DownSampleTexure->clearRenderTarget(m_D3D->getDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getWorldMatrix(worldMatrix);

	m_DownSampleTexure->getOrthoMatrix(orthoMatrix);

	m_D3D->turnZBufferOff();

	m_SmallWindow->render(m_D3D->getDeviceContext());

	result = m_TextureShader->render(m_D3D->getDeviceContext(), m_SmallWindow->getIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
									 m_RenderTexture->getShaderResourceView());
	if (!result)
	{
		return false;
	}

	m_D3D->turnZBufferOn();

	m_D3D->setBackBufferRenderTarget();

	m_D3D->resetViewport();

	return true;
}

bool Graphics::renderHorizontalBlurToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	float screenSizeX;
	bool result;

	screenSizeX = (float)m_HorizontalBlurTexture->getTextureWidth();

	m_HorizontalBlurTexture->setRenderTarget(m_D3D->getDeviceContext());

	m_HorizontalBlurTexture->clearRenderTarget(m_D3D->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getWorldMatrix(worldMatrix);

	m_HorizontalBlurTexture->getOrthoMatrix(orthoMatrix);

	m_D3D->turnZBufferOff();

	m_SmallWindow->render(m_D3D->getDeviceContext());

	result = m_HorizontalBlurShader->render(m_D3D->getDeviceContext(), m_SmallWindow->getIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
											m_DownSampleTexure->getShaderResourceView(), screenSizeX);
	if (!result)
	{
		return false;
	}

	m_D3D->turnZBufferOn();

	m_D3D->setBackBufferRenderTarget();

	m_D3D->resetViewport();

	return true;
}

bool Graphics::renderVerticalBlurToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	float screenSizeY;
	bool result;

	screenSizeY = (float)m_VerticalBlurTexture->getTextureHeight();

	m_VerticalBlurTexture->setRenderTarget(m_D3D->getDeviceContext());

	m_VerticalBlurTexture->clearRenderTarget(m_D3D->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getWorldMatrix(worldMatrix);

	m_VerticalBlurTexture->getOrthoMatrix(orthoMatrix);

	m_D3D->turnZBufferOff();

	m_SmallWindow->render(m_D3D->getDeviceContext());

	result = m_VerticalBlurShader->render(m_D3D->getDeviceContext(), m_SmallWindow->getIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
										  m_HorizontalBlurTexture->getShaderResourceView(), screenSizeY);
	if (!result)
	{
		return false;
	}

	m_D3D->turnZBufferOn();

	m_D3D->setBackBufferRenderTarget();

	m_D3D->resetViewport();

	return true;
}

bool Graphics::upSampleTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	m_UpSampleTexure->setRenderTarget(m_D3D->getDeviceContext());

	m_UpSampleTexure->clearRenderTarget(m_D3D->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->render();

	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getWorldMatrix(worldMatrix);

	m_UpSampleTexure->getOrthoMatrix(orthoMatrix);

	m_D3D->turnZBufferOff();

	m_FullScreenWindow->render(m_D3D->getDeviceContext());

	result = m_TextureShader->render(m_D3D->getDeviceContext(), m_FullScreenWindow->getIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
									 m_VerticalBlurTexture->getShaderResourceView());
	if (!result)
	{
		return false;
	}

	m_D3D->turnZBufferOn();

	m_D3D->setBackBufferRenderTarget();

	m_D3D->resetViewport();

	return true;
}

bool Graphics::render2DTextureScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	m_D3D->beginScene(0.0f, 0.0f, 0.4f, 0.0f);

	m_Camera->render();

	m_Camera->getViewMatrix(viewMatrix);
	m_D3D->getWorldMatrix(worldMatrix);
	m_D3D->getOrthoMatrix(orthoMatrix);

	m_D3D->turnZBufferOff();

	m_FullScreenWindow->render(m_D3D->getDeviceContext());

	result = m_TextureShader->render(m_D3D->getDeviceContext(), m_FullScreenWindow->getIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
									 m_UpSampleTexure->getShaderResourceView());
	if (!result)
	{
		return false;
	}

	m_D3D->turnZBufferOn();

	m_D3D->endScene();

	return true;
}
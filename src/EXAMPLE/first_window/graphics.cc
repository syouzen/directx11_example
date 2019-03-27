#include "graphics.h"

Graphics::Graphics()
{
	m_D3D = 0;
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
	
	return true; 
}
void Graphics::shutdown()
{
	if (m_D3D)
	{
		m_D3D->shutdown();
		delete m_D3D;
	}
}

bool Graphics::frame()
{
	bool result;

	result = this->render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::render()
{
	m_D3D->beginScene(1.0f, 0.0f, 0.0f, 1.0f);

	/* Rendering Call */

	m_D3D->endScene();
	
	return true;
}
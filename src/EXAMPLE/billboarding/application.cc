#include "application.h"

Application::Application()
{
	m_Input = 0;
	m_Graphics = 0;
	m_Timer = 0;
	m_Position = 0;
}

Application::Application(const Application& other)
{
}

Application::~Application()
{
}

bool Application::initialize()
{
	int screenWidth, screenHeight;
	bool result;
	
	screenWidth = 0;
	screenHeight = 0;

	this->initializeWindows(screenWidth, screenHeight);

	m_Input = new Input;
	if (!m_Input)
	{
		return false;
	}

	result = m_Input->initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK); 
		return false;
	}

	m_Graphics = new Graphics;
	if (!m_Graphics)
	{
		return false;
	}

	result = m_Graphics->initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	m_Timer = new Timer;
	if (!m_Timer)
	{
		return false;
	}

	result = m_Timer->initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	m_Position = new Position;
	if (!m_Position)
	{
		return false;
	}

	m_Position->setPosition(0.0f, 1.5f, -11.0f);

	return true;
}

void Application::shutdown()
{
	if (m_Position)
	{
		delete m_Position; 
		m_Position = 0;
	}

	if (m_Timer)
	{
		delete m_Timer; 
		m_Timer = 0;
	}
	
	if (m_Graphics)
	{
		m_Graphics->shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	if (m_Input)
	{
		m_Input->shutdown();
		delete m_Input;
		m_Input = 0;
	}

	this->shutdownWindows();

	return;
}

void Application::run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = this->frame();
			if (!result)
			{
				MessageBox(m_hwnd, L"frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}

		if (m_Input->isEscapePressed())
		{
			done = true;
		}
	}
}

bool Application::frame()
{
	bool result, keyDown;
	float positionX, positionY, positionZ;
	
	m_Timer->frame();

	result = m_Input->frame();
	if (!result)
	{
		return false;
	}
	
	if (m_Input->isEscapePressed() == true)
	{
		return false;
	}

	m_Position->setFrameTime(m_Timer->getTime());

	keyDown = m_Input->isLeftPressed();
	m_Position->moveLeft(keyDown);

	keyDown = m_Input->isRightPressed();
	m_Position->moveRight(keyDown);

	m_Position->getPosition(positionX, positionY, positionZ);

	result = m_Graphics->frame(positionX, positionY, positionZ);
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK Application::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void Application::initializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	applicationHandle = this;
	m_hinstance = GetModuleHandle(NULL);
	m_applicationName = L"First Window";
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
							WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(false);
}

void Application::shutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	applicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	default:
	{
		return applicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
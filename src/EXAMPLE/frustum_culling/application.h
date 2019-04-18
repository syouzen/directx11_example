#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "input.h"
#include "graphics.h"
#include "timer.h" 
#include "position.h"

class Application
{
public:
	Application();
	Application(const Application&);
	~Application();

	bool initialize();
	void shutdown();
	void run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool frame();
	void initializeWindows(int&, int&);
	void shutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	Input* m_Input;
	Graphics* m_Graphics;
	Timer* m_Timer;
	Position* m_Position;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static Application* applicationHandle = 0;
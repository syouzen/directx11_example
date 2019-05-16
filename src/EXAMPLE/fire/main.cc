#include "application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Application* app;
	bool result;
	
	app = new Application;
	if (!app)
	{
		return 0;
	}

	result = app->initialize();
	if (result)
	{
		app->run();
	}

	app->shutdown();
	delete app;

	return 0;
}
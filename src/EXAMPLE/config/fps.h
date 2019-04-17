#pragma once

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>

class Fps
{
public:
	Fps();
	Fps(const Fps&);
	~Fps();

	void initialize();
	void frame();
	int getFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};
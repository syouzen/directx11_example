#include "Fps.h"

Fps::Fps()
{
}

Fps::Fps(const Fps& other)
{
}

Fps::~Fps()
{
}

void Fps::initialize()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
	return;
}

void Fps::frame()
{
	m_count++;

	if (timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;

		m_startTime = timeGetTime();
	}
}

int Fps::getFps()
{
	return m_fps;
}
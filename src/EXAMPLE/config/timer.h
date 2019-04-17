#pragma once

#include <windows.h>

class Timer
{
public:
	Timer();
	Timer(const Timer&);
	~Timer();

	bool initialize();
	void frame();

	float getTime();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};
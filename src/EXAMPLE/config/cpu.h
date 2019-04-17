#pragma once

#pragma comment(lib, "pdh.lib")

#include <pdh.h>

class Cpu
{
public:
	Cpu();
	Cpu(const Cpu&);
	~Cpu();

	void initialize();
	void shutdown();
	void frame();
	int getCpuPercentage();

private:
	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
};
#pragma once

#include <d3dx10math.h>
#include <stdlib.h>
#include <time.h>

class ModelList
{
private:
	struct ModelInfoType
	{
		D3DXVECTOR4 color;
		float positionX, positionY, positionZ;
	};

public:
	ModelList();
	ModelList(const ModelList&);
	~ModelList();

	bool initialize(int);
	void shutdown();

	int getModelCount();
	void getData(int, float&, float&, float&, D3DXVECTOR4&);

private:
	int m_modelCount;
	ModelInfoType* m_ModelInfoList;
};
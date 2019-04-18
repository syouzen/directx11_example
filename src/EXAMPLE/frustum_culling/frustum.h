#pragma once

#include <d3dx10math.h>

class Frustum
{
public:
	Frustum();
	Frustum(const Frustum&);
	~Frustum();

	void constructFrustum(float, D3DXMATRIX, D3DXMATRIX);

	bool checkPoint(float, float, float);
	bool checkCube(float, float, float, float);
	bool checkSphere(float, float, float, float);
	bool checkRectangle(float, float, float, float, float, float);

private:
	D3DXPLANE m_planes[6];
};
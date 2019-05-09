#pragma once

#include <d3dx10math.h>

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void setPosition(float, float, float);
	void setRotation(float, float, float);

	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getRotation();

	void render();
	void getViewMatrix(D3DXMATRIX&);

	void renderReflection(float); 
	D3DXMATRIX getReflectionViewMatrix();

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_reflectionViewMatrix;
};
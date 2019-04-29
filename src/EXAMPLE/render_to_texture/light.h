#pragma once

#include <d3dx10math.h>

class Light 
{ 
public: 
	Light(); 
	Light(const Light&); 
	~Light(); 
	
	void setAmbientColor(float, float, float, float);
	void setDiffuseColor(float, float, float, float); 
	void setDirection(float, float, float);

	D3DXVECTOR4 getAmbientColor();
	D3DXVECTOR4 getDiffuseColor(); 
	D3DXVECTOR3 getDirection(); 

private: 
	D3DXVECTOR4 m_ambientColor;
	D3DXVECTOR4 m_diffuseColor; 
	D3DXVECTOR3 m_direction; 
};
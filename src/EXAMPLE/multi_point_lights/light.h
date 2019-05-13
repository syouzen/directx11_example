#pragma once

#include <d3dx10math.h>

class Light 
{ 
public: 
	Light(); 
	Light(const Light&); 
	~Light(); 
	
	void setDiffuseColor(float, float, float, float); 
	void setPosition(float, float, float);
	D3DXVECTOR4 getDiffuseColor(); 
	D3DXVECTOR4 getPosition(); 

private: 
	D3DXVECTOR4 m_diffuseColor; 
	D3DXVECTOR4 m_position;
};
#include "light.h"

Light::Light()
{
}

Light::Light(const Light& other)
{
}

Light::~Light()
{
}

void Light::setDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
}

void Light::setPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR4(x, y, z, 1.0f);
}

D3DXVECTOR4 Light::getDiffuseColor()
{
	return m_diffuseColor;
}

D3DXVECTOR4 Light::getPosition()
{
	return m_position;
}
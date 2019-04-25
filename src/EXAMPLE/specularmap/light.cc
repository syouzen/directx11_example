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

void Light::setDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
}

void Light::setSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = D3DXVECTOR4(red, green, blue, alpha);
}

void Light::setSpecularPower(float power)
{
	m_specularPower = power;
}

D3DXVECTOR4 Light::getDiffuseColor()
{
	return m_diffuseColor;
}

D3DXVECTOR3 Light::getDirection()
{
	return m_direction;
}

D3DXVECTOR4 Light::getSpecularColor()
{
	return m_specularColor;
}

float Light::getSpecularPower()
{
	return m_specularPower;
}
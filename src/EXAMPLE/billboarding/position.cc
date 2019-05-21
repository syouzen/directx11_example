#include "position.h"

Position::Position()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_frameTime = 0.0f;

	m_leftSpeed = 0.0f;
	m_rightSpeed = 0.0f;
}


Position::Position(const Position& other)
{
}

Position::~Position()
{
}

void Position::setPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void Position::setRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

void Position::getPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}

void Position::getRotation(float& x, float& y, float& z)
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}

void Position::setFrameTime(float time)
{
	m_frameTime = time;
	return;
}

void Position::moveLeft(bool keydown)
{
	float radians;

	if (keydown)
	{
		m_leftSpeed += m_frameTime * 0.001f;

		if (m_leftSpeed > (m_frameTime * 0.03f))
		{
			m_leftSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_leftSpeed -= m_frameTime * 0.0007f;

		if (m_leftSpeed < 0.0f)
		{
			m_leftSpeed = 0.0f;
		}
	}

	radians = m_rotationY * 0.0174532925f;

	m_positionX -= cosf(radians) * m_leftSpeed;
	m_positionZ -= sinf(radians) * m_leftSpeed;

	return;
}


void Position::moveRight(bool keydown)
{
	float radians;

	if (keydown)
	{
		m_rightSpeed += m_frameTime * 0.001f;

		if (m_rightSpeed > (m_frameTime * 0.03f))
		{
			m_rightSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_rightSpeed -= m_frameTime * 0.0007f;

		if (m_rightSpeed < 0.0f)
		{
			m_rightSpeed = 0.0f;
		}
	}

	radians = m_rotationY * 0.0174532925f;

	m_positionX += cosf(radians) * m_rightSpeed;
	m_positionZ += sinf(radians) * m_rightSpeed;

	return;
}
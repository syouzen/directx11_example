#pragma once

#include <math.h>

class Position
{
public:
	Position();
	Position(const Position&);
	~Position();

	void setPosition(float, float, float);
	void setRotation(float, float, float);

	void getPosition(float&, float&, float&);
	void getRotation(float&, float&, float&);

	void setFrameTime(float);

	void moveLeft(bool);
	void moveRight(bool);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_frameTime;
	float m_leftSpeed, m_rightSpeed;
};
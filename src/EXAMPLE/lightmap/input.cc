#include "input.h"

Input::Input()
{
}

Input::Input(const Input& other)
{
}

Input::~Input()
{
}

void Input::initialize()
{
	int i;
	
	for (i = 0; i<256; i++)
	{
		m_keys[i] = false;
	}
}

void Input::keyDown(unsigned int input)
{
	m_keys[input] = true;
}

void Input::keyUp(unsigned int input)
{
	m_keys[input] = false;
}

bool Input::isKeyDown(unsigned int key)
{
	return m_keys[key];
}
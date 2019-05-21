#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class Input 
{ 
  public: 
	  Input();
	  Input(const Input&);
	  ~Input();

	  bool initialize(HINSTANCE, HWND, int, int);
	  void shutdown();
	  bool frame();

	  bool isEscapePressed();
	  void getMouseLocation(int&, int&);

	  bool isLeftPressed();
	  bool isRightPressed();

private:
	bool readKeyboard();
	bool readMouse();
	void processInput();

private:
	IDirectInput8 * m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256]; 
	DIMOUSESTATE m_mouseState; 

	int m_screenWidth, m_screenHeight; 
	int m_mouseX, m_mouseY;
};
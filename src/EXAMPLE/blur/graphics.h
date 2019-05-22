#pragma once

#include "d3d.h"
#include "camera.h"
#include "model.h"
#include "render_texture.h"
#include "ortho_window.h"
#include "texture_shader.h"
#include "horizontal_blur_shader.h"
#include "vertical_blur_shader.h"

const bool FULL_SCREEN = false; 
const bool VSYNC_ENABLED = true; 
const float SCREEN_DEPTH = 1000.0f; 
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool initialize(int, int, HWND);
	void shutdown();
	bool frame();

private:
	bool render(float);

	bool renderSceneToTexture(float);
	bool downSampleTexture();
	bool renderHorizontalBlurToTexture();
	bool renderVerticalBlurToTexture();
	bool upSampleTexture();
	bool render2DTextureScene();

private:
	D3D * m_D3D;
	Camera* m_Camera;
	Model* m_Model;
	TextureShader* m_TextureShader;
	HorizontalBlurShader* m_HorizontalBlurShader;
	VerticalBlurShader* m_VerticalBlurShader;
	RenderTexture *m_RenderTexture, *m_DownSampleTexure, *m_HorizontalBlurTexture, *m_VerticalBlurTexture, *m_UpSampleTexure;
	OrthoWindow *m_SmallWindow, *m_FullScreenWindow;
};
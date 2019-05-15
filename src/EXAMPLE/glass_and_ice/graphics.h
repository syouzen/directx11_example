#pragma once

#include "d3d.h"
#include "camera.h"
#include "model.h"
#include "render_texture.h"
#include "texture_shader.h"
#include "glass_shader.h"

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
	bool renderToTexture(float);
	bool render(float);

private:
	D3D * m_D3D;
	Camera* m_Camera;
	Model* m_Model;
	Model* m_WindowModel;
	RenderTexture* m_RenderTexture;
	TextureShader* m_TextureShader;
	GlassShader* m_GlassShader;
};
#pragma once

#include "d3d.h"
#include "camera.h"
#include "model.h"
#include "light.h"
#include "render_texture.h"
#include "refraction_shader.h"
#include "light_shader.h"
#include "water_shader.h"

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
	bool render();

private:
	bool renderRefractionToTexture();
	bool renderReflectionToTexture();
	bool renderScene();

private:
	D3D* m_D3D;
	Camera* m_Camera;
	Model *m_GroundModel, *m_WallModel, *m_BathModel, *m_WaterModel;
	Light* m_Light;
	RenderTexture *m_RefractionTexture, *m_ReflectionTexture;
	LightShader* m_LightShader;
	RefractionShader* m_RefractionShader;
	WaterShader* m_WaterShader;
	float m_waterHeight, m_waterTranslation;
};
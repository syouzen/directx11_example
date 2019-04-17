#pragma once

#include "font.h" 
#include "shader.h"

class Text
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Text();
	Text(const Text&);
	~Text();

	bool initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void shutdown();
	bool render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	bool setMousePosition(int, int, ID3D11DeviceContext*);
	bool setFps(int, ID3D11DeviceContext*);
	bool setCpu(int, ID3D11DeviceContext*);

private:
	bool initializeSentence(SentenceType**, int, ID3D11Device*);
	bool updateSentence(SentenceType*, const char*, int, int, float, float, float, ID3D11DeviceContext*);
	void releaseSentence(SentenceType**);
	bool renderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);

private:
	Font * m_Font;
	Shader* m_FontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;
	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
};
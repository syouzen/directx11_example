#include "font.h"

Font::Font()
{
	m_Font = 0;
	m_Texture = 0;
}

Font::Font(const Font& other)
{
}

Font::~Font()
{
}

bool Font::initialize(ID3D11Device* device, const char* fontFilename, const WCHAR* textureFilename)
{
	bool result;
	
	result = loadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	result = loadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}


void Font::shutdown()
{
	releaseTexture();
	releaseFontData();
}

bool Font::loadFontData(const char* filename)
{
	ifstream fin;
	int i;
	char temp;

	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	for (i = 0; i<95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	fin.close();

	return true;
}

void Font::releaseFontData()
{
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}
}

bool Font::loadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;
	
	m_Texture = new Texture;
	if (!m_Texture)
	{
		return false;
	}

	result = m_Texture->initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}


void Font::releaseTexture()
{
	if (m_Texture)
	{
		m_Texture->shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* Font::getTexture()
{
	return m_Texture->getTexture();
}

void Font::buildVertexArray(void* vertices, const char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	vertexPtr = (VertexType*)vertices;

	numLetters = (int)strlen(sentence);

	index = 0;

	for (i = 0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f); 
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f); 
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f); 
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY, 0.0f);  
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f); 
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}
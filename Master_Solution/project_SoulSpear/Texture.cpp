#include "Texture.h"
#include <gl_core_4_4.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(std::string filePath)
	:	m_textureID(-1),
		m_textureWidth(0),
		m_textureHeight(0),
		m_textureFormat(0)

{
	LoadTexture();
}

Texture::~Texture()
{
	if (m_textureID != -1)
	{
		glDeleteTextures(1, &m_textureID);
	}
}

Texture::Texture(Texture&& other)
{
	m_textureID		= other.m_textureID;
	m_textureWidth	= other.m_textureWidth;
	m_textureHeight = other.m_textureHeight;
	m_textureFormat = other.m_textureFormat;
	m_texturePath	= other.m_texturePath;

	other.m_textureID = -1;

}

void Texture::LoadTexture()
{
	unsigned char* data = stbi_load(m_texturePath.c_str(), &m_textureWidth,
		&m_textureHeight, &m_textureFormat, STBI_default);
	assert(data != nullptr && "Unable to load the texture at that path");

	int internalFormat = 0;
	switch (m_textureFormat)
	{
	case 1:
		internalFormat = GL_RED;
		break;
	case 2:
		internalFormat = GL_RG;
		break;
	case 3:
		internalFormat = GL_RGB;
		break;
	case 4:
		internalFormat = GL_RGBA;
		break;
	default:
		assert(false && "If you hit this, STBI is not working!");
		break;
	}

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	// Send Data to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_textureWidth, 
		m_textureHeight, 0, internalFormat, GL_UNSIGNED_BYTE, data);

	// SETTING: Filtering (If missing texture will show black)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// SETTING: Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);

	stbi_image_free(data);
}

#pragma once
#include <string>
class Texture
{
public:
	Texture(std::string filePath);
	~Texture();

	Texture(const Texture& other) = delete;
	Texture(Texture&& other);

	unsigned int GetTextureID() const { return m_textureID; }

private:
	void LoadTexture();

	unsigned int m_textureID;
	int m_textureWidth;
	int m_textureHeight;
	int m_textureFormat;

	std::string m_texturePath;
};
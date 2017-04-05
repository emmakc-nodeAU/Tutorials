#pragma once
#include <GLFW\glfw3.h>
#include "Application.h"
#include <glm/mat4x4.hpp>
#include <vector>
#include <Texture.h>

namespace aie
{
	class Texture;
}
class RenderData;
class Shader;
class Camera;

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	using OBJMesh = std::vector<RenderData*>;

	// Camera as an object
	Camera* m_camera;

	// Render and Shader
	RenderData* m_gridRenderData;
	Shader* m_gridShader;
	
	// BUNNY
	OBJMesh m_bunny;
	Shader* m_bunnyShader;

	// SOULSPEAR
	OBJMesh m_soulSpear;
	//Shader* m_soulSpearShader;
	Texture* m_soulSpearDiffuse;

	// Textures
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	// Stores OpenGL Texture
	unsigned int m_texture;

	// Rendering Textures
	unsigned int m_programID;
};

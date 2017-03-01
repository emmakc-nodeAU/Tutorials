#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

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

	// Render and Shader
	RenderData* m_gridRenderData;
	Shader* m_gridShader;

	// Camera
	Camera* m_camera;
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// Textures
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	// Stores OpenGL Texture
	unsigned int m_texture;

	// Rendering Textures
	unsigned int m_program;
};

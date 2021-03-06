#pragma once
#include <GLFW\glfw3.h>
#include "Application.h"
#include <glm/mat4x4.hpp>
#include <vector>
class RenderData;
class Shader;
class Texture;
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

	// Immediate camera
	//glm::mat4	m_viewMatrix;
	//glm::mat4	m_projectionMatrix;

	// Render and Shader
	RenderData* m_gridRenderData;
	Shader* m_gridShader;
	
	// BUNNY
	OBJMesh m_bunny;
	Shader* m_bunnyShader;

	// SoulSpear
	OBJMesh m_soulSpear;
	Shader* m_soulSpearShader;
	Texture* m_soulSpearDiffuse;

	//// BUDDHA
	//OBJMesh m_buddha;
	//Shader* m_buddhaShader;

	//// DRAGON
	//OBJMesh m_dragon;
	//Shader* m_dragonShader;

	//// LUCY
	//OBJMesh m_lucy;
	//Shader* m_lucyShader;


	// Textures
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	// Stores OpenGL Texture
	unsigned int m_texture;

	// Rendering Textures
	unsigned int m_programID;
};

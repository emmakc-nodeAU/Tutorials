////////////////////////////////////////////////////////////
//
//	Name:		Emma Cameron
//	Date:		3 March 2017
//	Project:	Week 4: Lighting
//	Class:		Year 2 Programming, with Matt Dodd
//	
////////////////////////////////////////////////////////////

#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

class Camera;

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	//virtual Camera* getCamera();
	//virtual int getTexture();

protected:

	// Camera
	Camera* m_camera;
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// Texture DIFFUSE
	int imageWidthA = 0, imageHeightA = 0, imageFormatA = 0;

	// Texture TRANS
	int imageWidthB = 0, imageHeightB = 0, imageFormatB = 0;

	// Stores OpenGL Texture
	unsigned int m_textureDiffuse;
	unsigned int m_textureTrans;

	// Rendering Textures
	unsigned int m_programID;
};
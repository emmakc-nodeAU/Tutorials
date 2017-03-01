#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "CameraFly.h"
#include <stb_image.h>
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	
	/*
					/////	TUTORIAL: CAMERA	//////
			1. Created Class: Camera

			2. Base Class
				- Camera transforms (View, Projection), 
				- World Transform
				- Set Position Camera
				- Set Projection

			3. Derived Class
				- First Person Camera
				- Third Person Camera
				- RTS camera
				- Fly Camera (Key/WASD Mouse movement)


	
	*/



	m_camera = new CameraFly(m_window,  5.0f);
	m_camera->SetPerspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);
	m_camera->LookAt(glm::vec3(10, 10, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));

	/*
				///// TUTORIAL: TEXTURES /////

				1. LOAD: Read in a Texture, specify path.
	*/
	// Load Texture path
	unsigned char* data = stbi_load("./data/textures/EarthTextures/earth_diffuse.jpg", &imageWidth, &imageHeight, &imageFormat, STBI_default);
	// Generate Texture handle
	glGenTextures(1, &m_texture);
	// Bind texture to slot
	glBindTexture(GL_TEXTURE_2D, m_texture);
	// Specify the data for the texture: format, resolution, variable
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	// Specify filtering: resolution, ratio for texel: pixel.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Free the data
	stbi_image_free(data);

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// rotate camera
	m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, 10, glm::cos(time) * 10),
							   vec3(0), vec3(0, 1, 0));

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// demonstrate a few shapes
	Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(0, 0.5f, 1, 0.25f));
	Gizmos::addSphere(vec3(5, 0, 5), 1, 8, 8, vec4(1, 0, 0, 0.5f));
	Gizmos::addRing(vec3(5, 0, -5), 1, 1.5f, 8, vec4(0, 1, 0, 1));
	Gizmos::addDisk(vec3(-5, 0, 5), 1, 16, vec4(1, 1, 0, 1));
	Gizmos::addArc(vec3(-5, 0, -5), 0, 2, 1, 8, vec4(1, 0, 1, 1));

	mat4 t = glm::rotate(time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);
	Gizmos::addCylinderFilled(vec3(0), 0.5f, 1, 5, vec4(0, 1, 1, 1), &t);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	// CAMERA FLY VIEW:
	m_camera->Update(deltaTime);
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	//// update perspective in case window resized
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
	//									  getWindowWidth() / (float)getWindowHeight(),
	//									  0.1f, 1000.f);

	// CAMERA: OLD VIEW
	//Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// CAMERA FLY VIEW:
	Gizmos::draw(m_camera->GetProjectionView());
}
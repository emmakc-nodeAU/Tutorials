#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <stb_image.h>
#include <gl_core_4_4.h>

#include "CameraFly.h"
#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"

#include "GeometryHelper.h"
#include "RenderData.h"
#include "Shader.h"

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
	
	// CAMERA
	m_camera = new CameraFly(m_window, 15.0f);

	m_camera->SetPerspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);

	m_camera->LookAt(glm::vec3(10, 10, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
	
	// GRID	 
	m_gridRenderData = GeometryHelper::CreateGrid(10, 10, 10, 10, glm::vec4(1, 0, 0, 1));
	m_gridShader = new Shader("./shaders/grid.vert", "./shaders/grid.frag");

	// BUNNY
	//m_bunny = GeometryHelper::LoadOBJFromDisk("./models/stanford/bunny.obj");
	//m_bunnyShader = new Shader("./shaders/obj.vert", "./shaders/obj.frag");

	return true;
}

void Application3D::shutdown() 
{
	// END - BUNNY
	//delete m_bunnyShader;
	//for (auto& renderData : m_bunny)
	//{
	//	delete renderData;
	//}
	
	// END - GRID
	delete m_gridRenderData;
	delete m_gridShader;

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) 
{
	// GET: Time
	float time = getTime();

	// CAMERA FLY VIEW:
	m_camera->Update(deltaTime);

	Gizmos::clear();

	// GRID: Draw lines using gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) 
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);

		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	// GRID: Axis transform
	Gizmos::addTransform(mat4(1));
	mat4 t = glm::rotate(time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);

	// rotate immediate camera
	//m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, 10, glm::cos(time) * 10),
		//vec3(0), vec3(0, 1, 0));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draws in wire form

	// demonstrate a few shapes
	//Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(0, 0.5f, 1, 0.25f));
	//Gizmos::addSphere(vec3(5, 0, 5), 1, 8, 8, vec4(1, 0, 0, 0.5f));
	//Gizmos::addRing(vec3(5, 0, -5), 1, 1.5f, 8, vec4(0, 1, 0, 1));
	//Gizmos::addDisk(vec3(-5, 0, 5), 1, 16, vec4(1, 1, 0, 1));
	//Gizmos::addArc(vec3(-5, 0, -5), 0, 2, 1, 8, vec4(1, 0, 1, 1));
	//Gizmos::addCylinderFilled(vec3(0), 0.5f, 1, 5, vec4(0, 1, 1, 1), &t);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if(input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() 
{
	// wipe the screen to the background colour
	clearScreen();

	// CAMERA
	Gizmos::draw(m_camera->GetProjectionView());

	// DRAW - GRID
	glUseProgram(m_gridShader->GetProgramID());

	int loc = glGetUniformLocation(m_gridShader->GetProgramID(), "projectionViewWorldMatrix");
	assert(loc != -1);
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(m_camera->GetProjectionView()));

	glm::mat4 modelMatrix(1);
	loc = glGetUniformLocation(m_gridShader->GetProgramID(), "modelMatrix");
	assert(loc != -1);
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(modelMatrix));

	m_gridRenderData->Render();

	// DRAW - BUNNY
	//glUseProgram(m_bunnyShader->GetProgramID());
	//
	//loc = glGetUniformLocation(m_bunnyShader->GetProgramID(), "projectionViewWorldMatrix");
	//assert(loc != -1);
	//glUniformMatrix4fv(loc, 1, false, glm::value_ptr(m_camera->GetProjectionView()));
	//
	//modelMatrix[3].x = 0;
	//
	//loc = glGetUniformLocation(m_bunnyShader->GetProgramID(), "modelMatrix");
	//assert(loc != -1);
	//glUniformMatrix4fv(loc, 1, false, glm::value_ptr(modelMatrix));
	//
	//for (auto& renderData : m_bunny)
	//{
	//	renderData->Render();
	//}
}
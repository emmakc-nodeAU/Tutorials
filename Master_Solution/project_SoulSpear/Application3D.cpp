#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <stb_image.h>
#include <gl_core_4_4.h>

//#include "Camera.h"
#include "CameraFly.h"
#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include "Texture.h"
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

	// Create simple camera transforms
	//m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
	//										getWindowWidth() / (float)getWindowHeight(),
	//										0.1f, 1000.f);
	
	// CAMERA - Fly
	m_camera = new CameraFly(m_window, 15.0f);
	m_camera->SetPerspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);
	m_camera->LookAt(glm::vec3(10, 10, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
	
	// PASS IN OBJECT - Soulspear
	//m_soulSpearShader = new Shader("./shaders/soulspeak_normal.tga");
	m_soulSpearDiffuse = new Texture("./textures/soulspear_diffuse.tga");

	// PASS IN OBJECT - GRID	 
	m_gridRenderData = GeometryHelper::CreateGrid(10, 10, 10, 10, glm::vec4(1, 0, 0, 1));
	m_gridShader = new Shader("./shaders/grid.vert", "./shaders/grid.frag");

	// PASS IN OBJECT - BUNNY
	m_bunny = GeometryHelper::LoadOBJFromDisk("./models/stanford/bunny.obj");
	m_bunnyShader = new Shader("./shaders/obj.vert", "./shaders/obj.frag");

	//// PASS IN OBJECT - BUDDHA
	//m_buddha = GeometryHelper::LoadOBJFromDisk("./models/stanford/buddha.obj");
	//m_buddhaShader = new Shader("./shaders/obj.vert", "./shaders/obj.frag");

	//// PASS IN OBJECT - DRAGON
	//m_dragon = GeometryHelper::LoadOBJFromDisk("./models/stanford/dragon.obj");
	//m_dragonShader = new Shader("./shaders/obj.vert", "./shaders/obj.frag");

	//// PASS IN OBJECT - LUCY
	//m_lucy = GeometryHelper::LoadOBJFromDisk("./models/stanford/lucy.obj");
	//m_lucyShader = new Shader("./shaders/obj.vert", "./shaders/obj.frag");

	return true;
}

void Application3D::shutdown() 
{
	// END - BUNNY
	for (auto& renderData : m_bunny)
	{
		delete renderData;
	}
	//// END - BUDDHA
	//for (auto& renderData : m_buddha)
	//{
	//	delete renderData;
	//}
	//// END - DRAGON
	//for (auto& renderData : m_dragon)
	//{
	//	delete renderData;
	//}
	//// END - LUCY
	//for (auto& renderData : m_lucy)
	//{
	//	delete renderData;
	//}

	// END - GRID
	delete m_gridRenderData;
	delete m_gridShader;
	delete m_soulSpearDiffuse;
	Gizmos::destroy();
}

void Application3D::update(float deltaTime) 
{
	// query time since application started
	float time = getTime();

	// rotate immediate camera
	//m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, 10, glm::cos(time) * 10),
		//vec3(0), vec3(0, 1, 0));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draws in wire form

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
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

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// demonstrate a few shapes
	//Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(0, 0.5f, 1, 0.25f));
	//Gizmos::addSphere(vec3(5, 0, 5), 1, 8, 8, vec4(1, 0, 0, 0.5f));
	//Gizmos::addRing(vec3(5, 0, -5), 1, 1.5f, 8, vec4(0, 1, 0, 1));
	//Gizmos::addDisk(vec3(-5, 0, 5), 1, 16, vec4(1, 1, 0, 1));
	//Gizmos::addArc(vec3(-5, 0, -5), 0, 2, 1, 8, vec4(1, 0, 1, 1));

	mat4 t = glm::rotate(time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);
	//Gizmos::addCylinderFilled(vec3(0), 0.5f, 1, 5, vec4(0, 1, 1, 1), &t);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if(input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	// CAMERA FLY VIEW:
	m_camera->Update(deltaTime);
}

void Application3D::draw() 
{

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
	//									  getWindowWidth() / (float)getWindowHeight(),
	//									  0.1f, 1000.f);
	//
	//glm::mat4 projView = m_projectionMatrix * m_viewMatrix;

	// CAMERA FLY VIEW:
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

	// DRAW - SOULSPEAR
	glUseProgram(m_soulSpearShader->GetProgramID());
	loc = glGetUniformLocation(m_soulSpearShader->GetProgramID(), "projectionViewWorldMatrix");
	assert(loc != -1);
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(m_camera->GetProjectionView()));

	loc = glGetUniformLocation(m_soulSpearShader->GetProgramID(), "modelMatrix");
	assert(loc != -1);
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(modelMatrix));

	// Reference TextureSlot 0:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_soulSpearDiffuse->GetTextureID());

	loc = glGetUniformLocation(m_soulSpearShader->GetProgramID(), "diffuse");
	assert(loc != -1);
	glUniformli(loc, 0);

	for (auto& renderData : m_soulSpear)
	{
		renderData->Render();
	}
	// Texture doesnt sit nicely on Object.
	// FIX: In shader, reverse y coords, not uv.y = 1- uv.y // flip y values
	// STBI_Should_Vertically_Flip_Image (1) to flip, (0) not flip.. in constructor bool flip.



	// DRAW - BUNNY
	//glUseProgram(m_bunnyShader->GetProgramID());
	//loc = glGetUniformLocation(m_bunnyShader->GetProgramID(), "projectionViewWorldMatrix");
	//assert(loc != -1);
	//glUniformMatrix4fv(loc, 1, false, glm::value_ptr(m_camera->GetProjectionView()));

	//modelMatrix = glm::mat4(1);
	//modelMatrix[3].x = 0;
	//glUseProgram(m_bunnyShader->GetProgramID());
	//loc = glGetUniformLocation(m_bunnyShader->GetProgramID(), "modelMatrix");
	//assert(loc != -1);
	//glUniformMatrix4fv(loc, 1, false, glm::value_ptr(modelMatrix));

	//for (auto& renderData : m_bunny)
	//{
	//	renderData->Render();
	//}

	//// DRAW - BUDDHA
	//glUseProgram(m_buddhaShader->GetProgramID());
	//loc = glGetUniformLocation(m_buddhaShader->GetProgramID(), "projectionViewWorldMatrix");
	//assert(loc != -1);
	//glUniformMatrix4fv(loc, 1, false, glm::value_ptr(projView));

	//modelMatrix = glm::mat4(1);
	//modelMatrix[3].x = -5;
	//loc = glGetUniformLocation(m_buddhaShader->GetProgramID(), "modelMatrix");
	//assert(loc != -1);
	//glUniformMatrix4fv(loc, 1, false, glm::value_ptr(modelMatrix));

	//for (auto& renderData : m_buddha)
	//{
	//	renderData->Render();
	//}

	//// DRAW - DRAGON
	//glUseProgram(m_dragonShader->GetProgramID());
	//loc = glGetUniformLocation(m_dragonShader->GetProgramID(), "projectionViewWorldMatrix");
	//assert(loc != -1);
	//glUniformMatrix4fv(loc, 1, false, glm::value_ptr(projView));

	//loc = glGetUniformLocation(m_dragonShader->GetProgramID(), "modelMatrix");
	//assert(loc != -1);
	//glUniformMatrix4fv(loc, 1, false, glm::value_ptr(modelMatrix));

	//for (auto& renderData : m_dragon)
	//{
	//	renderData->Render();
	//}

	//// DRAW - LUCY
	//glUseProgram(m_lucyShader->GetProgramID());
	//loc = glGetUniformLocation(m_lucyShader->GetProgramID(), "projectionViewWorldMatrix");
	//assert(loc != -1);
	//glUniformMatrix4fv(loc, 1, false, glm::value_ptr(projView));

	//loc = glGetUniformLocation(m_lucyShader->GetProgramID(), "modelMatrix");
	//assert(loc != -1);
	//glUniformMatrix4fv(loc, 1, false, glm::value_ptr(modelMatrix));

	//for (auto& renderData : m_lucy)
	//{
	//	renderData->Render();
	//}


	// CAMERA: OLD VIEW
	//Gizmos::draw(m_projectionMatrix * m_viewMatrix);

}
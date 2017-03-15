////////////////////////////////////////////////////////////
//
//	Name:		Emma Cameron
//	Date:		1 March 2017
//	Project:	Week 4: Lighting
//	Class:		Year 2 Programming, with Matt Dodd
//	
////////////////////////////////////////////////////////////
#include "RenderShader.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <stb_image.h>
#include <gl_core_4_4.h>
#include <imgui_glfw3.h>

#include "CameraFly.h"
#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;


RenderShader::RenderShader()
{
}

RenderShader::~RenderShader()
{
}

// FUNCTION 1. SETUP
bool RenderShader::startup()
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(64000, 64000, 64000, 64000);

	// create simple camera transforms
	//	vec3(1): camera's actual position off world space
	//	vec3(2): camera's facing direction or 'lookat' on grid 
	//	vec3(3): camera's up.

	////////////////////////////// CREATE: TEXTURE ///////////////////////////////////
	// TEXTURE 1: Load Texture path: earth_diffuse
	int imageWidthA = 0, imageHeightA = 0, imageFormatA = 0;
	unsigned char* dataDiffuse = stbi_load("./textures/earth_diffuse.jpg", 
		&imageWidthA, &imageHeightA, &imageFormatA, STBI_default);
	// Generate Texture handle
	glGenTextures(1, &m_textureDiffuse);
	// Bind texture to slot
	glBindTexture(GL_TEXTURE_2D, m_textureDiffuse);
	// Specify the data for the texture: format, resolution, variable
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidthA, imageHeightA, 0, GL_RGB, GL_UNSIGNED_BYTE, dataDiffuse);
	// Specify filtering: resolution, ratio for texel: pixel.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(dataDiffuse);

	glBindTexture(GL_TEXTURE_2D, 0);

	// TEXTURE 2: Load: Texture 2: earth_cloud_trans
	int imageWidthB = 0, imageHeightB = 0, imageFormatB = 0;
	unsigned char* dataTrans = stbi_load("./textures/earth_cloud_trans.jpg",
		&imageWidthB, &imageHeightB, &imageFormatB, STBI_default);
	// Generate Texture handle
	glGenTextures(1, &m_textureTrans);
	// Bind texture to slot
	glBindTexture(GL_TEXTURE_2D, m_textureTrans);
	// Specify the data for the texture: format, resolution, variable
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, imageWidthB, imageHeightB, 0, GL_RED, GL_UNSIGNED_BYTE, dataTrans);
	// Specify filtering: resolution, ratio for texel: pixel.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	// Free the data
	stbi_image_free(dataTrans);

	////// CAMERA - FLY //////
	m_camera = new CameraFly(m_window, 5.0f);
	m_camera->SetPerspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);
	m_camera->LookAt(glm::vec3(10, 10, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));

	// FUNCTION 5. CREATE: SHADER
	setupShader();

	// FUNCTION 6. CREATE: QUAD
	generateTexturedQuad();

	return true;
}

// FUNCTION 2. SHUTDOWN
void RenderShader::shutdown()
{
	Gizmos::destroy();
}

// FUNCTION 3. UPDATE
void RenderShader::update(float deltaTime)
{
	// query time since application started
	float time = getTime();

	// Update camera
	m_camera->Update(deltaTime);

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

// FUNCTION 4. DRAW
void RenderShader::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);
	// BIND: Shader Program
	glm::mat4 projView = m_projectionMatrix * m_viewMatrix;
	
	// Bind: Texture program
	glUseProgram(m_programID);

	int loc = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE,
		&(m_camera->GetProjectionView()[0][0]));

	//// TEXTURE 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureDiffuse);

	// Tell Shader location of Texure
	loc = glGetUniformLocation(m_programID, "diffuse");
	glUniform1i(loc, 0);

	//// TEXTURE 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textureTrans);

	// Tell Shader location of Texure
	loc = glGetUniformLocation(m_programID, "trans");
	glUniform1i(loc, 1);


	// Draw
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

// FUNCTION 5: CREATE SHADER
// SHADER
void RenderShader::setupShader()
{
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 position; \
							layout(location=1) in vec2 texCoord; \
							out vec2 vTexCoord; \
							uniform mat4 projectionViewWorldMatrix; \
							void main() \
								{ \
								vTexCoord = texCoord; \
								gl_Position = projectionViewWorldMatrix * \
								position; \
								}";

	//// Warped Sin Layer - Not working
	//const char* fsSource = "#version 410\n \
	//						in vec2 vTexCoord; \
	//						out vec4 pixelColour; \
	//						uniform float time; \
	//						uniform sampler2D diffuse; \
	//						uniform sampler2D trans; \
	//						void main() \
	//							{ \
	//							vec2 warpedST = vTexCoord + vec2 (sin(time), 0); \
	//							pixelColour = texture(diffuse, warpedST);\
	//							}";
	// MultiTexture Layers
	const char* fsSource = "#version 410\n \
							in vec2 vTexCoord; \
							out vec4 fragColor; \
							uniform sampler2D diffuse; \
							uniform sampler2D trans; \
							void main() \
								{ \
								fragColor.rgb = texture(trans,vTexCoord).rrr * texture(diffuse,vTexCoord).rgb; \
								fragColor.a = 1;\
								}";
	int success = GL_FALSE;

	unsigned int	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(	vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);

	unsigned int	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(	fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	// CLEANUP AttachedShaders:
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// FUNCTION 6. CREATE QUAD
void RenderShader::generateTexturedQuad()
{
	// QUAD
	float vertexData[] =
	{
		-5, 0, 5, 1, 0, 1,
		5, 0, 5, 1, 1,1,
		5, 0, -5, 1, 1,0,
		-5, 0, -5, 1, 0, 0,
	};

	unsigned int indexData[] =
	{
		0, 1, 2,
		0, 2, 3,
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4,
		vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,
		indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		sizeof(float) * 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



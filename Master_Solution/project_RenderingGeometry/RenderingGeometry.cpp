////////////////////////////////////////////////////////////
//
//	Name:		Emma Cameron
//	Date:		17 February 2017
//	Project:	Week 2: Rendering Geometry
//	Class:		Year 2 Programming, with Matt Dodd
//	
////////////////////////////////////////////////////////////

#include "RenderingGeometry.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "..\bootstrap\Application.h"
#include <imgui_glfw3.h>
#include <gl_core_4_4.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;


RenderingGeometry::RenderingGeometry() 
{
}

RenderingGeometry::~RenderingGeometry() 
{
}

// FUNCTION 1. SETUP
bool RenderingGeometry::startup() 
{
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(64000, 64000, 64000, 64000);

	// create simple camera transforms
	//	vec3(1): camera's actual position off world space
	//	vec3(2): camera's facing direction or 'lookat' on grid 
	//	vec3(3): camera's up.

	////////////////////////////// CAMERA: VIEW MATRIX ///////////////////////////////////
	// CAMERA: VIEW MATRIX
	m_viewMatrix = glm::lookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));

	// CAMERA: PROJECTION MATRIX (Orthographic)
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	// FUNCTION 5. CREATE: SHADER
	setupShader();
	generateGrid(10, 10);
	return true;
}

// FUNCTION 2. SHUTDOWN
void RenderingGeometry::shutdown() 
{

	Gizmos::destroy();
}

// FUNCTION 3. UPDATE
void RenderingGeometry::update(float deltaTime) 
{

	// query time since application started
	float time = getTime();

	////////////////////////////// CAMERA: ROTATING ///////////////////////////////////
	// Currently Rotating Camera

	m_viewMatrix = glm::lookAt(vec3(
		glm::sin(time) * 10, 10, 
		glm::cos(time) * 10),
		vec3(0), 
		vec3(0, 1, 0));

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
void RenderingGeometry::draw()
{

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);

	glm::mat4 projView = m_projectionMatrix * m_viewMatrix;

	glUseProgram(m_programID);

	// TIME SCALE
	unsigned int timeUniform = glGetUniformLocation(m_programID, "time");
	glUniform1f(timeUniform, getTime());

	// HEIGHT SCALE
	//unsigned int heightScale = glGetUniformLocation(m_programID, "heightScale");
	//glUniform1f(heightScale, heightScale());

	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(projView));

	glBindVertexArray(m_VAO);

	unsigned int indexCount = m_numberOfIndices;

	// RENDER: triangles
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}
// FUNCTION 5: CREATE SHADER
// SHADER
void RenderingGeometry::setupShader()
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

	const char* fsSource = "#version 410\n \
							in vec2 vTexCoord; \
							out vec4 fragColor; \
							void main() \
								{ \
								fragColor = texture(diffuse,vTexCoord); \
								}";

	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&fsSource, 0);
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

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// GRID: Vertex points, stored in the vertex buffer
void RenderingGeometry::generateGrid(unsigned int rows, unsigned int cols)
{
	// 1. Allocate memory to hold vertex data
	Vertex* aoVertices = new Vertex[rows * cols];
	// 2. Populate meory with vertex data

	// ROWS
	for (unsigned int r = 0; r < rows; ++r)
	{
		// COLUMNS
		for (unsigned int c = 0; c < cols; ++c)
		{
			Vertex& vert = aoVertices[r * cols + c];
			vert.position = glm::vec4((float)c, 0, (float)r, 1.0f);
			// COLOUR OF GRID / WIREFRAME
			vert.colour = glm::vec4(0.5f / 255, 0.5f, 0.5f, 1.0f);

			// Create a position
			//aoVertices[r*cols + c].position = vec4((float)c, 0, (float)r, 1);
			// Create a colour
			//vec3 colour = vec3(sinf((c / (float)(cols - 1))*(r / (float)(rows - 1))));
			//aoVertices[r*cols + c].colour = vec4(colour, 1);
		}
	}
	// 3. Send data to graphics card
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_numberOfIndices = (rows - 1) * (cols - 1) * 6;
	unsigned int* auiIndices = new unsigned int[m_numberOfIndices];
	
	unsigned int index = 0;

	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			// Square (With x2 Triangles)
			// 0 |\   | 1
			//	 | \  |
			//	 |  \ |
			// 3 |___\| 2

			// Tri A
			// 0 |\
			//   | \
			// 	 |  \
			// 3 |___\ 2

			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			// Tri B
			// 0 \   | 1
			//    \  |
			//     \ |
			//      \| 2

			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}
	}

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Generate Vertex Array Object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Bind the Buffers: 
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	glBindVertexArray(0);

	// 4. Delete allocated memory
	delete[] aoVertices;
	delete[] auiIndices;
}




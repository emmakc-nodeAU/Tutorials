////////////////////////////////////////////////////////////
//
//	Name:		Emma Cameron
//	Date:		23 February 2017
//	Project:	Week 3: Texturing
//	Class:		Year 2 Programming, with Matt Dodd
//	
////////////////////////////////////////////////////////////
#pragma once

//#include "RenderShader.h"
#include <glm/glm.hpp>
#include "Application3D.h"

///////////////////////////////////////////////////////////////////////////
//	STORAGE INFORMATION:
//	eg. Position, colour for a Quad( a square made up of x2 triangles)
//	Create array memory to hold vector information:
//	1. Storage Type: Struct (pos, color), array size 4(4 points in a quad)
//	2. [0] = position, colour	|
//	3. [1] = position, colour	| --\	1st Memory storage 
//	4. [2] = position, colour	| --/	(Vertex Info Buffer Array)
//	5. [3] = position, colour	|
//
///////////////////////////////////////////////////////////////////////////


class Camera;

// 1. CREATE: VERTEX BUFFER
struct Vertex
{
	glm::vec4 position;
	glm::vec4 colour;
};

class RenderShader : public Application3D {
private:

public:

	RenderShader();
	virtual ~RenderShader();

	// FUNCTION 1. SETUP
	virtual bool startup();
	// FUNCTION 2. SHUTDOWN
	virtual void shutdown();
	// FUNCTION 3. UPDATE
	virtual void update(float deltaTime);
	// FUNCTION 4. DRAW / RENDER
	virtual void draw();


protected:
	// FUNCTION 5. CREATE: SHADER
	void setupShader();

	// FUNCTION 6. CREATE: QUAD
	void generateTexturedQuad();

	// VARIABLES: VIRTUAL CAMERA 
	// 1. VIEW MATRIX
	glm::mat4	m_viewMatrix;
	// 2. PROJECTION MATRIX (orthographic, not perspective)
	glm::mat4	m_projectionMatrix;
	// 3. WORLD MATRIX
	glm::mat4	m_worldMatrix;

	// BUFFER VARIABLES: (Remember to clean these up after)
	// VARIABLES: VERTEX ARRAY OBJECT, VERTEX BUFFER OBJ, INDEX BUFFER OBJ
	unsigned int m_VAO;	// Holds vertex position, texture coord data
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_numberOfIndices; // not used anywhere, couldnt keep up with tut

	// Rendering Shader Texture
	unsigned int m_programID;
};
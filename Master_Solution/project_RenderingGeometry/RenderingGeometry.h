////////////////////////////////////////////////////////////
//
//	Name:		Emma Cameron
//	Date:		17 February 2017
//	Project:	Week 2: Rendering Geometry
//	Class:		Year 2 Programming, with Matt Dodd
//	
////////////////////////////////////////////////////////////
#pragma once

#include "RenderingGeometry.h"
#include <glm/glm.hpp>
#include "Application.h"

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

// 1. CREATE: VERTEX BUFFER
struct Vertex
{
	glm::vec4 position;
	glm::vec4 colour;
};

class RenderingGeometry : public aie::Application {
private:

public:

	RenderingGeometry();
	virtual ~RenderingGeometry();

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
	// FUNCTION 6. CREATE: GRID COORDS
	void generateGrid(unsigned int rows, unsigned int cols);
	
	// VARIABLES: VIRTUAL CAMERA 
	// 1. VIEW MATRIX
	glm::mat4	m_viewMatrix;
	// 2. PROJECTION MATRIX (orthographic, not perspective)
	glm::mat4	m_projectionMatrix;
	// 3. WORLD MATRIX
	glm::mat4	m_worldMatrix;

	// CAMERA: VIEW MATRIX INVERSE
	

	// VARIABLES: VERTEX ARRAY, VERTEX BUFFER, INDEX BUFFER
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_numberOfIndices; // not used anywhere, couldnt keep up with tut
	
	// Shader
	unsigned int m_programID;
};
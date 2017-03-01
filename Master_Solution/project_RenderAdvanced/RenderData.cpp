#include "RenderData.h"
#include <gl_core_4_4.h>
#include <assert.h>

// Constructor
RenderData::RenderData()
	// Initialise
	: m_VAO(-1)
	, m_VBO(-1)
	, m_IBO(-1)
	, m_numbersofIndices(-1)
{
}

// Destructor
RenderData::~RenderData()
{
	// DESTROY BUFFER
	if(m_IBO != -1)
		glDeleteBuffers(1, &m_IBO);
	if (m_VBO != -1)
		glDeleteBuffers(1, &m_VBO);
	if (m_VAO != -1)
		glDeleteVertexArrays(1, &m_VAO);
}

// Move constructor
RenderData::RenderData(RenderData&& other)
{
	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
	m_IBO = other.m_IBO;
	m_numbersofIndices = other.m_numbersofIndices;

	other.m_VAO = -1;
	other.m_VBO = -1;
	other.m_IBO = -1;
}

// DEFENSIVE CODING - Assists with Debug / Avoids Heartache
void RenderData::GenerateBuffers()
{
	// IF ASSERT FAILS, THIS WILL TELL YOU WHAT HAPPENED
	assert(m_VAO == -1 && "Buffers have already been created for this object!");
	assert(m_VBO == -1);
	assert(m_IBO == -1);
	
	// BUFFER
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBindVertexArray(0);
}

void RenderData::Bind() const
{
	assert(m_VAO != -1 && "You are binding an invalid");
	glBindVertexArray(m_VAO);
}

void RenderData::Unbind() const
{
	glBindVertexArray(0);
}

void RenderData::Render() const
{
	Bind();
	glDrawElements(GL_TRIANGLES, m_numbersofIndices,
		GL_UNSIGNED_INT, 0);
	Unbind();
}

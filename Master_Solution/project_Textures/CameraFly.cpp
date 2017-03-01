#include "CameraFly.h"
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

CameraFly::CameraFly(GLFWwindow* window, float speed)
	: m_speed(speed)
	, m_window(window)
{

}

void CameraFly::Update(float deltaTime)
{
	/*
	Query Camera Direction, forward direction, right, up
	Move direction of keyboard inputs
	Update 
	*/
	glm::mat4 transform = GetTransform();
	glm::vec4 vRight = transform[0];	// where RIGHT is stored
	glm::vec4 vUp = transform[1];		// where UP is stored
	glm::vec4 vForward = transform[2];	// where FORWARD is stored

	// Final Direction to Move towards
	glm::vec4 moveDirection;

	// Keyboard
	// MOVE FORWARD
	if (glfwGetKey(m_window, GLFW_KEY_W))
	{
		moveDirection += (vForward * m_speed);
	}
	// MOVE BACKWARD
	if (glfwGetKey(m_window, GLFW_KEY_S))
	{
		moveDirection -= (vForward * m_speed);
	}
	// MOVE LEFT
	if (glfwGetKey(m_window, GLFW_KEY_A))
	{
		moveDirection -= (vRight * m_speed);
	}
	// MOVE RIGHT
	if (glfwGetKey(m_window, GLFW_KEY_D))
	{
		moveDirection += (vRight * m_speed);
	}
	//// UP
	//if (glfwGetKey(m_window, GLFW_KEY_D))
	//{
	//	m_worldTransform += (vUp * m_speed);
	//}
	//// DOWN
	//if (glfwGetKey(m_window, GLFW_KEY_D))
	//{
	//	m_worldTransform -= (vUp * m_speed);
	//}

	// Check moveDirection < 0
	if (glm::length(moveDirection) > 0.0f)
	{
		transform[3] += moveDirection * deltaTime;
		SetTransform(transform);
	}
}
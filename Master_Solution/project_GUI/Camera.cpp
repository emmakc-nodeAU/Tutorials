#include "Camera.h"

Camera::Camera()
	: m_worldTransform(1)
	, m_projectionTransform(1)
	// ^ Initialiser list using above variables.
{
	UpdateViewFromWorld();
}

void Camera::SetPosition(const glm::vec3& position)
{
	m_worldTransform[3] = glm::vec4(position, 1);
	UpdateViewFromWorld();
}

void Camera::LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 worldUp)
{
	m_viewTransform = glm::lookAt(eye, center, worldUp);
	UpdateWorldFromView();
}

void Camera::SetPerspective(float vFOV, float aspectRatio, float nearDistance, float farDistance)
{
	m_projectionTransform = glm::perspective(
		vFOV, aspectRatio, nearDistance, farDistance);
}

// PRIVATE:
void Camera::UpdateViewFromWorld()
{
	m_viewTransform = glm::inverse(m_worldTransform);
}

void Camera::UpdateWorldFromView()
{
	m_worldTransform = glm::inverse(m_viewTransform);
}
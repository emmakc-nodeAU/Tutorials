#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class  Camera
{
public:
	Camera();
	virtual ~Camera() = default;

	virtual void Update(float deltaTime) {}

	void SetPosition(const glm::vec3& pos);
	glm::vec3 GetPosition() const { return glm::vec3(m_worldTransform[3].x, m_worldTransform[3].y, m_worldTransform[3].z); }

	void LookAt(glm::vec3 eye, glm::vec3 centre, glm::vec3 worldUp);

	void SetPerspective(float vFOV, float aspectRatio, float nearDistance, float farDistance);

	void SetTransform(const glm::mat4 trans) { m_worldTransform = trans; UpdateViewFromWorld(); }
	glm::mat4 GetTransform() const { return m_worldTransform; }

	glm::mat4 GetView() const { return m_viewTransform; }
	glm::mat4 GetProjection() const { return m_projectionTransform; }
	glm::mat4 GetProjectionView() const { return m_projectionTransform * m_viewTransform; }



private:

	// World Matrix inverse
	void UpdateViewFromWorld();

	// View Matrix inverse
	void UpdateWorldFromView();

	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;

};

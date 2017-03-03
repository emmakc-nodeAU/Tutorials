#pragma once
#include "Camera.h"

struct GLFWwindow;

class CameraFly : public Camera
{
public:
	CameraFly(GLFWwindow* window, float speed);
	~CameraFly() = default;

	void SetSpeed(float speed) { m_speed = speed; }
	float GetSpeed() const { return m_speed; }

	void Update(float deltaTime) override;

private:
	GLFWwindow* m_window;
	float m_speed;
};
